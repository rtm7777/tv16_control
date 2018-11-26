#include "main.h"
#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "spi.h"
#include "tim.h"
#include "max7219.h"
#include "inverter.h"

__STATIC_INLINE void DelayMicro(__IO uint32_t micros)
{
  micros *= (SystemCoreClock / 1000000) / 9;
  /* Wait till done */
  while (micros--) ;
}

int divider_mode = 0; // PC14: 0 - spead measurement mode, 1 - divider mode

void SystemClock_Config(void);

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();

  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Encoder_Start_IT(&htim4, TIM_CHANNEL_1);
  HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_1);

  HAL_Delay(100);
  Init_7219();
  Clear_7219(0);
  Clear_7219(1);

  while (1)
  {
    HAL_Delay(100);

    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) != divider_mode)
    {
      if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == 0)
      {
        divider_mode = 0;
        __HAL_TIM_SET_AUTORELOAD(&htim3, 1499);
        __HAL_TIM_SET_AUTORELOAD(&htim4, 64799);
        TIM3->EGR = TIM_EGR_UG;
        TIM4->EGR = TIM_EGR_UG;

      }
      else
      {
        divider_mode = 1;
        __HAL_TIM_SET_AUTORELOAD(&htim3, 200);
        __HAL_TIM_SET_AUTORELOAD(&htim4, 2399);
        TIM3->EGR = TIM_EGR_UG;
        TIM4->EGR = TIM_EGR_UG;
      }
    }
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_RCC_EnableCSS();

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance==TIM3)
  {
    int counter = __HAL_TIM_GET_COUNTER(&htim4);
    if (divider_mode) {
      int degree = 0;
      if (counter == 0)
      {
        degree = 0;
      } else {
        degree = counter * 15;
      }
      Clear_7219(0);
      Number_7219_dot(0, degree);
    }
    else
    {
      TIM4->CNT = 0x0000;
      int speed = 0;
      if (__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4))
      {
        speed = (64799 - counter) / 30;
        if (counter == 0)
        {
          speed = 0;
        }
      }
      else
      {
        speed = counter / 30;
      }
      Clear_7219(0);
      Number_7219_non_decoding(0, speed);
    }
  }

}

void _Error_Handler(char *file, int line)
{
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{ 

}
#endif /* USE_FULL_ASSERT */
