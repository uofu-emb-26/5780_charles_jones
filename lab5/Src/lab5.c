#include "main.h"
#include "stm32f0xx_hal.h"
#include <assert.h>
#include "hal_gpio.h"

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

  // DEBUG LED
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  GPIOC->MODER |= (1 << (2*6)) | (1 << (2*7)) | (1 << (2*8)) | (1 << (2*9));

  My_HAL_RCC_GPIOB_CLK_Enable(); // Enable the GPIOC clock
  My_HAL_RCC_GPIOC_CLK_Enable(); // Enable the GPIOB clock

  GPIO_InitTypeDef initStr = {0};
  My_HAL_GPIO_Init(GPIOB, &initStr); // Initialize pins
  My_HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins

  RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

  // set TIMINGR registers
  I2C_TIMINGR();
  
  // Enable I2C
  I2C2->CR1 = I2C_CR1_PE;

  I2C2->CR2 = 0;
  I2C2->CR2 |= (0x69 << 1);
  I2C2->CR2 |= (2 << 16);
  I2C2->CR2 |= (0 << 10);
  I2C2->CR2 |= (1 << 13);

  while(!(I2C2->ISR & I2C_ISR_TXIS) && !(I2C2->ISR & I2C_ISR_NACKF));
  I2C2->TXDR = 0x20;

  while(!(I2C2->ISR & I2C_ISR_TXIS) && !(I2C2->ISR & I2C_ISR_NACKF));
  I2C2->TXDR = 0x0B;

  while(!(I2C2->ISR & I2C_ISR_TC));

  I2C2->CR2 |= I2C_CR2_STOP;

  while (1)
{
  uint8_t xl, xh, yl, yh;
  int16_t x, y;

  I2C2->CR2 = 0;
  I2C2->CR2 |= (0x69 << 1);
  I2C2->CR2 |= (1 << 16);
  I2C2->CR2 |= (0 << 10);
  I2C2->CR2 |= (1 << 13);

  while(!(I2C2->ISR & I2C_ISR_TXIS) && !(I2C2->ISR & I2C_ISR_NACKF));
  I2C2->TXDR = (0x28 | 0x80);

  while(!(I2C2->ISR & I2C_ISR_TC)); 

  I2C2->CR2 = 0;
  I2C2->CR2 |= (0x69 << 1);
  I2C2->CR2 |= (4 << 16);
  I2C2->CR2 |= (1 << 10);
  I2C2->CR2 |= (1 << 13);

  while(!(I2C2->ISR & I2C_ISR_RXNE) && !(I2C2->ISR & I2C_ISR_NACKF));
  xl = I2C2->RXDR;
  while(!(I2C2->ISR & I2C_ISR_RXNE) && !(I2C2->ISR & I2C_ISR_NACKF));
  xh = I2C2->RXDR;
  while(!(I2C2->ISR & I2C_ISR_RXNE) && !(I2C2->ISR & I2C_ISR_NACKF));
  yl = I2C2->RXDR;
  while(!(I2C2->ISR & I2C_ISR_RXNE) && !(I2C2->ISR & I2C_ISR_NACKF));
  yh = I2C2->RXDR;

  while(!(I2C2->ISR & I2C_ISR_TC));

  I2C2->CR2 |= I2C_CR2_STOP;

  x = (int16_t)((xh << 8) | xl);
  y = (int16_t)((yh << 8) | yl);

  // Clear
  GPIOC->ODR &= ~((1<<6)|(1<<7)|(1<<8)|(1<<9));

  // X axis
  if (x > 1000)        GPIOC->ODR |= (1<<9);  // +X
  else if (x < -1000)  GPIOC->ODR |= (1<<8);  // -X

  // Y axis
  if (y > 1000)        GPIOC->ODR |= (1<<6);  // +Y
  else if (y < -1000)  GPIOC->ODR |= (1<<7);  // -Y

  HAL_Delay(100);
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
