# lbcardnew Public Functions Reference

This document lists all public functions in `melee/lb/lbcardnew.c` with descriptions of their observed/presumed behavior based on code analysis.

## Error Conversion & Initialization

| Function | Description |
|----------|-------------|
| `lb_80019BB8(int card_result)` | Converts Dolphin CARD library result codes to lbcardnew error codes (0-15 range). Handles negative error codes from CARD operations. |
| `lb_80019CB0(int result)` | Main task dispatcher. Processes queued card tasks in order, executing each task's operation based on its type. Returns 11 (result_pending_ops) if async operations are still in progress. |
| `lb_80019EF0(int chan, UNK_T save_data, UNK_T status_out, UNK_T callback)` | Initializes the global card state structure for a given channel. Sets up callbacks, clears error states, and resets the task queue. |
| `fn_8001A0B0(int file_idx, int hsd_error)` | Callback handler for per-file HSD card operations. Converts HSD errors and updates per-file error tracking in the global state. |
| `lbCardNew_AllocWorkArea(void)` | Allocates work area buffers (0xA000 bytes) and lib area buffer (0x2000 bytes) for card operations if not already allocated. |
| `lb_8001C5A4(void)` | Clears work area pointers (does not free memory). |
| `lb_8001C5BC(void)` | Resets HSD card state and reinitializes the card system for channel 0. |

## Card Mounting & Checking

| Function | Description |
|----------|-------------|
| `lb_8001A184(void)` | Probes the memory card and initiates async mount operation. Updates pending_ops counter and saves probe/mount errors. |
| `lb_8001A3A4(void)` | Initiates async card check operation (CARDCheckAsync). Used to verify card integrity after mounting. |
| `lb_8001C404(int chan)` | Simple wrapper around CARDProbeEx. Returns converted error code without mounting. |

## File Operations - Setup

| Function | Description |
|----------|-------------|
| `lb_8001A4CC(const char* filename, void* file_entries)` | Queues an OPEN_FILE task. Stores filename pointer and file entries array for later processing by lb_8001A594. |
| `lb_8001A594(char* filename, void* file_entries)` | Opens/creates a card file. Checks sector size, gets free space, initializes HSD card state, and either opens existing file or prepares for new file creation with 9 logical sub-files. |
| `lb_8001A860(void)` | Normalizes saved_error values: converts 0 and 2 to 1. Purpose unclear from context (possibly legacy result code mapping). |

## File Operations - CRUD

| Function | Description |
|----------|-------------|
| `lb_8001A8A4(void)` | Formats the memory card asynchronously if the card is mounted (unk_80 flag set). |
| `lb_8001A9CC(char* filename)` | Deletes a file asynchronously from the memory card. |
| `lb_8001AAE4(const char* old_name, const char* new_name)` | Renames a file asynchronously on the memory card. |
| `lb_8001AC04(const char* filename)` | Creates a new file with comment/banner/icons metadata using HSD card API. |

## File Operations - Read/Write

| Function | Description |
|----------|-------------|
| `lb_8001ACEC(UNK_T file_entries)` | Reads all 9 logical files from an opened card file. Iterates through file_sizes array and issues async read requests for non-zero sized files. |
| `lb_8001AE38(UNK_T file_entries)` | Writes all 9 logical files to an opened card file. Iterates through file_sizes array and issues async write requests for non-zero sized files. |
| `lb_8001AF84(void)` | Updates the card file header (comment, banner, icons) using HSD card API. Called after writing file data. |
| `lb_8001B068(void)` | Reads the card file header (comment, banner, icons) using HSD card API. Called before reading file data. |

## Snapshot & Status Operations

| Function | Description |
|----------|-------------|
| `lb_8001B14C(void)` | Lists all snapshot files on the card. Scans all 127 file slots, filters by company/gameName, parses numeric filenames as timestamps, sorts by time, and populates snapshot_entries array. Also updates free_blocks and free_files counts. |
| `lb_8001BFD8(int chan, lbCardNew_SnapshotEntry* snapshot_entries, int* free_blocks, int* free_files)` | Public wrapper for listing snapshots. Sets up task queue to mount, check, and list snapshots with free space info. |

## File Lookup

| Function | Description |
|----------|-------------|
| `lb_8001B614(const char* filename)` | Searches for a specific file by name across all 127 file slots. Returns error 0xD (file not found) if not found after checking all slots. |
| `lb_8001B6E0(s32 file_idx)` | Returns the card_error value for a specific file index (0-8) from the per-file error tracking array. |
| `lb_8001B6F8(void)` | Polls for card operation completion. Calls HSD's poll function and checks pending_ops. If complete, runs lb_80019CB0 to process results. |
| `lb_8001B760(int result)` | Blocking wait loop. If result is 11 (pending), repeatedly calls lb_8001B6F8 until operations complete. |

## High-Level Convenience Functions

| Function | Description |
|----------|-------------|
| `lb_8001B7E0(int chan, char* filename, void* file_entries, void* save_data, int* status_out)` | Complete load operation: mounts card, checks it, opens file, normalizes result. Synchronous wrapper that blocks until completion. |
| `lb_8001B8C8(int chan)` | Complete format operation: mounts card, checks it, formats. Synchronous wrapper that blocks until completion. |
| `lb_8001B99C(int chan, const char* filename, UNK_T status_out)` | Delete file (non-blocking variant). Sets up task queue but returns immediately without waiting for completion. |
| `lb_8001BA44(int chan, const char* filename, UNK_T status_out)` | Delete file (blocking variant). Sets up task queue and waits for completion using lb_8001B6F8 loop. |
| `lb_8001BB48(int chan, char* filename, void* file_entries, void* save_data, char* comment, void* banner, void* icons, UNK_T status_out)` | Create file with metadata (non-blocking). Sets up mount/check/create task chain. |
| `lb_8001BC18(int chan, char* filename, void** file_entries, void* save_data, char* comment, void* banner, void* icons, UNK_T status_out)` | Create file with metadata (blocking). Same as BB48 but waits for completion. |
| `lb_8001BD34(int chan, const char* filename, UNK_T file_entries, UNK_T status_out)` | Open and read file (blocking). Mounts, checks, opens file, reads all 9 logical files. |
| `lb_8001BE30(int chan, const char* filename, UNK_T file_entries, char* comment, void* banner, void* icons, UNK_T status_out, UNK_T callback)` | Update file metadata and write (callback variant). Sets up task chain with custom completion callback. |
| `lb_8001BF04(int chan, char* filename, void* file_entries, char* comment, void* banner, void* icons, UNK_T status_out)` | Read file header then read file data (blocking). Used for loading save files with metadata. |
| `lb_8001C0F4(int chan, const char* name_a, const char* name_b, const char* name_c, UNK_T status_out)` | Complex rename operation. Performs triple rename swap (a→c, b→a, c→b) likely for atomic backup/restore operations. |
| `lb_8001C2D8(int chan, const char* company, const char* game_name, const char* filename)` | Find file by company/gameName/filename triplet. Sets up FIND_FILE task to search card. |
| `lb_8001C4A8(void* file_entries, void* icon_data)` | Configures HSD card state with icon info and file entry parameters. Returns block count needed for the configured files. |

## Task Types (Internal Enum)

The following task types are queued and processed by `lb_80019CB0`:

| Value | Task Type | Description |
|-------|-----------|-------------|
| 0x00 | CARD_TASK_MOUNT_CARD | Mount the memory card |
| 0x01 | CARD_TASK_CHECK_CARD | Check card integrity |
| 0x02 | CARD_TASK_OPEN_FILE | Open/create named file |
| 0x03 | CARD_TASK_UNK_0x03 | Result normalization (0,2→1) |
| 0x04 | CARD_TASK_FORMAT_CARD | Format the card |
| 0x05 | CARD_TASK_DELETE_FILE | Delete specified file |
| 0x06 | CARD_TASK_RENAME_FILE | Rename file |
| 0x07 | CARD_TASK_CREATE_FILE | Create new file with metadata |
| 0x08 | CARD_TASK_READ_FILES | Read 9 logical files |
| 0x09 | CARD_TASK_WRITE_FILES | Write 9 logical files |
| 0x0A | CARD_TASK_SET_STATUS | Update header metadata |
| 0x0B | CARD_TASK_READ_HEADER | Read header metadata |
| 0x0C | CARD_TASK_LIST_SNAPSHOTS | List all snapshot files |
| 0x0D | CARD_TASK_FIND_FILE | Search for file by criteria |
| 0x0E | CARD_TASK_NONE | Empty/task slot available |

## Key Structures

### CardEntry
```c
struct CardEntry {
    int file_size;   // Size of this logical file (bytes)
    int file_flags;  // Flags for this file
    u8* data;        // Pointer to file data buffer
};
```

### lbCardNew_SnapshotEntry
Used for listing save file snapshots (exact definition in header).

## Error Codes

| Code | Meaning |
|------|---------|
| 0 | Success |
| 1 | Normalized success (from 0 or 2) |
| 2 | CARD access error |
| 3 | CARD open/read/write error |
| 4 | File not found / null filename |
| 5 | Insufficient space |
| 6 | No free file slots |
| 7 | Null filename |
| 9 | CARD removed during operation |
| 10 | CARD formatting/error |
| 11 | Operations pending (async in progress) |
| 12 | Bad sector size (not 8KB) |
| 13 | File not found (search exhausted) |
| 14 | General CARD error |
| 15 | Fatal CARD error (-1, -2, -3 from CARD layer) |
