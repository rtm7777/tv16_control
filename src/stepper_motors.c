#include "stepper_motors.h"
#include "rtc.h"

// 10000 - spindle revolutions count before next oiling
const uint32_t spindle_oil_count = 10000 * 80;

uint8_t oiling = 0;
uint8_t oiling_count = 0;
uint16_t spindle_revs_count = 0;

void Oil_Spindle(void)
{
  if (!oiling)
  {
    oiling = 1;
  }
}

void Update_Spindle_Revs(uint8_t count)
{
  spindle_revs_count += count;
  if (spindle_revs_count >= spindle_oil_count)
  {
    spindle_revs_count = 0;
    Oil_Spindle();
  }
}

void HandleOiling(void)
{
  if (oiling)
  {
    if (!oiling_count)
    {
      HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    }
    oiling_count++;
    if (oiling_count >= 3)
    {
      oiling = 0;
      oiling_count = 0;
      HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_1);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
    }
  }
}