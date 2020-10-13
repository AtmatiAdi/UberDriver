/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define Button_Pin GPIO_PIN_13
#define Button_GPIO_Port GPIOC
#define DVR_3_WAKE_Pin GPIO_PIN_14
#define DVR_3_WAKE_GPIO_Port GPIOC
#define DVR_3_ENGATE_Pin GPIO_PIN_15
#define DVR_3_ENGATE_GPIO_Port GPIOC
#define DRV_3_A3_Pin GPIO_PIN_0
#define DRV_3_A3_GPIO_Port GPIOC
#define DRV_3_A2_Pin GPIO_PIN_1
#define DRV_3_A2_GPIO_Port GPIOC
#define DRV_3_A3C2_Pin GPIO_PIN_2
#define DRV_3_A3C2_GPIO_Port GPIOC
#define DVR_3_SPI_Pin GPIO_PIN_3
#define DVR_3_SPI_GPIO_Port GPIOC
#define DVR_3_NFAULT_Pin GPIO_PIN_0
#define DVR_3_NFAULT_GPIO_Port GPIOA
#define DVR_3_C_Pin GPIO_PIN_1
#define DVR_3_C_GPIO_Port GPIOA
#define DVR_3_B_Pin GPIO_PIN_2
#define DVR_3_B_GPIO_Port GPIOA
#define DVR_3_A_Pin GPIO_PIN_3
#define DVR_3_A_GPIO_Port GPIOA
#define DRV_2_A3_Pin GPIO_PIN_4
#define DRV_2_A3_GPIO_Port GPIOA
#define DRV_2_A2_Pin GPIO_PIN_5
#define DRV_2_A2_GPIO_Port GPIOA
#define DRV_2_A1_Pin GPIO_PIN_6
#define DRV_2_A1_GPIO_Port GPIOA
#define DVR_2_SPI_Pin GPIO_PIN_7
#define DVR_2_SPI_GPIO_Port GPIOA
#define DVR_2_NFAULT_Pin GPIO_PIN_4
#define DVR_2_NFAULT_GPIO_Port GPIOC
#define DRV_1_A1_Pin GPIO_PIN_5
#define DRV_1_A1_GPIO_Port GPIOC
#define DRV_1_A2_Pin GPIO_PIN_0
#define DRV_1_A2_GPIO_Port GPIOB
#define DRV_1_A3_Pin GPIO_PIN_1
#define DRV_1_A3_GPIO_Port GPIOB
#define DVR_2_ENGATE_Pin GPIO_PIN_2
#define DVR_2_ENGATE_GPIO_Port GPIOB
#define DVR_1_SPI_Pin GPIO_PIN_12
#define DVR_1_SPI_GPIO_Port GPIOB
#define DVR_2_A_Pin GPIO_PIN_6
#define DVR_2_A_GPIO_Port GPIOC
#define DVR_2_B_Pin GPIO_PIN_7
#define DVR_2_B_GPIO_Port GPIOC
#define DVR_2_C_Pin GPIO_PIN_8
#define DVR_2_C_GPIO_Port GPIOC
#define DVR_2_WAKE_Pin GPIO_PIN_9
#define DVR_2_WAKE_GPIO_Port GPIOC
#define DVR_1_C_Pin GPIO_PIN_8
#define DVR_1_C_GPIO_Port GPIOA
#define DVR_1_B_Pin GPIO_PIN_9
#define DVR_1_B_GPIO_Port GPIOA
#define DVR_1_A_Pin GPIO_PIN_10
#define DVR_1_A_GPIO_Port GPIOA
#define DVR_1_ENGATE_Pin GPIO_PIN_11
#define DVR_1_ENGATE_GPIO_Port GPIOA
#define DVR_1_WAKE_Pin GPIO_PIN_12
#define DVR_1_WAKE_GPIO_Port GPIOA
#define DVR_1_NFAULT_Pin GPIO_PIN_15
#define DVR_1_NFAULT_GPIO_Port GPIOA
#define LED_BUILTIN_Pin GPIO_PIN_8
#define LED_BUILTIN_GPIO_Port GPIOB
#define LED_2_Pin GPIO_PIN_9
#define LED_2_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
