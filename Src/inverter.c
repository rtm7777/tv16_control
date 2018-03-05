#include "inverter.h"

//-----------------------------------------------------
void StartInverter(int direction)
{
  if (direction && HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12))
  {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
  }
  else if (direction && HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15))
  {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
  }
}
//------------------------------------------------------
void StopInverter()
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
}
//------------------------------------------------------
