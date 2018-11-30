#ifndef STEPPER_MOTORS_H_
#define STEPPER_MOTORS_H_

#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim1;

void Oil_Spindle(void);
void HandleOiling(void);

#endif /* STEPPER_MOTORS_H_ */