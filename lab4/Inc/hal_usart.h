#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void usart3_init_min(void);
void usart3_write_char(char c);
char usart3_read_char(void);
void USART3_4_IRQHandler(void);