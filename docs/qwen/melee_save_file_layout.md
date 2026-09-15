# Melee Save File Layout - Actual Usage

## Overview

The `lb_803BAB74` array in `lbcardgame.c` defines the **actual** layout of Melee's save file. This is a `CardEntry` array with 10 elements (9 data slots + 1 terminator).

## The CardEntry Structure

```c
struct CardEntry {
    int file_size;   // Size in bytes (or -1 for terminator)
    int file_flags;  // File flags (0-3)
    u8* data;        // Pointer to data buffer (NULL in manifest)
};
```

## Actual Slot Usage in Melee

Looking at `lbcardgame.c:lb_803BAB74`:

| Slot | File Size | File Flags | Data Source | Purpose |
|------|-----------|------------|-------------|---------|
| 0 | 0 bytes | 3 | `gmMainLib_GetSaveData()` | **Main save data header/metadata** (empty file, just metadata) |
| 1 | 0x1790 (6032) | 0 | `gmMainLib_GetSaveData()` | **Primary game save data** - unlocks, match stats, trophies, fighter data, settings |
| 2 | 0x1F2C (7980) | 1 | `gmMainLib_8015CC4C()[0..6]` | **Replay data** - 6 replay buffers |
| 3 | 0x1F2C (7980) | 1 | NULL | **Custom stage slot 1** |
| 4 | 0x1F2C (7980) | 1 | NULL | **Custom stage slot 2** |
| 5 | 0x1F2C (7980) | 1 | NULL | **Custom stage slot 3** |
| 6 | 0x1F2C (7980) | 1 | NULL | **Custom stage slot 4** |
| 7 | 0x1F2C (7980) | 1 | NULL | **Custom stage slot 5** |
| 8 | 0x1F2C (7980) | 1 | NULL | **Custom stage slot 6** |
| 9 | -1 | 0 | NULL | **Terminator** (marks end of array) |

## Key Insights

### File Flags Meaning
- **Flag 0**: Standard file (main save data)
- **Flag 1**: Replay/stage data (different redundancy handling)
- **Flag 3**: Metadata-only file (Slot 0 - no actual data, just CARD file metadata)

### Data Breakdown

#### Slot 1 - Main Save Data (0x1790 bytes)
Contains the entire `struct gmm_x1868`:
- Character unlocks (bitmask)
- Stage unlocks (bitmask)  
- Feature unlocks (score display, random stage, etc.)
- Match statistics (time, stock, coin, bonus, stamina matches)
- Game preferences (`struct GamePrefs`)
- Trophy data (count, category flags, per-trophy flags for 290 trophies)
- Fighter persistent data (`struct FighterData[25]` - one per character)
- Name tag data (`struct NameTagDataBank[2]`)

#### Slots 2-8 - Replay/Stage Data (0x1F2C bytes each)
- **Slot 2**: 6 replay buffers (accessed via `gmMainLib_8015CC4C()`)
- **Slots 3-8**: 6 custom stage slots

### Why This Design?

1. **Atomic Operations**: All 9 logical files are read/written together as one physical CARD file
2. **Separation of Concerns**: 
   - Slot 0 handles CARD file metadata separately from game data
   - Slot 1 contains critical progression data
   - Slots 2-8 contain user-generated content (replays, stages)
3. **File Flags**: Different redundancy/write strategies for different data types
4. **Efficiency**: One physical file on the memory card instead of multiple files

## How It Works

When saving:
1. `lb_8001BC18()` is called with the `lb_803BAB74` manifest
2. `lb_8001A4CC()` sets up the file entries
3. `setupCardEntries()` iterates through all non-terminator entries
4. Each entry's data is written to its logical file slot within the physical CARD file
5. All slots are written atomically as one operation

When loading:
1. Same process in reverse
2. Each slot's data is loaded into its corresponding buffer
3. Error status tracked per-slot independently

## Total Save Size

- **Slot 0**: 0 bytes (metadata only)
- **Slot 1**: 6,032 bytes (main save)
- **Slots 2-8**: 7,980 bytes × 7 = 55,860 bytes (replays + stages)
- **Total**: ~62 KB of logical data

This fits within the hsd_3A94 layer's wear-leveling system which manages physical blocks on the memory card.
