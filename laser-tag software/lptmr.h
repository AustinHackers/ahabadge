#ifndef _LPTMR_H_
#define _LPTMR_H_
#include "fsl_lptmr_driver.h"

uint32_t get_ms();
void delay_ms(uint32_t delay);
void lptmr_init(lptmr_callback_t cb);
void lptmr_start();
#endif
/* vim: set expandtab ts=4 sw=4: */
