#include "stepper_motors.h"
#include "rtc.h"

uint8_t oiling = 0;
uint8_t oiling_count = 0;

void Oil_Spindle(void)
{
  if (!oiling)
  {
    oiling = 1;
  }
}

void HandleOiling(void)
{
  if (oiling)
  {
    oiling_count++;
    HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
    if (oiling_count >= 3)
    {
      oiling = 0;
      oiling_count = 0;
      HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_1);
    }
  }
}