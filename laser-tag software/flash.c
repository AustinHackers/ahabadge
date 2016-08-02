#include <stdint.h>
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
    return 0;
}

/* vim: set expandtab ts=4 sw=4: */
