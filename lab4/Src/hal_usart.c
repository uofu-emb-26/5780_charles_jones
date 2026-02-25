#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void usart3_init_min(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    // enable GPIOC clock
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    // set PC10 & PC11 to alternate function
    GPIOC->MODER &= ~((3U<<(10*2)) | (3U<<(11*2)));
    GPIOC->MODER |=  ((2U<<(10*2)) | (2U<<(11*2)));

    // select AF1 (USART3)
    GPIOC->AFR[1] &= ~((0xF << ((10-8)*4)) | (0xF << ((11-8)*4)));
    GPIOC->AFR[1] |=  ((1   << ((10-8)*4)) | (1   << ((11-8)*4)));

    // 4) baud rate
    // CHANGE THIS ONE NUMBER to match your system clock (Hz).
    // Common: 8000000 (8 MHz) or 48000000 (48 MHz)
    uint32_t fclk = 8000000;

    USART3->BRR = (uint16_t)(8000000U / 9600U);

    // 5) enable TX and RX, then enable USART
    USART3->CR1 = USART_CR1_TE | USART_CR1_RE;
    USART3->CR1 |= USART_CR1_UE;
}

void usart3_write_char(char c)
{
    while ((USART3->ISR & USART_ISR_TXE) == 0) { }
    USART3->TDR = (uint8_t)c;
}

char usart3_read_char(void)
{
    while ((USART3->ISR & USART_ISR_RXNE) == 0) { }
    return (char)USART3->RDR;
}
