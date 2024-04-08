/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include <iostream>

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
void SWO_ITM_enable(void);

//---------------------------------------------------------------------------------------------------------//
/**
  * @brief  The application entry point.
  * @retval int
*/
int main(void)
{
  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  /* Initialize all configured peripherals */
  /* Configure GPIO */
  MX_GPIO_Init();
  /* Configure SWO debug interface */
  SWO_ITM_enable();

  /* Infinite loop */
  while (1)
  {
    //test code here
    HAL_Delay(1000);
    HAL_GPIO_TogglePin(LED_OK_GPIO_Port, LED_OK_Pin);
    static int counter = 0;
    std::cout << "iteration #" << ++counter << std::endl;

  }
}
//---------------------------------------------------------------------------------------------------------//

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 240;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}
//---------------------------------------------------------------------------------------------------------//

/**
  * @brief MPU Configuration
  * @retval None
  */
void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}
//---------------------------------------------------------------------------------------------------------//

/**
  * @brief SWO debug interface configuration
  * @retval None
  */
void SWO_ITM_enable(void)
{
  /*
    This functions recommends system speed of 480000000Hz and will
    use SWO clock speed of 2000000Hz

    # GDB OpenOCD commands to connect to this:
    monitor tpiu config internal - uart off 480000000
    monitor itm port 0 on

    Code Gen Ref: https://gist.github.com/mofosyne/178ad947fdff0f357eb0e03a42bcef5c
  */

  /* Setup SWO and SWO funnel (Note: SWO_BASE and SWTF_BASE not defined in stm32h743xx.h) */
  // DBGMCU_CR : Enable D3DBGCKEN D1DBGCKEN TRACECLKEN Clock Domains
  DBGMCU->CR =  DBGMCU_CR_DBG_CKD3EN | DBGMCU_CR_DBG_CKD1EN | DBGMCU_CR_DBG_TRACECKEN; // DBGMCU_CR
  // SWO_LAR & SWTF_LAR : Unlock SWO and SWO Funnel
  *((uint32_t *)(0x5c003fb0)) = 0xC5ACCE55; // SWO_LAR
  *((uint32_t *)(0x5c004fb0)) = 0xC5ACCE55; // SWTF_LAR
  // SWO_CODR  : 480000000Hz -> 2000000Hz
  // Note: SWOPrescaler = ((sysclock_Hz / SWOSpeed_Hz) - 1) --> 0x0000c7 = 239 = (480000000 / 2000000) - 1)
  *((uint32_t *)(0x5c003010)) = ((480000000 /  2000000) - 1); // SWO_CODR
  // SWO_SPPR : (2:  SWO NRZ, 1:  SWO Manchester encoding)
  *((uint32_t *)(0x5c0030f0)) = 0x00000002; // SWO_SPPR
  // SWTF_CTRL : enable SWO
  *((uint32_t *)(0x5c004000)) |= 0x1; // SWTF_CTRL
}
//---------------------------------------------------------------------------------------------------------//

/**
  * @brief  Function for write data to SWO port.
  * @param file, ptr, len Write to file [file], string [ptr] with length [len]
  * @retval int
  */
extern "C" {
int _write(int file, char *ptr, int len)
{
  int DataIdx;
	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		ITM_SendChar(*ptr++);
	}
	return len;
}
}
//---------------------------------------------------------------------------------------------------------//

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}
//---------------------------------------------------------------------------------------------------------//

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */
