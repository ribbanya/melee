# hsd_3A94: Main Card Library for Melee

## Overview
`hsd_3A94.c/h` is the **main card library** used by Super Smash Bros. Melee for managing memory card operations. It sits at Layer 2 of the card subsystem architecture, between the Dolphin OS CARD API (Layer 1) and the game-level `lbcardnew` system (Layer 3).

## Key Characteristics

### Architecture
- **Single Physical File**: Manages one CARD file containing up to 9 logical files (sub-files)
- **Async Command Queue**: 128-entry ring buffer for queuing card operations
- **Request Queue**: 32-entry queue for higher-level requests
- **Block Mirroring**: Uses sequence numbers and block IDs for redundancy and corruption detection

### Core Data Structures

#### CardState (0x464 bytes)
Main state structure containing:
- `sector_buf`: Working buffer for sector operations
- `chan`: CARD channel (port) number
- `sector_size`: Size of sectors (typically 8KB)
- `file_info`: Dolphin CARDFileInfo for the physical file
- `file_flags[9]`, `file_sizes[9]`, `file_data[9]`: Arrays for 9 logical files
- `block_ids[64]`, `block_seqs[64]`: Block metadata for mirroring
- `comment[64]`, `icon_info`: File metadata
- `header_size`: Total header size (comment + banner + icons + digests)

#### CardCmdType Enum (0x00-0x11)
Command types supported by the ring buffer:
- `CARD_CMD_NONE` (0x00)
- `CARD_CMD_WRITE_BLOCK` (0x01)
- `CARD_CMD_READ_BLOCK` (0x02)
- `CARD_CMD_VERIFY_BLOCK` (0x05)
- `CARD_CMD_CHECK_VERIFIED` (0x06)
- `CARD_CMD_CREATE_FILE` (0x07)
- `CARD_CMD_SET_STATUS` (0x08)
- `CARD_CMD_WRITE_HEADER` (0x09)
- `CARD_CMD_VERIFY_HEADER` (0x0A)
- `CARD_CMD_READ_HEADER` (0x0B)
- `CARD_CMD_GET_STATUS` (0x0C)
- `CARD_CMD_SCAN_BLOCK` (0x0D)
- `CARD_CMD_REPAIR` (0x0E)
- `CARD_CMD_READ_SECTOR` (0x0F)
- `CARD_CMD_WRITE_SECTOR` (0x10)
- `CARD_CMD_SCAN_FILE` (0x11)

#### CardActiveType Enum
Tracks the type of active operation:
- `CARD_ACTIVE_NONE` (0x00)
- `CARD_ACTIVE_READ_FILE` (0x01)
- `CARD_ACTIVE_WRITE_FILE` (0x02) - file_flags 0
- `CARD_ACTIVE_WRITE_FILE_1_2` (0x03) - file_flags 1 or 2
- `CARD_ACTIVE_WRITE_FILE_3` (0x04) - file_flags 3
- `CARD_ACTIVE_OPEN_OR_READ_HEADER` (0x05)
- `CARD_ACTIVE_CREATE_FILE` (0x06)
- `CARD_ACTIVE_SET_STATUS` (0x07)

## API Usage Flow

### 1. Initialize Card State
```c
hsd_803B24E4(CardState* state, int chan, int sector_size, void* work_buf);
```
Sets up the CardState structure with channel, sector size, and working buffer.

### 2. Open/Create Physical File
```c
// Open existing file
hsd_803B2550(CardState* state, const char* filename, CardCallback callback);

// Or create new file
hsd_803B286C(CardState* state, const char* filename, const char* comment, 
              void* banner, void* icons, CardCallback callback);
```

### 3. Read Header (Optional)
```c
hsd_803B27F4(CardState* state, void* comment, void* banner, void* icons, 
              CardCallback callback);
```
Queues a header read into provided buffers. Returns 0 when queued (not when complete).

### 4. Update Header/Status (Optional)
```c
hsd_803B2928(CardState* state, const char* comment, void* banner, void* icons, 
              CardCallback callback);
```

### 5. Read Logical Files
```c
hsd_803B29D8(CardState* state, int file_idx, u8* buf, CardCallback callback);
```
Queues a read of logical file `file_idx` (0-8) into buffer `buf`.

### 6. Write Logical Files
```c
hsd_803B2A4C(CardState* state, int file_idx, u8* buf, CardCallback callback);
```
Queues a write of logical file `file_idx` (0-8) from buffer `buf`.

### 7. Process Command Queue (Every Frame)
```c
hsd_803AAA48(void);
```
**Must be called every frame** to process the command ring buffer and execute pending operations. This function drives all async CARD operations to completion.

## Callback Pattern
All operations are asynchronous and use callbacks:
```c
typedef void (*CardCallback)(int file_idx, int result);
```
- `file_idx`: Which logical file completed (or 0 for header operations)
- `result`: 0 on success, negative value on error

## Return Value Convention
- Returns **0** when operation is successfully queued (NOT when complete)
- Returns **negative value** if queue is full or operation failed to queue
- A negative return means no request was queued and callback will NOT be invoked

## Key Implementation Details

### Block Structure
Each physical block has a 0x20-byte header:
- `digest[0x10]`: Checksum/digest
- `id_hi`, `id_lo`: Block ID (16-bit)
- `seq`: Sequence number for mirroring
- `file_table[0x0C]`: File allocation table
- `data[]`: Actual data payload

### Error Handling
- CARD errors are propagated through the `_card_result` global
- Result value 1 indicates data verified (skip subsequent writes)
- Result value 2 indicates verify mismatch

### Global State
The library uses several global variables in `.sbss` and `.bss`:
- `hsd_804D1138`: Active request structure
- `hsd_804D1148[128]`: Command ring buffer
- `hsd_804D2348[32]`: Request queue
- `hsd_804D7980`: Ring head (next command to run)
- `hsd_804D7984`: Ring tail (next free slot)
- `_card_result`: Current operation result

## Relationship to Other Layers

### Layer 1 (Dolphin CARD SDK)
- Direct hardware access via `CARDMountAsync`, `CARDReadAsync`, `CARDWriteAsync`, etc.
- Provides basic file I/O primitives

### Layer 2 (hsd_3A94) ← This Library
- Manages 9 logical files within one physical file
- Handles block mirroring and verification
- Provides async command queue system

### Layer 3 (lbcardnew)
- Game-level save management
- Task queue system built on top of hsd_3A94
- Provides blocking and non-blocking convenience wrappers

## Notes
- Filename used by Melee: "SuperSmashBros0110290334"
- Total logical data: ~62KB across 9 slots
- Slot 0: Metadata/header (flag 3)
- Slot 1: Main save data (~6KB, flag 0)
- Slots 2-8: Custom data (~8KB each, flag 1)
