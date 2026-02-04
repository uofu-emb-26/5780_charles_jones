#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void My_HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    (void)GPIO_Init;

    if (GPIOx == GPIOC){
        // set MODER
        GPIOC->MODER &= ~(3u << (2*6) | 3u << (2*7) | 3u << (2*8) | 3u << (2*9));
        GPIOC->MODER |= ((1u << (2*6)) | (1u << (2*7)) | (1u << (2*8)) | (1u << (2*9)));

        // set OTYPER
        GPIOC->OTYPER &= ~(1u << (6) | 1u << (7) | 1u << (8) | 1u << (9));

        // set OSPEEDR
        GPIOC->OSPEEDR &= ~(3u << (2*6) | 3u << (2*7) | 3u << (2*8) | 3u << (2*9));

        // set PUDR
        GPIOC->PUPDR &= ~(3u << (2*6) | 3u << (2*7) | 3u << (2*8) | 3u << (2*9));
    }

    if (GPIOx == GPIOA) {

    // PA0 input mode (MODER = 00)
    GPIOA->MODER &= ~(3u << (2*0));

    // Low speed (OSPEEDR = 00)
    GPIOA->OSPEEDR &= ~(3u << (2*0));

    // Pull-down resistor (PUPDR = 10)
    GPIOA->PUPDR &= ~(3u << (2*0));
    GPIOA->PUPDR |=  (2u << (2*0));
    }
}

/*
void My_HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
}
*/


GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    if (GPIOx->IDR & GPIO_Pin) {
        return GPIO_PIN_SET;
    } else {
        return GPIO_PIN_RESET;
    }
}


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

void My_HAL_RCC_GPIOA_CLK_Enable(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
}

void Configure_EXTI0_Rising(void)
{
    // Map PA0 -> EXTI0 (often default, but do it explicitly)
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    SYSCFG->EXTICR[0] &= ~(0xFu << 0);   // EXTI0 = PA0

    EXTI->IMR  |=  (1u << 0);
    EXTI->RTSR |=  (1u << 0);
    EXTI->FTSR &= ~(1u << 0);
    EXTI->PR = (1u << 0);

    // set priority and enable
    NVIC_SetPriority(EXTI0_1_IRQn, 1);
    NVIC_EnableIRQ(EXTI0_1_IRQn);
}
