#ifndef _FLASH_H_
#define _FLASH_H_

#include <stdint.h>

#define IMAGE_COUNT	(5)
extern uint8_t *images[IMAGE_COUNT];

int flash_init();
int flash_write(uint32_t addr, uint8_t *data, uint32_t len);

#endif
