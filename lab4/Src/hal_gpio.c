#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void My_HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    if (GPIOx == GPIOC)
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

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

GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{

}


void My_HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{

}


void My_HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{

}


void My_HAL_RCC_GPIOC_CLK_Enable(void)
{

}

void My_HAL_RCC_GPIOA_CLK_Enable(void)
{

}

void led_red_on(void)   { GPIOC->ODR |=  (1U<<6); }
void led_red_off(void)  { GPIOC->ODR &= ~(1U<<6); }
void led_red_toggle(void){GPIOC->ODR ^= (1U<<6); }

void led_blue_on(void)  { GPIOC->ODR |=  (1U<<7); }
void led_blue_off(void) { GPIOC->ODR &= ~(1U<<7); }
void led_blue_toggle(void){GPIOC->ODR ^= (1U<<7); }
