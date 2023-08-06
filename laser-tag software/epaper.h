#ifndef _EPAPER_H_
#define _EPAPER_H_

#include <stdint.h>

#ifdef BADGE_V1
#define EPD_W (232)
#define EPD_H (128)
#else
#define EPD_W (264)
#define EPD_H (176)
#endif

void EPD_Init();
int EPD_Draw(const uint8_t *old_image, const uint8_t *new_image);

#endif
