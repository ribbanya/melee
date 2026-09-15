# lbcardnew CardEntry Slot Usage in Super Smash Bros. Melee

## Actual Slot Layout (from lbcardgame.c:lb_803BAB74)

```c
static struct CardEntry lb_803BAB74[10] = {
    { 0,     3, NULL },      // Slot 0: Metadata/header (0 bytes, flag 3)
    { 0x1790, 0, NULL },      // Slot 1: Main save data (6,032 bytes, flag 0)
    { 0x1F2C, 1, NULL },      // Slot 2: Custom names/data (7,980 bytes, flag 1)
    { 0x1F2C, 1, NULL },      // Slot 3: Custom names/data (7,980 bytes, flag 1)
    { 0x1F2C, 1, NULL },      // Slot 4: Custom names/data (7,980 bytes, flag 1)
    { 0x1F2C, 1, NULL },      // Slot 5: Custom names/data (7,980 bytes, flag 1)
    { 0x1F2C, 1, NULL },      // Slot 6: Custom names/data (7,980 bytes, flag 1)
    { 0x1F2C, 1, NULL },      // Slot 7: Custom names/data (7,980 bytes, flag 1)
    { 0x1F2C, 1, NULL },      // Slot 8: Custom names/data (7,980 bytes, flag 1)
    { -1,    0, NULL },       // Terminator
};
```

## What Each Slot Actually Contains

### Slot 0: File Header/Metadata (0 bytes, flag=3)
- **Size**: 0 bytes (empty file)
- **Flag**: 3 (HSD_CARD_FLAG_COMMENT)
- **Purpose**: Stores only the CARD file header with comment, banner, and icons
- **Data pointer**: NULL
- This slot exists purely to maintain the physical CARD file structure with game identification

### Slot 1: Main Save Data (6,032 bytes, flag=0)
- **Size**: 0x1790 = 6,032 bytes
- **Flag**: 0 (standard data file)
- **Purpose**: Contains ALL progression data:
  - Unlocked characters bitmask
  - Unlocked stages bitmask  
  - Unlocked features (score display, random stage, etc.)
  - Match statistics (time, stock, coin, bonus, stamina matches)
  - Trophy count and category flags
  - Trophy ownership flags (all 294 trophies)
  - Game settings/preferences (GamePrefs struct)
  - Fighter-specific data for all characters
- **Data pointer**: `gmMainLib_GetSaveData()` - returns pointer to `struct gmm_x1868`

### Slots 2-8: Name Tag Data Banks (7,980 bytes each, flag=1)
- **Size**: 0x1F2C = 7,980 bytes each
- **Flag**: 1 (HSD_CARD_FLAG_EXTRA_ICON - indicates special handling)
- **Purpose**: Custom name tags for players
- **Count**: 7 slots × 2 name banks per slot = 14 total name tag sets
- **Data pointer**: `gmMainLib_8015CC4C()[i]` - returns array of `struct NameTagDataBank`
- **Structure**: Each `NameTagDataBank` contains custom player names and associated data

## File Flag Meanings

| Flag | Constant | Meaning |
|------|----------|---------|
| 0 | (none) | Standard data file |
| 1 | HSD_CARD_FLAG_EXTRA_ICON | Special icon/data handling |
| 3 | HSD_CARD_FLAG_COMMENT | Comment/banner/icons only (no data) |

## Key Observations

1. **No replay system**: Melee does not store replay data in the save file
2. **No custom stages**: Custom stages are not saved to memory card
3. **No backup/redundancy**: The 9 slots are NOT for wear leveling or snapshots
4. **Single atomic write**: All slots are written together as one CARD file operation
5. **Total size**: ~62KB of logical data stored in one physical CARD file

## Memory Card File Details

- **Filename**: "SuperSmashBros0110290334" (includes build date: 2001/10/29 03:34)
- **Company**: "HAL"
- **Game Name**: Varies by region:
  - JP: "大乱闘スマッシュブラザーズＤＸ  セーブデータ"
  - EN: "Super Smash Bros. Melee         Game Data"
- **Physical location**: Memory Card Slot A (channel 0)

## References

- CardEntry definition: `src/melee/lb/lbcardnew.h`
- Slot initialization: `src/melee/lb/lbcardgame.c:48-55`
- Save data structure: `src/melee/gm/types.h:293` (struct gmm_x1868)
- Name tag data: `src/melee/gm/types.h` (struct NameTagDataBank)
