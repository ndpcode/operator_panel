/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_it.h"

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers             */
/******************************************************************************/

/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  HAL_RCC_NMI_IRQHandler();
  while (1)
  {
  }
}
//---------------------------------------------------------------------------------------------------------//

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}
//---------------------------------------------------------------------------------------------------------//

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  while (1)
  {
  }
}
//---------------------------------------------------------------------------------------------------------//

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  while (1)
  {
  }
}
//---------------------------------------------------------------------------------------------------------//

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  while (1)
  {
  }
}
//---------------------------------------------------------------------------------------------------------//

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
}
//---------------------------------------------------------------------------------------------------------//

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
}
//---------------------------------------------------------------------------------------------------------//

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
}
//---------------------------------------------------------------------------------------------------------//

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}
//---------------------------------------------------------------------------------------------------------//

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(DETECT_24V_Pin);
}
//---------------------------------------------------------------------------------------------------------//

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(BUT_MENU_Pin);
  HAL_GPIO_EXTI_IRQHandler(BUT_ESC_Pin);
  HAL_GPIO_EXTI_IRQHandler(BUT_LEFT_Pin);
  HAL_GPIO_EXTI_IRQHandler(BUT_UP_Pin);
  HAL_GPIO_EXTI_IRQHandler(BUT_DOWN_Pin);
}
//---------------------------------------------------------------------------------------------------------//

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(BUT_RIGHT_Pin);
  HAL_GPIO_EXTI_IRQHandler(BUT_START_Pin);
  HAL_GPIO_EXTI_IRQHandler(BUT_STOP_Pin);
}
//---------------------------------------------------------------------------------------------------------//
