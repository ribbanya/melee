#include <melee/ft/chara/ftPikachu/ftpikachu.h>
#include <melee/ft/ft_unknown_006.h>
#include <melee/ft/ftcamera.h>
#include <melee/ft/types.h>

ActionState as_table_pichu[] = {
    { 0x00000127, 0x00340111, 0x12000000, ftPikachu_80124908,
      ftPikachu_Stub_80124B6C, ftPikachu_80124B74,
      ftPikachu_ActionChange_80124BB4, func_800761C8 },
    { 0x00000128, 0x00340511, 0x12000000, ftPikachu_80124A20,
      ftPikachu_Stub_80124B70, ftPikachu_80124B94,
      ftPikachu_ActionChange_80124C20, func_800761C8 },
    { 0x00000129, 0x00340212, 0x13000000, ftPikachu_80124F64,
      ftPikachu_Stub_80124FDC, ftPikachu_80124FE4, ftPikachu_80125084,
      func_800761C8 },
    { 0x0000012A, 0x00340212, 0x13000000, ftPikachu_801251BC,
      ftPikachu_801252FC, ftPikachu_8012535C, ftPikachu_8012539C,
      func_800761C8 },
    { 0x0000012C, 0x00340212, 0x13000000, ftPikachu_Stub_8012590C,
      ftPikachu_Stub_8012594C, ftPikachu_Stub_80125954, ftPikachu_Stub_801259D4,
      func_800761C8 },
    { 0x0000012D, 0x00340212, 0x13000000, ftPikachu_80125B34,
      ftPikachu_Stub_80125BAC, ftPikachu_80125BB4, ftPikachu_80125C44,
      func_800761C8 },
    { 0x0000012B, 0x00340212, 0x13000000, ftPikachu_8012557C,
      ftPikachu_Stub_801256B4, ftPikachu_801256BC, ftPikachu_801256FC,
      func_800761C8 },
    { 0x0000012E, 0x00340612, 0x13000000, ftPikachu_80124FA0,
      ftPikachu_Stub_80124FE0, ftPikachu_80125024, ftPikachu_801250C0,
      func_800761C8 },
    { 0x0000012F, 0x00340612, 0x13000000, ftPikachu_8012525C,
      ftPikachu_8012532C, ftPikachu_8012537C, ftPikachu_801253D8,
      func_800761C8 },
    { 0x0000012C, 0x00340612, 0x13000000, ftPikachu_80125910,
      ftPikachu_Stub_80125950, ftPikachu_80125958, ftPikachu_801259D8,
      func_800761C8 },
    { 0x00000131, 0x00340612, 0x13000000, ftPikachu_80125B70,
      ftPikachu_Stub_80125BB0, ftPikachu_80125BF4, ftPikachu_80125C80,
      func_800761C8 },
    { 0x00000130, 0x00340612, 0x13000000, ftPikachu_8012561C,
      ftPikachu_Stub_801256B8, ftPikachu_801256DC, ftPikachu_80125738,
      func_800761C8 },
    { 0x00000132, 0x00340113, 0x14000000, ftPikachu_80125ED8,
      ftPikachu_Stub_80125F50, ftPikachu_80125F58, ftPikachu_80125FD8,
      func_800761C8 },
    { 0x00000133, 0x00340113, 0x14000000, ftPikachu_80126144,
      ftPikachu_Stub_80126424, ftPikachu_801265D4, ftPikachu_80126614,
      func_800761C8 },
    { 0x00000134, 0x00340113, 0x14000000, ftPikachu_80127198,
      ftPikachu_Stub_801272D8, ftPikachu_801272E0, ftPikachu_8012738C,
      func_800761C8 },
    { 0x00000135, 0x00340513, 0x14000000, ftPikachu_80125F14,
      ftPikachu_Stub_80125F54, ftPikachu_80125F78, ftPikachu_80126014,
      func_800761C8 },
    { 0x00000136, 0x00340513, 0x14000000, ftPikachu_801262B4,
      ftPikachu_Stub_80126428, ftPikachu_801265F4, ftPikachu_801267C8,
      func_800761C8 },
    { 0x00000137, 0x00340513, 0x14000000, ftPikachu_80127228,
      ftPikachu_Stub_801272DC, ftPikachu_80127310, ftPikachu_801273D4,
      func_800761C8 },
    { 0x00000138, 0x00340014, 0x15000000, ftPikachu_ActionChange_80127CDC, NULL,
      ftPikachu_801280C8, ftPikachu_PrepCallActionChange_801281EC,
      func_800761C8 },
    { 0x00000139, 0x00340014, 0x15000000, ftPikachu_ActionChange_80127DE4, NULL,
      ftPikachu_80128108, ftPikachu_PrepCallActionChange_8012823C,
      func_800761C8 },
    { 0x0000013A, 0x00340014, 0x15000000, ftPikachu_ActionChange_80127FB0, NULL,
      ftPikachu_80128148, ftPikachu_PrepCallActionChange_8012828C,
      func_800761C8 },
    { 0x0000013B, 0x00340014, 0x15000000, ftPikachu_80128050, NULL,
      ftPikachu_801281AC, ftPikachu_PrepCallActionChange_801282DC,
      func_800761C8 },
    { 0x0000013C, 0x00340414, 0x15000000, ftPikachu_ActionChange_80127D60, NULL,
      ftPikachu_801280E8, ftPikachu_PrepCallActionChange_80128214,
      func_800761C8 },
    { 0x0000013D, 0x00340414, 0x15000000, ftPikachu_ActionChange_80127EC0, NULL,
      ftPikachu_80128128, ftPikachu_PrepCallActionChange_80128264,
      func_800761C8 },
    { 0x0000013E, 0x00340414, 0x15000000, ftPikachu_ActionChange_80128000, NULL,
      ftPikachu_80128168, ftPikachu_PrepCallActionChange_801282B4,
      func_800761C8 },
    { 0x0000013F, 0x00340414, 0x15000000, ftPikachu_8012808C, NULL,
      ftPikachu_801281CC, ftPikachu_PrepCallActionChange_80128304,
      func_800761C8 },
};
