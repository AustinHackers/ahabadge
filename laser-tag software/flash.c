#include <stdint.h>
#include <string.h>
#include "flash.h"
#include "fsl_debug_console.h"
#include "SSD_FTFx.h"

FLASH_SSD_CONFIG flash_config = {
    .ftfxRegBase = FTFA_BASE,
    .PFlashBase = 0x00000000,
    .PFlashSize = FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE
        * FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT,
    .DFlashBase = 0,
    .DFlashSize = 0,
    .EERAMBase = 0,
    .EEESize = 0,
    .DebugEnable = 0,
    .CallBack = NULL_CALLBACK,
};

uint8_t ram_fn[0x30];
pFLASHCOMMANDSEQUENCE pFlashCommandSequence;
uint8_t *images[IMAGE_COUNT];

int flash_init()
{
    uint32_t rc;

    rc = FlashInit(&flash_config);
    debug_printf("FlashInit returned %u\r\n", rc);
    if (FTFx_OK != rc) {
        return -1;
    }
    pFlashCommandSequence = (pFLASHCOMMANDSEQUENCE)RelocateFunction(
        (intptr_t)ram_fn,
        sizeof ram_fn,
        (intptr_t)FlashCommandSequence);

    /* Our images are in the 2nd block and take up 6k bytes */
    int i;
    for (i = 0; i < IMAGE_COUNT; ++i) {
        images[i] = (uint8_t *)flash_config.PFlashBase
            + FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE + i * 1024 * 6;
    }
    return 0;
}

#define SECTOR_SIZE (FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE)

int flash_write_sector(uint32_t addr, uint8_t *data)
{
    debug_printf("flash_write_sector to 0x%08x\r\n", addr);
    if (addr % SECTOR_SIZE) {
        debug_printf("bad addr\r\n");
        return -1;
    }
    if (memcmp((void *)addr, data, SECTOR_SIZE) == 0) {
        debug_printf("No change\r\n");
        return 0;
    }
    uint32_t rc = FlashEraseSector(&flash_config, addr, SECTOR_SIZE,
        pFlashCommandSequence);
    if (FTFx_OK != rc) {
        debug_printf("Error erasing sector 0x%08x: %u\r\n", addr, rc);
        return -1;
    }
    rc = FlashProgram(&flash_config, addr, SECTOR_SIZE, data,
        pFlashCommandSequence);
    if (FTFx_OK != rc) {
        debug_printf("Error programming sector 0x%08x: %u\r\n", addr, rc);
        return -1;
    }
    return 0;
}

/* vim: set expandtab ts=4 sw=4: */
