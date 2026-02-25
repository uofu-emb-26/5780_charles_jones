#include "main.h"
#include "stm32f0xx_hal.h"
#include "hal_usart.h"
#include "hal_gpio.h"

void SystemClock_Config(void);

volatile char g_rx_char = 0;
volatile int  g_rx_ready = 0;

static void usart3_write_string(const char *s)
{
    while (*s) usart3_write_char(*s++);
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();

  usart3_init_min();

  GPIO_InitTypeDef init = {0};
  My_HAL_GPIO_Init(GPIOC, &init);

  usart3_write_string("CMD? ");

  char cmd_color = 0;
  int  have_first = 0;

  while (1)
  {
    if (!g_rx_ready) continue;

        char c = g_rx_char;
        g_rx_ready = 0;

        usart3_write_char(c);

        if (!have_first)
        {
            // color
            if (c == 'r' || c == 'b')
            {
                cmd_color = c;
                have_first = 1;
            }
            else
            {
                usart3_write_string("\r\nERR: unknown color\r\nCMD? ");
            }
        }
        else
        {
            // action
            if (c == '0' || c == '1' || c == '2')
            {
                if (cmd_color == 'r')
                {
                    if (c == '0') led_red_off();
                    else if (c == '1') led_red_on();
                    else led_red_toggle();
                }
                else if (cmd_color == 'b')
                {
                    if (c == '0') led_blue_off();
                    else if (c == '1') led_blue_on();
                    else led_blue_toggle();
                }

                usart3_write_string("\r\nOK: ");
                usart3_write_char(cmd_color);
                usart3_write_char(c);
                usart3_write_string("\r\nCMD? ");

                // reset
                have_first = 0;
            }
            else
            {
                usart3_write_string("\r\nERR: action must be 0/1/2\r\nCMD? ");
                have_first = 0;
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
