#ifndef __tim_H
#define __tim_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

extern void _Error_Handler(char *, int);

void MX_TIM3_Init(void);
void MX_TIM4_Init(void);


#ifdef __cplusplus
}
#endif
#endif /*__ tim_H */
