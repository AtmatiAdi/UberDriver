/*
 * Control.h
 *
 *  Created on: Feb 26, 2022
 *      Author: atmat
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#define PHASE_A			1
#define PHASE_B			2
#define PHASE_C			3
#define STATE_LOW		0
#define STATE_HIGH		1
#define STATE_FLOATING	2
#define	STATE_F_LOW		2
#define	STATE_F_HIGH	3

#define MIN_TICKS  		0x0000000f
#define MAX_TICKS  		0x000fffff

#include "DRV8305.h"
#include "main.h"

void Control_Init(
		TIM_HandleTypeDef *_htim1,
		TIM_HandleTypeDef *_htim2,
		TIM_HandleTypeDef *_htim3,
		TIM_HandleTypeDef *_htim4,
		ADC_HandleTypeDef *_hadc1);
void SetZero_A();
void SetZero_B();
void SetZero_C();
void SetPulse_AH(uint16_t value);
void SetPulse_BH(uint16_t value);
void SetPulse_CH(uint16_t value);
void SetFloating_A();
void SetFloating_B();
void SetFloating_C();
void SixStep(uint32_t Speed, uint16_t Value);
void SixStep_rev(uint32_t Speed, uint16_t Value);
uint32_t WaitForCross(uint8_t num, uint8_t val);
uint32_t WaitForCross2(uint8_t num, uint8_t val);
uint32_t BEMF_SixStep(uint16_t Value, uint16_t LastTicks);
uint32_t BEMF_SixStep_rev(uint16_t Value, uint16_t LastTicks);
uint32_t BEMF_SixStep_TEST(uint16_t Value, uint16_t LastTicks);
uint32_t BEMF_SixStep_TEST_rev(uint16_t Value, uint16_t LastTicks);
uint32_t BEMF_SixStep_TEST_2_rev(uint16_t Value, uint16_t ticks_30deg);
uint32_t BEMF_SixStep_TEST_3_rev(uint16_t Value, uint16_t LastTicks, float ticks);
void WaitForRise(uint8_t Phase);
void WaitForFall(uint8_t Phase);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void Accelerate_01(uint8_t Value, uint8_t Speed);
void Delay_Tick(uint32_t val);
void BEMF_Observer();
void EnableScan();
uint8_t IsScanReady();
uint8_t* GetScanData();
#endif /* INC_CONTROL_H_ */
