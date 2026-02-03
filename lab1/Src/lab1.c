#include "main.h"
#include "stm32f0xx_hal.h"
#include <assert.h>

void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();

  /* This example uses HAL library calls to control
     the GPIOC peripheral. You’ll be redoing this code
     with hardware register access. */

  My_HAL_RCC_GPIOC_CLK_Enable(); // Enable the GPIOC clock
  My_HAL_RCC_GPIOA_CLK_Enable(); // Enable the GPIOC clock
  assert((RCC->AHBENR & RCC_AHBENR_GPIOCEN) != 0);

  GPIO_InitTypeDef btnStr = { GPIO_PIN_0, 0, 0, 0 };
  My_HAL_GPIO_Init(GPIOA, &btnStr);

  // Set up config struc to pass to the initializer
  GPIO_InitTypeDef initStr = {GPIO_PIN_6 | GPIO_PIN_7,
                              GPIO_MODE_OUTPUT_PP,
                              GPIO_SPEED_FREQ_LOW,
                              GPIO_NOPULL};
  My_HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins PC6 & PC7
  assert(((GPIOC->OTYPER >> 6) & 0x1) == 0);
  assert(((GPIOC->OTYPER >> 7) & 0x1) == 0);

  assert(((GPIOC->OSPEEDR >> (2*6)) & 0x3) == 0);
  assert(((GPIOC->OSPEEDR >> (2*7)) & 0x3) == 0);

  assert(((GPIOC->PUPDR >> (2*6)) & 0x3) == 0);
  assert(((GPIOC->PUPDR >> (2*7)) & 0x3) == 0);

  My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
  My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);

  uint32_t debouncer = 0;

  while (1) {
      debouncer <<= 1;

      uint32_t input_signal =
          (My_HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET);

      if (input_signal) {
          debouncer |= 1u;
      }

      // one-shot when it becomes steadily high (press)
      if (debouncer == 0x7FFFFFFF) {
          My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7);
      }
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add their own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add their own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */
