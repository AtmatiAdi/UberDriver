/*
 * Control.h
 *
 *  Created on: Feb 26, 2022
 *      Author: atmat
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#define COLLECT_DATA	0	// 1 - Collection wil be performed, otherwise not.
#define SCOTTER_PROGRAM 0	// SCOTTER program

#define MIN_TICKS  		0x0000000f
#define MAX_TICKS  		0x00ffffff

#include "stm32f1xx_hal.h"
#include "main.h"



void Control_Init(
		TIM_HandleTypeDef htim1_,
		TIM_HandleTypeDef htim2_,
		TIM_HandleTypeDef htim3_,
		TIM_HandleTypeDef htim4_,
		ADC_HandleTypeDef hadc1_,
		UART_HandleTypeDef huart1_);


void SetZero_A();
void SetZero_B();
void SetZero_C();
void SetPulse_AH(uint8_t value);
void SetPulse_BH(uint8_t value);
void SetPulse_CH(uint8_t value);
void SetFloating_A();
void SetFloating_B();
void SetFloating_C();
void StartPWM();
void SixStep(uint32_t Speed, uint16_t Value);
void SixStep_rev(uint32_t Speed, uint16_t Value);
uint32_t WaitForCross(uint8_t num, uint8_t val);
uint32_t WaitForCross2(uint8_t num, uint8_t val);
uint32_t BEMF_SixStep(uint16_t Value, uint16_t LastTicks);
uint32_t BEMF_SixStep_rev(uint16_t Value, uint16_t LastTicks);
uint32_t BEMF_SixStep_TEST(uint16_t Value, uint16_t LastTicks);
uint32_t BEMF_SixStep_TEST_rev(uint16_t Value, uint16_t LastTicks);
void WaitForRise(uint8_t Phase);
void WaitForFall(uint8_t Phase);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void Accelerate_01(uint8_t Value, uint8_t Speed);

#endif /* INC_CONTROL_H_ */
