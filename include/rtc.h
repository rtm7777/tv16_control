#ifndef __rtc_H
#define __rtc_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "main.h"

extern RTC_HandleTypeDef hrtc;

void MX_RTC_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__ rtc_H */
