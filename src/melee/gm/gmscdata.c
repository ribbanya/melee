#include "gmscdata.h"

#include "gm_1A33.h"
#include "gm_unsplit.h"
#include "gmadventure.h"
#include "gmallstar.h"
#include "gmapproach.h"
#include "gmclassic.h"
#include "gmdebugmode.h"
#include "gmfixedcamera.h"
#include "gmgiant.h"
#include "gmhanyucss.h"
#include "gmhanyusss.h"
#include "gmhomerun.h"
#include "gmhowto.h"
#include "gminvisible.h"
#include "gmlightning.h"
#include "gmmenu.h"
#include "gmmenumode.h"
#include "gmmovieend.h"
#include "gmmultiman.h"
#include "gmomake15.h"
#include "gmopening.h"
#include "gmprogressive.h"
#include "gmregtyfall.h"
#include "gmresult.h"
#include "gmsinglebutton.h"
#include "gmslomo.h"
#include "gmstaffroll.h"
#include "gmstamina.h"
#include "gmsupersudden.h"
#include "gmtiny.h"
#include "gmtitle.h"
#include "gmtitlemode.h"
#include "gmtoulib.h"
#include "gmtoumode.h"
#include "gmtoycollection.h"
#include "gmtoygallery.h"
#include "gmtoylottery.h"
#include "gmtrainingmode.h"
#include "gmvsmelee.h"
#include "gmvsmode.h"
#include "types.h"

#include "gm/gmcameramode.h"
#include "gm/gmmenumode.h"
#include "gm/gmprogressivemode.h"
#include "gm/gmtrainingmode.h"
#include "if/ifprize.h"
#include "mn/mncharsel.h"
#include "mn/mnmain.h"
#include "mn/mnstagesel.h"
#include "ty/toy.h"
#include "ty/tydisplay.h"
#include "ty/tyfigupon.h"
#include "vi/vi0102.h"
#include "vi/vi0401.h"
#include "vi/vi0402.h"
#include "vi/vi0501.h"
#include "vi/vi0502.h"
#include "vi/vi0601.h"
#include "vi/vi0801.h"
#include "vi/vi1101.h"
#include "vi/vi1201v1.h"
#include "vi/vi1201v2.h"
#include "vi/vi1202.h"

static GameScene scenes[] = {
    {
        GS_TITLE,
        gm_Scene_Title_OnFrame,
        gm_Scene_Title_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_MENU,
        mnMain_Scene_OnFrame,
        mnMain_Scene_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_VS,
        gm_Scene_Vs_OnFrame,
        gm_Scene_Vs_OnEnter,
        gm_Scene_Vs_OnExit,
        NULL,
    },
    {
        GS_SUDDEN_DEATH,
        gm_Scene_Vs_OnFrame,
        gm_Scene_SuddenDeath_OnEnter,
        gm_Scene_Vs_OnExit,
        NULL,
    },
    {
        GS_TRAINING_MODE,
        gm_Scene_Training_OnFrame,
        gm_Scene_Training_OnEnter,
        gm_Scene_Vs_OnExit,
        NULL,
    },
    {
        GS_RESULTS,
        NULL,
        gm_Scene_Results_OnEnter,
        gm_Scene_Results_OnExit,
        NULL,
    },
    {
        GS_DEBUG_MENU,
        NULL,
        gm_Scene_DebugMenu_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_CSS,
        mnCharSel_Scene_OnFrame,
        mnCharSel_Scene_OnEnter,
        mnCharSel_Scene_OnExit,
        NULL,
    },
    {
        GS_SSS,
        mnStageSel_Scene_OnFrame,
        mnStageSel_Scene_OnEnter,
        mnStageSel_Scene_OnExit,
        NULL,
    },
    {
        GS_UNK10,
        NULL,
        NULL,
        NULL,
        NULL,
    },
    {
        GS_TOY_GALLERY,
        Toy_Scene_OnFrame,
        Toy_Scene_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_TOY_LOTTERY,
        tyFigupon_Scene_OnFrame,
        tyFigupon_Scene_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_TOY_COLLECTION,
        tyDisplay_Scene_OnFrame,
        tyDisplay_Scene_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_INTRO_NORMAL,
        gm_Scene_IntroNormal_OnFrame,
        gm_Scene_IntroNormal_OnEnter,
        gm_Scene_IntroNormal_OnLeave,
        NULL,
    },
    {
        GS_REGEND_TOYFALL,
        gm_Scene_ToyFall_OnFrame,
        gm_Scene_ToyFall_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_REGEND_CONGRATS,
        gm_Scene_Congrats_OnFrame,
        gm_Scene_Congrats_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_CUTSCENE_LUIGI,
        vi0102_Scene_OnFrame,
        vi0102_Scene_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_CUTSCENE_BRINSTAR,
        vi0401_Scene_OnFrame,
        vi0401_Scene_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_CUTSCENE_EXPLOSION,
        vi0402_Scene_OnFrame,
        vi0402_Scene_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_CUTSCENE_3KIRBYS,
        vi0501_Scene_OnFrame,
        vi0501_Scene_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_CUTSCENE_GIANTKIRBY,
        vi0502_Scene_OnFrame,
        vi0502_Scene_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_CUTSCENE_STARFOX,
        vi0601_Scene_OnFrame,
        vi0601_Scene_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_CUTSCENE_FZERO,
        vi0801_Scene_OnFrame,
        vi0801_Scene_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_CUTSCENE_METAL,
        vi1101_Scene_OnFrame,
        vi1101_Scene_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_CUTSCENE_BOWSERTOY,
        vi1201v1_Scene_OnFrame,
        vi1201v1_Scene_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_CUTSCENE_GIGATRANSFORM,
        vi1201v2_Scene_OnFrame,
        vi1201v2_Scene_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_CUTSCENE_GIGADEFEATED,
        vi1202_Scene_OnFrame,
        vi1202_Scene_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_MOVIE_OPENING,
        gm_Scene_Opening_OnFrame,
        gm_Scene_Opening_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_MOVIE_END,
        gm_Scene_MovieEnd_OnFrame,
        gm_Scene_MovieEnd_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_MOVIE_HOWTO,
        gm_Scene_HowTo_OnFrame,
        gm_Scene_HowTo_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_MOVIE_OMAKE15,
        gm_Scene_Omake15_OnFrame,
        gm_Scene_Omake15_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_INTRO_EASY,
        gm_Scene_IntroEasy_OnFrame,
        gm_Scene_IntroEasy_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_INTRO_ALLSTAR,
        gm_Scene_IntroAllstar_OnFrame,
        gm_Scene_IntroAllstar_OnEnter,
        NULL,
        NULL,
    },
    {
        GS_GAMEOVER,
        NULL,
        gm_Scene_GOver_OnEnter,
        gm_Scene_GOver_OnExit,
        NULL,
    },
    {
        GS_COMING_SOON,
        NULL,
        gm_801A0C6C_OnEnter,
        gm_801A0E0C_OnLeave,
        NULL,
    },
    {
        GS_TOU_SETUP,
        gm_8019628C_OnFrame,
        gm_801963B4_OnEnter,
        gm_801964A4_OnLeave,
        NULL,
    },
    {
        GS_TOU_BRACKET,
        gm_8019B2DC_OnFrame,
        gm_8019B8C4_OnEnter,
        gm_8019B9C8_OnLeave,
        NULL,
    },
    {
        GS_TOU_ALT,
        gm_8019DF8C_OnFrame,
        gm_8019ECAC_OnEnter,
        gm_8019EE54_OnLeave,
        NULL,
    },
    {
        GS_PRIZE_INTERFACE,
        NULL,
        un_802FEBE0_OnEnter,
        un_802FED10_OnLeave,
        NULL,
    },
    {
        GS_PROG_SCAN,
        gm_801AD620_OnFrame,
        gm_801AD874_OnEnter,
        gm_801AD8EC_OnLeave,
        NULL,
    },
    {
        GS_APPROACH,
        gm_801ADC88_OnFrame,
        gm_801ADCE4_OnEnter,
        gm_801ADDA8_OnLeave,
        NULL,
    },
    {
        GS_MEMCARD,
        gm_801AF568_OnFrame,
        gm_801B0264_OnEnter,
        gm_801B0304_OnLeave,
        NULL,
    },
    {
        GS_STAFFROLL,
        gm_801AA7C4_OnFrame,
        gm_801AC6D8_OnEnter,
        gm_801ACC90_OnLeave,
        NULL,
    },
    {
        GS_CAMERA_VS,
        gmCamera_801A34FC_OnFrame,
        gmCamera_801A3634_OnEnter,
        gmCamera_801A367C_OnLeave,
        NULL,
    },
    {
        GS_COUNT,
        NULL,
        NULL,
        NULL,
        NULL,
    },
};

static GameMode modes[] = {
    {
        1,
        GM_TITLE,
        NULL,
        NULL,
        NULL,
        gm_803DD6A0_Scenes,
    },
    {
        0,
        GM_VS,
        gm_801A55EC_OnLoad,
        gm_801A5614_OnUnload,
        gm_801A5598_OnInit,
        gm_803DD9A0_Scenes,
    },
    {
        1,
        GM_CLASSIC,
        gmClassic_OnLoad,
        NULL,
        gmClassic_OnInit,
        gm_803DDC58_Scenes,
    },
    {
        0,
        GM_ADVENTURE,
        gm_801B5214_OnLoad,
        NULL,
        gm_801B51CC_OnInit,
        gm_803DE1B8_Scenes,
    },
    {
        1,
        GM_ALLSTAR,
        gm_801B60A4_OnLoad,
        NULL,
        gm_801B62D8_OnInit,
        gm_803DE930_Scenes,
    },
    {
        0,
        GM_DEBUG,
        NULL,
        NULL,
        NULL,
        gm_803DD6D0_Scenes,
    },
    {
        0,
        GM_DEBUG_SOUND_TEST,
        NULL,
        NULL,
        NULL,
        gm_803DD888_Scenes,
    },
    {
        0,
        GM_HANYU_CSS,
        NULL,
        NULL,
        NULL,
        gm_803DFAA8_Scenes,
    },
    {
        0,
        GM_HANYU_SSS,
        NULL,
        NULL,
        NULL,
        gm_803DFAD8_Scenes,
    },
    {
        1,
        GM_CAMERA_MODE,
        NULL,
        NULL,
        gm_801B2B7C_OnInit,
        gm_CameraModeScenes,
    },
    {
        0,
        GM_TOY_GALLERY,
        NULL,
        NULL,
        Toy_OnInit_803122D0,
        gm_803DFA18_Scenes,
    },
    {
        0,
        GM_TOY_LOTTERY,
        NULL,
        NULL,
        NULL,
        gm_803DFA48_Scenes,
    },
    {
        0,
        GM_TOY_COLLECTION,
        NULL,
        NULL,
        NULL,
        gm_803DFA78_Scenes,
    },
    {
        0,
        GM_DEBUG_VS,
        NULL,
        NULL,
        NULL,
        gm_803DDA78_Scenes,
    },
    {
        1,
        GM_TARGET_TEST,
        gm_801B6808_OnLoad,
        NULL,
        gm_801B67E8_OnInit,
        gm_803DECB8_Scenes,
    },
    {
        0,
        GM_SUPER_SUDDEN_DEATH_VS,
        gm_801B8D88_OnLoad,
        NULL,
        gm_801B8D60_OnInit,
        gm_803DEEB0_Scenes,
    },
    {
        0,
        GM_INVISIBLE_VS,
        gm_801BA4EC_OnLoad,
        NULL,
        gm_801BA4C4_OnInit,
        gm_803DF390_Scenes,
    },
    {
        0,
        GM_SLOMO_VS,
        gm_801BA6E4_OnLoad,
        NULL,
        gm_801BA6BC_OnInit,
        gm_803DF468_Scenes,
    },
    {
        0,
        GM_LIGHTNING_VS,
        gm_801BA8DC_OnLoad,
        NULL,
        gm_801BA8B4_OnInit,
        gm_803DF540_Scenes,
    },
    {
        0,
        GM_CHALLENGER_APPROACH,
        gm_801737E8_OnLoad,
        NULL,
        NULL,
        gm_803DFE48_Scenes,
    },
    {
        1,
        GM_MENU,
        NULL,
        NULL,
        NULL,
        gm_803DD8B8_Scenes,
    },
    {
        0,
        GM_CLASSIC_GOVER,
        gm_801A50B8_OnLoad,
        NULL,
        NULL,
        gm_803DFB08,
    },
    {
        0,
        GM_ADVENTURE_GOVER,
        gm_801A5130_OnLoad,
        NULL,
        NULL,
        gm_803DFB08,
    },
    {
        0,
        GM_ALLSTAR_GOVER,
        gm_801A51A8_OnLoad,
        NULL,
        NULL,
        gm_803DFB08,
    },
    {
        1,
        GM_OPENING_MV,
        gm_801A5220_OnLoad,
        NULL,
        NULL,
        gm_803DFBC8_Scenes,
    },
    {
        0,
        GM_DEBUG_CUTSCENE,
        NULL,
        NULL,
        NULL,
        gm_803DFC70_Scenes,
    },
    {
        0,
        GM_DEBUG_GOVER,
        NULL,
        NULL,
        NULL,
        gm_803DFB80_Scenes,
    },
    {
        1,
        GM_TOURNAMENT,
        NULL,
        NULL,
        NULL,
        gm_803DDAC0_Scenes,
    },
    {
        1,
        GM_TRAINING,
        gm_801B23C4_OnLoad,
        NULL,
        gm_801B2298_OnInit,
        gm_803DDB80_Scenes,
    },
    {
        0,
        GM_TINY_VS,
        gm_801B8F98_OnLoad,
        NULL,
        gm_801B8F70_OnInit,
        gm_803DEF88_Scenes,
    },
    {
        0,
        GM_GIANT_VS,
        gm_801B91A8_OnLoad,
        NULL,
        gm_801B9180_OnInit,
        gm_803DF060_Scenes,
    },
    {
        1,
        GM_STAMINA_VS,
        gm_801B95D8_OnLoad,
        NULL,
        gm_801B95B0_OnInit,
        gm_803DF138_Scenes,
    },
    {
        1,
        GM_HOME_RUN_CONTEST,
        gm_801B9EE4_OnLoad,
        NULL,
        gm_801B9EB8_OnInit,
        gm_803DF198_Scenes,
    },
    {
        1,
        GM_10MAN_VS,
        gm_801B685C,
        NULL,
        gm_801B6834,
        gm_803DED00_Scenes,
    },
    {
        1,
        GM_100MAN_VS,
        gm_801B685C,
        NULL,
        gm_801B6834,
        gm_803DED48_Scenes,
    },
    {
        1,
        GM_3MIN_VS,
        gm_801B685C,
        NULL,
        gm_801B6834,
        gm_803DED90_Scenes,
    },
    {
        1,
        GM_15MIN_VS,
        gm_801B685C,
        NULL,
        gm_801B6834,
        gm_803DEDD8_Scenes,
    },
    {
        1,
        GM_ENDLESS_VS,
        gm_801B685C,
        NULL,
        gm_801B6834,
        gm_803DEE20_Scenes,
    },
    {
        1,
        GM_CRUEL_VS,
        gm_801B685C,
        NULL,
        gm_801B6834,
        gm_803DEE68_Scenes,
    },
    {
        0,
        GM_PROGRESSIVE_SCAN,
        NULL,
        NULL,
        NULL,
        gm_803DFDA8_Scenes,
    },
    {
        1,
        GM_BOOT,
        NULL,
        NULL,
        NULL,
        gmBoot_BootScenes,
    },
    {
        1,
        GM_MEMCARD,
        NULL,
        NULL,
        NULL,
        gmBoot_MemCardScenes,
    },
    {
        0,
        GM_FIXED_CAMERA_VS,
        gm_801BA0EC_OnLoad,
        NULL,
        gm_801BA0C4_OnInit,
        gm_803DF1E0_Scenes,
    },
    {
        1,
        GM_EVENT,
        gm_801BBEA8_OnLoad,
        gm_801BBFE4_OnUnload,
        gm_801BBA60_OnInit,
        gm_803DF618_Scenes,
    },
    {
        0,
        GM_SINGLE_BUTTON_VS,
        gm_801BA2EC_OnLoad,
        NULL,
        gm_801BA2C4_OnInit,
        gm_803DF2B8_Scenes,
    },
    {
        0,
        GM_COUNT,
        NULL,
        NULL,
        NULL,
        NULL,
    },
};

GameScene* gm_GetAllGameSceneHandlers(void)
{
    return scenes;
}

GameMode* gm_GetAllGameModes(void)
{
    return modes;
}
