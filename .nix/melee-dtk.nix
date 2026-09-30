{
  stdenvNoCC,
  lib,
  decomp-toolkit,
  devkitppc,
  mwcc,
  objdiff,
  ninja,
  python3,
  wibo,
  main-dol,
  sjiswrap,
  # Sample the .dat archives' types too (configure.py --dat-dwarf): the
  # melee-dat tool, the DWARF build of the game, orig/GALE01/files, and
  # what compiles the samples like the DWARF build
  melee-dat ? null,
  melee-dwarf ? null,
  dat-files ? null,
  clang ? null,
  aurora-src ? null,
  newlib ? null,
}:
let
  withDat = melee-dat != null;
in
assert
  withDat
  -> melee-dwarf != null && dat-files != null && clang != null && aurora-src != null && newlib != null;
stdenvNoCC.mkDerivation (finalAttrs: {
  name = "doldecomp-melee";

  src = lib.fileset.toSource {
    root = ../.;
    fileset = lib.fileset.unions [
      ../config
      ../configure.py
      ../flake.lock
      ../libs
      ../orig/GALE01/sys/.gitkeep
      ../src
      ../tools/download_tool.py
      ../tools/ninja_syntax.py
      ../tools/project.py
      ../tools/transform_dep.py
    ];
  };

  postPatch = ''
    ln -sfT ${mwcc}/GC tools/mwcc_compiler
    ln -sfT ${main-dol} orig/GALE01/sys/main.dol
  ''
  + lib.optionalString withDat ''
    ln -sfT ${dat-files} orig/GALE01/files
  '';

  nativeBuildInputs = [
    decomp-toolkit
    devkitppc
    ninja
    python3
    wibo
  ]
  ++ lib.optional withDat clang;

  env = lib.optionalAttrs withDat {
    AURORA_SRC = "${aurora-src}";
    NEWLIB_INCLUDE = "${newlib}/powerpc-none-eabi/include";
  };

  configurePhase = ''
    runHook preConfigure
    python3 ./configure.py ${toString finalAttrs.configureFlags}
    runHook postConfigure
  '';

  configureFlags = [
    "--wrapper=wibo"
    "--dtk=${decomp-toolkit}/bin/dtk"
    "--objdiff=${objdiff}/bin/objdiff-cli"
    "--binutils=${devkitppc}/bin"
    "--sjiswrap=${sjiswrap}"
    "--compilers=${mwcc}"
  ]
  ++ lib.optionals withDat [
    "--melee-dat=${melee-dat}/bin/melee-dat"
    "--dat-dwarf=${melee-dwarf}/melee.elf"
  ];

  installPhase = ''
    runHook preInstall
    cp build/GALE01/report.json $out
    runHook postInstall
  '';

  strictDeps = true;
  __structuredAttrs = true;
})
