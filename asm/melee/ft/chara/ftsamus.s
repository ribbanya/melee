.include "macros.inc"

.section .data
.global lbl_803CE510
lbl_803CE510:
    .asciz "PlSs.dat"
    .balign 4
.global lbl_803CE51C
lbl_803CE51C:
    .asciz "ftDataSamus"
    .balign 4
.global lbl_803CE528
lbl_803CE528:
    .asciz "PlSsNr.dat"
    .balign 4
.global lbl_803CE534
lbl_803CE534:
    .asciz "PlySamus5K_Share_joint"
    .balign 4
.global lbl_803CE54C
lbl_803CE54C:
    .asciz "PlSsPi.dat"
    .balign 4
.global lbl_803CE558
lbl_803CE558:
    .asciz "PlySamus5KPi_Share_joint"
    .balign 4
.global lbl_803CE574
lbl_803CE574:
    .asciz "PlSsBk.dat"
    .balign 4
.global lbl_803CE580
lbl_803CE580:
    .asciz "PlySamus5KBk_Share_joint"
    .balign 4
.global lbl_803CE59C
lbl_803CE59C:
    .asciz "PlSsGr.dat"
    .balign 4
.global lbl_803CE5A8
lbl_803CE5A8:
    .asciz "PlySamus5KGr_Share_joint"
    .balign 4
.global lbl_803CE5C4
lbl_803CE5C4:
    .asciz "PlSsLa.dat"
    .balign 4
.global lbl_803CE5D0
lbl_803CE5D0:
    .asciz "PlySamus5KLa_Share_joint"
    .balign 4
.global lbl_803CE5EC
lbl_803CE5EC:
    .asciz "PlSsAJ.dat"
    .balign 4
.global lbl_803CE5F8
lbl_803CE5F8:
    .asciz "ftDemoResultMotionFileSamus"
    .balign 4
.global lbl_803CE614
lbl_803CE614:
    .asciz "ftDemoIntroMotionFileSamus"
    .balign 4
.global lbl_803CE630
lbl_803CE630:
    .asciz "ftDemoEndingMotionFileSamus"
    .balign 4
.global lbl_803CE64C
lbl_803CE64C:
    .asciz "ftDemoViWaitMotionFileSamus"
    .balign 4
.global lbl_803CE668
lbl_803CE668:
    .4byte lbl_803CE5F8
    .4byte lbl_803CE614
    .4byte lbl_803CE630
    .4byte lbl_803CE64C
.global lbl_803CE678
lbl_803CE678:
    .4byte lbl_803CE528
    .4byte lbl_803CE534
    .4byte NULL
    .4byte lbl_803CE54C
    .4byte lbl_803CE558
    .4byte NULL
    .4byte lbl_803CE574
    .4byte lbl_803CE580
    .4byte NULL
    .4byte lbl_803CE59C
    .4byte lbl_803CE5A8
    .4byte NULL
    .4byte lbl_803CE5C4
    .4byte lbl_803CE5D0
    .4byte NULL
    .4byte NULL
.global lbl_803CE6B8
lbl_803CE6B8:
    .4byte 0x0003F7A6
    .4byte 0x0003F7A9
    .4byte 0x0003F7AC
    .4byte 0x0003F7AF
    .4byte 0x0003F7B2
    .4byte NULL

#### this data below here belongs in yoshi
.global as_table_yoshi
as_table_yoshi:
    .4byte 0x00000025
    .4byte 0x00100000
    .4byte 0x01000000
    .4byte lbl_8012C030
    .4byte lbl_8012C114
    .4byte lbl_8012C194
    .4byte lbl_8012C1B4
    .4byte func_800761C8
    .4byte 0xFFFFFFFF
    .4byte 0x00180000
    .4byte 0x01000000
    .4byte lbl_8012C2F4
    .4byte lbl_8012C3EC
    .4byte lbl_8012C45C
    .4byte lbl_8012C47C
    .4byte func_800761C8
    .4byte 0x00000027
    .4byte 0x00100000
    .4byte 0x01000000
    .4byte lbl_8012C54C
    .4byte lbl_8012C59C
    .4byte lbl_8012C5C0
    .4byte lbl_8012C5E0
    .4byte func_800761C8
    .4byte 0x00000028
    .4byte 0x00100000
    .4byte 0x01000000
    .4byte lbl_8012C7A4
    .4byte lbl_8012C80C
    .4byte lbl_8012C810
    .4byte lbl_8012C830
    .4byte func_800761C8
    .4byte 0x00000025
    .4byte 0x00101073
    .4byte 0x01000000
    .4byte lbl_8012CAD0
    .4byte lbl_8012CBBC
    .4byte lbl_8012CBDC
    .4byte lbl_8012CBFC
    .4byte func_800761C8
    .4byte 0x00000127
    .4byte 0x00340011
    .4byte 0x12000000
    .4byte lbl_8012D550
    .4byte NULL
    .4byte lbl_8012DC30
    .4byte lbl_8012DD70
    .4byte func_800761C8
    .4byte 0x00000128
    .4byte 0x00340011
    .4byte 0x12000000
    .4byte lbl_8012D658
    .4byte NULL
    .4byte lbl_8012DC90
    .4byte lbl_8012DDE8
    .4byte func_800761C8
    .4byte 0x00000128
    .4byte 0x00340011
    .4byte 0x12000000
    .4byte lbl_8012D5C8
    .4byte NULL
    .4byte lbl_8012DC70
    .4byte lbl_8012DDC0
    .4byte func_800761C8
    .4byte 0x00000129
    .4byte 0x00340011
    .4byte 0x12000000
    .4byte lbl_8012D948
    .4byte NULL
    .4byte lbl_8012DD10
    .4byte lbl_8012DE88
    .4byte func_800761C8
    .4byte 0x00000129
    .4byte 0x00340011
    .4byte 0x12000000
    .4byte lbl_8012D7D8
    .4byte NULL
    .4byte lbl_8012DCF0
    .4byte lbl_8012DE60
    .4byte func_800761C8
    .4byte 0x0000012A
    .4byte 0x00340411
    .4byte 0x12000000
    .4byte lbl_8012D58C
    .4byte NULL
    .4byte lbl_8012DC50
    .4byte lbl_8012DD98
    .4byte func_800761C8
    .4byte 0x0000012B
    .4byte 0x00340411
    .4byte 0x12000000
    .4byte lbl_8012D760
    .4byte NULL
    .4byte lbl_8012DCD0
    .4byte lbl_8012DE38
    .4byte func_800761C8
    .4byte 0x0000012B
    .4byte 0x00340411
    .4byte 0x12000000
    .4byte lbl_8012D6D0
    .4byte NULL
    .4byte lbl_8012DCB0
    .4byte lbl_8012DE10
    .4byte func_800761C8
    .4byte 0x0000012C
    .4byte 0x00340411
    .4byte 0x12000000
    .4byte lbl_8012DB74
    .4byte NULL
    .4byte lbl_8012DD50
    .4byte lbl_8012DED8
    .4byte func_800761C8
    .4byte 0x0000012C
    .4byte 0x00340411
    .4byte 0x12000000
    .4byte lbl_8012DA04
    .4byte NULL
    .4byte lbl_8012DD30
    .4byte lbl_8012DEB0
    .4byte func_800761C8
    .4byte 0x0000012D
    .4byte 0x00340212
    .4byte 0x13000000
    .4byte lbl_8012F654
    .4byte lbl_80130DB8
    .4byte lbl_801310C4
    .4byte lbl_801317DC
    .4byte func_800761C8
    .4byte 0x0000012E
    .4byte 0x00340212
    .4byte 0x13000000
    .4byte lbl_8012F750
    .4byte lbl_80130DBC
    .4byte lbl_801310E8
    .4byte lbl_8013187C
    .4byte func_800761C8
    .4byte 0x0000012F
    .4byte 0x00340212
    .4byte 0x13000000
    .4byte lbl_8012FAC4
    .4byte lbl_80130F7C
    .4byte lbl_801312EC
    .4byte lbl_80131B60
    .4byte func_800761C8
    .4byte 0x00000130
    .4byte 0x00340212
    .4byte 0x13000000
    .4byte lbl_8012FFF4
    .4byte lbl_80130FD8
    .4byte lbl_80131618
    .4byte lbl_80131E74
    .4byte func_800761C8
    .4byte 0x00000131
    .4byte 0x00340612
    .4byte 0x13000000
    .4byte lbl_80130368
    .4byte lbl_80130FDC
    .4byte lbl_80131654
    .4byte lbl_80131ED4
    .4byte func_800761C8
    .4byte 0x00000132
    .4byte 0x00340612
    .4byte 0x13000000
    .4byte lbl_80130428
    .4byte lbl_80130FEC
    .4byte lbl_8013168C
    .4byte lbl_80131F80
    .4byte func_800761C8
    .4byte 0x00000133
    .4byte 0x00340612
    .4byte 0x13000000
    .4byte lbl_801306A8
    .4byte lbl_80131050
    .4byte lbl_80131718
    .4byte lbl_8013245C
    .4byte func_800761C8
    .4byte 0x00000134
    .4byte 0x00340612
    .4byte 0x13000000
    .4byte lbl_80130A24
    .4byte lbl_801310B4
    .4byte lbl_801317A4
    .4byte lbl_801328F0
    .4byte func_800761C8
    .4byte 0x00000135
    .4byte 0x00340113
    .4byte 0x14000000
    .4byte lbl_8012E4DC
    .4byte NULL
    .4byte lbl_8012E594
    .4byte lbl_8012E5D4
    .4byte func_800761C8
    .4byte 0x00000136
    .4byte 0x00340513
    .4byte 0x14000000
    .4byte lbl_8012E538
    .4byte NULL
    .4byte lbl_8012E5B4
    .4byte lbl_8012E5FC
    .4byte func_800761C8
    .4byte 0x00000137
    .4byte 0x00340214
    .4byte 0x15000000
    .4byte lbl_8012E814
    .4byte NULL
    .4byte lbl_8012E88C
    .4byte lbl_8012E924
    .4byte func_800761C8
    .4byte 0x00000138
    .4byte 0x00340214
    .4byte 0x15000000
    .4byte lbl_8012EA40
    .4byte NULL
    .4byte lbl_8012EA7C
    .4byte lbl_8012EA9C
    .4byte func_800761C8
    .4byte 0x00000139
    .4byte 0x00340614
    .4byte 0x15000000
    .4byte lbl_8012E850
    .4byte NULL
    .4byte lbl_8012E8CC
    .4byte func_8012E944
    .4byte func_800761C8
