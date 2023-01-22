#include <melee/ft/chara/ftSamus/ftsamus.h>
#include <melee/ft/ft_unknown_006.h>
#include <melee/ft/ftcamera.h>
#include <melee/ft/types.h>

ActionState as_table_samus[] = {
    { 295, FLAGS_ZERO, 0x01000000, ftSamus_80128C04, ftSamus_80128D3C,
      ftSamus_80128E88, ftSamus_80128F60, func_800761C8 },
    { 296, FLAGS_ZERO, 0x01000000, ftSamus_80128CA0, ftSamus_80128E68,
      ftSamus_80128EF8, ftSamus_80128FD4, func_800761C8 },
    { 297, 0x00340111, 0x12000000, ftSamus_80129684, ftSamus_80129B18,
      ftSamus_80129C88, ftSamus_80129D48, func_800761C8 },
    { 298, 0x00340111, 0x12000000, ftSamus_80129774, ftSamus_80129B1C,
      ftSamus_80129CA8, ftSamus_80129DC8, func_800761C8 },
    { 299, 0x00340111, 0x12000000, ftSamus_80129940, ftSamus_80129C78,
      ftSamus_80129CC8, ftSamus_80129E68, func_800761C8 },
    { 300, 0x00340111, 0x12000000, ftSamus_801299D0, ftSamus_80129C7C,
      ftSamus_80129CE8, ftSamus_80129EE8, func_800761C8 },
    { 301, 0x00340511, 0x12000000, ftSamus_80129A14, ftSamus_80129C80,
      ftSamus_80129D08, ftSamus_80129F68, func_800761C8 },
    { 302, 0x00340511, 0x12000000, ftSamus_80129A98, ftSamus_80129C84,
      ftSamus_80129D28, ftSamus_80129FE8, func_800761C8 },
    { 303, 0x00340112, 0x13000000, ftSamus_8012A380, ftSamus_8012A3F8,
      ftSamus_8012A400, ftSamus_8012A468, func_800761C8 },
    { 304, 0x00340912, 0x13000000, ftSamus_8012A4E0, ftSamus_8012A558,
      ftSamus_8012A560, ftSamus_8012A5C8, func_800761C8 },
    { 305, 0x00340512, 0x13000000, ftSamus_8012A3BC, ftSamus_8012A3FC,
      ftSamus_8012A420, ftSamus_8012A4A4, func_800761C8 },
    { 306, 0x00340D12, 0x13000000, ftSamus_8012A51C, ftSamus_8012A55C,
      ftSamus_8012A580, ftSamus_8012A604, func_800761C8 },
    { 307, 0x00340213, 0x14000000, ftSamus_8012A81C, ftSamus_8012A96C,
      ftSamus_8012AB0C, ftSamus_8012AC00, func_800761C8 },
    { 308, 0x00340613, 0x14000000, ftSamus_8012A8C4, ftSamus_8012AA3C,
      ftSamus_8012ABB4, ftSamus_8012ACF8, func_800761C8 },
    { 309, 0x00340114, 0x15000000, ftSamus_8012B150, ftSamus_8012B358,
      ftSamus_8012B3A8, ftSamus_8012B488, func_800761C8 },
    { 310, 0x00340514, 0x15000000, ftSamus_8012B264, ftSamus_8012B3A4,
      ftSamus_8012B41C, ftSamus_8012B4FC, func_800761C8 },
    { 311, 0x00200000, 0x01000000, func_800C3D6C, func_800C4384, func_800C438C,
      func_800C447C, func_800761C8 },
    { 312, 0x00C00000, 0x01000000, func_800C4380, func_800C4388, func_800C4438,
      func_80082B78, func_800761C8 },
};

char lbl_803CE510[] = "PlSs.dat";
char lbl_803CE51C[] = "ftDataSamus";
char lbl_803CE528[] = "PlSsNr.dat";
char lbl_803CE534[] = "PlySamus5K_Share_joint";
char lbl_803CE54C[] = "PlSsPi.dat";
char lbl_803CE558[] = "PlySamus5KPi_Share_joint";
char lbl_803CE574[] = "PlSsBk.dat";
char lbl_803CE580[] = "PlySamus5KBk_Share_joint";
char lbl_803CE59C[] = "PlSsGr.dat";
char lbl_803CE5A8[] = "PlySamus5KGr_Share_joint";
char lbl_803CE5C4[] = "PlSsLa.dat";
char lbl_803CE5D0[] = "PlySamus5KLa_Share_joint";
char lbl_803CE5EC[] = "PlSsAJ.dat";
char lbl_803CE5F8[] = "ftDemoResultMotionFileSamus";
char lbl_803CE614[] = "ftDemoIntroMotionFileSamus";
char lbl_803CE630[] = "ftDemoEndingMotionFileSamus";
char lbl_803CE64C[] = "ftDemoViWaitMotionFileSamus";

Fighter_DemoStrings lbl_803CE668 = {
    lbl_803CE5F8,
    lbl_803CE614,
    lbl_803CE630,
    lbl_803CE64C,
};

Fighter_CostumeStrings lbl_803CE678[] = {
    { lbl_803CE528, lbl_803CE534, NULL }, { lbl_803CE54C, lbl_803CE558, NULL },
    { lbl_803CE574, lbl_803CE580, NULL }, { lbl_803CE59C, lbl_803CE5A8, NULL },
    { lbl_803CE5C4, lbl_803CE5D0, NULL },
};
