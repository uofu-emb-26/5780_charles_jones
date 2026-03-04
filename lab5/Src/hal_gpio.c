#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void My_HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    (void)GPIO_Init;

if (GPIOx == GPIOB) {
    // PB11, PB13 = AF (10)
    GPIOB->MODER &= ~((3u<<(2*11)) | (3u<<(2*13)));
    GPIOB->MODER |=  ((2u<<(2*11)) | (2u<<(2*13)));

    // PB14 = output (01)
    GPIOB->MODER &= ~(3u<<(2*14));
    GPIOB->MODER |=  (1u<<(2*14));

    // open-drain for I2C pins, push-pull for PB14
    GPIOB->OTYPER |=  (1u<<11) | (1u<<13);
    GPIOB->OTYPER &= ~(1u<<14);

    // no internal pulls on I2C pins
    GPIOB->PUPDR &= ~((3u<<(2*11)) | (3u<<(2*13)));

    // high speed on I2C pins (helps edges)
    GPIOB->OSPEEDR |= (3u<<(2*11)) | (3u<<(2*13));

    // AFR: PB11 AF1 (I2C2_SDA), PB13 AF5 (I2C2_SCL)
    GPIOB->AFR[1] &= ~((0xFu<<(4*(11-8))) | (0xFu<<(4*(13-8))));
    GPIOB->AFR[1] |=  ((1u  <<(4*(11-8))) | (5u  <<(4*(13-8))));

    // PB14 high (address select)
    GPIOB->ODR |= (1u<<14);
}

    if (GPIOx == GPIOC) {

        // PC0 output mode
        GPIOC->MODER &= ~(3 << (2*0));
        GPIOC->MODER |=  (1 << (2*0));

        // push-pull
        GPIOC->OTYPER &= ~(1 << 0);

        // drive HIGH → enables I2C mode
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
    I2C2->TIMINGR = (1 << 28) | (4 << 20) | (2 << 16) | (0xF << 8) | (0x13 << 0);
} 
