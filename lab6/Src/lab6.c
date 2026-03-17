#include "main.h"
#include "stm32f0xx_hal.h"

void SystemClock_Config(void);

// Sine Wave: 8-bit, 32 samples/cycle
const uint8_t sine_table[32] = {127,151,175,197,216,232,244,251,254,251,244,
232,216,197,175,151,127,102,78,56,37,21,9,2,0,2,9,21,37,56,78,102};

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

  // Enable clocks
  RCC->AHBENR  |= RCC_AHBENR_GPIOCEN;
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
  RCC->APB1ENR |= RCC_APB1ENR_DACEN;

  // Enable HSI14 clock for ADC
  RCC->CR2 |= RCC_CR2_HSI14ON;
  while(!(RCC->CR2 & RCC_CR2_HSI14RDY));

  // Configure LEDs PC6-PC9 as output
  GPIOC->MODER |= (1 << 12) | (1 << 14) | (1 << 16) | (1 << 18);

  // Configure PC0 as analog mode
  GPIOC->MODER |= (3 << 0);

  // Configure ADC - 8bit, continuous, software trigger
  ADC1->CFGR1 |= (2 << 3);
  ADC1->CFGR1 |= (1 << 13);
  ADC1->CFGR1 &= ~(1 << 10);

  // Select channel 10 (PC0)
  ADC1->CHSELR |= (1 << 10);

  // Calibrate - must be disabled first
  ADC1->CR &= ~ADC_CR_ADEN;
  ADC1->CR |= ADC_CR_ADCAL;
  while(ADC1->CR & ADC_CR_ADCAL);

  // Enable ADC
  ADC1->CR |= ADC_CR_ADEN;
  // Wait for ADC ready
  while(!(ADC1->ISR & ADC_ISR_ADRDY));

  // Start conversion
  ADC1->CR |= ADC_CR_ADSTART;

  GPIOA->MODER |= (3 << 8);

  DAC->CR |= DAC_CR_TSEL1;   // software trigger
  DAC->CR |= DAC_CR_EN1;     // enable channel 1

  while (1)
  {
    // Wait for conversion to complete
    while(!(ADC1->ISR & ADC_ISR_EOC));

          // Read ADC value
        uint32_t adc_val = ADC1->DR;
        
        // Turn LEDs on/off based on value
        if(adc_val > 64)
            GPIOC->ODR |= (1 << 6);
        else
            GPIOC->ODR &= ~(1 << 6);
            
        if(adc_val > 128)
            GPIOC->ODR |= (1 << 7);
        else
            GPIOC->ODR &= ~(1 << 7);
            
        if(adc_val > 192)
            GPIOC->ODR |= (1 << 8);
        else
            GPIOC->ODR &= ~(1 << 8);
            
        if(adc_val > 230)
            GPIOC->ODR |= (1 << 9);
        else
            GPIOC->ODR &= ~(1 << 9);

      
    // DAC - waveform generation
    static uint32_t i = 0;
    DAC->DHR8R1 = sine_table[i];
    i = (i + 1) % 32;
    HAL_Delay(1);
  }
  return -1;
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
