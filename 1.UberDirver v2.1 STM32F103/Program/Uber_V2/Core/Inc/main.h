/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PWM_PRESC 5-1
#define PWM_PERIOD 128
#define ENGATE_Pin GPIO_PIN_0
#define ENGATE_GPIO_Port GPIOD
#define NFAULT_Pin GPIO_PIN_1
#define NFAULT_GPIO_Port GPIOD
#define ADC_IC_Pin GPIO_PIN_0
#define ADC_IC_GPIO_Port GPIOA
#define ADC_UMOT_Pin GPIO_PIN_1
#define ADC_UMOT_GPIO_Port GPIOA
#define ADC_UC_Pin GPIO_PIN_2
#define ADC_UC_GPIO_Port GPIOA
#define ADC_UB_Pin GPIO_PIN_3
#define ADC_UB_GPIO_Port GPIOA
#define ADC_UA_Pin GPIO_PIN_4
#define ADC_UA_GPIO_Port GPIOA
#define SPI_SCK_Pin GPIO_PIN_5
#define SPI_SCK_GPIO_Port GPIOA
#define SPI_MISO_Pin GPIO_PIN_6
#define SPI_MISO_GPIO_Port GPIOA
#define SPI_MOSI_Pin GPIO_PIN_7
#define SPI_MOSI_GPIO_Port GPIOA
#define ADC_IB_Pin GPIO_PIN_0
#define ADC_IB_GPIO_Port GPIOB
#define ADC_IA_Pin GPIO_PIN_1
#define ADC_IA_GPIO_Port GPIOB
#define SPI_CS_Pin GPIO_PIN_2
#define SPI_CS_GPIO_Port GPIOB
#define PWM_CL_Pin GPIO_PIN_8
#define PWM_CL_GPIO_Port GPIOA
#define PWM_CH_Pin GPIO_PIN_9
#define PWM_CH_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_10
#define LED_GPIO_Port GPIOA
#define D2_INPUT_Pin GPIO_PIN_11
#define D2_INPUT_GPIO_Port GPIOA
#define D1_INPUT_Pin GPIO_PIN_12
#define D1_INPUT_GPIO_Port GPIOA
#define PWM_AH_Pin GPIO_PIN_15
#define PWM_AH_GPIO_Port GPIOA
#define PWM_AL_Pin GPIO_PIN_3
#define PWM_AL_GPIO_Port GPIOB
#define PWM_BH_Pin GPIO_PIN_4
#define PWM_BH_GPIO_Port GPIOB
#define PWM_BL_Pin GPIO_PIN_5
#define PWM_BL_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
