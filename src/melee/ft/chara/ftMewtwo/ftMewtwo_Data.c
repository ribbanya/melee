#include <melee/ft/chara/ftMewtwo/ftmewtwo.h>
#include <melee/ft/ft_unknown_006.h>
#include <melee/ft/ftcamera.h>
#include <melee/ft/types.h>

ActionState as_table_mewtwo[] = {
    { 295, 0x00340111, 0x12000000, ftMewtwo_SpecialNStart_Anim,
      ftMewtwo_SpecialNStart_IASA, ftMewtwo_SpecialNStart_Phys,
      ftMewtwo_SpecialNStart_Coll, func_800761C8 },
    { 296, 0x003C0111, 0x12000000, ftMewtwo_SpecialNLoop_Anim,
      ftMewtwo_SpecialNLoop_IASA, ftMewtwo_SpecialNLoop_Phys,
      ftMewtwo_SpecialNLoop_Coll, func_800761C8 },
    { 297, 0x003C0111, 0x12000000, ftMewtwo_SpecialNFull_Anim,
      ftMewtwo_SpecialNFull_IASA, ftMewtwo_SpecialNFull_Phys,
      ftMewtwo_SpecialNFull_Coll, func_800761C8 },
    { 298, 0x00340111, 0x12000000, ftMewtwo_SpecialNCancel_Anim,
      ftMewtwo_SpecialNCancel_IASA, ftMewtwo_SpecialNCancel_Phys,
      ftMewtwo_SpecialNCancel_Coll, func_800761C8 },
    { 299, 0x00340111, 0x12000000, ftMewtwo_SpecialNEnd_Anim,
      ftMewtwo_SpecialNEnd_IASA, ftMewtwo_SpecialNEnd_Phys,
      ftMewtwo_SpecialNEnd_Coll, func_800761C8 },
    { 300, 0x00340511, 0x12000000, ftMewtwo_SpecialAirNStart_Anim,
      ftMewtwo_SpecialAirNStart_IASA, ftMewtwo_SpecialAirNStart_Phys,
      ftMewtwo_SpecialAirNStart_Coll, func_800761C8 },
    { 301, 0x003C0511, 0x12000000, ftMewtwo_SpecialAirNLoop_Anim,
      ftMewtwo_SpecialAirNLoop_IASA, ftMewtwo_SpecialAirNLoop_Phys,
      ftMewtwo_SpecialAirNLoop_Coll, func_800761C8 },
    { 302, 0x003C0511, 0x12000000, ftMewtwo_SpecialAirNFull_Anim,
      ftMewtwo_SpecialAirNFull_IASA, ftMewtwo_SpecialAirNFull_Phys,
      ftMewtwo_SpecialAirNFull_Coll, func_800761C8 },
    { 303, 0x00340511, 0x12000000, ftMewtwo_SpecialAirNCancel_Anim,
      ftMewtwo_SpecialAirNCancel_IASA, ftMewtwo_SpecialAirNCancel_Phys,
      ftMewtwo_SpecialAirNCancel_Coll, func_800761C8 },
    { 304, 0x00340511, 0x12000000, ftMewtwo_SpecialAirNEnd_Anim,
      ftMewtwo_SpecialAirNEnd_IASA, ftMewtwo_SpecialAirNEnd_Phys,
      ftMewtwo_SpecialAirNEnd_Coll, func_800761C8 },
    { 305, 0x00341012, 0x13000000, ftMewtwo_SpecialS_Anim,
      ftMewtwo_SpecialS_IASA, ftMewtwo_SpecialS_Phys, ftMewtwo_SpecialS_Coll,
      func_800761C8 },
    { 306, 0x00341412, 0x13000000, ftMewtwo_SpecialAirS_Anim,
      ftMewtwo_SpecialAirS_IASA, ftMewtwo_SpecialAirS_Phys,
      ftMewtwo_SpecialAirS_Coll, func_800761C8 },
    { 307, 0x00340013, 0x14000000, ftMewtwo_SpecialHiStart_Anim,
      ftMewtwo_SpecialHiStart_IASA, ftMewtwo_SpecialHiStart_Phys,
      ftMewtwo_SpecialHiStart_Coll, func_800761C8 },
    { 309, 0x00340013, 0x14000000, ftMewtwo_SpecialHi_Anim,
      ftMewtwo_SpecialHi_IASA, ftMewtwo_SpecialHi_Phys, ftMewtwo_SpecialHi_Coll,
      func_800761C8 },
    { 308, 0x00340013, 0x14000000, ftMewtwo_SpecialHiLost_Anim,
      ftMewtwo_SpecialHiLost_IASA, ftMewtwo_SpecialHiLost_Phys,
      ftMewtwo_SpecialHiLost_Coll, func_800761C8 },
    { 310, 0x00340413, 0x14000000, ftMewtwo_SpecialAirHiStart_Anim,
      ftMewtwo_SpecialAirHiStart_IASA, ftMewtwo_SpecialAirHiStart_Phys,
      ftMewtwo_SpecialAirHiStart_Coll, func_800761C8 },
    { 309, 0x00340413, 0x14000000, ftMewtwo_SpecialAirHi_Anim,
      ftMewtwo_SpecialAirHi_IASA, ftMewtwo_SpecialAirHi_Phys,
      ftMewtwo_SpecialAirHi_Coll, func_800761C8 },
    { 311, 0x00340413, 0x14000000, ftMewtwo_SpecialAirHiLost_Anim,
      ftMewtwo_SpecialAirHiLost_IASA, ftMewtwo_SpecialAirHiLost_Phys,
      ftMewtwo_SpecialAirHiLost_Coll, func_800761C8 },
    { 312, 0x00340114, 0x15000000, ftMewtwo_SpecialLw_Anim,
      ftMewtwo_SpecialLw_IASA, ftMewtwo_SpecialLw_Phys, ftMewtwo_SpecialLw_Coll,
      func_800761C8 },
    { 313, 0x00340514, 0x15000000, ftMewtwo_SpecialAirLw_Anim,
      ftMewtwo_SpecialAirLw_IASA, ftMewtwo_SpecialAirLw_Phys,
      ftMewtwo_SpecialAirLw_Coll, func_800761C8 },
};

char lbl_803D0D80[] = "PlMt.dat";
char lbl_803D0D8C[] = "ftDataMewtwo";
char lbl_803D0D9C[] = "PlMtNr.dat";
char lbl_803D0DA8[] = "PlyMewtwo5K_Share_joint";
char lbl_803D0DC0[] = "PlyMewtwo5K_Share_matanim_joint";
char lbl_803D0DE0[] = "PlMtRe.dat";
char lbl_803D0DEC[] = "PlyMewtwo5KRe_Share_joint";
char lbl_803D0E08[] = "PlyMewtwo5KRe_Share_matanim_joint";
char lbl_803D0E2C[] = "PlMtBu.dat";
char lbl_803D0E38[] = "PlyMewtwo5KBu_Share_joint";
char lbl_803D0E54[] = "PlyMewtwo5KBu_Share_matanim_joint";
char lbl_803D0E78[] = "PlMtGr.dat";
char lbl_803D0E84[] = "PlyMewtwo5KGr_Share_joint";
char lbl_803D0EA0[] = "PlyMewtwo5KGr_Share_matanim_joint";
char lbl_803D0EC4[] = "PlMtAJ.dat";

Fighter_DemoStrings lbl_803D0F4C = {
    "ftDemoResultMotionFileMewtwo",
    "ftDemoIntroMotionFileMewtwo",
    "ftDemoEndingMotionFileMewtwo",
    "ftDemoViWaitMotionFileMewtwo",
};

Fighter_CostumeStrings lbl_803D0F5C[] = {
    { lbl_803D0D9C, lbl_803D0DA8, lbl_803D0DC0 },
    { lbl_803D0DE0, lbl_803D0DEC, lbl_803D0E08 },
    { lbl_803D0E2C, lbl_803D0E38, lbl_803D0E54 },
    { lbl_803D0E78, lbl_803D0E84, lbl_803D0EA0 },
};
