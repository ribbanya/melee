//! `samples`: one instance of each archive type, compared with objdiff.
//!
//! `generate` writes, under the configured directory:
//! - `target/<unit>.o`: the sampled bytes from the archives
//! - `src/<unit>.c`: C generated from the current types
//! - `manifest.json`: the units, which `configure.py` turns into build
//!   edges compiling each source to `base/<unit>.o` and into `dat/<unit>`
//!   units of the root objdiff project
//! - `dep`: what the manifest was generated from, for ninja
//!
//! `report` compares them once ninja has built the base objects.

use super::{
    dwarf_path,
    project::{Check, Project},
};
use anyhow::{Context, Result, bail};
use globset::GlobSet;
use melee_dat::{
    config::{gather_files, get_config},
    hsd::Archive,
    samples::{CWriter, Picker, Sample, Skipped, Source, target_object},
};
use rayon::prelude::*;
use serde::Serialize;
use serde_json::json;
use std::{
    collections::{BTreeMap, BTreeSet},
    fs,
    io::{self, Write},
    path::{Path, PathBuf},
    process::Command as Process,
};

#[derive(clap::Args)]
pub struct Args {
    #[command(subcommand)]
    command: Command,
}

#[derive(clap::Subcommand)]
enum Command {
    /// List the samples: one instance of each type the walk finds
    List(List),

    /// Write the target objects, the C and their manifest
    Generate(Generate),

    /// Compare the samples with objdiff
    Report(Report),
}

#[derive(clap::Args)]
struct List {
    #[command(flatten)]
    check: Check,
    #[arg(long)]
    json: bool,
}

#[derive(clap::Args)]
struct Generate {
    #[command(flatten)]
    check: Check,
    /// Where the game's compiler looks for headers (repeatable)
    #[arg(short = 'I', long = "include")]
    include: Vec<PathBuf>,
}

#[derive(clap::Args)]
struct Report {
    /// Project config
    #[arg(default_value = "config/GALE01/dat.yml")]
    cfg_path: PathBuf,
    #[arg(short = 'p', long)]
    proj_path: Option<PathBuf>,
    /// Print objdiff's report instead of the table
    #[arg(long)]
    json: bool,
}

pub fn run(Args { command }: Args) -> Result<()> {
    match command {
        Command::List(args) => list(args),
        Command::Generate(args) => generate(args),
        Command::Report(args) => report(args),
    }
}

/// The samples and the types that have none.
fn pick(project: &Project) -> Result<(Vec<Sample>, Vec<Skipped>)> {
    let mut picker = Picker::new(&project.graph, &project.canonical);
    project.walk_all(&GlobSet::empty(), |w| {
        let offset = w
            .name
            .split_once('@')
            .and_then(|(_, o)| usize::from_str_radix(&o[2..], 16).ok())
            .unwrap_or(0);
        picker.add(w.file, offset, w.archive, &w.result);
        Ok(())
    })?;
    Ok(picker.finish())
}

#[derive(Serialize)]
struct Row<'a> {
    #[serde(rename = "type")]
    ty: &'a str,
    symbol: String,
    unit: &'a str,
    file: &'a str,
    archive: usize,
    offset: u32,
    size: u64,
    relocations: usize,
}

fn list(args: List) -> Result<()> {
    let project = Project::load(&args.check)?;
    let (samples, skipped) = pick(&project)?;
    let rows: Vec<Row> = samples
        .iter()
        .map(|s| Row {
            ty: &s.type_name,
            symbol: s.symbol(),
            unit: s.unit(),
            file: &s.location.file,
            archive: s.location.archive,
            offset: s.location.offset,
            size: s.size,
            relocations: s.relocs,
        })
        .collect();
    let mut out = io::stdout().lock();
    if args.json {
        let skipped: BTreeMap<&str, &str> = skipped
            .iter()
            .map(|s| (s.type_name.as_str(), s.reason.as_str()))
            .collect();
        let report = json!({ "samples": rows, "skipped": skipped });
        serde_json::to_writer_pretty(&mut out, &report)?;
        writeln!(out)?;
        return Ok(());
    }
    for r in &rows {
        let archive = match r.archive {
            0 => String::new(),
            a => format!("@0x{a:X}"),
        };
        writeln!(
            out,
            "{}: {} ({:#X} bytes, {} relocations) from {}{archive} at 0x{:X}",
            r.unit, r.ty, r.size, r.relocations, r.file, r.offset
        )?;
    }
    for s in &skipped {
        writeln!(out, "skipped: {} ({})", s.type_name, s.reason)?;
    }
    let mut units: Vec<_> = rows.iter().map(|r| r.unit).collect();
    units.sort();
    units.dedup();
    eprintln!(
        "{} samples in {} units, {} types skipped",
        rows.len(),
        units.len(),
        skipped.len()
    );
    Ok(())
}

/// The directory samples are generated in.
fn samples_dir(cfg_path: &Path, proj_path: Option<&PathBuf>) -> Result<PathBuf> {
    let config = get_config(proj_path, cfg_path)?;
    let proj = proj_path.cloned().unwrap_or_default();
    Ok(proj.join(config.samples.dir.as_str()))
}

/// `header` if an include directory has it, else the nearest parent
/// header that one does (`dolphin/mtx/GeoTypes.h` to `dolphin/mtx.h`).
fn find_header(dirs: &[PathBuf], header: &str) -> String {
    let exists = |h: &str| dirs.iter().any(|d| d.join(h).is_file());
    let mut candidate = header.to_owned();
    loop {
        if exists(&candidate) {
            return candidate;
        }
        let stem = candidate.strip_suffix(".h").unwrap_or(&candidate);
        match stem.rsplit_once('/') {
            Some((parent, _)) => candidate = format!("{parent}.h"),
            None => return header.to_owned(),
        }
    }
}

/// One unit of `manifest.json`, with paths relative to the manifest.
#[derive(Serialize, serde::Deserialize)]
struct ManifestUnit {
    /// The unit, e.g. `sysdolphin/baselib/jobj`.
    name: String,
    source: String,
    target: String,
    symbols: Vec<String>,
}

#[derive(Serialize, serde::Deserialize)]
struct Manifest {
    units: Vec<ManifestUnit>,
}

/// Write `bytes` unless the file already holds them, so that ninja only
/// rebuilds what changed.
fn write_if_changed(path: &Path, bytes: &[u8]) -> Result<bool> {
    if fs::read(path).is_ok_and(|old| old == bytes) {
        return Ok(false);
    }
    fs::create_dir_all(path.parent().unwrap())?;
    fs::write(path, bytes).with_context(|| format!("{}", path.display()))?;
    Ok(true)
}

/// Remove every file under `dir` that isn't in `keep`.
fn remove_stale(dir: &Path, keep: &BTreeSet<PathBuf>) -> Result<()> {
    let Ok(entries) = fs::read_dir(dir) else {
        return Ok(());
    };
    for entry in entries {
        let path = entry?.path();
        if path.is_dir() {
            remove_stale(&path, keep)?;
            // Fails unless it is now empty, which is the point
            let _ = fs::remove_dir(&path);
        } else if !keep.contains(&path) {
            fs::remove_file(&path)?;
        }
    }
    Ok(())
}

fn generate(args: Generate) -> Result<()> {
    let dir = samples_dir(&args.check.cfg_path, args.check.proj_path.as_ref())?;
    let project = Project::load(&args.check)?;
    let (mut samples, _) = pick(&project)?;
    // The DWARF build's headers aren't always the game's; include what
    // the game's compiler can find
    for sample in &mut samples {
        sample.header = find_header(&args.include, &sample.header);
        for include in &mut sample.includes {
            *include = find_header(&args.include, include);
        }
    }
    let mut units: BTreeMap<String, Vec<Sample>> = BTreeMap::new();
    for sample in samples {
        units.entry(sample.unit().to_owned()).or_default().push(sample);
    }

    // Each archive a sample reads, parsed once
    let mut files: BTreeMap<String, Vec<u8>> = BTreeMap::new();
    for sample in units.values().flatten() {
        let file = &sample.location.file;
        if !files.contains_key(file) {
            let path = project.base.join(file);
            let bytes = fs::read(&path)
                .with_context(|| format!("{}", path.display()))?;
            files.insert(file.clone(), bytes);
        }
    }
    let mut archives: BTreeMap<(&str, usize), Archive> = BTreeMap::new();
    for (file, bytes) in &files {
        for (at, archive) in Archive::parse_packed(bytes)? {
            archives.insert((file, at), archive);
        }
    }
    let sources: BTreeMap<(&str, usize), Source> =
        archives.iter().map(|(&k, a)| (k, Source::new(a))).collect();

    let writer = CWriter::new(&project.graph, &project.canonical);
    let mut manifest = Manifest { units: Vec::new() };
    let (mut keep_targets, mut keep_sources, mut keep_bases) =
        (BTreeSet::new(), BTreeSet::new(), BTreeSet::new());
    let mut changed = 0;
    for (unit, samples) in &units {
        let pairs: Vec<(&Sample, &Source)> = samples
            .iter()
            .map(|s| {
                let key = (s.location.file.as_str(), s.location.archive);
                (s, &sources[&key])
            })
            .collect();
        let target = dir.join("target").join(format!("{unit}.o"));
        let source = dir.join("src").join(format!("{unit}.c"));
        changed += usize::from(write_if_changed(&target, &target_object(&pairs)?)?);
        changed +=
            usize::from(write_if_changed(&source, writer.unit(&pairs)?.as_bytes())?);
        keep_bases.insert(dir.join("base").join(format!("{unit}.o")));
        keep_bases.insert(dir.join("base").join(format!("{unit}.d")));
        keep_targets.insert(target);
        keep_sources.insert(source);
        manifest.units.push(ManifestUnit {
            name: unit.clone(),
            source: format!("src/{unit}.c"),
            target: format!("target/{unit}.o"),
            symbols: samples.iter().map(Sample::symbol).collect(),
        });
    }
    // Units that no longer exist don't linger
    remove_stale(&dir.join("target"), &keep_targets)?;
    remove_stale(&dir.join("src"), &keep_sources)?;
    remove_stale(&dir.join("base"), &keep_bases)?;

    let manifest_path = dir.join("manifest.json");
    write_if_changed(
        &manifest_path,
        (serde_json::to_string_pretty(&manifest)? + "\n").as_bytes(),
    )?;

    // What the manifest depends on, for ninja: the types, every archive
    // and the configuration
    let mut deps = vec![dwarf_path(args.check.dwarf.clone())?];
    deps.extend(gather_files(&project.base, &project.include)?);
    let proj = args.check.proj_path.clone().unwrap_or_default();
    let config = get_config(args.check.proj_path.as_ref(), &args.check.cfg_path)?;
    deps.push(proj.join(&args.check.cfg_path));
    deps.push(proj.join(config.symbols.as_str()));
    let escape = |p: &Path| p.to_string_lossy().replace(' ', "\\ ");
    let mut dep = format!("{}:", escape(&manifest_path));
    for path in &deps {
        dep.push_str(" \\\n  ");
        dep.push_str(&escape(path));
    }
    dep.push('\n');
    fs::write(dir.join("dep"), dep)?;

    eprintln!(
        "{} units in {}, {changed} files changed",
        units.len(),
        dir.display()
    );
    Ok(())
}

/// A sample's match, from objdiff's diff of its unit.
#[derive(Serialize, Clone)]
struct SampleMatch {
    name: String,
    size: u64,
    match_percent: f64,
}

#[derive(Serialize, Default, Clone)]
struct MatchMeasures {
    total_samples: u64,
    matched_samples: u64,
    matched_samples_percent: f64,
    total_data: u64,
    /// Bytes, weighted by each sample's match.
    matched_data: f64,
    matched_data_percent: f64,
}

impl MatchMeasures {
    fn add(&mut self, sample: &SampleMatch) {
        self.total_samples += 1;
        self.matched_samples += u64::from(sample.match_percent >= 100.0);
        self.total_data += sample.size;
        self.matched_data += sample.size as f64 * sample.match_percent / 100.0;
        self.finish();
    }

    fn merge(&mut self, other: &MatchMeasures) {
        self.total_samples += other.total_samples;
        self.matched_samples += other.matched_samples;
        self.total_data += other.total_data;
        self.matched_data += other.matched_data;
        self.finish();
    }

    fn finish(&mut self) {
        let percent = |n: f64, d: f64| if d == 0.0 { 100.0 } else { n * 100.0 / d };
        self.matched_samples_percent =
            percent(self.matched_samples as f64, self.total_samples as f64);
        self.matched_data_percent =
            percent(self.matched_data, self.total_data as f64);
    }
}

#[derive(Serialize)]
struct UnitMatch {
    name: String,
    measures: MatchMeasures,
    samples: Vec<SampleMatch>,
}

/// objdiff's diff of one unit: each sample's match.
fn diff_unit(dir: &Path, unit: &str) -> Result<Vec<SampleMatch>> {
    let output = Process::new("objdiff-cli")
        .args(["diff", "-p"])
        .arg(dir)
        .args(["-u", unit, "-o", "-"])
        .output()
        .context("running objdiff-cli")?;
    if !output.status.success() {
        bail!(
            "objdiff-cli diff -u {unit}: {}",
            String::from_utf8_lossy(&output.stderr)
        );
    }
    let diff: serde_json::Value = serde_json::from_slice(&output.stdout)?;
    let symbols = diff["left"]["symbols"].as_array().cloned().unwrap_or_default();
    Ok(symbols
        .iter()
        .filter_map(|s| {
            let name = s["name"].as_str()?;
            if !name.starts_with("sample_") {
                return None;
            }
            let size = s["size"].as_str().and_then(|v| v.parse().ok()).unwrap_or(0);
            // A symbol with no counterpart has no match percent
            let match_percent = s["match_percent"].as_f64().unwrap_or(0.0);
            Some(SampleMatch {
                name: name.to_owned(),
                size,
                match_percent,
            })
        })
        .collect())
}

fn report(args: Report) -> Result<()> {
    let dir = samples_dir(&args.cfg_path, args.proj_path.as_ref())?;
    let manifest: Manifest = serde_json::from_str(
        &fs::read_to_string(dir.join("manifest.json"))
            .context("no manifest.json: configure with --dat-dwarf and run ninja")?,
    )?;
    // The root objdiff project has the samples as `dat/<unit>`
    let dir = args.proj_path.clone().unwrap_or_else(|| PathBuf::from("."));
    let names: Vec<String> = manifest
        .units
        .iter()
        .map(|u| format!("dat/{}", u.name))
        .collect();
    let units: Vec<UnitMatch> = names
        .par_iter()
        .map(|name| {
            let samples = diff_unit(&dir, name)?;
            let mut measures = MatchMeasures::default();
            for sample in &samples {
                measures.add(sample);
            }
            Ok(UnitMatch {
                name: name.trim_start_matches("dat/").to_owned(),
                measures,
                samples,
            })
        })
        .collect::<Result<_>>()?;
    let mut total = MatchMeasures::default();
    for unit in &units {
        total.merge(&unit.measures);
    }

    let mut out = io::stdout().lock();
    if args.json {
        let report = json!({ "version": 1, "measures": total, "units": units });
        serde_json::to_writer_pretty(&mut out, &report)?;
        writeln!(out)?;
        return Ok(());
    }
    // Only what doesn't match
    let mut rows: Vec<(&str, &SampleMatch)> = units
        .iter()
        .flat_map(|u| u.samples.iter().map(move |s| (u.name.as_str(), s)))
        .filter(|(_, s)| s.match_percent < 100.0)
        .collect();
    rows.sort_by(|a, b| a.1.match_percent.total_cmp(&b.1.match_percent));
    for (unit, s) in &rows {
        writeln!(
            out,
            "{:6.1}%  {}  ({unit}, {:#X} bytes)",
            s.match_percent,
            s.name.trim_start_matches("sample_"),
            s.size
        )?;
    }
    writeln!(
        out,
        "{}/{} samples match, {:.2}% of their bytes",
        total.matched_samples, total.total_samples, total.matched_data_percent
    )?;
    Ok(())
}
