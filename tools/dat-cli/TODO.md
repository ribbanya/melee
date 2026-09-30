# TODO

## Walk findings

- `ItemStateDesc.x4_matanim_joint` and `x8_parameters` hold unrelocated
  values in some items' first state (10 cases: `GrCn.dat`, `ItCo.dat`, ...).
  They aren't always those pointer types.

- Kirby's copy-ability data (`ftDataKirbyCopy*`) is loaded through
  `((HSD_Archive**) &ft_80459B88)[kind]`, so it's typed `HSD_Archive`. The
  slot is really a `KirbyHatStruct*`, but the flat index disagrees with
  `ft_80459B88.hats[kind]` elsewhere by one.
- `Fighter_WaitAnimData.xC` points to `CmdUnion` animation scripts. The
  union can't be resolved without modelling the script's commands.
- `EffectDataTable` only has its two particle banks. The records after them
  (an `f32` and four pointers each) aren't typed.

Not errors:

- 18 objects reached as both `HSD_CameraAnim` and `HSD_WObjAnim`: the
  exporter reuses identical bytes. The walker could recognize this.
- `-1` in pointer fields means none. Counted, not reported.

## Stopgaps

- `ItemStateArray` uses `DAT_EXTENT`. Its length is the largest `anim_id` in
  the item kind's `ItemStateTable`, plus one. Replace with a `DAT_COUNT`
  based on `Article::kind` once the counts are available (item state enums,
  or reading the tables from the ELF).
- `ItemSpecialAttributes` covers 25 common item kinds. Missing:
  - types defined in `.c` files: G_Shell, MSBomb, StarRod, Hammer,
    StarRod_Star
  - inconsistent types: R_Shell, Foods, Kinoko
  - never used: ScBall, RabbitC, MetalB, Spycloak
  - all character items and Pokémon

## Coverage

- `Pl??AJ.dat` (fighter animations) aren't read: they're many archives
  packed together.
- Trophies (`Ty*.dat`): names come from tables inside archives.
- `Ef*`, `Sd*`, `Sm*`: no roots.
- 8 calls still use untyped `HSD_ArchiveGetPublicAddress`: `lbarchive.c`,
  the `tydisplay` wrapper, `ftdemo` motion data, `grdatfiles` stage info.
- About 15,000 `void*` fields aren't followed. Use `DAT_TYPE` where the type
  is known.

## Tool

- `DAT_NULLTERM` is parsed but the walker ignores it.
- Only pointers are checked against relocations. Wrong scalar types go
  unnoticed.
- `samples`: objdiff sample objects. No relocations yet; sizes are wrong.
- `src/symbols.rs`: parser for hand-written root bindings. Unused.
