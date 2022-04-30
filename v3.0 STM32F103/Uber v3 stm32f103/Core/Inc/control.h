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
#define MAX_ADC_TICKS 	5000			// 50khz -> 0.1sec
#define ADC_CHANNEL_A	ADC_CHANNEL_2
#define ADC_CHANNEL_B	ADC_CHANNEL_1
#define ADC_CHANNEL_C	ADC_CHANNEL_3

#define SCAN_SIZE		4096*4*0
#define MORE_TRACES		7

#include "DRV8305.h"
#include "main.h"

void Control_Init_Motor_Timer(TIM_HandleTypeDef *_htim1);
void Control_Init_ADC_IAIBICVDC(ADC_HandleTypeDef *_hadc1,
								ADC_HandleTypeDef *_hadc2);
void Control_Init_ADC_VAVBVCVDC(ADC_HandleTypeDef *_hadc1,
								ADC_HandleTypeDef *_hadc2);
void Control_Init_ADC_VPhaseVDC(ADC_HandleTypeDef *_hadc1,
								ADC_HandleTypeDef *_hadc2);

void SetZero_A();
void SetZero_B();
void SetZero_C();
void SetPulse_AH(uint16_t value);
void SetPulse_BH(uint16_t value);
void SetPulse_CH(uint16_t value);
void SetFloating_A();
void SetFloating_B();
void SetFloating_C();

void BEMF_Observer_Block();
void HALL_Observer_Block();
void FAST_HALL_Observer_Block();

void Six_Step_Block(uint16_t PWM_Value);
void DupkoSin_Block(uint16_t PWM_Value);

void Log_Scan(uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4);
void SetHallPin();
void Set_PWM(uint16_t value);
void Set_Observer_Div(uint8_t div);
void EnableScan();
uint8_t IsScanReady();
uint8_t* GetScanData();
void ADC_Change_Order(uint32_t channel);
void Hall_Change_Active(uint32_t pin);
#endif /* INC_CONTROL_H_ */
