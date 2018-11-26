#include "encoder.h"
#include "max7219.h"

uint16_t counter = 150;
uint16_t counterMin = 20;
uint16_t counterMax = 260;
uint8_t abOld = 0;

void Handle_Encoder_Change(void)
{
  enum { upMask = 0x66, downMask = 0x99 };
  uint8_t abNew = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) << 1) | HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11);
  uint8_t criterion = abNew^abOld;
  if (criterion==1 || criterion==2) {
    if (upMask & (1 << (2*abOld + abNew/2)))
    {
      if (counter < counterMax)
      {
        counter++;
      }
    }
    else if (counter > counterMin)
    {
      counter--;       // upMask = ~downMask
    }
  }
  abOld = abNew;        // Save new state


  SetFreq_7219(counter/4);
  UpdateFreqCurrent_2719(1);
}
