#include "global.h"
#include "sprite.h"
#include "trig.h"
#include "rng.h"
#include "decompress.h"
#include "task.h"
#include "sound.h"
#include "songs.h"
#include "palette.h"

// this file's functions
static void EvoSparkle_DummySpriteCb(struct Sprite* sprite);
static void EvoTask_BeginPreSet1_FadeAndPlaySE(u8 taskID);
static void EvoTask_CreatePreEvoSparkleSet1(u8 taskID);
static void EvoTask_WaitForPre1SparklesToGoUp(u8 taskID);
static void EvoTask_BeginPreSparklesSet2(u8 taskID);
static void EvoTask_CreatePreEvoSparklesSet2(u8 taskID);
static void EvoTask_DestroyPreSet2Task(u8 taskID);
static void EvoTask_BeginPostSparklesSet1(u8 taskID);
static void EvoTask_CreatePostEvoSparklesSet1(u8 taskID);
static void EvoTask_DestroyPostSet1Task(u8 taskID);
static void EvoTask_BeginPostSparklesSet2_AndFlash(u8 taskID);
static void EvoTask_CreatePostEvoSparklesSet2_AndFlash(u8 taskID);
static void EvoTask_BeginPostSparklesSet2_AndFlash_Trade(u8 taskID);
static void EvoTask_CreatePostEvoSparklesSet2_AndFlash_Trade(u8 taskID);
static void EvoTask_DestroyPostSet2AndFlashTask(u8 taskID);

// const data
static const u16 sEvoSparklePalette[] = INCBIN_U16("graphics/misc/evo_sparkle.gbapal");
static const u8 sEvoSparkleTiles[] = INCBIN_U8("graphics/misc/evo_sparkle.4bpp.lz");

static const struct CompressedSpriteSheet sEvoSparkleSpriteSheets[] =
{
    {sEvoSparkleTiles, 0x20, 1001},
    {NULL, 0, 0}
};
static const struct SpritePalette sEvoSparkleSpritePals[] =
{
    {sEvoSparklePalette, 1001},
    {NULL, 0}
};

static const struct OamData sOamData_EvoSparkle =
{
    .y = 160,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 0,
    .x = 0,
    .matrixNum = 0,
    .size = 0,
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sSpriteAnim_EvoSparkle[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_EvoSparkle[] =
{
    sSpriteAnim_EvoSparkle,
};

static const struct SpriteTemplate sEvoSparkleSpriteTemplate =
{
    .tileTag = 1001,
    .paletteTag = 1001,
    .oam = &sOamData_EvoSparkle,
    .anims = sSpriteAnimTable_EvoSparkle,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = EvoSparkle_DummySpriteCb
};

static const s16 sEvoSparkleMatricies[] =
{
    0x3C0, 0x380, 0x340, 0x300, 0x2C0, 0x280, 0x240, 0x200, 0x1C0,
    0x180, 0x140, 0x100, -4, 0x10, -3, 0x30, -2, 0x50,
    -1, 0x70, 0x1, 0x70, 0x2, 0x50, 0x3, 0x30, 0x4, 0x10
};

// code

static void EvoSparkle_DummySpriteCb(struct Sprite* sprite)
{

}

static void SetEvoSparklesMatrices(void)
{
    u16 i;
    for (i = 0; i < 12; i++)
    {
        SetOamMatrix(20 + i, sEvoSparkleMatricies[i], 0, 0, sEvoSparkleMatricies[i]);
    }
}

static void SpriteCB_PreEvoSparkleSet1(struct Sprite* sprite)
{
    if (sprite->pos1.y > 8)
    {
        u8 matrixNum;

        sprite->pos1.y = 88 - (sprite->data7 * sprite->data7) / 80;
        sprite->pos2.y = Sin((u8)(sprite->data6), sprite->data5) / 4;
        sprite->pos2.x = Cos((u8)(sprite->data6), sprite->data5);
        sprite->data6 += 4;
        if (sprite->data7 & 1)
            sprite->data5--;
        sprite->data7++;
        if (sprite->pos2.y > 0)
            sprite->subpriority = 1;
        else
            sprite->subpriority = 20;
        matrixNum = sprite->data5 / 4 + 20;
        if (matrixNum > 31)
            matrixNum = 31;
        sprite->oam.matrixNum = matrixNum;
    }
    else
        DestroySprite(sprite);
}

static void CreatePreEvoSparkleSet1(u8 arg0)
{
    u8 spriteID = CreateSprite(&sEvoSparkleSpriteTemplate, 120, 88, 0);
    if (spriteID != MAX_SPRITES)
    {
        gSprites[spriteID].data5 = 48;
        gSprites[spriteID].data6 = arg0;
        gSprites[spriteID].data7 = 0;
        gSprites[spriteID].oam.affineMode = 1;
        gSprites[spriteID].oam.matrixNum = 31;
        gSprites[spriteID].callback = SpriteCB_PreEvoSparkleSet1;
    }
}

static void SpriteCB_PreEvoSparkleSet2(struct Sprite* sprite)
{
    if (sprite->pos1.y < 88)
    {
        sprite->pos1.y = 8 + (sprite->data7 * sprite->data7) / 5;
        sprite->pos2.y = Sin((u8)(sprite->data6), sprite->data5) / 4;
        sprite->pos2.x = Cos((u8)(sprite->data6), sprite->data5);
        sprite->data5 = 8 + Sin((u8)(sprite->data7 * 4), 40);
        sprite->data7++;
    }
    else
        DestroySprite(sprite);
}

static void CreatePreEvoSparkleSet2(u8 arg0)
{
    u8 spriteID = CreateSprite(&sEvoSparkleSpriteTemplate, 120, 8, 0);
    if (spriteID != MAX_SPRITES)
    {
        gSprites[spriteID].data5 = 8;
        gSprites[spriteID].data6 = arg0;
        gSprites[spriteID].data7 = 0;
        gSprites[spriteID].oam.affineMode = 1;
        gSprites[spriteID].oam.matrixNum = 25;
        gSprites[spriteID].subpriority = 1;
        gSprites[spriteID].callback = SpriteCB_PreEvoSparkleSet2;
    }
}

static void SpriteCB_PostEvoSparkleSet1(struct Sprite* sprite)
{
    if (sprite->data5 > 8)
    {
        sprite->pos2.y = Sin((u8)(sprite->data6), sprite->data5);
        sprite->pos2.x = Cos((u8)(sprite->data6), sprite->data5);
        sprite->data5 -= sprite->data3;
        sprite->data6 += 4;
    }
    else
        DestroySprite(sprite);
}

static void CreatePostEvoSparkleSet1(u8 arg0, u8 arg1)
{
    u8 spriteID = CreateSprite(&sEvoSparkleSpriteTemplate, 120, 56, 0);
    if (spriteID != MAX_SPRITES)
    {
        gSprites[spriteID].data3 = arg1;
        gSprites[spriteID].data5 = 120;
        gSprites[spriteID].data6 = arg0;
        gSprites[spriteID].data7 = 0;
        gSprites[spriteID].oam.affineMode = 1;
        gSprites[spriteID].oam.matrixNum = 31;
        gSprites[spriteID].subpriority = 1;
        gSprites[spriteID].callback = SpriteCB_PostEvoSparkleSet1;
    }
}

static void SpriteCB_PostEvoSparkleSet2(struct Sprite* sprite)
{
    if (!(sprite->data7 & 3))
        sprite->pos1.y++;
    if (sprite->data6 < 128)
    {
        u8 matrixNum;

        sprite->pos2.y = -Sin((u8)(sprite->data6), sprite->data5);
        sprite->pos1.x = 120 + (sprite->data3 * sprite->data7) / 3;
        sprite->data6++;
        matrixNum = 31 - (sprite->data6 * 12 / 128);
        if (sprite->data6 > 64)
            sprite->subpriority = 1;
        else
        {
            sprite->invisible = 0;
            sprite->subpriority = 20;
            if (sprite->data6 > 112 && sprite->data6 & 1)
                sprite->invisible = 1;
        }
        if (matrixNum < 20)
            matrixNum = 20;
        sprite->oam.matrixNum = matrixNum;
        sprite->data7++;
    }
    else
        DestroySprite(sprite);
}

void CreatePostEvoSparkleSet2(u8 arg0)
{
    u8 spriteID = CreateSprite(&sEvoSparkleSpriteTemplate, 120, 56, 0);
    if (spriteID != MAX_SPRITES)
    {
        gSprites[spriteID].data3 = 3 - (Random() % 7);
        gSprites[spriteID].data5 = 48 + (Random() & 0x3F);
        gSprites[spriteID].data7 = 0;
        gSprites[spriteID].oam.affineMode = 1;
        gSprites[spriteID].oam.matrixNum = 31;
        gSprites[spriteID].subpriority = 20;
        gSprites[spriteID].callback = SpriteCB_PostEvoSparkleSet2;
    }
}

void LoadEvoSparkleSpriteAndPal(void)
{
    LoadCompressedObjectPic(&sEvoSparkleSpriteSheets[0]);
    LoadSpritePalettes(sEvoSparkleSpritePals);
}

u8 LaunchTask_PreEvoSparklesSet1(u16 arg0)
{
    u8 taskID = CreateTask(EvoTask_BeginPreSet1_FadeAndPlaySE, 0);
    gTasks[taskID].data[1] = arg0;
    return taskID;
}

#define tFrameCounter   data[15]

static void EvoTask_BeginPreSet1_FadeAndPlaySE(u8 taskID)
{
    SetEvoSparklesMatrices();
    gTasks[taskID].tFrameCounter = 0;
    BeginNormalPaletteFade(3 << gTasks[taskID].data[1], 0xA, 0, 0x10, 0x7FFF);
    gTasks[taskID].func = EvoTask_CreatePreEvoSparkleSet1;
    PlaySE(SE_W025);
}

static void EvoTask_CreatePreEvoSparkleSet1(u8 taskID)
{
    if (gTasks[taskID].tFrameCounter < 64)
    {
        if (!(gTasks[taskID].tFrameCounter & 7))
        {
            u8 i;
            for (i = 0; i < 4; i++)
                CreatePreEvoSparkleSet1((0x78 & gTasks[taskID].tFrameCounter) * 2 + i * 64);
        }
        gTasks[taskID].tFrameCounter++;
    }
    else
    {
        gTasks[taskID].tFrameCounter = 96;
        gTasks[taskID].func = EvoTask_WaitForPre1SparklesToGoUp;
    }
}

static void EvoTask_WaitForPre1SparklesToGoUp(u8 taskID)
{
    if (gTasks[taskID].tFrameCounter != 0)
        gTasks[taskID].tFrameCounter--;
    else
        DestroyTask(taskID);
}

u8 LaunchTask_PreEvoSparklesSet2(void)
{
    return CreateTask(EvoTask_BeginPreSparklesSet2, 0);
}

static void EvoTask_BeginPreSparklesSet2(u8 taskID)
{
    SetEvoSparklesMatrices();
    gTasks[taskID].tFrameCounter = 0;
    gTasks[taskID].func = EvoTask_CreatePreEvoSparklesSet2;
    PlaySE(SE_W062B);
}

static void EvoTask_CreatePreEvoSparklesSet2(u8 taskID)
{
    if (gTasks[taskID].tFrameCounter < 96)
    {
        if (gTasks[taskID].tFrameCounter < 6)
        {
            u8 i;
            for (i = 0; i < 9; i++)
                CreatePreEvoSparkleSet2(i * 16);
        }
        gTasks[taskID].tFrameCounter++;
    }
    else
        gTasks[taskID].func = EvoTask_DestroyPreSet2Task;
}

static void EvoTask_DestroyPreSet2Task(u8 taskID)
{
    DestroyTask(taskID);
}

u8 LaunchTask_PostEvoSparklesSet1(void)
{
    return CreateTask(EvoTask_BeginPostSparklesSet1, 0);
}

static void EvoTask_BeginPostSparklesSet1(u8 taskID)
{
    SetEvoSparklesMatrices();
    gTasks[taskID].tFrameCounter = 0;
    gTasks[taskID].func = EvoTask_CreatePostEvoSparklesSet1;
    PlaySE(SE_REAPOKE);
}

static void EvoTask_CreatePostEvoSparklesSet1(u8 taskID)
{
    if (gTasks[taskID].tFrameCounter < 48)
    {
        if (gTasks[taskID].tFrameCounter == 0)
        {
            u8 i;
            for (i = 0; i < 16; i++)
                CreatePostEvoSparkleSet1(i * 16, 4);
        }
        if (gTasks[taskID].tFrameCounter == 32)
        {
            u8 i;
            for (i = 0; i < 16; i++)
                CreatePostEvoSparkleSet1(i * 16, 8);
        }
        gTasks[taskID].tFrameCounter++;
    }
    else
        gTasks[taskID].func = EvoTask_DestroyPostSet1Task;
}

static void EvoTask_DestroyPostSet1Task(u8 taskID)
{
    DestroyTask(taskID);
}

u8 LaunchTask_PostEvoSparklesSet2AndFlash(u16 arg0)
{
    u8 taskID = CreateTask(EvoTask_BeginPostSparklesSet2_AndFlash, 0);
    gTasks[taskID].data[2] = arg0;
    return taskID;
}

static void EvoTask_BeginPostSparklesSet2_AndFlash(u8 taskID)
{
    SetEvoSparklesMatrices();
    gTasks[taskID].tFrameCounter = 0;
    CpuSet(&gPlttBufferFaded[0x20], &gPlttBufferUnfaded[0x20], 0x30);
    BeginNormalPaletteFade(0xFFF9001C, 0, 0, 0x10, 0x7FFF);
    gTasks[taskID].func = EvoTask_CreatePostEvoSparklesSet2_AndFlash;
    PlaySE(SE_W080);
}

static void EvoTask_CreatePostEvoSparklesSet2_AndFlash(u8 taskID)
{
    if (gTasks[taskID].tFrameCounter < 128)
    {
        u8 i;
        switch (gTasks[taskID].tFrameCounter)
        {
        default:
            if (gTasks[taskID].tFrameCounter < 50)
                CreatePostEvoSparkleSet2(Random() & 7);
            break;
        case 0:
            for (i = 0; i < 8; i++)
                CreatePostEvoSparkleSet2(i);
            break;
        case 32:
            BeginNormalPaletteFade(0xFFFF001C, 0x10, 0x10, 0, 0x7FFF);
            break;
        }
        gTasks[taskID].tFrameCounter++;
    }
    else
        gTasks[taskID].func = EvoTask_DestroyPostSet2AndFlashTask;
}

static void EvoTask_DestroyPostSet2AndFlashTask(u8 taskID)
{
    if (!gPaletteFade.active)
        DestroyTask(taskID);
}

u8 LaunchTask_PostEvoSparklesSet2AndFlash_Trade(u16 arg0)
{
    u8 taskID = CreateTask(EvoTask_BeginPostSparklesSet2_AndFlash_Trade, 0);
    gTasks[taskID].data[2] = arg0;
    return taskID;
}

static void EvoTask_BeginPostSparklesSet2_AndFlash_Trade(u8 taskID)
{
    SetEvoSparklesMatrices();
    gTasks[taskID].tFrameCounter = 0;
    CpuSet(&gPlttBufferFaded[0x20], &gPlttBufferUnfaded[0x20], 0x30);
    BeginNormalPaletteFade(0xFFF90001, 0, 0, 0x10, 0x7FFF);
    gTasks[taskID].func = EvoTask_CreatePostEvoSparklesSet2_AndFlash_Trade;
    PlaySE(SE_W080);
}

static void EvoTask_CreatePostEvoSparklesSet2_AndFlash_Trade(u8 taskID)
{
    if (gTasks[taskID].tFrameCounter < 128)
    {
        u8 i;
        switch (gTasks[taskID].tFrameCounter)
        {
        default:
            if (gTasks[taskID].tFrameCounter < 50)
                CreatePostEvoSparkleSet2(Random() & 7);
            break;
        case 0:
            for (i = 0; i < 8; i++)
                CreatePostEvoSparkleSet2(i);
            break;
        case 32:
            BeginNormalPaletteFade(0xFFFF0001, 0x10, 0x10, 0, 0x7FFF);
            break;
        }
        gTasks[taskID].tFrameCounter++;
    }
    else
        gTasks[taskID].func = EvoTask_DestroyPostSet2AndFlashTask;
}

