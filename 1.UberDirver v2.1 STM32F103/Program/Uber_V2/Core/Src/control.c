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


uint32_t ADC_Ticks = 0;
uint32_t BeforeCross_Ticks = 0;
uint32_t AfterCross_Ticks = 0;
uint32_t Step_Ticks = 0;
GPIO_TypeDef* Hall_GPIO_Port = HALL_A_GPIO_Port;
uint16_t Hall_GPIO_Pin = HALL_A_Pin;

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
uint8_t ADC_Meas_Enabled = 0;
uint8_t Step_Num = 0;
uint8_t Old_Step = 0;
uint16_t V_Floating_Old = 0;
uint8_t Old_Cross = 0;
uint8_t Cross = 0;
uint8_t BEMF_cnt_sign = 1;
uint8_t res = 0;
uint8_t BEMF_time_cnt = 1;
uint16_t BEMF_Angle = 330;
uint8_t Div = 16;
uint16_t BEMF_Treshold = 0;
uint16_t PWM_Value = 0;

void BEMF_Observer_Block(){
// Input Block
	uint16_t V_Floating = ADC_data[0];
	uint16_t V_DC = ADC_data[1];
	uint16_t V_Floating_Diff = 4095;
// 0 Cross Detection Block
	// differentiate BEMF to obtain value and sign of changes
	V_Floating_Diff = (4095 + V_Floating) - V_Floating_Old;
	V_Floating_Old = V_Floating;
	if((Cross == 0) && (BEMF_cnt_sign == 1) && (BEMF_time_cnt > 32)){
		if((Step_Num == 1) || (Step_Num == 3) || (Step_Num == 5)){
			// BEMF voltage will be decreasing -> '\'
			if (V_Floating_Diff < 4095){
				// If BEMF actually '\'
				if (V_Floating < V_DC/2 ) Cross = 1;
			}
		}else if((Step_Num == 2) || (Step_Num == 4) || (Step_Num == 6)){
			// Bemf voltage will be increasing -> '/'
			if (V_Floating_Diff > 4095){
				// If BEMF actually '/'
				if (V_Floating > V_DC/2 ) Cross = 1;
			}
		}
	}
// 0 Cross Counter Block
	// Change counting sign when 0-cross is detected
	if (Cross == 1) {
		BEMF_cnt_sign = 0;
		Cross = 0;
		BEMF_Angle += 30;
		if (Div > 30) BEMF_Treshold = 0;
		else BEMF_Treshold = BEMF_time_cnt/Div;
	}
	//Old_Cross = Cross;
	// Counter
	if (BEMF_cnt_sign) {
		BEMF_time_cnt ++;
	}else {
		if (BEMF_time_cnt > 0) BEMF_time_cnt --;
	}
	// When Counter reached TRESHOLD then change sign and update BEMF_Angle
	if((BEMF_time_cnt <= BEMF_Treshold) && (BEMF_cnt_sign == 0)){
		BEMF_Angle += 30;
		BEMF_cnt_sign = 1;
		BEMF_time_cnt = 0;
	}
	// Counter buffor reset before overflow
	if(BEMF_time_cnt == 255){
		BEMF_Angle += 60;
		BEMF_cnt_sign = 1;
		BEMF_time_cnt = 0;
	}
	Old_Step = Step_Num;
	if (BEMF_Angle >= 360) BEMF_Angle = 0;

	if (Scan_Is_enabled > 0){
		Scan_Data[Scan_iter] = V_Floating/16;
		Scan_iter ++ ;
		//Scan_Data[Scan_iter] = Cross*64;
		//Scan_iter ++ ;
		Scan_Data[Scan_iter] = BEMF_Angle/4;
		Scan_iter ++ ;
		Scan_Data[Scan_iter] = Step_Num * 8;
		Scan_iter ++ ;
		Scan_Data[Scan_iter] = BEMF_time_cnt;
		Scan_iter ++ ;
		if (Scan_iter >= 4095) Scan_Is_enabled = 0;
	}
	// Do nothing if pwm is ste to 0
	if(PWM_Value == 0){
		Old_Cross = 0;
		Cross = 0;
		BEMF_cnt_sign = 0;
		BEMF_time_cnt = 1;
		BEMF_Angle = 330;
		V_Floating_Old = 0;
	}
}

void Set_Observer_Div(uint8_t div){
	if(div < 1) Div = 1;
	else Div = div;
}

uint8_t Hall = 0;
uint8_t OldHall = 0xff;			// Starting state
uint8_t HALL_cnt_sign = 0;		// Starting state
uint8_t HALL_time_cnt = 1;		// Starting state
uint16_t HALL_Angle = 330;	// Starting state

uint16_t HALL_Treshold = 0;

void HALL_Observer_Block(){
// Input Block
	Hall = HAL_GPIO_ReadPin(Hall_GPIO_Port, Hall_GPIO_Pin);
// 0 Cross Counter Block
	// Change counting sign when 0-cross is detected
	if (Hall != OldHall){
		HALL_cnt_sign = 0;
		HALL_Angle  += 30;
		if (Div > 30) HALL_Treshold = 0;
		else HALL_Treshold = HALL_time_cnt/Div;
	}
	OldHall = Hall;
	// Countter
	if(HALL_cnt_sign){
		HALL_time_cnt++;
	} else {
		if (HALL_time_cnt > 0) HALL_time_cnt--;
	}
	// When Counter reached TRESHOLD then change sign and update BEMF_Angle
	if((HALL_time_cnt <= HALL_Treshold) && (HALL_cnt_sign == 0)){
		HALL_Angle += 30;
		HALL_cnt_sign = 1;
		HALL_time_cnt = 0;
	}
//	if (Old_Step != Step_Num) {
//		HALL_Angle += 30;
//		HALL_cnt_sign = 1;
//		HALL_time_cnt = 0;
//	}
	Old_Step = Step_Num;
	if(HALL_Angle >= 360) HALL_Angle = 0;


	if (Scan_Is_enabled > 0){
		Scan_Data[Scan_iter] = Hall*64;
		Scan_iter ++ ;
		Scan_Data[Scan_iter] = HALL_Angle/4;
		Scan_iter ++ ;
		Scan_Data[Scan_iter] = Step_Num * 8;
		Scan_iter ++ ;
		Scan_Data[Scan_iter] = HALL_time_cnt;
		Scan_iter ++ ;
		if (Scan_iter >= 4095) Scan_Is_enabled = 0;
	}
	// Do nothing if pwm is ste to 0
	if(PWM_Value == 0){
		OldHall = 0xff;
		HALL_cnt_sign = 0;
		HALL_time_cnt = 1;
		HALL_Angle = 330;
	}
}

void Six_Step_Block(uint16_t PWM_Value){
	// Do nothing if pwm is ste to 0
	if(PWM_Value == 0){
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		Step_Num = 0;
		Old_Step = 0;
		return;
	}
	Step_Num = (BEMF_Angle/60)+1;
	if(Step_Num != Old_Step){
		if(Step_Num == 1){
			SetPulse_CH(PWM_Value);
			SetZero_B();
			SetFloating_A();
			ADC_Change_Order(ADC_CHANNEL_A);
		}else if(Step_Num == 2){
			SetPulse_CH(PWM_Value);
			SetFloating_B();
			SetZero_A();
			ADC_Change_Order(ADC_CHANNEL_B);
		}else if(Step_Num == 3){
			SetFloating_C();
			SetPulse_BH(PWM_Value);
			SetZero_A();
			ADC_Change_Order(ADC_CHANNEL_C);
		}else if(Step_Num == 4){
			SetZero_C();
			SetPulse_BH(PWM_Value);
			SetFloating_A();
			ADC_Change_Order(ADC_CHANNEL_A);
		}else if(Step_Num == 5){
			SetZero_C();
			SetFloating_B();
			SetPulse_AH(PWM_Value);
			ADC_Change_Order(ADC_CHANNEL_B);
		}else if(Step_Num == 6){
			SetFloating_C();
			SetZero_B();
			SetPulse_AH(PWM_Value);
			ADC_Change_Order(ADC_CHANNEL_C);
		}
	}
}

void Set_PWM(uint16_t value){
	PWM_Value = value;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	BEMF_Observer_Block();
	//HALL_Observer_Block();
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
	ADC_Ticks ++;
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
	//htim1->Instance->CNT = 0;
	//htim2->Instance->CNT = 0;
	//htim3->Instance->CNT = 0;


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

uint32_t WaitForHall(uint8_t compare){
	ADC_Ticks = 0;

	while(Hall == compare){
		if(ADC_Ticks >= MAX_ADC_TICKS){
			return ADC_Ticks;
		}
	}
	return ADC_Ticks;
}


uint32_t WaitForBemf(uint8_t compare){
	ADC_Ticks = 0;
	if (compare == 0){
		while(ADC_data[0] > ADC_data[1]){
			if(ADC_Ticks >= MAX_ADC_TICKS){
				return ADC_Ticks;
			}
		}
	}else {
		while(ADC_data[0] < ADC_data[1]){
			if(ADC_Ticks >= MAX_ADC_TICKS){
				return ADC_Ticks;
			}
		}
	}
	return ADC_Ticks;
}

void Delay_20us(uint32_t ticks){
	ADC_Ticks = 0;
	while (ADC_Ticks <= ticks){}
}


void Hall_Change_Active(uint32_t pin){
	if (pin == ADC_CHANNEL_A){
		Hall_GPIO_Port = HALL_A_GPIO_Port;
		Hall_GPIO_Pin = HALL_A_Pin;
	}else if (pin == ADC_CHANNEL_B){
		Hall_GPIO_Port = HALL_B_GPIO_Port;
		Hall_GPIO_Pin = HALL_B_Pin;
	}else{
		Hall_GPIO_Port = HALL_C_GPIO_Port;
		Hall_GPIO_Pin = HALL_C_Pin;
	}
}

void ADC_Change_Order(uint32_t channel){
	ADC_ChannelConfTypeDef sConfig = {0};
	// Stop timer what triggers ADC conversions
	HAL_TIM_PWM_Stop_IT(htim1, TIM_CHANNEL_3);
	// Stop ADC DMA
	HAL_ADC_Stop_DMA(hadc1);
	// Change order of conversions
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.Channel = channel;

	if (HAL_ADC_ConfigChannel(hadc1, &sConfig) != HAL_OK)  Error_Handler();
	// Start ADC DMA
	HAL_ADC_Start_DMA(hadc1, ADC_data, 2);
	// Start timer what triggers ADC conversions
	HAL_TIM_PWM_Start_IT(htim1, TIM_CHANNEL_3);
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_3, 1);
	// Timers synchonisation
	htim1->Instance->CNT = 0;
	htim2->Instance->CNT = 0;
	htim3->Instance->CNT = 0;
	Hall_Change_Active(channel);
}

uint32_t WaitOneStep(uint32_t LastTicks, float div){
	float ticks = 0;
	ticks = LastTicks/8;	// 1/4
	Delay_Tick(ticks);

	ADC_Meas_Enabled = 1;
	if ((Step_Num == 1) || (Step_Num == 3) || (Step_Num == 5)) ticks += WaitForCross(0,0);
	else ticks += WaitForCross2(0,0);
	ADC_Meas_Enabled = 0;

	if (div < 10 ) Delay_Tick(ticks/(float)div);
	ticks += ticks/(float)div;
	return ticks;
}

uint32_t HALL_WaitOneStep(uint32_t BeforeCross_Ticks, float div){
	if  ((Step_Num == 1) || (Step_Num == 3) || (Step_Num == 5)) BeforeCross_Ticks += WaitForHall(1);
	else BeforeCross_Ticks += WaitForHall(0);
	BeforeCross_Ticks /= 2;
	if (div < 30 ) AfterCross_Ticks = BeforeCross_Ticks/div;
	else AfterCross_Ticks = 0;
	Step_Ticks = BeforeCross_Ticks + AfterCross_Ticks;
	Delay_20us(AfterCross_Ticks);
	return BeforeCross_Ticks;
}

uint32_t HALL_SixStep(uint16_t Value, uint32_t BeforeCross_Ticks, float div){
	if (div < 1) div = 1;
	////////////////////////////////////////////////////////// 1
	SetPulse_CH(Value);
	SetZero_B();
	SetFloating_A();
	ADC_Change_Order(ADC_CHANNEL_A);
	Step_Num = 1;

	BeforeCross_Ticks = HALL_WaitOneStep(BeforeCross_Ticks, div);
	////////////////////////////////////////////////////////// 2
	SetPulse_CH(Value);
	SetFloating_B();
	SetZero_A();
	ADC_Change_Order(ADC_CHANNEL_B);
	Step_Num = 2;

	BeforeCross_Ticks = HALL_WaitOneStep(BeforeCross_Ticks, div);
	////////////////////////////////////////////////////////// 3
	SetFloating_C();
	SetPulse_BH(Value);
	SetZero_A();
	ADC_Change_Order(ADC_CHANNEL_C);
	Step_Num = 3;

	BeforeCross_Ticks = HALL_WaitOneStep(BeforeCross_Ticks, div);
	////////////////////////////////////////////////////////// 4
	SetZero_C();
	SetPulse_BH(Value);
	SetFloating_A();
	ADC_Change_Order(ADC_CHANNEL_A);
	Step_Num = 4;

	BeforeCross_Ticks = HALL_WaitOneStep(BeforeCross_Ticks, div);
	////////////////////////////////////////////////////////// 5
	SetZero_C();
	SetFloating_B();
	SetPulse_AH(Value);
	ADC_Change_Order(ADC_CHANNEL_B);
	Step_Num = 5;

	BeforeCross_Ticks = HALL_WaitOneStep(BeforeCross_Ticks, div);
	////////////////////////////////////////////////////////// 6
	SetFloating_C();
	SetZero_B();
	SetPulse_AH(Value);
	ADC_Change_Order(ADC_CHANNEL_C);
	Step_Num = 6;

	BeforeCross_Ticks = HALL_WaitOneStep(BeforeCross_Ticks, div);
	return BeforeCross_Ticks;
}

uint32_t BEMF_SixStep(uint16_t Value, uint32_t BeforeCross_Ticks, float div){
	if (div < 1) div = 1;
	////////////////////////////////////////////////////////// 1
	SetPulse_CH(Value);
	SetZero_B();
	SetFloating_A();
	ADC_Change_Order(ADC_CHANNEL_A);
	Step_Num = 1;

	BeforeCross_Ticks = WaitOneStep(BeforeCross_Ticks, div);
	////////////////////////////////////////////////////////// 2
	SetPulse_CH(Value);
	SetFloating_B();
	SetZero_A();
	ADC_Change_Order(ADC_CHANNEL_B);
	Step_Num = 2;

	BeforeCross_Ticks = WaitOneStep(BeforeCross_Ticks, div);
	////////////////////////////////////////////////////////// 3
	SetFloating_C();
	SetPulse_BH(Value);
	SetZero_A();
	ADC_Change_Order(ADC_CHANNEL_C);
	Step_Num = 3;

	BeforeCross_Ticks = WaitOneStep(BeforeCross_Ticks, div);
	////////////////////////////////////////////////////////// 4
	SetZero_C();
	SetPulse_BH(Value);
	SetFloating_A();
	ADC_Change_Order(ADC_CHANNEL_A);
	Step_Num = 4;

	BeforeCross_Ticks = WaitOneStep(BeforeCross_Ticks, div);
	////////////////////////////////////////////////////////// 5
	SetZero_C();
	SetFloating_B();
	SetPulse_AH(Value);
	ADC_Change_Order(ADC_CHANNEL_B);
	Step_Num = 5;

	BeforeCross_Ticks = WaitOneStep(BeforeCross_Ticks, div);
	////////////////////////////////////////////////////////// 6
	SetFloating_C();
	SetZero_B();
	SetPulse_AH(Value);
	ADC_Change_Order(ADC_CHANNEL_C);
	Step_Num = 6;

	BeforeCross_Ticks = WaitOneStep(BeforeCross_Ticks, div);
	return BeforeCross_Ticks;
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
