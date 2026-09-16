#ifndef FRAY_BS_BSCARDGAME_H
#define FRAY_BS_BSCARDGAME_H

#include <melee/lb/lbcardgame.h>
#include <melee/lb/lbcardnew.h>

/* 01C600 */ void bs_8001C600(void);
/* 01C87C */ u32 bs_8001C87C(void);
/* 01C8BC */ int bs_8001C8BC(void);
/* 01CBAC */ void bsCardGame_SetCardStatus(LbCardStatus status);
/* 01CBBC */ lbCardResult bs_8001CBBC(void);
/* 01CC30 */ void fn_8001CC30(bool);
/* 01CC4C */ int bs_8001CC4C(void);
/* 01CC84 */ void bs_8001CC84(void);
/* 01CDB4 */ void bs_8001CDB4(void);
/* 01CE00 */ void bsCardGame_UpdatePowerTime(void);
/* 01CE78 */ u8 bsCardGame_DecideGameMode(void);
/* 01CF18 */ void bsCardGame_InitScene(void);
/* 01D164 */ void bsCardGame_LoadArchive(int);
/* 01D1F4 */ void bsCardGame_Reset(void);
/* 01D21C */ void bsCardGame_Init(void);

#endif
