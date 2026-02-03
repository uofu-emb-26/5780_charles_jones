#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void My_HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    (void)GPIO_Init;

    if (GPIOx == GPIOC){
        // set MODER
        GPIOC->MODER &= ~(3u << (2*6) | 3u << (2*7));
        GPIOC->MODER |= ((1u << (2*6)) | (1u << (2*7)));

        // set OTYPER
        GPIOC->OTYPER &= ~(1u << (6) | 1u << (7));

        // set OSPEEDR
        GPIOC->OSPEEDR &= ~(3u << (2*6) | 3u << (2*7));

        // set PUDR
        GPIOC->PUPDR &= ~(3u << (2*6) | 3u << (2*7));
    }
}

/*
void My_HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
}
*/

/*
GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    return -1;
}
*/

void My_HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    if (PinState == GPIO_PIN_SET) {
        GPIOx->ODR |= GPIO_Pin;     // set pin high
    } else {
        GPIOx->ODR &= ~GPIO_Pin;    // set pin low
    }
}


void My_HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->ODR ^= GPIO_Pin;
}


void My_HAL_RCC_GPIOC_CLK_Enable(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
}
