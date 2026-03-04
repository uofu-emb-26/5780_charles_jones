#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void My_HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    (void)GPIO_Init;

    if (GPIOx == GPIOB){
        // set MODER to alternate
        GPIOB->MODER &= ~(3 << (2*11) | 3 << (2*13) | 3 << (2*14));
        GPIOB->MODER |=  (2 << (2*11) | 2 << (2*13) | 1 << (2*14));

        // set open drain
        GPIOB->OTYPER |=  (1 << 11 | 1 << 13);

        // push - pull
        GPIOB->OTYPER &=  ~(1 << 14);

        // set I2C2_SDA
        GPIOB->AFRH &= ~(0xFu << 4 * (11 - 8) | 0xFu << 4 * (13 - 8));
        GPIOB->AFRH |= (1 << 4 * (11 - 8) | 5 << 4 * (13 - 8));

        // PB14 high
        GPIOB->ODR |=  (1 << 14);
    }

    if (GPIOx == GPIOC) {

        // PC0 input mode (MODER = 00)
        GPIOC->MODER &= ~(3 << (2*0));
        GPIOC->MODER |=  (1 << (2*0));

        // push pull
        GPIOC->OTYPER &= ~(1 << 0);

        // PC0 high
        GPIOC->ODR |= (1 << 0);
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

void My_HAL_RCC_GPIOB_CLK_Enable(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
}

void Configure_EXTI0_Rising(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    SYSCFG->EXTICR[0] &= ~(0xFu << 0);

    EXTI->IMR  |=  (1u << 0);
    EXTI->RTSR |=  (1u << 0);
    EXTI->FTSR &= ~(1u << 0);
    EXTI->PR = (1u << 0);

    // set priority and enable
    NVIC_SetPriority(EXTI0_1_IRQn, 3);
    NVIC_EnableIRQ(EXTI0_1_IRQn);
}

void I2C_TIMINGR(void)
{
    I2C2->TIMINGR |= (1 << 28);
    I2C2->TIMINGR |= (4 << 20);
    I2C2->TIMINGR |= (2 << 16);
    I2C2->TIMINGR |= (0xFu << 8);
    I2C2->TIMINGR |= (0x13 << 0);
} 
