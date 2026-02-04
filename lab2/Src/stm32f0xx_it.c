#include "main.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_it.h"

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
   while (1)
  {
  }
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  static volatile uint32_t tickCount = 0;

  HAL_IncTick();

  tickCount++;

  if (tickCount >= 200)
  {
    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);  // blue LED
    tickCount = 0;
  }
}

void EXTI0_1_IRQHandler(void)
{
  static volatile uint32_t i = 0;
  // Check pending flag
  if (EXTI->PR & (1u << 0)) {
    EXTI->PR = (1u << 0);
    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);

    for (i = 0; i < 1500000; i++) { __NOP(); }

    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
  }
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

