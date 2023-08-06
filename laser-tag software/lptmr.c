#include "lptmr.h"

static const lptmr_user_config_t g_lptmrConfig = {
    .timerMode = kLptmrTimerModeTimeCounter,
    .freeRunningEnable = false,
    .prescalerEnable = false,
    .prescalerClockSource = kClockLptmrSrcLpoClk,
    .isInterruptEnabled = true,
};

static lptmr_state_t lptmrState;
static lptmr_callback_t user_cb;
static uint32_t total_ms;

static void lptmr_call_back(void)
{
  total_ms += 100;
  user_cb();
}

uint32_t get_ms() {
  uint32_t us = LPTMR_DRV_GetCurrentTimeUs(LPTMR0_IDX);
  return total_ms + us / 1000;
}

void delay_ms(uint32_t delay)
{
  uint32_t until = get_ms() + delay;
  while(get_ms() < until);
}

void lptmr_init(lptmr_callback_t cb)
{
    LPTMR_DRV_Init(LPTMR0_IDX, &lptmrState, &g_lptmrConfig);
    LPTMR_DRV_SetTimerPeriodUs(LPTMR0_IDX, 100000);
    user_cb = cb;
    total_ms = 0;
    LPTMR_DRV_InstallCallback(LPTMR0_IDX, lptmr_call_back);
}

void lptmr_start()
{
  LPTMR_DRV_Start(LPTMR0_IDX);
}
/* vim: set expandtab ts=4 sw=4: */
