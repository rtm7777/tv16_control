#ifndef MAX7219_H_
#define MAX7219_H_

#include "stm32f1xx_hal.h"

int Symbol(int number);
int SymbolWithDot (int number);
void Send_7219 (int chipNumber, uint8_t rg, uint8_t dt);
void Clear_7219 (int chipNumber);
void SetFreq_7219 (int frequency);
void SetCurrent_7219 (int current);
void UpdateFreqCurrent_2719 (int chipNumber);
void Number_7219_non_decoding (int chipNumber, volatile long n);
void Number_7219_dot (int chipNumber, volatile long n);
void Init_7219 ();

#endif /* MAX7219_H_ */
