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
uint8_t ADC_Meas_Enabled = 0;
uint8_t Step_Num = 0;

void EnableScan(){
	Scan_Is_enabled = 1;
}

uint8_t IsScanReady(){
	if (Scan_iter >= 4095) return 1;
	else return 0;
}

uint8_t* GetScanData(){
	Scan_iter = 0;
	Scan_Is_enabled = 0;
	return Scan_Data;
}

void BEMF_Observer(){
	// Input Block
	uint16_t V_Floating = ADC_data[0];
	uint16_t V_DC = ADC_data[1];
	// 0 Cross Detection Block

	if (Scan_Is_enabled > 0){
		Scan_Data[Scan_iter] = V_Floating/16;
		Scan_iter ++ ;
		Scan_Data[Scan_iter] = V_DC/16;
		Scan_iter ++ ;
		Scan_Data[Scan_iter] = ADC_Meas_Enabled * 64;
		Scan_iter ++ ;
		Scan_Data[Scan_iter] = Step_Num * 8;
		Scan_iter ++ ;
		if (Scan_iter >= 4095) Scan_Is_enabled = 0;
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
	HAL_ADC_Start_DMA(hadc1, ADC_data, 2);
	HAL_TIM_PWM_Start_IT(htim1, TIM_CHANNEL_3);
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_3, 1);


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
	__HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_1, 0);
	//HAL_GPIO_WritePin(PWM_AL_GPIO_Port, PWM_AL_Pin, 0);
	__HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_2, PWM_COUNTER);
}

void SetFloating_B(){
	__HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_1, 0);
	//HAL_GPIO_WritePin(PWM_BL_GPIO_Port, PWM_BL_Pin, 0);
	__HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_2, PWM_COUNTER);
}

void SetFloating_C(){
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

	while(ADC_data[num] > val+ADC_data[1]/2){
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

	while(ADC_data[num] < val+ADC_data[1]/2){
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


void ADC_Change_Order(char channel){
	ADC_ChannelConfTypeDef sConfig = {0};
	// Stop timer what triggers ADC conversions
	HAL_TIM_PWM_Stop_IT(htim1, TIM_CHANNEL_3);
	// Stop ADC DMA
	HAL_ADC_Stop_DMA(hadc1);
	// Change order of conversions
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	if(channel == 'A'){
		sConfig.Channel = ADC_CHANNEL_4;
	}else if(channel == 'B'){
		sConfig.Channel = ADC_CHANNEL_3;
	}else if(channel == 'C'){
		sConfig.Channel = ADC_CHANNEL_2;
	}
	if (HAL_ADC_ConfigChannel(hadc1, &sConfig) != HAL_OK)  Error_Handler();
	// Start ADC DMA
	HAL_ADC_Start_DMA(hadc1, ADC_data, 2);
	// Start timer what triggers ADC conversions
	HAL_TIM_PWM_Start_IT(htim1, TIM_CHANNEL_3);
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_3, 1);
	// Timers synchonisation
	//htim1->Instance->CNT = 0;
	//htim2->Instance->CNT = 0;
	//htim3->Instance->CNT = 0;
}

uint32_t WaitOneStep(uint32_t beforeCross, float div){
	if (div < 1) div = 1;
	uint16_t Diode_Delay = 10000; // 10000 -> 166us
	uint32_t ticks = Diode_Delay;
	uint32_t afterCross;

	Delay_Tick(Diode_Delay);
	ADC_Meas_Enabled = 1;
	ticks += WaitForCross(0,0);
	ADC_Meas_Enabled = 0;

	afterCross = (ticks) / (float)div; // Median of old before cross and new before cross
	if (div < 10 ) {
		Delay_Tick(afterCross);
		ticks += afterCross;
	}
	return ticks/2;
}

uint32_t BEMF_SixStep(uint16_t Value, uint32_t LastTicks, float div){
	if (div < 1) div = 1;
	float ticks = 0;
	uint16_t Diode_Delay;
	////////////////////////////////////////////////////////// 1
	SetPulse_CH(Value);
	SetZero_B();
	SetFloating_A();
	ADC_Change_Order('A');
	Step_Num = 1;

	ticks = LastTicks/8;	// 1/4
	Delay_Tick(ticks);
	ADC_Meas_Enabled = 1;
	ticks += WaitForCross(0,0);
	ADC_Meas_Enabled = 0;
	if (div < 10 ) Delay_Tick(ticks/(float)div);
	ticks += ticks/(float)div;
	////////////////////////////////////////////////////////// 2
	SetPulse_CH(Value);
	SetFloating_B();
	SetZero_A();
	ADC_Change_Order('B');
	Step_Num = 2;

	ticks = ticks/8;	// 1/4
	Delay_Tick(ticks);
	ADC_Meas_Enabled = 1;
	ticks += WaitForCross2(0,0);	// 1/2
	ADC_Meas_Enabled = 0;
	if (div < 10 ) Delay_Tick(ticks/(float)div);
	ticks += ticks/(float)div;
	////////////////////////////////////////////////////////// 3
	SetFloating_C();
	SetPulse_BH(Value);
	SetZero_A();
	ADC_Change_Order('C');
	Step_Num = 3;

	ticks = ticks/8;	// 1/4
	Delay_Tick(ticks);
	ADC_Meas_Enabled = 1;
	ticks += WaitForCross(0,0);
	ADC_Meas_Enabled = 0;
	if (div < 10 ) Delay_Tick(ticks/(float)div);
	ticks += ticks/(float)div;
	////////////////////////////////////////////////////////// 4
	SetZero_C();
	SetPulse_BH(Value);
	SetFloating_A();
	ADC_Change_Order('A');
	Step_Num = 4;

	ticks = ticks/8;	// 1/4
	Delay_Tick(ticks);
	ADC_Meas_Enabled = 1;
	ticks += WaitForCross2(0,0);
	ADC_Meas_Enabled = 0;
	if (div < 10 ) Delay_Tick(ticks/(float)div);
	ticks += ticks/(float)div;
	////////////////////////////////////////////////////////// 5
	SetZero_C();
	SetFloating_B();
	SetPulse_AH(Value);
	ADC_Change_Order('B');
	Step_Num = 5;

	ticks = ticks/8;	// 1/4
	Delay_Tick(ticks);
	ADC_Meas_Enabled = 1;
	ticks += WaitForCross(0,0);
	ADC_Meas_Enabled = 0;
	if (div < 10 ) Delay_Tick(ticks/(float)div);
	ticks += ticks/(float)div;
	////////////////////////////////////////////////////////// 6
	SetFloating_C();
	SetZero_B();
	SetPulse_AH(Value);
	ADC_Change_Order('C');
	Step_Num = 6;

	ticks = ticks/8;	// 1/4
	Delay_Tick(ticks);
	ADC_Meas_Enabled = 1;
	ticks += WaitForCross2(0,0);
	ADC_Meas_Enabled = 0;
	if (div < 10 ) Delay_Tick(ticks/(float)div);
	ticks += ticks/(float)div;
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
