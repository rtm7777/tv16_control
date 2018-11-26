#ifndef INVERTER_H_
#define INVERTER_H_

#include "stm32f1xx_hal.h"

void StartInverter(int direction);
void StopInverter(void);

#endif /* INVERTER_H_ */