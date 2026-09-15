# The 9 Logical File Slots in Melee's Save System

## Overview

The "9 save file slots" are **NOT** 9 separate memory card files visible to the user. Instead, they represent **9 logical sub-files packed into a single physical CARD file** on the memory card.

This is a critical distinction for understanding Melee's save architecture and designing your new format.

## Architecture Layers

```
User sees:                    Actual structure:
┌─────────────────┐          ┌─────────────────────────────┐
│ Memory Card     │          │ Physical CARD file          │
│ (visible in GC) │          │ (e.g., "melee_save.dat")    │
│                 │          │                             │
│ ┌─────────────┐ │          │ ┌─────────────────────────┐ │
│ │ melee_save  │ │ ← One    │ │ Header (comment+banner) │ │
│ │   .dat      │ │   file   │ │ + icons (0x30 bytes)    │ │
│ └─────────────┘ │          │ ├─────────────────────────┤ │
│                 │          │ │ Logical File 0          │ │ ← CardEntry[0]
│                 │          │ │ (size, flags, data ptr) │ │
│                 │          │ ├─────────────────────────┤ │
│                 │          │ │ Logical File 1          │ │ ← CardEntry[1]
│                 │          │ │ (size, flags, data ptr) │ │
│                 │          │ ├─────────────────────────┤ │
│                 │          │ │ ...                     │ │
│                 │          │ ├─────────────────────────┤ │
│                 │          │ │ Logical File 8          │ │ ← CardEntry[8]
│                 │          │ │ (size, flags, data ptr) │ │
│                 │          │ └─────────────────────────┘ │
└─────────────────┘          └─────────────────────────────┘
```

## Key Data Structures

### CardEntry (lbcardnew layer)
```c
struct CardEntry {
    int file_size;   // Size of this logical file's data
    int file_flags;  // Flags (not boolean flags, but file type/mode)
    u8* data;        // Pointer to buffer in RAM
};
```

### CardState (hsd_3A94 layer)
```c
struct CardState {
    // ... header info ...
    int file_flags[9];   // Flags for each logical file
    int file_sizes[9];   // Sizes for each logical file  
    u8* file_data[9];    // RAM buffers for each logical file
    // ... wear leveling, block management ...
};
```

### Internal Status Tracking
```c
struct lb_80432A68_38_t {
    s32 card_error;  // Translated error code (0-15)
    s32 unk_4;       // HSD error code
};
// Array of 9: tracks status of each logical file operation
struct lb_80432A68_38_t unk_38[9];
```

## How It Works

### 1. Single Physical File
When you create a save in Melee, it creates **one** file on the memory card (e.g., `"melee_save.dat"`). This file contains:
- A 0x30-byte header (comment, banner, icons)
- Up to 9 logical files packed together
- Wear leveling metadata (block_ids, block_seqs for 64 blocks)

### 2. Logical File Management
Each of the 9 slots can hold different types of data:
- **Slot 0**: Main save data (game state, progress)
- **Slot 1**: Replay data
- **Slot 2**: Custom stage data
- **Slot 3-8**: Additional saves, backups, or unused

The game decides what goes in each slot based on `file_flags` and `file_size`.

### 3. Operations on Logical Files

When you call functions like `lb_8001ACEC` (read) or `lb_8001AE38` (write):

```c
// Pseudocode from lb_8001ACEC (read all 9 files)
for (i = 0; i < 9; i++) {
    if (entry[i].file_size != 0) {
        hsd_803B29D8(&card_state, i, entries[i].data, callback);
        // Tracks result in unk_38[i]
    }
}
```

Each logical file is read/written **independently** but as part of the same physical file operation.

### 4. Error Tracking Per Slot
The `unk_38[9]` array tracks the status of each logical file operation separately:
- `unk_38[0].card_error` - Error for logical file 0
- `unk_38[1].card_error` - Error for logical file 1
- etc.

This allows partial success: files 0-3 might succeed while 4 fails.

## Why 9 Slots?

### Advantages:
1. **Atomic operations**: All 9 logical files are saved/loaded together
2. **Wear leveling**: Single physical file means better wear distribution
3. **Space efficiency**: Shared header, no per-file overhead
4. **Flexibility**: Different data types in one save file
5. **Redundancy**: Can store backup copies in multiple slots

### Trade-offs:
1. **Fixed limit**: Maximum 9 logical files per physical save
2. **All-or-nothing**: If physical file corrupts, all 9 logical files affected
3. **Complexity**: Must manage 9 separate buffers and track 9 statuses

## Implications for Your New Format

When designing your save format, consider:

### Option A: Follow Melee's Pattern
- Use one physical file with multiple logical sections
- Good for: Related data that should be saved atomically
- Example: Game state + replays + settings in one file

### Option B: Separate Physical Files
- Each save type gets its own CARD file
- Good for: Independent data that doesn't need atomic saves
- Example: Separate files for "main save", "replays", "custom stages"

### Option C: Hybrid Approach
- Primary data in one physical file (using logical slots)
- Large/optional data in separate files
- Good for: Balancing atomicity with flexibility

## Key Functions to Study

| Function | Purpose | Logical File Usage |
|----------|---------|-------------------|
| `lb_8001ACEC` | Read all 9 logical files | Iterates 0-8 |
| `lb_8001AE38` | Write all 9 logical files | Iterates 0-8 |
| `lb_8001B6E0` | Get error for specific slot | Takes file_idx (0-8) |
| `hsd_803AC3E0` | Configure one logical file | Sets size/flags/data for slot i |
| `hsd_803B29D8` | Async read one logical file | Operates on slot i |
| `hsd_803B2A4C` | Async write one logical file | Operates on slot i |

## Example: Setting Up Logical Files

```c
// From setupCardEntries() in lbcardnew.c
void setupCardEntries(CardState* ctx, void* icon, CardEntry* entries) {
    hsd_SetCardIconInfo(ctx, icon);  // Set header
    
    int i = 0;
    while (entries[i].file_size != -1) {  // -1 terminates array
        if (entries[i].file_size != 0) {
            hsd_803AC3E0(ctx, i, 
                        entries[i].file_size,
                        entries[i].file_flags,
                        entries[i].data);
        }
        i++;
    }
}
```

This configures up to 9 logical files, then they're all written/read together.

## Summary

The "9 slots" are **logical subdivisions within a single physical memory card file**, not 9 separate saves. This design allows Melee to:
- Save multiple related data types atomically
- Efficiently use memory card space
- Track individual file status within batch operations
- Implement wear leveling across the entire save

For your new format, decide whether this logical subdivision model fits your needs, or if separate physical files would be more appropriate.
