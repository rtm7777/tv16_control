#include "max7219.h"

uint8_t aTxBuf[1]={0};
extern SPI_HandleTypeDef hspi1;
char dg0=5;
char dg1=8;

int freq = 5;
int current = 0;

char symbols[]={
  0x7e, // 0
  0x30, // 1
  0x6d, // 2
  0x79, // 3
  0x33, // 4
  0x5b, // 5
  0x5f, // 6
  0x70, // 7
  0x7f, // 8
  0x7b, // 9
  0x01, // -
  0x00, // ""
  0x09, // =

  0x77, // A
  0x1F, // b
  0x4E, // C
  0x3D, // d
  0x4F, // E
  0x47, // F
  0x5e, // G
  0x37, // H
  0x06, // I
  0x3C, // J
  0x0E, // L
  0x15, // n
  0x1D, // o
  0x67, // P
  0x73, // q
  0x05, // r
  0x5B, // S
  0x0F, // t
  0x3E, // U
  0x3B, // Y
};

#define cs0_set() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define cs1_set() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET)
#define cs0_reset() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define cs1_reset() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET)

int Symbol(int number)
{
  return symbols[number];
}

int SymbolWithDot(int symbol)
{
  return symbol |= 1 << 7;
}

//------------------------------------------------------
void Send_7219 (int chipNumber, uint8_t rg, uint8_t dt)
{
  (chipNumber) ? (cs1_set()) : (cs0_set());

  aTxBuf[0]=rg;
  HAL_SPI_Transmit (&hspi1, (uint8_t*)aTxBuf, 1, 5000);
  aTxBuf[0]=dt;
  HAL_SPI_Transmit (&hspi1, (uint8_t*)aTxBuf, 1, 5000);

  (chipNumber) ? (cs1_reset()) : (cs0_reset());
}

//------------------------------------------------------
void Clear_7219(int chipNumber)
{
  uint8_t i=dg0;

  if (chipNumber)
    i = dg1;

  do
  {
    Send_7219(chipNumber, i, 0x00); //for non decoding mode
    // Send_7219(i,0xF); //for decoding mode
  } while (--i);
}

//------------------------------------------------------
void Number_7219_non_decoding(int chipNumber, volatile long n)
{
  uint8_t ng=0;
  if(n<0)
  {
    ng=1;
    n*=-1;
  }
  uint8_t i=0;
  do
  {
    Send_7219(chipNumber, ++i, symbols[n%10]);
    n/=10;
  } while(n);
  if(ng)
  {
    Send_7219(chipNumber, i+1, symbols[10]);
  }
}

void SetFreq_7219(int frequency)
{
  freq = frequency;
}

void SetCurrent_7219(int current)
{
  current = current;
}

void UpdateFreqCurrent_2719(int chipNumber)
{
  Clear_7219(1);

  if (current < 10)
  {
    Send_7219(chipNumber, 1, symbols[current]);
    Send_7219(chipNumber, 2, SymbolWithDot(symbols[0]));
  }
  else
  {
    Send_7219(chipNumber, 1, symbols[current%10]);
    Send_7219(chipNumber, 2, SymbolWithDot(symbols[(current/10)%10]));
  }
  Send_7219(chipNumber, 3, symbols[12]);
  Send_7219(chipNumber, 4, symbols[1]);
  if (freq < 10)
  {
    Send_7219(chipNumber, 5, symbols[11]);
    Send_7219(chipNumber, 6, symbols[freq]);
  }
  else
  {
    Send_7219(chipNumber, 5, symbols[freq%10]);
    Send_7219(chipNumber, 6, symbols[(freq/10)%10]);
  }
  Send_7219(chipNumber, 7, symbols[12]);
  Send_7219(chipNumber, 8, symbols[18]);
}

//------------------------------------------------------
void Number_7219_dot(int chipNumber, volatile long n)
{
  if (n == 0)
  {
    Send_7219(chipNumber, 1, symbols[0]);
    Send_7219(chipNumber, 2, symbols[0]);
    Send_7219(chipNumber, 3, SymbolWithDot(symbols[0]));
    return;
  }
  uint8_t i=0;
  do
  {
    i++;
    if (i == 3)
    {
      Send_7219(chipNumber, i, SymbolWithDot(symbols[n%10]));
    }
    else
    {
      Send_7219(chipNumber, i, symbols[n%10]);
    }
    n/=10;
  } while(n);
  if (i < 3)
  {
    Send_7219(chipNumber, i+1, SymbolWithDot(symbols[0]));
  }
}

//-------------------------------------------------------
void Init_7219(void)
{
  Send_7219(0, 0x09, 0x00); // decoding mode
  Send_7219(0, 0x0B, dg0-1); // number of used digits
  Send_7219(0, 0x0A, 0x05); // light intensity
  Send_7219(0, 0x0C, 0x01);
  Clear_7219(0);

  Send_7219(1, 0x09, 0x00); // decoding mode
  Send_7219(1, 0x0B, dg1-1); // number of used digits
  Send_7219(1, 0x0A, 0x0A); // light intensity
  Send_7219(1, 0x0C, 0x01);
  Clear_7219(1);
}
