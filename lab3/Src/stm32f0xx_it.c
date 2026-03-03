#include "main.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_it.h"

volatile int duty = 0;
volatile int dir  = 1;

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
  HAL_IncTick();
}

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF)
    {
        TIM2->SR &= ~TIM_SR_UIF;

        GPIOC->ODR ^= (1U << 8);
        GPIOC->ODR ^= (1U << 9);

        int top = (int)TIM3->ARR;

        duty += dir * 150;

        if (duty >= top) { duty = top; dir = -1; }
        if (duty <= 0)   { duty = 0;   dir =  1; }

        TIM3->CCR2 = duty;

        TIM3->CCR1 = top - duty;
    }
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

