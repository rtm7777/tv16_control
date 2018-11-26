#ifndef __spi_H
#define __spi_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;

extern void _Error_Handler(char *, int);

void MX_SPI1_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__ spi_H */
