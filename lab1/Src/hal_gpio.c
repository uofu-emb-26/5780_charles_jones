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

    if (GPIOx == GPIOA){

        // set MODER
        GPIOC->MODER &= ~(3u << (2*0));
        
        // set OSPEEDR
        GPIOC->OSPEEDR &= ~(3u << (2*0));

        // set PUDR
        GPIOC->PUPDR &= ~(3u << (2*0));
        GPIOC->PUPDR |= (2u << (2*0));
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

/*
void My_HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
}
*/

/*
void My_HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
}
*/
