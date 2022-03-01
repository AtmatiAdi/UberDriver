/*
 * control.c
 *
 *  Created on: Feb 26, 2022
 *      Author: atmat
 */

#include "control.h"

#define COLLECT_DATA	0	// 1 - Collection wil be performed, otherwise not.


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



void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
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

	HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 0);
	// ADC and Timer Configuration
	HAL_ADC_Start_DMA(hadc1, ADC_data, 7);
	HAL_TIM_PWM_Start_IT(htim1, TIM_CHANNEL_3);
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_3, 3);

	HAL_Delay(1);
}

void StartPWM(){
	HAL_TIM_PWM_Start(htim2, TIM_CHANNEL_1);
	//HAL_TIM_PWM_Start(htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(htim3, TIM_CHANNEL_1);
	//HAL_TIM_PWM_Start(htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(htim1, TIM_CHANNEL_2);
	//HAL_TIM_PWM_Start(htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(htim1, TIM_CHANNEL_3);

	htim1->Instance->CNT = 0;
	htim2->Instance->CNT = 0;
	htim3->Instance->CNT = 0;

	HAL_TIM_Base_Start(htim4);
}

void SetZero_A(){
	__HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_1, 0);
	HAL_GPIO_WritePin(PWM_AL_GPIO_Port, PWM_AL_Pin, 1);
}

void SetZero_B(){
	__HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_1, 0);
	HAL_GPIO_WritePin(PWM_BL_GPIO_Port, PWM_BL_Pin, 1);
}

void SetZero_C(){
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_2, 0);
	HAL_GPIO_WritePin(PWM_CL_GPIO_Port, PWM_CL_Pin, 1);
}

void SetPulse_AH(uint8_t value){
	__HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_1, value);
	HAL_GPIO_WritePin(PWM_AL_GPIO_Port, PWM_AL_Pin, 0);
}

void SetPulse_BH(uint8_t value){
	__HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_1, value);
	HAL_GPIO_WritePin(PWM_BL_GPIO_Port, PWM_BL_Pin, 0);
}

void SetPulse_CH(uint8_t value){
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_2, value);
	HAL_GPIO_WritePin(PWM_CL_GPIO_Port, PWM_CL_Pin, 0);
}

void SetFloating_A(){
	__HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_1, 0);
	HAL_GPIO_WritePin(PWM_AL_GPIO_Port, PWM_AL_Pin, 0);
}

void SetFloating_B(){
	__HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_1, 0);
	HAL_GPIO_WritePin(PWM_BL_GPIO_Port, PWM_BL_Pin, 0);
}

void SetFloating_C(){
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_2, 0);
	HAL_GPIO_WritePin(PWM_CL_GPIO_Port, PWM_CL_Pin, 0);
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
	float div = 6;//1.5;
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

uint8_t test = 0;
void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim){
	test = __HAL_TIM_GET_COUNTER(htim1);
	if(htim->Instance == TIM1){
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3){
			test = __HAL_TIM_GET_COUNTER(htim1);
			test = __HAL_TIM_GET_COUNTER(htim1);
		}
	}
}
