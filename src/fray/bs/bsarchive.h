#ifndef FRAY_BS_BSARCHIVE_H
#define FRAY_BS_BSARCHIVE_H

#include "bsio.h"
#include "Runtime/platform.h"
#include <dolphin/types.h>

#define BISIM_ARCHIVE_MAGIC 0x47414C53

typedef enum {
    BisimBlob_None,
    BisimBlob_GlobalSnapshot,
    BisimBlob_SeededHashes,
} BisimBlobType;

typedef struct {
    u32 magic;
    u8 header_size;
    u8 version; ///< ::BisimVersion
    u8 reserved;
    u8 type; ///< ::BisimBlobType
    u32 hash;
    u32 size;
    u32 flags;
} Bisim_ArchiveHeader;
ASSERT_SIZE(Bisim_ArchiveHeader, 20);

typedef struct {
    Bisim_ArchiveHeader header;
    u8* data;
} Bisim_Archive;

void bsArchive_SetHeader(Bisim_ArchiveHeader* ah, const BsIO_Cursor* c,
                         BisimBlobType type, u32 flags);

#endif
