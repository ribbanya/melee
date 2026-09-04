/** @file
 * Each of these belong in their respective character file. They are all in
 * .bss, except for Link's, which is initialized to zero and so appears in
 * .data. See #CostumeListsForeachCharacter to determine indices. They are of
 * variable size, but the data is used uniformly across all characters, so the
 * first 0x18 bytes are probably used by #UnkCostumeStruct and then the rest is
 * character-specific .bss data used elsewhere.
 */
#ifndef GALE01_4599F0
#define GALE01_4599F0

#include "ft/forward.h"

#include "ft/types.h"

/* 4599F0 */ extern UnkCostumeStruct lbl_804599F0[5];
/* 45A330 */ extern UnkCostumeStruct ftCl_CostumeList[5];
/* 45A3A8 */ extern UnkCostumeStruct ftDr_CostumeList[5];
/* 45A420 */ extern UnkCostumeStruct ftFc_CostumeList[4];
/* 45A480 */ extern UnkCostumeStruct ftPc_CostumeList[4];
/* 45A4E0 */ extern UnkCostumeStruct ftGw_CostumeList[4];
/* 45A540 */ extern UnkCostumeStruct ftGn_CostumeList[5];
/* 45A5B8 */ extern UnkCostumeStruct ftFe_CostumeList[5];
/* 45A690 */ extern UnkCostumeStruct ftMh_CostumeList[1];
/* 45A6A8 */ extern UnkCostumeStruct ftCh_CostumeList[1];
/* 45A630 */ extern UnkCostumeStruct ftBo_CostumeList[1];
/* 45A648 */ extern UnkCostumeStruct ftGl_CostumeList[1];
/* 45A660 */ extern UnkCostumeStruct ftGk_CostumeList[1];
/* 45A678 */ extern UnkCostumeStruct ftSb_CostumeList[1];

#endif
