# Card IO System Research Notes

## Overview

The GameCube Memory Card IO system in Super Smash Bros. Melee consists of three distinct layers:

### Layer 1: dolphin/card (Direct Hardware Access)
**Location:** `/workspace/libs/dolphin/src/dolphin/card/`

This is the lowest level - the Dolphin SDK's direct hardware interface to the memory card.

**Key Structures:**
- `CARDFileInfo` - File handle information (chan, fileNo, offset, length, iBlock)
- `CARDDir` - Directory entry structure (32-byte filename, gameName, company, timestamps, block allocation)
- `CARDStat` - Extended file status with banner/icon metadata
- `CARDControl` - Memory card controller state

**Key Constants:**
- `CARD_FILENAME_MAX = 32`
- `CARD_MAX_FILE = 127`
- `CARD_ICON_MAX = 8`
- `CARD_COMMENT_SIZE = 64`
- Sector size: 0x2000 (8KB)
- Block size: 0x2000 (8KB)

**Core Functions:**
- `CARDMountAsync()` - Mount the memory card
- `CARDCheckAsync()` - Check card integrity
- `CARDOpen()` - Open a file
- `CARDReadAsync()` / `CARDWriteAsync()` - Raw file I/O
- `CARDCreateAsync()` - Create new file
- `CARDDeleteAsync()` - Delete file
- `CARDRenameAsync()` - Rename file
- `CARDGetStatus()` / `CARDSetStatusAsync()` - Get/set file metadata
- `CARDFreeBlocks()` - Query free space
- `CARDFormatAsync()` - Format the card

### Layer 2: sysdolphin/baselib/hsd_3A94 (Light Abstraction for Async Card IO)
**Location:** `/workspace/src/sysdolphin/baselib/hsd_3A94.c|h`

This layer provides an asynchronous command queue system that manages multiple logical files within a single physical CARD file.

**Key Concepts:**
- **CardState**: Manages up to 9 logical files within one physical CARD file
- **Sector-based I/O**: Works with 8KB sectors
- **Block mirroring**: Supports redundant copies for reliability (file_flags modes 0-3)
- **Command Ring Buffer**: 128-entry command queue for async operations
- **Request Queue**: 32-entry higher-level request queue

**CardState Structure (0x464 bytes):**
```c
struct CardState {
    u8* sector_buf;          // Working buffer for sector I/O
    s32 chan;                // Memory card channel (0 or 1)
    u32 sector_size;         // Usually 0x2000
    CARDFileInfo file_info;  // Physical file handle

    s32 file_no;             // Logical file index
    u32 header_size;         // Header bytes (comment + banner + icons)

    int file_flags[9];       // File modes (0-3 for different mirror strategies)
    int file_sizes[9];       // Size of each logical file
    u8* file_data[9];        // Data buffers for each logical file

    s32 block_ids[64];       // Block ID mapping (negated = stale, -0x7FFF = free)
    s32 block_seqs[64];      // Block sequence numbers for wear leveling

    char comment[64];        // File comment
    CardIconInfo icon_info;  // Banner/icon format info
    CARDStat stat;           // CARD status metadata
    u8 digest[0x30];         // File integrity digest

    s32 num_blocks;          // Total blocks used
};
```

**Command Types (CardCmdType):**
- `CARD_CMD_WRITE_BLOCK` / `CARD_CMD_READ_BLOCK` - Block-level I/O
- `CARD_CMD_VERIFY_BLOCK` - Verify block integrity
- `CARD_CMD_CREATE_FILE` - Create physical CARD file
- `CARD_CMD_SET_STATUS` - Update file metadata
- `CARD_CMD_SCAN_FILE` - Scan all blocks and rebuild file table
- `CARD_CMD_REPAIR` - Repair corrupted/stale blocks

**Request Types (CardRequestType):**
- `CARD_REQ_READ_FILE` / `CARD_REQ_WRITE_FILE` - Logical file I/O
- `CARD_REQ_CREATE_FILE` - Create with metadata
- `CARD_REQ_SET_STATUS` - Update header
- `CARD_REQ_OPEN_FILE` - Open existing file
- `CARD_REQ_READ_HEADER` - Read comment/banner/icons

**File Flags Modes:**
- Mode 0: Single copy (no redundancy)
- Mode 1: Dual copy with staleness tracking
- Mode 2: Dual copy (alternate strategy)
- Mode 3: Triple copy for critical data

### Layer 3: melee/lb/lbcardnew (Game Implementation)
**Location:** `/workspace/src/melee/lb/lbcardnew.c|h`

This is Melee's high-level save file management system built on top of hsd_3A94.

**Key Structures:**

**CardEntry** - Describes a logical file within the CARD file:
```c
struct CardEntry {
    int file_size;   // Size in bytes (-1 terminates array)
    int file_flags;  // Redundancy mode (0-3)
    u8* data;        // Data buffer pointer
};
```

**lbCardNew_SnapshotEntry** - Save file snapshot info:
```c
struct lbCardNew_SnapshotEntry {
    u32 time;        // Timestamp from filename
    s16 file_no;     // CARD file number
    u16 blocks;      // Blocks used
};
```

**Task System:**
The game uses a task queue (11 slots max) to sequence async operations:
- `CARD_TASK_MOUNT_CARD` - Initialize card
- `CARD_TASK_CHECK_CARD` - Verify integrity
- `CARD_TASK_OPEN_FILE` - Open save file
- `CARD_TASK_CREATE_FILE` - Create new save
- `CARD_TASK_READ_FILES` / `CARD_TASK_WRITE_FILES` - I/O
- `CARD_TASK_DELETE_FILE` / `CARD_TASK_RENAME_FILE` - File management
- `CARD_TASK_SET_STATUS` / `CARD_TASK_READ_HEADER` - Metadata
- `CARD_TASK_LIST_SNAPSHOTS` - Enumerate saves
- `CARD_TASK_FIND_FILE` - Search for file

**Error Codes:**
```
0  = OK
2  = Card changed/remount needed
3  = No file / file not found
4  = File doesn't exist (create needed)
5  = Insufficient space
6  = No file slots available
9  = Card write-protected
10 = Corrupted data
12 = Bad sector size (not 8KB)
13 = File not found (search failure)
14 = Device error
15 = Fatal error
```

**Main API Functions:**
- `lb_8001B7E0()` - Mount, check, and open/create file
- `lb_8001B8C8()` - Format card
- `lb_8001B99C()` / `lb_8001BA44()` - Delete file
- `lb_8001BB48()` / `lb_8001BC18()` - Create file with metadata
- `lb_8001BD34()` - Read files
- `lb_8001BE30()` - Write files
- `lb_8001BF04()` - Update status/metadata
- `lb_8001BFD8()` - List snapshots (free space + existing saves)
- `lb_8001C2D8()` - Check if file exists

## Save File Format Analysis

### Physical CARD File Structure
Each save file is stored as a single CARD file containing:
1. **Header Sector** (first 8KB block):
   - Comment (64 bytes)
   - Banner image (96x32 pixels, C8 or RGB5A3)
   - Icon images (up to 8 icons, 32x32 pixels each)
   - CardIconInfo metadata
   - Digest/hash for integrity

2. **Logical Files** (up to 9 files):
   - Each logical file has its own buffer
   - Files can use different redundancy modes
   - Block IDs track version/copy status
   - Sequence numbers enable wear leveling

### Filename Convention
Save files use timestamp-based naming:
- Filename: `"XXXXXXXXXX"` where X is decimal digits
- Represents seconds since some epoch
- Allows sorting by recency
- Checked via `isdigit(fileName[0])`

### File Metadata (CARDStat)
```c
struct CARDStat {
    char fileName[32];      // Timestamp string
    u32 length;             // File size in bytes
    u32 time;               // Last modified time
    u8 gameName[4];         // Game identifier (e.g., "GALE")
    u8 company[2];          // Publisher code (e.g., "01" Nintendo)
    u8 bannerFormat;        // Banner encoding type
    u32 iconAddr;           // Offset to icon data
    u16 iconFormat;         // Icon encoding per slot
    u16 iconSpeed;          // Icon animation speed
    u32 commentAddr;        // Offset to comment
    // ... additional offsets for banner/icon/data
};
```

## Recommendations for New Save Format

When designing your new save file format, consider:

1. **Use the existing hsd_3A94 layer** - Don't bypass to raw CARD I/O unless necessary
2. **Leverage logical files** - Split save data into logical components (settings, progress, replays, etc.)
3. **Choose appropriate file_flags** - Use redundancy mode based on data criticality
4. **Timestamp filenames** - Follow the numeric naming convention for compatibility
5. **Respect sector boundaries** - Align data to 8KB sectors for efficiency
6. **Use CARDStat metadata** - Store version info in gameName/company fields if needed
7. **Plan for migration** - Include version headers in your logical file formats

## Key Source Files to Study

**Layer 1 (Hardware):**
- `libs/dolphin/include/dolphin/card.h` - Main header
- `libs/dolphin/include/dolphin/card/CARDStat.h` - Metadata structures
- `libs/dolphin/src/dolphin/card/CARDDir.c` - Directory management
- `libs/dolphin/src/dolphin/card/CARDRead.c` / `CARDWrite.c` - I/O implementation

**Layer 2 (HSD Abstraction):**
- `src/sysdolphin/baselib/hsd_3A94.h` - Interface definitions
- `src/sysdolphin/baselib/hsd_3A94.c` - Command queue implementation
- `src/sysdolphin/baselib/hsd_3B27.c` - Request builder (if available)

**Layer 3 (Melee Implementation):**
- `src/melee/lb/lbcardnew.h` - Function declarations
- `src/melee/lb/lbcardnew.c` - Full implementation
- `src/melee/lb/lbcardgame.c` - Higher-level game save logic
- `src/melee/lb/types.h` - Structure definitions
- `src/melee/lb/forward.h` - Type forward declarations
