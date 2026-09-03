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
/* 459B28 */ extern UnkCostumeStruct ft_80459B28[4];
/* 459A98 */ extern UnkCostumeStruct ft_80459A98[6];
/* 459CA0 */ extern UnkCostumeStruct ft_80459CA0[5];
/* 459C10 */ extern UnkCostumeStruct ft_80459C10[6];
/* 45A090 */ extern UnkCostumeStruct ft_8045A090[4];
/* 459D18 */ extern UnkCostumeStruct ft_80459D18[5];
/* 459D90 */ extern UnkCostumeStruct ft_80459D90[4];
/* 459DF0 */ extern UnkCostumeStruct ft_80459DF0[5];
/* 459E68 */ extern UnkCostumeStruct ft_80459E68[4];
/* 459EC8 */ extern UnkCostumeStruct ft_80459EC8[4];
/* 459F28 */ extern UnkCostumeStruct ft_80459F28[4];
/* 459F88 */ extern UnkCostumeStruct ft_80459F88[5];
/* 45A000 */ extern UnkCostumeStruct ft_8045A000[6];
/* 45A1F8 */ extern UnkCostumeStruct ft_8045A1F8[5];
/* 45A2D0 */ extern UnkCostumeStruct ft_8045A2D0[4];
/* 45A270 */ extern UnkCostumeStruct ft_8045A270[4];
/* 45A0F0 */ extern UnkCostumeStruct ft_8045A0F0[5];
/* 45A168 */ extern UnkCostumeStruct ft_8045A168[5];
/* 45A330 */ extern UnkCostumeStruct ft_8045A330[5];
/* 45A3A8 */ extern UnkCostumeStruct ft_8045A3A8[5];
/* 45A420 */ extern UnkCostumeStruct ft_8045A420[4];
/* 45A480 */ extern UnkCostumeStruct ft_8045A480[4];
/* 45A4E0 */ extern UnkCostumeStruct ft_8045A4E0[4];
/* 45A540 */ extern UnkCostumeStruct ft_8045A540[5];
/* 45A5B8 */ extern UnkCostumeStruct ft_8045A5B8[5];
/* 45A690 */ extern UnkCostumeStruct ft_8045A690[1];
/* 45A6A8 */ extern UnkCostumeStruct ft_8045A6A8[1];
/* 45A630 */ extern UnkCostumeStruct ft_8045A630[1];
/* 45A648 */ extern UnkCostumeStruct ft_8045A648[1];
/* 45A660 */ extern UnkCostumeStruct ft_8045A660[1];
/* 45A678 */ extern UnkCostumeStruct ft_8045A678[1];

#endif
