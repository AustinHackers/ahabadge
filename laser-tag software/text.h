#ifndef _TEXT_H_
#define _TEXT_H_

#include <stdint.h>

#define EPAPER_WIDTH 232
#define EPAPER_HEIGHT 128

#ifndef GLITCHDEBUG
#define printf(...)
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

uint8_t text_to_image(const char* ptext, uint8_t length, uint8_t *pimage, uint8_t offset_x, uint8_t offset_y, uint8_t upper_x, uint8_t upper_y);

#endif
