/*
 * control.c
 *
 *  Created on: Feb 26, 2022
 *      Author: atmat
 */

#include "control.h"

#define COLLECT_DATA	0	// 1 - Collection wil be performed, otherwise not.
#define PWM_COUNTER     639	// 640-1 = 639

static TIM_HandleTypeDef *htim1;
static TIM_HandleTypeDef *htim2;
static TIM_HandleTypeDef *htim3;
static TIM_HandleTypeDef *htim4;
static ADC_HandleTypeDef *hadc1;

uint16_t ADC_data[7] = {0,0,0,0,0,0,0};	// try 16bit
uint8_t IsNewVal = 0;
uint16_t data_num = 0;
uint8_t tim_num = 0;
int cnt = 0;
uint16_t Value = 8;

uint8_t Scan_Is_enabled = 0;
uint8_t Scan_Data[4096];
uint16_t Scan_iter = 0;

char FloatingPhasae = 0;

void EnableScan(){
	Scan_Is_enabled = 1;
}

uint8_t IsScanReady(){
	if (Scan_iter == 4095) return 1;
	else return 0;
}

uint8_t* GetScanData(){
	Scan_iter = 0;
	Scan_Is_enabled = 0;
	return Scan_Data;
}

void BEMF_Observer(){
	// Input Block
	uint16_t V_A = ADC_data[0];
	uint16_t V_B = ADC_data[1];
	uint16_t V_C = ADC_data[2];
	uint16_t V_DC = ADC_data[3];
	// Floating Phase Detect Block
	uint16_t V_Floating = 0;
	switch(FloatingPhasae){
		case 'A':{
			V_Floating = V_A;
			break;
		}
		case 'B':{
			V_Floating = V_B;
			break;
		}
		case 'C':{
			V_Floating = V_C;
			break;
		}
	}

	if (Scan_Is_enabled > 0){
		Scan_Data[Scan_iter] = V_Floating/16;
		Scan_iter ++ ;
		if (Scan_iter == 4095) Scan_Is_enabled = 0;
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	BEMF_Observer();
	// Collect data
	uint8_t div = 1;
	if(COLLECT_DATA == 1){
		if((data_num < (4096-4)) && cnt >= div){

			//data[data_num] = ADC_data[0];
			data_num++;
			//data[data_num] = ADC_data[1];
			data_num++;
			//data[data_num] = ADC_data[2];
			data_num++;
			//data[data_num] = ADC_data[3];
			data_num++;
			cnt = 0;
		}
		if((tim_num < (64-4))){
			//TIM_B[tim_num] = __HAL_TIM_GET_COUNTER(htim3);
			//TIM_C[tim_num] = __HAL_TIM_GET_COUNTER(htim1);
			//TIM_A[tim_num] = __HAL_TIM_GET_COUNTER(htim2);
			tim_num++;
		}
	}
	IsNewVal = 1;
	cnt ++;
}

void Control_Init(
		TIM_HandleTypeDef *_htim1,
		TIM_HandleTypeDef *_htim2,
		TIM_HandleTypeDef *_htim3,
		TIM_HandleTypeDef *_htim4,
		ADC_HandleTypeDef *_hadc1){
	htim1 = _htim1;
	htim2 = _htim2;
	htim3 = _htim3;
	htim4 = _htim4;
	hadc1 = _hadc1;

	// Disable gate driver output
	HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 0);

	// Timer for precise time counting
	HAL_TIM_Base_Start(htim4);

	// Timers for mosfet control
	HAL_TIM_PWM_Start(htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(htim2, TIM_CHANNEL_2); 	// LOW POLARITY
	HAL_TIM_PWM_Start(htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(htim3, TIM_CHANNEL_2);	// LOW POLARITY
	HAL_TIM_PWM_Start(htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(htim1, TIM_CHANNEL_1);  	// LOW POLARITY

	// Timer for ADC releasing
	HAL_TIM_PWM_Start(htim1, TIM_CHANNEL_3);

	// Control timers basic synchronisation
	htim1->Instance->CNT = 0;
	htim2->Instance->CNT = 0;
	htim3->Instance->CNT = 0;


	// ADC and Timer Configuration
	HAL_ADC_Start_DMA(hadc1, ADC_data, 7);
	HAL_TIM_PWM_Start_IT(htim1, TIM_CHANNEL_3);
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_3, 3);


	HAL_Delay(1);
}

void SetZero_A(){
	__HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_1, 0);
	//HAL_GPIO_WritePin(PWM_AL_GPIO_Port, PWM_AL_Pin, 1);
	__HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_2, 0);
}

void SetZero_B(){
	__HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_1, 0);
	//HAL_GPIO_WritePin(PWM_BL_GPIO_Port, PWM_BL_Pin, 1);
	__HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_2, 0);
}

void SetZero_C(){
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_2, 0);
	//HAL_GPIO_WritePin(PWM_CL_GPIO_Port, PWM_CL_Pin, 1);
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_1, 0);
}
///////////////////
void SetPulse_AH(uint16_t value){
	__HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_1, value);
	//HAL_GPIO_WritePin(PWM_AL_GPIO_Port, PWM_AL_Pin, 0);
	__HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_2, value);
}

void SetPulse_BH(uint16_t value){
	__HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_1, value);
	//HAL_GPIO_WritePin(PWM_BL_GPIO_Port, PWM_BL_Pin, 0);
	__HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_2, value);
}

void SetPulse_CH(uint16_t value){
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_2, value);
	//HAL_GPIO_WritePin(PWM_CL_GPIO_Port, PWM_CL_Pin, 0);
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_1, value);
}
///////////////////
void SetFloating_A(){
	FloatingPhasae = 'A';
	__HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_1, 0);
	//HAL_GPIO_WritePin(PWM_AL_GPIO_Port, PWM_AL_Pin, 0);
	__HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_2, PWM_COUNTER);
}

void SetFloating_B(){
	FloatingPhasae = 'B';
	__HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_1, 0);
	//HAL_GPIO_WritePin(PWM_BL_GPIO_Port, PWM_BL_Pin, 0);
	__HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_2, PWM_COUNTER);
}

void SetFloating_C(){
	FloatingPhasae = 'C';
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_2, 0);
	//HAL_GPIO_WritePin(PWM_CL_GPIO_Port, PWM_CL_Pin, 0);
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_1, PWM_COUNTER);
}

void SixStep(uint32_t Speed, uint16_t Value){
	// 1
	SetPulse_AH(Value);
	SetZero_B();
	SetFloating_C();
	Delay_Tick(Speed);
	//data[ data_num-1] = 0;
	//2
	SetPulse_AH(Value);
	SetFloating_B();
	SetZero_C();
	Delay_Tick(Speed);
	//data[ data_num-1] = 0;
	//3
	SetFloating_A();
	SetPulse_BH(Value);
	SetZero_C();
	Delay_Tick(Speed);
	//data[ data_num-1] = 0;
	//4
	SetZero_A();
	SetPulse_BH(Value);
	SetFloating_C();
	Delay_Tick(Speed);
	//data[ data_num-1] = 0;
	//5
	SetZero_A();
	SetFloating_B();
	SetPulse_CH(Value);
	Delay_Tick(Speed);
	//data[ data_num-1] = 0;
	//6
	SetFloating_A();
	SetZero_B();
	SetPulse_CH(Value);
	Delay_Tick(Speed);
	//data[ data_num-1] = 0;
}

void SixStep_rev(uint32_t Speed, uint16_t Value){
	// 1
	SetPulse_CH(Value);
	SetZero_B();
	SetFloating_A();
	Delay_Tick(Speed);
	//data[ data_num-1] = 0;
	//2
	SetPulse_CH(Value);
	SetFloating_B();
	SetZero_A();
	Delay_Tick(Speed);
	//data[ data_num-1] = 0;
	//3
	SetFloating_C();
	SetPulse_BH(Value);
	SetZero_A();
	Delay_Tick(Speed);
	//data[ data_num-1] = 0;
	//4
	SetZero_C();
	SetPulse_BH(Value);
	SetFloating_A();
	Delay_Tick(Speed);
	//data[ data_num-1] = 0;
	//5
	SetZero_C();
	SetFloating_B();
	SetPulse_AH(Value);
	Delay_Tick(Speed);
	//data[ data_num-1] = 0;
	//6
	SetFloating_C();
	SetZero_B();
	SetPulse_AH(Value);
	Delay_Tick(Speed);
	//data[ data_num-1] = 0;
}

uint32_t WaitForCross(uint8_t num, uint8_t val){
	__HAL_TIM_SET_COUNTER(htim4,0);
	uint32_t ret_val = 0;

	while(ADC_data[num] > val+ADC_data[3]/2){
		if (__HAL_TIM_GET_COUNTER(htim4) > 0x7fff){
			ret_val += __HAL_TIM_GET_COUNTER(htim4);
			__HAL_TIM_SET_COUNTER(htim4,0);
		}
		if((ret_val + __HAL_TIM_GET_COUNTER(htim4) ) > MAX_TICKS){
			break;
		}
	}
	/*while(1){
		if (__HAL_TIM_GET_COUNTER(htim4) > 0x7fff){
			ret_val += __HAL_TIM_GET_COUNTER(htim4);
			__HAL_TIM_SET_COUNTER(htim4,0);
		}
		if (IsNewVal == 1){
			if(ADC_data[num] < val+ADC_data[3]/2){

			}
			IsNewVal = 0;
		}
	}*/

	ret_val += __HAL_TIM_GET_COUNTER(htim4);
	return ret_val;
}

uint32_t WaitForCross2(uint8_t num, uint8_t val){
	__HAL_TIM_SET_COUNTER(htim4,0);
	uint32_t ret_val = 0;

	while(ADC_data[num] < val+ADC_data[3]/2){
		if (__HAL_TIM_GET_COUNTER(htim4) > 0x7fff){
			ret_val += __HAL_TIM_GET_COUNTER(htim4);
			__HAL_TIM_SET_COUNTER(htim4,0);
		}
		if((ret_val + __HAL_TIM_GET_COUNTER(htim4) ) > MAX_TICKS){
			break;
		}
	}

	ret_val += __HAL_TIM_GET_COUNTER(htim4);
	return ret_val;
}

uint32_t BEMF_SixStep(uint16_t Value, uint16_t LastTicks){
	uint32_t ticks = 0;
	//uint8_t div = 2;
	////////////////////////////////////////////////////////// 1
	SetPulse_AH(Value);
	SetZero_B();
	SetFloating_C();
	ticks = WaitForCross(2,0);
	//Delay_Tick(ticks/div);
	if ((ticks > MAX_TICKS) || (ticks < MIN_TICKS)){
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		HAL_Delay(100);
	}
	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 2
	SetPulse_AH(Value);
	SetFloating_B();
	SetZero_C();
	ticks = WaitForCross2(1,0);
	//Delay_Tick(ticks/div);
	if ((ticks > MAX_TICKS) || (ticks < MIN_TICKS)){
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		HAL_Delay(100);
	}
	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 3
	SetFloating_A();
	SetPulse_BH(Value);
	SetZero_C();
	ticks = WaitForCross(0,0);
	//Delay_Tick(ticks/div);
	if ((ticks > MAX_TICKS) || (ticks < MIN_TICKS)){
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		HAL_Delay(100);
	}
	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 4
	SetZero_A();
	SetPulse_BH(Value);
	SetFloating_C();
	ticks = WaitForCross2(2,0);
	//Delay_Tick(ticks/div);
	if ((ticks > MAX_TICKS) || (ticks < MIN_TICKS)){
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		HAL_Delay(100);
	}
	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 5
	SetZero_A();
	SetFloating_B();
	SetPulse_CH(Value);
	ticks = WaitForCross(1,0);
	//Delay_Tick(ticks/div);
	if ((ticks > MAX_TICKS) || (ticks < MIN_TICKS)){
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		HAL_Delay(100);
	}
	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 6
	SetFloating_A();
	SetZero_B();
	SetPulse_CH(Value);
	ticks = WaitForCross2(0,0);
	//Delay_Tick(ticks/div);
	if ((ticks > MAX_TICKS) || (ticks < MIN_TICKS)){
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		HAL_Delay(100);
	}
	//data[ data_num-1] = 0;
	return ticks;
}

uint32_t BEMF_SixStep_rev(uint16_t Value, uint16_t LastTicks){
	uint32_t ticks = 0;
	//uint8_t div = 2;
	////////////////////////////////////////////////////////// 1
	SetPulse_CH(Value);
	SetZero_B();
	SetFloating_A();
	ticks = WaitForCross(0,0);
	//Delay_Tick(ticks/div);
	if ((ticks > MAX_TICKS) || (ticks < MIN_TICKS)){
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		HAL_Delay(100);
	}
	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 2
	SetPulse_CH(Value);
	SetFloating_B();
	SetZero_A();
	ticks = WaitForCross2(1,0);
	//Delay_Tick(ticks/div);
	if ((ticks > MAX_TICKS) || (ticks < MIN_TICKS)){
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		HAL_Delay(100);
	}
	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 3
	SetFloating_C();
	SetPulse_BH(Value);
	SetZero_A();
	ticks = WaitForCross(2,0);
	//Delay_Tick(ticks/div);
	if ((ticks > MAX_TICKS) || (ticks < MIN_TICKS)){
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		HAL_Delay(100);
	}
	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 4
	SetZero_C();
	SetPulse_BH(Value);
	SetFloating_A();
	ticks = WaitForCross2(0,0);
	//Delay_Tick(ticks/div);
	if ((ticks > MAX_TICKS) || (ticks < MIN_TICKS)){
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		HAL_Delay(100);
	}
	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 5
	SetZero_C();
	SetFloating_B();
	SetPulse_AH(Value);
	ticks = WaitForCross(1,0);
	//Delay_Tick(ticks/div);
	if ((ticks > MAX_TICKS) || (ticks < MIN_TICKS)){
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		HAL_Delay(100);
	}
	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 6
	SetFloating_C();
	SetZero_B();
	SetPulse_AH(Value);
	ticks = WaitForCross2(2,0);
	//Delay_Tick(ticks/div);
	if ((ticks > MAX_TICKS) || (ticks < MIN_TICKS)){
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		HAL_Delay(100);
	}
	//data[ data_num-1] = 0;
	return ticks;
}

uint32_t BEMF_SixStep_TEST(uint16_t Value, uint16_t LastTicks){
	float ticks = 0;
	float div = 6;//1.5;
	////////////////////////////////////////////////////////// 1
	//if(rx_buffer[0] == 0) return 0;
	SetPulse_AH(Value);
	SetZero_B();
	SetFloating_C();
	ticks = LastTicks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross(2,0);
	Delay_Tick(ticks/div);

	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 2
	//if(rx_buffer[0] == 0) return 0;
	SetPulse_AH(Value);
	SetFloating_B();
	SetZero_C();
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross2(1,0);	// 1/2
	Delay_Tick(ticks/div);

	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 3
	//if(rx_buffer[0] == 0) return 0;
	SetFloating_A();
	SetPulse_BH(Value);
	SetZero_C();
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross(0,0);
	Delay_Tick(ticks/div);

	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 4
	//if(rx_buffer[0] == 0) return 0;
	SetZero_A();
	SetPulse_BH(Value);
	SetFloating_C();
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross2(2,0);
	Delay_Tick(ticks/div);

	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 5
	//if(rx_buffer[0] == 0) return 0;
	SetZero_A();
	SetFloating_B();
	SetPulse_CH(Value);
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross(1,0);
	Delay_Tick(ticks/div);

	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 6
	//if(rx_buffer[0] == 0) return 0;
	SetFloating_A();
	SetZero_B();
	SetPulse_CH(Value);
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross2(0,0);
	Delay_Tick(ticks/div);

	//data[ data_num-1] = 0;
	return ticks;
}

uint32_t BEMF_SixStep_TEST_rev(uint16_t Value, uint16_t LastTicks){
	float ticks = 0;
	float div = 1.5;//1.5;
	////////////////////////////////////////////////////////// 1
	//if(rx_buffer[0] == 0) return 0;
	SetPulse_CH(Value);
	SetZero_B();
	SetFloating_A();
	ticks = LastTicks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross(0,0);
	Delay_Tick(ticks/div);

	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 2
	//if(rx_buffer[0] == 0) return 0;
	SetPulse_CH(Value);
	SetFloating_B();
	SetZero_A();
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross2(1,0);	// 1/2
	Delay_Tick(ticks/div);

	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 3
	//if(rx_buffer[0] == 0) return 0;
	SetFloating_C();
	SetPulse_BH(Value);
	SetZero_A();
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross(2,0);
	Delay_Tick(ticks/div);

	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 4
	//if(rx_buffer[0] == 0) return 0;
	SetZero_C();
	SetPulse_BH(Value);
	SetFloating_A();
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross2(0,0);
	Delay_Tick(ticks/div);

	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 5
	//if(rx_buffer[0] == 0) return 0;
	SetZero_C();
	SetFloating_B();
	SetPulse_AH(Value);
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross(1,0);
	Delay_Tick(ticks/div);

	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 6
	//if(rx_buffer[0] == 0) return 0;
	SetFloating_C();
	SetZero_B();
	SetPulse_AH(Value);
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross2(2,0);
	Delay_Tick(ticks/div);

	//data[ data_num-1] = 0;
	return ticks;
}

uint32_t BEMF_SixStep_TEST_3_rev(uint16_t Value, uint16_t LastTicks, float div){

	if (div < 1) div = 1;
	float ticks = 0;
	//float div = 1.5;//1.5;
	////////////////////////////////////////////////////////// 1
	//if(rx_buffer[0] == 0) return 0;
	SetPulse_CH(Value);
	SetZero_B();
	SetFloating_A();
	ticks = LastTicks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross(0,0);
	if (div < 10 ) Delay_Tick(ticks/(float)div);

	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 2
	//if(rx_buffer[0] == 0) return 0;
	SetPulse_CH(Value);
	SetFloating_B();
	SetZero_A();
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross2(1,0);	// 1/2
	if (div < 10 ) Delay_Tick(ticks/(float)div);

	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 3
	//if(rx_buffer[0] == 0) return 0;
	SetFloating_C();
	SetPulse_BH(Value);
	SetZero_A();
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross(2,0);
	if (div < 10 ) Delay_Tick(ticks/(float)div);

	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 4
	//if(rx_buffer[0] == 0) return 0;
	SetZero_C();
	SetPulse_BH(Value);
	SetFloating_A();
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross2(0,0);
	if (div < 10 ) Delay_Tick(ticks/(float)div);

	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 5
	//if(rx_buffer[0] == 0) return 0;
	SetZero_C();
	SetFloating_B();
	SetPulse_AH(Value);
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross(1,0);
	if (div < 10 ) Delay_Tick(ticks/(float)div);

	//data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 6
	//if(rx_buffer[0] == 0) return 0;
	SetFloating_C();
	SetZero_B();
	SetPulse_AH(Value);
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross2(2,0);
	if (div < 10 ) Delay_Tick(ticks/(float)div);

	//data[ data_num-1] = 0;
	return ticks;
}

uint32_t BEMF_SixStep_TEST_2_rev(uint16_t Value, uint16_t ticks_30deg){
	float ticks_diode_15deg = 0;
	float ticks_cross_15deg = 0;
	////////////////////////////////////////////////////////// 1
	SetPulse_CH(Value);
	SetZero_B();
	SetFloating_A();

	ticks_diode_15deg = ticks_30deg / 2;
	Delay_Tick(ticks_diode_15deg);
	ticks_cross_15deg = WaitForCross(0,0);
	ticks_30deg = (ticks_diode_15deg + ticks_cross_15deg);
	//Delay_Tick(ticks_cross_15deg);
	////////////////////////////////////////////////////////// 2
	SetPulse_CH(Value);
	SetFloating_B();
	SetZero_A();

	ticks_diode_15deg = ticks_30deg / 2;
	Delay_Tick(ticks_diode_15deg);
	ticks_cross_15deg = WaitForCross2(1,0);
	ticks_30deg = (ticks_diode_15deg + ticks_cross_15deg);
	//Delay_Tick(ticks_cross_15deg);
	////////////////////////////////////////////////////////// 3
	SetFloating_C();
	SetPulse_BH(Value);
	SetZero_A();

	ticks_diode_15deg = ticks_30deg / 2;
	Delay_Tick(ticks_diode_15deg);
	ticks_cross_15deg = WaitForCross(2,0);
	ticks_30deg = (ticks_diode_15deg + ticks_cross_15deg);
	//Delay_Tick(ticks_cross_15deg);
	////////////////////////////////////////////////////////// 4
	SetZero_C();
	SetPulse_BH(Value);
	SetFloating_A();

	ticks_diode_15deg = ticks_30deg / 2;
	Delay_Tick(ticks_diode_15deg);
	ticks_cross_15deg = WaitForCross2(0,0);
	ticks_30deg = (ticks_diode_15deg + ticks_cross_15deg);
	//Delay_Tick(ticks_cross_15deg);
	////////////////////////////////////////////////////////// 5
	SetZero_C();
	SetFloating_B();
	SetPulse_AH(Value);

	ticks_diode_15deg = ticks_30deg / 2;
	Delay_Tick(ticks_diode_15deg);
	ticks_cross_15deg = WaitForCross(1,0);
	ticks_30deg = (ticks_diode_15deg + ticks_cross_15deg);
	//Delay_Tick(ticks_cross_15deg);
	////////////////////////////////////////////////////////// 6
	SetFloating_C();
	SetZero_B();
	SetPulse_AH(Value);

	ticks_diode_15deg = ticks_30deg / 2;
	Delay_Tick(ticks_diode_15deg);
	ticks_cross_15deg = WaitForCross2(2,0);
	ticks_30deg = (ticks_diode_15deg + ticks_cross_15deg);
	//Delay_Tick(ticks_cross_15deg);

	return ticks_30deg;
}

void WaitForRise(uint8_t Phase){
	while(1){
		if(IsNewVal){
			if(ADC_data[Phase] > ADC_data[3]/2) return;
		}
	}
}

void WaitForFall(uint8_t Phase){
	while(1){
		if(IsNewVal){
			if(ADC_data[Phase] < ADC_data[3]/2) return;
		}
	}
}

void Accelerate_01(uint8_t Value, uint8_t Speed){		// Chinczyk - Value = 8; Speed = 0x0007ffff;
	for (; Speed > 0x0000ffff; Speed = Speed - 0x00001fff){		// 1
		SixStep(Speed, Value);
	}

	//
	for (; Speed > 0x00006fff; Speed = Speed - 0x000001ff){		// 1
		SixStep(Speed, Value);
	}
	for (; Speed > 0x00006000; Speed = Speed - 0x0000000f){		// 1
		SixStep(Speed, Value);
	}
	for (; Speed > 0x00005000; Speed = Speed - 0x00000001){		// 1
		SixStep(Speed, Value);
	}
}


void Delay_Tick(uint32_t val){
	__HAL_TIM_SET_COUNTER(htim4,0);
	if (val > MAX_TICKS) val = MAX_TICKS;
	while(val > 0x00007fff){
		val -= 0x00007fff;
		while(__HAL_TIM_GET_COUNTER(htim4) < 0x7fff);
		__HAL_TIM_SET_COUNTER(htim4,0);
	}
	while(__HAL_TIM_GET_COUNTER(htim4) < val);
}

void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim){
	uint8_t test = __HAL_TIM_GET_COUNTER(htim1);
	if(htim->Instance == TIM1){
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3){
			test = __HAL_TIM_GET_COUNTER(htim1);
			test = __HAL_TIM_GET_COUNTER(htim1);
		}
	}
}
