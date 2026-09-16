#include "bsarchive.h"

#include "bisimulation.h"
#include "bshash.h"
#include "bsio.h"

void bsArchive_SetHeader(Bisim_ArchiveHeader* ah, const BsIO_Cursor* c,
                         BisimBlobType type, u32 flags)
{
    ah->magic = BISIM_ARCHIVE_MAGIC;
    ah->header_size = sizeof(*ah);
    ah->version = BisimVersion_Current;
    ah->type = type;
    ah->hash = bsHash_Cursor(bsHash_Init(), c);
    ah->size = c->pos;
    ah->flags = flags;

    OSReport("%.4s, 0x%X, %d, %d, 0x%08X, %u, 0x%08X\n", &ah->magic,
             ah->header_size, ah->version, ah->type, ah->hash, ah->size,
             ah->flags);
}
