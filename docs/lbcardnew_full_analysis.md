# lbcardnew Full Function Analysis

## Overview
`lbcardnew.c` implements Layer 3 of the Melee card subsystem - the game-level save management API. It provides a task queue system that wraps the `hsd_3A94` (Layer 2) async operations into higher-level blocking and non-blocking functions for mounting, reading, writing, creating, deleting, and renaming save files.

## Global State Structure
**Symbol:** `lb_80432A68` (static global, size 0x8B0 = 2224 bytes)

### Key Fields:
- `work_area` (0x00): 40KB buffer for Dolphin CARD operations
- `lib_area` (0x04): 8KB buffer for HSD card state
- `chan` (0x08): Memory card channel (0 or 1)
- `status` (0x10): Pointer to store card status output
- `comment/banner/icons` (0x14-0x1C): Metadata pointers for save file header
- `snapshot_entries` (0x20): Output array for snapshot listing
- `free_blocks/free_files` (0x24-0x28): Output pointers for free space info
- `saved_error` (0x34): Last error code (0-15 scale)
- `unk_38[9]` (0x38): Per-slot error tracking (CardEntry results)
- `memsize/sectorsize` (0x84-0x88): Card capacity info from probe
- `unused_bytes/unused_files` (0x8C-0x90): Free space from CARDFreeBlocks
- `file_info` (0x94): CARDFileInfo struct for opened file handle
- `card_state` (0xA8): HSD_CardState struct (~1100 bytes) for Layer 2 ops
- `x50C` (0x50C): Completion callback function pointer
- `task_array[14]` (0x510): Ring buffer of pending CardTask structs
- `pending_ops` (0x8AC): Count of async operations in flight

## CardTask Structure (0x54 bytes each)
Used for queuing operations in `task_array`:
- `type` (0x00): CardTaskType enum (0x00-0x0E)
- `result_mask` (0x04): Bitmask; bit N allows task to run after result N
- `file_entries` (0x08): Pointer to CardEntry array for read/write ops
- `filename_ptr` (0x0C): Pointer to filename string (or NULL)
- `filename` (0x10): Local copy of filename (32 bytes max + 1)
- `new_filename` (0x31): For rename operations (32 bytes + 1)

## Task Types (CardTaskType enum)
| Value | Symbol | Description |
|-------|--------|-------------|
| 0x00 | CARD_TASK_MOUNT_CARD | Mount physical card |
| 0x01 | CARD_TASK_CHECK_CARD | Check card integrity |
| 0x02 | CARD_TASK_OPEN_FILE | Open existing file, scan contents |
| 0x03 | CARD_TASK_UNK_0x03 | Normalize results (0,2→1) |
| 0x04 | CARD_TASK_FORMAT_CARD | Format the memory card |
| 0x05 | CARD_TASK_DELETE_FILE | Delete named file |
| 0x06 | CARD_TASK_RENAME_FILE | Rename file |
| 0x07 | CARD_TASK_CREATE_FILE | Create new file with metadata |
| 0x08 | CARD_TASK_READ_FILES | Read all 9 logical files |
| 0x09 | CARD_TASK_WRITE_FILES | Write all 9 logical files |
| 0x0A | CARD_TASK_SET_STATUS | Update header/icons/status |
| 0x0B | CARD_TASK_READ_HEADER | Read header/icons only |
| 0x0C | CARD_TASK_LIST_SNAPSHOTS | Enumerate save snapshots |
| 0x0D | CARD_TASK_FIND_FILE | Search for file by name |
| 0x0E | CARD_TASK_NONE | Empty/task slot available |

## Error Codes
### Dolphin/HSD → Game Conversion (`lb_80019BB8`)
| Input | Output | Meaning |
|-------|--------|---------|
| -1, -2, -3 | 0xF | Fatal hardware error |
| -4 | 4 | File not found |
| 0 | 0 | Success |
| -5, -128 | 0xE | Device busy/no card |
| -6, -13 | 9 | Wrong device/format |
| 1, -14 | 0xD | Other error |

### HSD-Specific Conversion (`convert_hsdcard_error`)
| HSD Error | Game Error |
|-----------|------------|
| 0, 1 | 0 (OK) |
| -0x101 to -0x105 | 2 (Corrupted data?) |
| -0x106, -0x107 | 3 (Bad checksum?) |
| -0x108 to -0x10B | 10 (Write protected?) |
| Other | Pass through `lb_80019BB8` |

### Direct Error Constants
- `error_ok = 0`
- `error_null_filename = 7`
- `error_bad_sector_size = 12`
- `result_pending_ops = 11` (async operation in progress)

---

## Function-by-Function Analysis

### Initialization Functions

#### `lbCardNew_AllocWorkArea()` (01C550)
**Purpose:** Allocate static work buffers if not already allocated.
**Action:** 
- Allocates 0xA000 (40KB) for `work_area` (Dolphin CARD)
- Allocates 0x2000 (8KB) for `lib_area` (HSD state)
**Usage:** Call once at game startup before any card operations.

#### `lb_8001C5A4()` (01C5A4)
**Purpose:** Clear work area pointers (cleanup).
**Action:** Sets `work_area` and `lib_area` to NULL.

#### `lb_8001C5BC()` (01C5BC)
**Purpose:** Full reset of card subsystem.
**Action:**
- Calls `hsd_803B2374()` (HSD cleanup)
- Calls `lb_80019EF0(0, NULL, NULL, NULL)` to reset state
- Clears `pending_ops`
**Usage:** Called when quitting to menu or changing game mode.

#### `lb_80019EF0(int chan, UNK_T save_data, UNK_T status_out, UNK_T callback)` (019EF0)
**Purpose:** Initialize global state for a new card session.
**Parameters:**
- `chan`: Memory card channel (0 or 1)
- `save_data`: Pointer to icon data (stored in `unk_C`)
- `status_out`: Pointer to store card status result
- `callback`: Function called on task completion (stored in `x50C`)
**Action:**
- Stores parameters in global state
- Sets `saved_error = 16` (initial "no error yet" state)
- Initializes all 9 `unk_38` entries: `card_error=16`, `unk_4=-1`
- Clears `unk_80` (mounted flag)
- Stores callback
- Resets entire `task_array` to `CARD_TASK_NONE`
**Usage:** First step of every high-level card operation.

---

### Core Engine Functions

#### `lb_80019C38()` (019C38)
**Purpose:** Find next available task slot.
**Returns:** Pointer to first `CardTask` with `type == CARD_TASK_NONE`.
**Logic:** Linear scan of `task_array[0..13]`; asserts if all slots full.
**Usage:** Internal helper for queuing new tasks.

#### `reset_task_array()` (static)
**Purpose:** Clear all pending tasks (used on error recovery).
**Action:** Sets all 14 task slots to `CARD_TASK_NONE`.

#### `lb_80019CB0(int result)` (019CB0) - **THE MAIN STATE MACHINE**
**Purpose:** Process task queue until completion or async wait.
**Parameter:** `result` - Previous task result code (bitmask index)
**Returns:** Final result code, or `result_pending_ops` (11) if async.

**Algorithm:**
```
loop forever:
    find first non-NONE task in task_array
    if no task found: break
    
    if (task->result_mask & (1 << result)) == 0:
        reset_task_array()  // dependency failed
        break
    
    switch task->type:
        CARD_TASK_MOUNT_CARD:   result = lb_8001A184()
        CARD_TASK_CHECK_CARD:   result = lb_8001A3A4()
        CARD_TASK_OPEN_FILE:    result = lb_8001A594(filename, entries)
        CARD_TASK_UNK_0x03:     result = lb_8001A860()
        CARD_TASK_FORMAT_CARD:  result = lb_8001A8A4()
        CARD_TASK_DELETE_FILE:  result = lb_8001A9CC(filename)
        CARD_TASK_RENAME_FILE:  result = lb_8001AAE4(old, new)
        CARD_TASK_CREATE_FILE:  result = lb_8001AC04(filename)
        CARD_TASK_READ_FILES:   result = lb_8001ACEC(entries)
        CARD_TASK_WRITE_FILES:  result = lb_8001AE38(entries)
        CARD_TASK_SET_STATUS:   result = lb_8001AF84()
        CARD_TASK_READ_HEADER:  result = lb_8001B068()
        CARD_TASK_LIST_SNAPSHOTS: result = lb_8001B14C()
        CARD_TASK_FIND_FILE:    result = lb_8001B614(filename)
    
    task->type = CARD_TASK_NONE  // mark complete
    
    if result != result_pending_ops:
        continue  // process next task immediately
    break  // wait for async completion

if result != pending_ops AND x50C callback exists:
    call x50C(result)
    x50C = NULL

if result != pending_ops AND unk_80 (mounted) is true:
    CARDUnmount(chan)
    unk_80 = 0

return result
```

**Key Insights:**
- Tasks execute sequentially in a single loop iteration if synchronous
- If a task returns `result_pending_ops` (11), the loop breaks and waits for callback
- `result_mask` enforces dependencies: task only runs if previous result bit is set
- On final completion, calls user callback and unmounts card
- This is the heart of the "blocking wrapper" pattern

#### `lb_8001B6F8()` (01B6F8)
**Purpose:** Poll async operations and pump state machine.
**Returns:** Final result or `result_pending_ops`.
**Action:**
1. Calls `hsd_803AAA48()` (HSD update function - processes Layer 2 queue)
2. Disables interrupts
3. If `pending_ops != 0`: return `result_pending_ops`
4. Else: get `saved_error`
5. Re-enable interrupts
6. If not pending: call `lb_80019CB0(result)` to continue state machine
**Usage:** Called in a while-loop by blocking functions to wait for async ops.

#### `lb_8001B760(int result)` (01B760)
**Purpose:** Blocking wait helper.
**Logic:**
```c
if (result == 11) {  // if pending
    while ((result = lb_8001B6F8()) == 11) {}  // poll until done
}
return result;
```
**Usage:** Wraps the polling loop for callers who get `result_pending_ops`.

---

### Mounting & Checking

#### `lb_8001A184()` (01A184) - Mount Card Task
**Purpose:** Probe and mount physical memory card.
**Returns:** 0 (success), error code, or `result_pending_ops`.

**Steps:**
1. Clear `pending_ops = 0`
2. Call `CARDProbeEx(chan, &memsize, &sectorsize)`
3. Convert result via `lb_80019BB8()` → store in `saved_error`
4. If success:
   - Set `*status = 0` if status pointer exists
   - Assert `work_area` is allocated
   - Disable interrupts
   - Call `CARDMountAsync(chan, work_area, NULL, onCardComplete)`
   - If mount_result is 0, -6, or -0xD: set `unk_80 = true` (mounted flag)
   - If `saved_error == 0`: increment `pending_ops`
5. Restore interrupts
6. If `pending_ops > 0`: return `result_pending_ops`
7. Else: return `saved_error`

**Callback:** `onCardComplete(chan, result)` decrements `pending_ops` and updates `saved_error`.

#### `lb_8001A3A4()` (01A3A4) - Check Card Task
**Purpose:** Verify card integrity after mount.
**Returns:** 0 (success), error code, or `result_pending_ops`.

**Steps:**
1. Clear `pending_ops = 0`
2. Disable interrupts
3. Call `CARDCheckAsync(chan, onCardComplete)`
4. Convert result → `saved_error`
5. If success: increment `pending_ops`
6. Restore interrupts
7. Return `result_pending_ops` or `saved_error`

**Note:** Uses same `onCardComplete` callback as mount.

---

### File Operations

#### `lb_8001A4CC(const char* filename, void* file_entries)` (01A4CC)
**Purpose:** Queue OPEN_FILE task.
**Action:**
- Get next task slot
- Set `type = CARD_TASK_OPEN_FILE`
- Set `result_mask = 1` (runs after result 0, i.e., after successful check)
- Copy `filename` to task's local buffer, set `filename_ptr`
- Store `file_entries` pointer
**Usage:** Always followed by `CARD_TASK_UNK_0x03` in high-level wrappers.

#### `lb_8001A594(char* filename, void* file_entries)` (01A594) - Open File Task
**Purpose:** Open existing save file and initialize HSD state.
**Returns:** 0 (found), 4 (not found), 5 (no space), 6 (no file slots), or error.

**Steps:**
1. Clear `pending_ops = 0`
2. Assert `sectorsize == 0x2000` (8KB); else error 12
3. Call `CARDFreeBlocks(chan, &unused_bytes, &unused_files)`
4. If `filename == NULL`: error 7
5. Try `CARDOpen(chan, filename, &file_info)` then `CARDClose()`
   - This just checks existence
6. Initialize HSD state: `hsd_803B24E4(&card_state, chan, 0x2000, lib_area)`
7. If file exists (`open_result == 0`):
   - Call `hsd_803B2550(&card_state, filename, fn_8001A0B0)` to open/scans
   - Convert HSD error → `saved_error`
   - If success: `pending_ops++`
8. Else if `file_entries == NULL`: error 4 (not found, can't create)
9. Else if `unused_files == 0`: error 6 (no file slots)
10. Else (file doesn't exist but we have entries):
    - Call `setupCardEntries(&card_state, unk_C, file_entries)` to configure 9 slots
    - Check space: if `unused_bytes < (required_sectors << 13)`: error 5 (no space)
    - Else: error 4 (will create later?)
11. Return `result_pending_ops` or `saved_error`

**Helper:** `setupCardEntries(ctx, icon, entry_array)` loops through CardEntry[] and calls `hsd_SetCardIconInfo()` + `hsd_803AC3E0()` for each non-zero slot.

#### `lb_8001A860()` (01A860) - Result Normalization Task
**Purpose:** Normalize open-file results.
**Logic:**
```c
pending_ops = 0;
switch (saved_error) {
    case 1: break;       // keep as-is
    case 0:              // success → treat as "needs creation"
    case 2:              // some error → also "needs creation"
        saved_error = 1;
        break;
}
return saved_error;
```
**Observation:** Maps 0 and 2 to 1. Purpose unclear; possibly indicates "file ready for create/write".

#### `lb_8001AC04(const char* filename)` (01AC04) - Create File Task
**Purpose:** Create new save file with metadata.
**Returns:** HSD error converted to game error, or `result_pending_ops`.

**Steps:**
1. Call `hsd_803B286C(&card_state, filename, comment, banner, icons, fn_8001A0B0)`
   - This creates the physical CARD file with header info
2. Convert HSD error → `saved_error`
3. If success: `pending_ops++`
4. Return `result_pending_ops` or `saved_error`

**Callback:** `fn_8001A0B0(file_idx, hsd_error)` stores per-slot error and decrements `pending_ops`.

#### `lb_8001A9CC(char* filename)` (01A9CC) - Delete File Task
**Purpose:** Delete save file.
**Steps:**
1. Clear `pending_ops`
2. Disable interrupts
3. Call `CARDDeleteAsync(chan, filename, onCardComplete)`
4. Convert result → `saved_error`
5. If success: `pending_ops++`
6. Restore interrupts
7. Return `result_pending_ops` or `saved_error`

#### `lb_8001AAE4(const char* old_name, const char* new_name)` (01AAE4) - Rename File Task
**Purpose:** Rename save file.
**Steps:** Same pattern as delete, but calls `CARDRenameAsync()`.

#### `lb_8001A8A4()` (01A8A4) - Format Card Task
**Purpose:** Format entire memory card.
**Precondition:** `unk_80` (mounted flag) must be true.
**Steps:**
1. Clear `pending_ops`
2. If mounted: disable interrupts, call `CARDFormatAsync(chan, onCardComplete)`
3. Convert result, increment `pending_ops` if success
4. Restore interrupts
5. Return `result_pending_ops` or `saved_error`

---

### Read/Write Operations

#### `lb_8001ACEC(void* file_entries)` (01ACEC) - Read Files Task
**Purpose:** Read all 9 logical files from opened save.
**Parameter:** `file_entries` - Array of 9 `CardEntry` structs with destination buffers.

**Steps:**
1. Set `saved_error = 0`
2. Loop `i = 0..8`:
   - Cache `file_sizes[i]` and `file_flags[i]` from `card_state`
   - If `file_sizes[i] != 0`:
     - Call `hsd_803B29D8(&card_state, i, entries[i].data, fn_8001A0B0)`
     - Store HSD error in `unk_38[i]`
     - Convert to game error
     - If success: `pending_ops++`
     - Else: update `saved_error`
3. Return `result_pending_ops` or `saved_error`

**Note:** Reads ALL non-zero slots in parallel (async). Callback tracks completion.

#### `lb_8001AE38(void* file_entries)` (01AE38) - Write Files Task
**Purpose:** Write all 9 logical files to opened save.
**Pattern:** Identical to read, but calls `hsd_803B2A4C()` (write instead of read).

#### `lb_8001AF84()` (01AF84) - Set Status Task
**Purpose:** Update file header (comment, banner, icons) and CARD status.
**Steps:**
1. Call `hsd_803B2928(&card_state, comment, banner, icons, fn_8001A0B0)`
2. Convert error, increment `pending_ops` if success
3. Return `result_pending_ops` or `saved_error`

#### `lb_8001B068()` (01B068) - Read Header Task
**Purpose:** Read file header (comment, banner, icons) without reading data slots.
**Steps:** Same as Set Status, but calls `hsd_803B27F4()` (read header).

---

### Snapshot & Utility Functions

#### `lb_8001B14C()` (01B14C) - List Snapshots Task
**Purpose:** Enumerate all save files on card matching current game.
**Output:** Populates `snapshot_entries` array (up to 127 entries), sorted by timestamp descending.

**Algorithm:**
1. Get current disk ID for company/gameName matching
2. Set `*free_blocks = unused_bytes / 0x2000`
3. Set `*free_files = unused_files`
4. Allocate temporary node array (127 entries)
5. Loop `file_no = 0..0x7E` (126 max files):
   - Call `CARDGetStatus(chan, file_no, &stat)`
   - Check: company matches (2 chars), gameName matches (4 chars), filename starts with digit
   - Parse filename as timestamp (`strtoul`)
   - Insert into sorted linked list (descending by time)
6. Copy sorted list to `snapshot_entries[]`
7. Fill remaining slots with `file_no = -1`
8. Free temp array
9. Return 0 (always succeeds, just enumeration)

**Structure:** `lbCardNew_SnapshotEntry` contains `{ time, file_no, blocks }`.

#### `lb_8001B614(const char* filename)` (01B614) - Find File Task
**Purpose:** Check if specific file exists on card.
**Returns:** 0 (found) or 0xD (not found).

**Algorithm:**
1. Loop `fileno = 0..0x7E`:
   - Call `CARDGetStatus(chan, fileno, &card_stat)`
   - Check: company matches `x2C` (2 chars), gameName matches `x2F` (4 chars), filename exact match
   - If found: `saved_error = 0`, break
2. If loop completes: `saved_error = 0xD`
3. Return `saved_error`

**Note:** Synchronous; no async ops.

#### `lb_8001B6E0(s32 file_idx)` (01B6E0)
**Purpose:** Get error code for specific logical file slot.
**Returns:** `unk_38[file_idx].card_error`
**Usage:** After read/write, check per-slot errors.

#### `lb_8001C404(int chan)` (01C404)
**Purpose:** Quick probe without mounting.
**Returns:** Result of `CARDProbeEx()` converted via `lb_80019BB8()`.

#### `lb_8001C4A8(void* file_entries, void* icon_data)` (01C4A8)
**Purpose:** Setup CardEntry array and compute required sectors.
**Returns:** Number of 8KB sectors needed for all files.

**Steps:**
1. Initialize `card_state` with `hsd_803B24E4()`
2. Call `hsd_SetCardIconInfo()` with icon_data
3. Loop through `CardEntry[]` until `file_size == -1`:
   - If `file_size != 0`: call `hsd_803AC3E0()` to register slot
4. Return `hsd_803B2674(&card_state)` (sector count)

**Usage:** Pre-calculate space needed before creating file.

---

## High-Level Wrapper Functions

These functions combine multiple tasks into common workflows. All follow pattern:
1. Call `lb_80019EF0()` to init state
2. Queue tasks with `setup_task()` or direct assignment
3. Call `lb_80019CB0(0x10)` to start
4. If pending: poll with `lb_8001B6F8()` until done

### `lb_8001B7E0()` (01B7E0) - Load Game (Blocking)
**Purpose:** Mount, check, open existing save, and normalize.
**Params:** chan, filename, file_entries, save_data, status_out
**Task Chain:**
1. MOUNT_CARD (mask 0x10000)
2. CHECK_CARD (mask 0x201)
3. OPEN_FILE (via `lb_8001A4CC`)
4. UNK_0x03 (mask -1)
**Returns:** Final result after blocking wait.

### `lb_8001B8C8(int chan)` (01B8C8) - Format Card (Blocking)
**Task Chain:** MOUNT → CHECK → FORMAT
**Returns:** 0 (success) or error.

### `lb_8001B99C()` / `lb_8001BA44()` (01B99C/01BA44) - Delete File
**Difference:** `lb_8001B99C` doesn't block; `lb_8001BA44` blocks.
**Task Chain:** MOUNT → CHECK → OPEN (dummy) → UNK_0x03 → DELETE_FILE

### `lb_8001BB48()` / `lb_8001BC18()` (01BB48/01BC18) - Create New Save
**Params:** chan, filename, file_entries, save_data, comment, banner, icons, status
**Task Chain:** MOUNT → CHECK → OPEN → UNK_0x03 → CREATE_FILE
**Difference:** `lb_8001BB48` doesn't block; `lb_8001BC18` blocks.

### `lb_8001BD34()` (01BD34) - Read Save (Blocking)
**Task Chain:** MOUNT → CHECK → OPEN → UNK_0x03 → READ_FILES
**Usage:** Load existing save data into buffers.

### `lb_8001BE30()` (01BE30) - Write Save with Callback (Non-blocking)
**Task Chain:** MOUNT → CHECK → OPEN → UNK_0x03 → SET_STATUS → WRITE_FILES
**Special:** Accepts callback parameter stored in `x50C`.

### `lb_8001BF04()` (01BF04) - Read Save with Header (Blocking)
**Task Chain:** MOUNT → CHECK → OPEN → UNK_0x03 → READ_HEADER → READ_FILES
**Usage:** Load save with metadata (comment/banner/icons).

### `lb_8001BFD8()` (01BFD8) - List Saves (Blocking)
**Params:** chan, snapshot_entries[], free_blocks*, free_files*
**Task Chain:** MOUNT → CHECK → OPEN (dummy) → UNK_0x03 → LIST_SNAPSHOTS
**Usage:** Show save file selection menu.

### `lb_8001C0F4()` (01C0F4) - Rotate Three Filenames
**Purpose:** Atomic rename cycle: A→C, B→A, C→B (backup rotation?).
**Task Chain:** MOUNT → CHECK → OPEN(A) → UNK_0x03 → RENAME(A→C) → OPEN(B) → UNK_0x03 → RENAME(B→A) → OPEN(C) → UNK_0x03 → RENAME(C→B)
**Complex:** 10+ tasks queued sequentially.

### `lb_8001C2D8()` (01C2D8) - Find File by Name
**Params:** chan, company, game_name, filename
**Task Chain:** MOUNT → CHECK → OPEN (dummy) → UNK_0x03 → FIND_FILE
**Usage:** Check if specific save exists.

---

## Key Patterns & Insights

### 1. Blocking Wrapper Pattern
High-level functions hide async complexity:
```c
result = lb_80019CB0(0x10);
if (result == 11) {
    while ((result = lb_8001B6F8()) == 11) {}
}
return result;
```
This turns async operations into synchronous calls for simple use cases.

### 2. Task Dependency System
`result_mask` controls execution order:
- Mask `0x10000` (bit 16): Runs after initial setup
- Mask `0x201` (bits 0,1,9): Runs after results 0, 1, or 9
- Mask `-1` (all bits): Always runs
- Mask `3` (bits 0,1): Runs after results 0 or 1

### 3. Per-Slot Error Tracking
`unk_38[9]` array stores individual errors for each logical file:
- `card_error`: Game-scale error (0-15)
- `unk_4`: Raw HSD error code
Allows partial success detection (e.g., 8/9 files read successfully).

### 4. Sector Size Assertion
All functions assert `sectorsize == 0x2000` (8KB). Melee assumes standard GameCube memory card format.

### 5. Filename Handling
- Max length: `CARD_FILENAME_MAX` (typically 32)
- Stored in task struct with extra byte for null termination
- Company (2 chars) + GameName (4 chars) used for filtering saves

### 6. Interrupt Safety
Functions that call Dolphin CARD async APIs wrap with `OSDisableInterrupts()` / `OSRestoreInterrupts()` to prevent race conditions on shared state.

### 7. Memory Management
- Static global state (`lb_80432A68`) avoids dynamic allocation during ops
- Work areas allocated once at startup
- Temporary allocations (e.g., snapshot nodes) freed before return

---

## Typical Usage Flow

### Loading a Save:
```c
// 1. Allocate work areas once at startup
lbCardNew_AllocWorkArea();

// 2. Call blocking load function
CardEntry entries[9];
// ... initialize entries with buffers ...
int result = lb_8001BD34(chan, "SuperSmashBros0110290334", entries, &status);

if (result == 0) {
    // Success: data loaded into entries[].data
} else {
    // Handle error (4 = not found, etc.)
}
```

### Creating a New Save:
```c
CardEntry entries[9];
// ... configure entries with sizes and data ...
char* comment = "Melee Save";
void* banner = ...;
void* icons = ...;

int result = lb_8001BC18(chan, "SuperSmashBros0110290334", entries, 
                         save_data, comment, banner, icons, &status);
```

### Listing Saves (Menu):
```c
lbCardNew_SnapshotEntry snapshots[127];
int free_blocks, free_files;

int result = lb_8001BFD8(chan, snapshots, &free_blocks, &free_files);

for (int i = 0; i < 127 && snapshots[i].file_no != -1; i++) {
    printf("Save at %u: file #%d, %d blocks\n", 
           snapshots[i].time, snapshots[i].file_no, snapshots[i].blocks);
}
```

---

## Unresolved Mysteries

1. **CARD_TASK_UNK_0x03**: Why map results 0 and 2 to 1? Possibly legacy behavior or specific to Melee's create-if-not-exists logic.

2. **result_mask values**: Some masks like `0x10000` (bit 16) don't correspond to any defined result code. May be internal flags.

3. **file_entries type casting**: Code casts `void*` to `CardEntry*` but structure alignment seems off in some places (marked with `@todo this seems fake`).

4. **unk_80 flag**: Used to track mount state, but cleared on every completion. Unclear why unmount happens in `lb_80019CB0` rather than explicit call.

5. **Error 16**: Initial `saved_error` value, never defined in enum. Possibly "no operation performed yet" sentinel.
