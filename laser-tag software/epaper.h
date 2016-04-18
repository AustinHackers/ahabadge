#ifndef _EPAPER_H_
#define _EPAPER_H_

#include <stdint.h>

void EPD_Init();
int EPD_Draw(const uint8_t *old_image, const uint8_t *new_image);
void EPD_Tick();

#endif
