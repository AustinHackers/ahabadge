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

    /* Our images are in the 2nd block and take up 4096 bytes */
    int i;
    for (i = 0; i < IMAGE_COUNT; ++i) {
        images[i] = (uint8_t *)flash_config.PFlashBase
            + FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE + i * 4096;
    }
    return 0;
}

#define SECTOR_SIZE (FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE)
#define MIN(a, b)   ((a) < (b) ? (a) : (b))
uint8_t RMW_buffer[SECTOR_SIZE];

int flash_write(uint32_t addr, uint8_t *data, uint32_t len)
{
    uint32_t sector;
    uint32_t end = addr + len;

    for (sector = addr / SECTOR_SIZE * SECTOR_SIZE;
            sector < end;
            data += SECTOR_SIZE - (addr - sector),
                len -= SECTOR_SIZE - (addr - sector),
                sector += SECTOR_SIZE,
                addr = sector) {
        uint32_t rc;

        memcpy(RMW_buffer, (void *)sector, SECTOR_SIZE);
        memcpy(&RMW_buffer[addr - sector],
                data,
                MIN(SECTOR_SIZE - (addr - sector), len));
        rc = FlashEraseSector(&flash_config, sector, SECTOR_SIZE,
                pFlashCommandSequence);
        if (FTFx_OK != rc) {
            debug_printf("Error erasing sector 0x%08x: %u\r\n", sector, rc);
            return -1;
        }
        rc = FlashProgram(&flash_config, sector, SECTOR_SIZE, RMW_buffer,
                pFlashCommandSequence);
        if (FTFx_OK != rc) {
            debug_printf("Error programming sector 0x%08x: %u\r\n", sector, rc);
            return -1;
        }
    }
    return 0;
}

/* vim: set expandtab ts=4 sw=4: */
