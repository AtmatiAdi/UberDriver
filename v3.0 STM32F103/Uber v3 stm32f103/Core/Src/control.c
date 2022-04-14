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
//static TIM_HandleTypeDef *htim2;
static ADC_HandleTypeDef *hadc1;
static ADC_HandleTypeDef *hadc2;

uint32_t ADC_data[8] = {0,0,0,0,0,0,0,0};	// try 16bit
uint16_t Meas[6] = {0,0,0,0,0,0};
uint16_t data_num = 0;
uint8_t tim_num = 0;
uint16_t Value = 8;

uint8_t Scan_Is_enabled = 0;
uint8_t Scan_Data[SCAN_SIZE];
uint16_t Scan_iter = 0;
uint8_t trace_num = 0;

uint32_t ADC_Ticks = 0;
uint32_t BeforeCross_Ticks = 0;
uint32_t AfterCross_Ticks = 0;
uint32_t Step_Ticks = 0;
GPIO_TypeDef* Hall_GPIO_Port = HALL_A_GPIO_Port;
uint16_t Hall_GPIO_Pin = HALL_A_Pin;

void Control_Init_Motor_Timer(TIM_HandleTypeDef *_htim1){
	htim1 = _htim1;

	// Disable gate driver output
	HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 0);

	// Timers for mosfet control
	HAL_TIM_PWM_Start(htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(htim1, TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Start(htim1, TIM_CHANNEL_1); // turn on complementary channel
	HAL_TIMEx_PWMN_Start(htim1, TIM_CHANNEL_2); // turn on complementary channel
	HAL_TIMEx_PWMN_Start(htim1, TIM_CHANNEL_3); // turn on complementary channel

	// Timer for ADC releasing
	HAL_TIM_PWM_Start(htim1, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start_IT(htim1, TIM_CHANNEL_4);
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_4, 1);
}

ADC_InjectionConfTypeDef sConfigInjected_1 = {0};
ADC_InjectionConfTypeDef sConfigInjected_2 = {0};
// Remember to make meas when all phases are gnd
void Control_Init_ADC_IAIBICVDC(ADC_HandleTypeDef *_hadc1,
								ADC_HandleTypeDef *_hadc2){
	hadc1 = _hadc1;
	hadc2 = _hadc2;
	// HAL ADC CONFIGURATION
	  ADC_MultiModeTypeDef multimode = {0};
	  // Common config
	  hadc1->Instance = ADC1;
	  hadc1->Init.ScanConvMode = ADC_SCAN_ENABLE;
	  hadc1->Init.ContinuousConvMode = DISABLE;
	  hadc1->Init.DiscontinuousConvMode = DISABLE;
	  hadc1->Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_CC2;
	  hadc1->Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc1->Init.NbrOfConversion = 1;
	  if (HAL_ADC_Init(hadc1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  // Configure the ADC multi-mode
	  multimode.Mode = ADC_DUALMODE_INJECSIMULT;
	  if (HAL_ADCEx_MultiModeConfigChannel(hadc1, &multimode) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  // Configure Injected Channel
	  sConfigInjected_1.InjectedChannel = ADC_CHANNEL_6;
	  sConfigInjected_1.InjectedRank = 1;
	  sConfigInjected_1.InjectedNbrOfConversion = 2;
	  sConfigInjected_1.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	  sConfigInjected_1.ExternalTrigInjecConv = ADC_EXTERNALTRIGINJECCONV_T1_CC4;
	  sConfigInjected_1.AutoInjectedConv = DISABLE;
	  sConfigInjected_1.InjectedDiscontinuousConvMode = DISABLE;
	  sConfigInjected_1.InjectedOffset = 0;
	  if (HAL_ADCEx_InjectedConfigChannel(hadc1, &sConfigInjected_1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  // Configure Injected Channel
	  sConfigInjected_1.InjectedChannel = ADC_CHANNEL_5;
	  sConfigInjected_1.InjectedRank = 2;
	  if (HAL_ADCEx_InjectedConfigChannel(hadc1, &sConfigInjected_1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  //Common config
	  hadc2->Instance = ADC2;
	  hadc2->Init.ScanConvMode = ADC_SCAN_ENABLE;
	  hadc2->Init.ContinuousConvMode = DISABLE;
	  hadc2->Init.DiscontinuousConvMode = DISABLE;
	  hadc2->Init.ExternalTrigConv = ADC_SOFTWARE_START;
	  hadc2->Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc2->Init.NbrOfConversion = 1;
	  if (HAL_ADC_Init(hadc2) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  // Configure Injected Channel
	  sConfigInjected_2.InjectedChannel = ADC_CHANNEL_4;
	  sConfigInjected_2.InjectedRank = 1;
	  sConfigInjected_2.InjectedNbrOfConversion = 2;
	  sConfigInjected_2.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	  sConfigInjected_2.AutoInjectedConv = DISABLE;
	  sConfigInjected_2.InjectedDiscontinuousConvMode = DISABLE;
	  sConfigInjected_2.InjectedOffset = 0;
	  if (HAL_ADCEx_InjectedConfigChannel(hadc2, &sConfigInjected_2) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  //Configure Injected Channel
	  sConfigInjected_2.InjectedChannel = ADC_CHANNEL_0;
	  sConfigInjected_2.InjectedRank = 2;
	  if (HAL_ADCEx_InjectedConfigChannel(hadc2, &sConfigInjected_2) != HAL_OK)
	  {
	    Error_Handler();
	  }

	// ADC Configuration
	HAL_ADCEx_Calibration_Start(hadc1);
	HAL_ADCEx_Calibration_Start(hadc2);
	HAL_ADC_Start(hadc2);
	//HAL_ADC_Start_DMA(hadc, ADC_data, 2);
	//HAL_ADCEx_MultiModeStart_DMA(hadc1, ADC_data, 2);
	HAL_ADCEx_InjectedStart_IT(hadc1);
	HAL_ADCEx_InjectedStart(hadc2);
}

void Control_Init_ADC_VAVBVCVDC(ADC_HandleTypeDef *_hadc1,
								ADC_HandleTypeDef *_hadc2){
	hadc1 = _hadc1;
	hadc2 = _hadc2;
	// HAL ADC CONFIGURATION
	  ADC_MultiModeTypeDef multimode = {0};
	  // Common config
	  hadc1->Instance = ADC1;
	  hadc1->Init.ScanConvMode = ADC_SCAN_ENABLE;
	  hadc1->Init.ContinuousConvMode = DISABLE;
	  hadc1->Init.DiscontinuousConvMode = DISABLE;
	  hadc1->Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_CC2;
	  hadc1->Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc1->Init.NbrOfConversion = 1;
	  if (HAL_ADC_Init(hadc1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  // Configure the ADC multi-mode
	  multimode.Mode = ADC_DUALMODE_INJECSIMULT;
	  if (HAL_ADCEx_MultiModeConfigChannel(hadc1, &multimode) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  // Configure Injected Channel
	  sConfigInjected_1.InjectedChannel = ADC_CHANNEL_2;
	  sConfigInjected_1.InjectedRank = 1;
	  sConfigInjected_1.InjectedNbrOfConversion = 2;
	  sConfigInjected_1.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	  sConfigInjected_1.ExternalTrigInjecConv = ADC_EXTERNALTRIGINJECCONV_T1_CC4;
	  sConfigInjected_1.AutoInjectedConv = DISABLE;
	  sConfigInjected_1.InjectedDiscontinuousConvMode = DISABLE;
	  sConfigInjected_1.InjectedOffset = 0;
	  if (HAL_ADCEx_InjectedConfigChannel(hadc1, &sConfigInjected_1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  // Configure Injected Channel
	  sConfigInjected_1.InjectedChannel = ADC_CHANNEL_1;
	  sConfigInjected_1.InjectedRank = 2;
	  if (HAL_ADCEx_InjectedConfigChannel(hadc1, &sConfigInjected_1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  //Common config
	  hadc2->Instance = ADC2;
	  hadc2->Init.ScanConvMode = ADC_SCAN_ENABLE;
	  hadc2->Init.ContinuousConvMode = DISABLE;
	  hadc2->Init.DiscontinuousConvMode = DISABLE;
	  hadc2->Init.ExternalTrigConv = ADC_SOFTWARE_START;
	  hadc2->Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc2->Init.NbrOfConversion = 1;
	  if (HAL_ADC_Init(hadc2) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  // Configure Injected Channel
	  sConfigInjected_2.InjectedChannel = ADC_CHANNEL_3;
	  sConfigInjected_2.InjectedRank = 1;
	  sConfigInjected_2.InjectedNbrOfConversion = 2;
	  sConfigInjected_2.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	  sConfigInjected_2.AutoInjectedConv = DISABLE;
	  sConfigInjected_2.InjectedDiscontinuousConvMode = DISABLE;
	  sConfigInjected_2.InjectedOffset = 0;
	  if (HAL_ADCEx_InjectedConfigChannel(hadc2, &sConfigInjected_2) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  //Configure Injected Channel
	  sConfigInjected_2.InjectedChannel = ADC_CHANNEL_0;
	  sConfigInjected_2.InjectedRank = 2;
	  if (HAL_ADCEx_InjectedConfigChannel(hadc2, &sConfigInjected_2) != HAL_OK)
	  {
	    Error_Handler();
	  }

	// ADC Configuration
	HAL_ADCEx_Calibration_Start(hadc1);
	HAL_ADCEx_Calibration_Start(hadc2);
	HAL_ADC_Start(hadc2);
	//HAL_ADC_Start_DMA(hadc, ADC_data, 2);
	//HAL_ADCEx_MultiModeStart_DMA(hadc1, ADC_data, 2);
	HAL_ADCEx_InjectedStart_IT(hadc1);
	HAL_ADCEx_InjectedStart(hadc2);
}

void Control_Init_ADC_VPhaseVDC(ADC_HandleTypeDef *_hadc1,
								ADC_HandleTypeDef *_hadc2){
	hadc1 = _hadc1;
	hadc2 = _hadc2;
	// HAL ADC CONFIGURATION
	  ADC_MultiModeTypeDef multimode = {0};
	  // Common config
	  hadc1->Instance = ADC1;
	  hadc1->Init.ScanConvMode = ADC_SCAN_ENABLE;
	  hadc1->Init.ContinuousConvMode = DISABLE;
	  hadc1->Init.DiscontinuousConvMode = DISABLE;
	  hadc1->Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_CC2;
	  hadc1->Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc1->Init.NbrOfConversion = 1;
	  if (HAL_ADC_Init(hadc1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  // Configure the ADC multi-mode
	  multimode.Mode = ADC_DUALMODE_INJECSIMULT;
	  if (HAL_ADCEx_MultiModeConfigChannel(hadc1, &multimode) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  // Configure Injected Channel
	  sConfigInjected_1.InjectedChannel = ADC_CHANNEL_1;
	  sConfigInjected_1.InjectedRank = 1;
	  sConfigInjected_1.InjectedNbrOfConversion = 1;
	  sConfigInjected_1.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	  sConfigInjected_1.ExternalTrigInjecConv = ADC_EXTERNALTRIGINJECCONV_T1_CC4;
	  sConfigInjected_1.AutoInjectedConv = DISABLE;
	  sConfigInjected_1.InjectedDiscontinuousConvMode = DISABLE;
	  sConfigInjected_1.InjectedOffset = 0;
	  if (HAL_ADCEx_InjectedConfigChannel(hadc1, &sConfigInjected_1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  //Common config
	  hadc2->Instance = ADC2;
	  hadc2->Init.ScanConvMode = ADC_SCAN_ENABLE;
	  hadc2->Init.ContinuousConvMode = DISABLE;
	  hadc2->Init.DiscontinuousConvMode = DISABLE;
	  hadc2->Init.ExternalTrigConv = ADC_SOFTWARE_START;
	  hadc2->Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc2->Init.NbrOfConversion = 1;
	  if (HAL_ADC_Init(hadc2) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  // Configure Injected Channel
	  sConfigInjected_2.InjectedChannel = ADC_CHANNEL_0;
	  sConfigInjected_2.InjectedRank = 1;
	  sConfigInjected_2.InjectedNbrOfConversion = 1;
	  sConfigInjected_2.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	  sConfigInjected_2.AutoInjectedConv = DISABLE;
	  sConfigInjected_2.InjectedDiscontinuousConvMode = DISABLE;
	  sConfigInjected_2.InjectedOffset = 0;
	  if (HAL_ADCEx_InjectedConfigChannel(hadc2, &sConfigInjected_2) != HAL_OK)
	  {
	    Error_Handler();
	  }

	// ADC Configuration
	HAL_ADCEx_Calibration_Start(hadc1);
	HAL_ADCEx_Calibration_Start(hadc2);
	HAL_ADC_Start(hadc2);
	//HAL_ADC_Start_DMA(hadc, ADC_data, 2);
	//HAL_ADCEx_MultiModeStart_DMA(hadc1, ADC_data, 2);
	HAL_ADCEx_InjectedStart_IT(hadc1);
	HAL_ADCEx_InjectedStart(hadc2);
}


void ADC_Change_Order(uint32_t channel){
	// Stop timer what triggers ADC conversions
	HAL_TIM_PWM_Stop_IT(htim1, TIM_CHANNEL_4);
	// Change order of conversions
	sConfigInjected_1.InjectedChannel = channel;

	if (HAL_ADCEx_InjectedConfigChannel(hadc1, &sConfigInjected_1) != HAL_OK){
		Error_Handler();
	}
	// Start timer what triggers ADC conversions
	HAL_TIM_PWM_Start_IT(htim1, TIM_CHANNEL_4);
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_4, 1);
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


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	//BEMF_Observer_Block();
	//HALL_Observer_Block();

	ADC_Ticks ++;
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc){
	BEMF_Observer_Block();
	//HALL_Observer_Block();

	ADC_Ticks ++;
}

void EnableScan(){
	Scan_Is_enabled = 1;
}

uint8_t IsScanReady(){
	if (Scan_iter >= SCAN_SIZE) return 1;
	else return 0;
}

uint8_t* GetScanData(){
	Scan_iter = 0;
	Scan_Is_enabled = 0;
	trace_num = 0;
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
uint16_t BEMF_time_cnt = 1;
uint16_t BEMF_Angle = 330;
uint8_t Div = 16;
uint16_t BEMF_Treshold = 0;
uint16_t PWM_Value = 0;
uint16_t BEMF_delay = 32;
uint16_t Angle = 0;
//uint16_t Ticks_Diff = 0;
void BEMF_Observer_Block(){
	// Input Block
	uint16_t V_Floating = hadc1->Instance->JDR1;
	uint16_t V_DC 		= hadc2->Instance->JDR1;
	//uint16_t V_Floating_Diff = 4095;
	// 0 Cross Detection Block
	// differentiate BEMF to obtain value and sign of changes
	//V_Floating_Diff = (4095 + V_Floating) - V_Floating_Old;
	if((Cross == 0) && (BEMF_cnt_sign == 1) && (BEMF_time_cnt > BEMF_delay)){
		if((Step_Num == 1) || (Step_Num == 3) || (Step_Num == 5)){
			// BEMF voltage will be decreasing -> '\'
			//if (V_Floating_Diff < 4095){
				// If BEMF actually '\'

				if (V_Floating < V_DC/2 ) Cross = 1;
			//}
		}else if((Step_Num == 2) || (Step_Num == 4) || (Step_Num == 6)){
			// Bemf voltage will be increasing -> '/'
			//if (V_Floating_Diff > 4095){
				// If BEMF actually '/'
				//if (V_Floating > 0 && V_Floating_Old) BEMF_delay = BEMF_time_cnt * 1.5;
				if (V_Floating > V_DC/2 ) Cross = 1;
			//}
		}
	}
	//V_Floating_Old = V_Floating;
	// 0 Cross Counter Block
	// Change counting sign when 0-cross is detected
	if (Cross == 1) {
		BEMF_cnt_sign = 0;
		BEMF_delay = BEMF_time_cnt/4;
		//if(BEMF_delay > 32) BEMF_delay = 32;
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
	if(BEMF_time_cnt >= 1024){
		BEMF_delay = BEMF_time_cnt/4;
		BEMF_Angle += 60;
		BEMF_cnt_sign = 1;
		BEMF_time_cnt = 0;

	}
	//Old_Step = Step_Num;
	if (BEMF_Angle >= 360) BEMF_Angle = 0;
	Angle = BEMF_Angle;

	if (Scan_Is_enabled > 0){
		Scan_iter += trace_num * 4;

		Scan_Data[Scan_iter] = V_Floating/8;
		Scan_iter ++ ;
		Scan_Data[Scan_iter] = HAL_GPIO_ReadPin(Hall_GPIO_Port, Hall_GPIO_Pin)*64;
		Scan_iter ++ ;
		Scan_Data[Scan_iter] = Step_Num * 16;
		Scan_iter ++ ;
		Scan_Data[Scan_iter] = BEMF_time_cnt;
		Scan_iter ++ ;

		Scan_iter += (MORE_TRACES-trace_num) * 4;
		if (Scan_iter >= SCAN_SIZE) {
			trace_num++;
			if(trace_num > MORE_TRACES){
				Scan_Is_enabled = 0;
			} else Scan_iter = 0;
		}
	}
	// Do nothing if pwm is ste to 0
	if(PWM_Value == 0){
		Old_Cross = 0;
		Cross = 0;
		BEMF_cnt_sign = 0;
		BEMF_time_cnt = 1;
		BEMF_Angle = 330;
		V_Floating_Old = 0;
		BEMF_delay = 32;
	}

	Six_Step_Block(PWM_Value);
}

void Set_Observer_Div(uint8_t div){
	if(div < 1) Div = 1;
	else Div = div;
}

uint8_t Hall = 0;
uint8_t OldHall = 0xff;			// Starting state
uint8_t HALL_cnt_sign = 0;		// Starting state
uint16_t HALL_time_cnt = 1;		// Starting state
uint16_t HALL_Angle = 0;	// UWAZAC Z GOWNO PRZESUNIECIAMI

uint16_t First_Half_Upper = 1;
uint16_t Second_Half_Upper = 0;
uint8_t Approx_Angle = 0;
uint16_t HALL_Treshold = 0;
uint8_t HALL_Is_Running = 0;
uint16_t HALL_Angles[7] = {0,300,180,240,60,0,120};
uint8_t HALL_Start_Pos = 0;
short I_A;
short I_B;
short I_C;
short I_Alfa;
short I_Beta;

void HALL_Observer_Block(){
	// A, B, C Currents
	Meas[0] = hadc1->Instance->JDR1;
	Meas[1] = hadc1->Instance->JDR2;
	Meas[2] = hadc2->Instance->JDR1;
	Meas[3] = hadc2->Instance->JDR2;
//	I_A = (ADC_data[1] >> 16) - 2047; // Crurrent A
//	I_B = (ADC_data[0] >> 16) - 2047; // Current B
//	I_C = (ADC_data[0]) - 2047;	// Current C
//	Meas[3] = ADC_data[1];	// Voltage DC
	// Alfa, Beta Currents
//	I_Alfa = I_A;
//	I_Beta = (I_B - I_C)/sqrt(3);

	if(PWM_Value == 0){
		HALL_Is_Running = 0;
		OldHall = 0xff;
		HALL_cnt_sign = 1;
		HALL_time_cnt = 0;
		First_Half_Upper = 0;
		HALL_Angle = 0;	// UWAZAC Z GOWNO PRZESUNIECIAMI
		DupkoSin_Block(PWM_Value);
		return;
	}
	// Input Block
	Hall = HAL_GPIO_ReadPin(Hall_GPIO_Port, Hall_GPIO_Pin);
	// Initail state reading
	if (HALL_Is_Running == 0){
		HALL_Is_Running = 1;
		HALL_Start_Pos = HAL_GPIO_ReadPin(HALL_A_GPIO_Port,HALL_A_Pin)
						+ (HAL_GPIO_ReadPin(HALL_B_GPIO_Port,HALL_B_Pin) << 1)
						+ (HAL_GPIO_ReadPin(HALL_C_GPIO_Port, HALL_C_Pin) << 2);
		HALL_Angle = HALL_Angles[HALL_Start_Pos];
		Angle = HALL_Angle;
		HALL_cnt_sign = 1;
		DupkoSin_Block(PWM_Value);	// it sets up hall pin
		OldHall = HAL_GPIO_ReadPin(Hall_GPIO_Port, Hall_GPIO_Pin);
		return;
	}

// 0 Cross Counter Block
	// Change counting sign when 0-cross is detected
	if (Hall != OldHall){
		HALL_cnt_sign = 0;
		HALL_Angle  += 30;
		if (Div > 30) HALL_Treshold = 0;
		else HALL_Treshold = HALL_time_cnt/Div;
		// For angle aproximation
		First_Half_Upper = HALL_time_cnt;
		Second_Half_Upper = HALL_time_cnt - HALL_Treshold;
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
	// Counter buffor reset before overflow
	if(HALL_time_cnt == 1024){
//		HALL_Angle += 60;
		HALL_cnt_sign = 1;
		HALL_time_cnt = 0;
		First_Half_Upper = 0;
		HALL_Is_Running = 0;
		return;
	}
	if(First_Half_Upper){
		if(HALL_cnt_sign){
			// cnt is increasing
			Approx_Angle = ((uint16_t)(HALL_time_cnt)*30)/First_Half_Upper;
			if(Approx_Angle > 30 ) Approx_Angle = 30;
		}else{
			// cnt is decreasing
			Approx_Angle = ((uint16_t)(HALL_time_cnt-HALL_Treshold)*30)/Second_Half_Upper;
			Approx_Angle = 30 - Approx_Angle;
		}
	}

	if(HALL_Angle >= 360) HALL_Angle = 0;
	Angle = HALL_Angle + Approx_Angle;

	if (Scan_Is_enabled > 0){
		Scan_iter += trace_num * 4;

		Scan_Data[Scan_iter] = Angle/2;
		Scan_iter ++ ;
		Scan_Data[Scan_iter] = (uint8_t)(Meas[0]/16);
		Scan_iter ++ ;
		Scan_Data[Scan_iter] = (uint8_t)(Meas[1]/16);
		Scan_iter ++ ;
		Scan_Data[Scan_iter] = 0;
		Scan_iter ++ ;

		//		Scan_Data[Scan_iter] = ((((uint32_t)DupkoSin[tmpAngle])*PWM_Value)/640)/3;
		//		Scan_iter ++ ;
		//		tmpAngle += 120;
		//		if(tmpAngle >= 360) tmpAngle -= 360;
		//
		//		Scan_Data[Scan_iter] = ((((uint32_t)DupkoSin[tmpAngle])*PWM_Value)/640)/3;
		//		Scan_iter ++ ;
		//		tmpAngle += 120;
		//		if(tmpAngle >= 360) tmpAngle -= 360;
		//
		//		Scan_Data[Scan_iter] = ((((uint32_t)DupkoSin[tmpAngle])*PWM_Value)/640)/3;
		//		Scan_iter ++ ;0

		Scan_iter += (MORE_TRACES-trace_num) * 4;
		if (Scan_iter >= SCAN_SIZE) {
			trace_num++;
			if(trace_num > MORE_TRACES){
				Scan_Is_enabled = 0;
			} else Scan_iter = 0;
		}
	}

	// Do nothing if pwm is ste to 0

	//DupkoSin_Block(PWM_Value);
	Six_Step_Block(PWM_Value);
//	SetZero_A();
//	SetZero_B();
//	SetZero_C();
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
	Step_Num = (Angle/60)+1;
	if(Step_Num != Old_Step){
		if(Step_Num == 1){
			SetPulse_CH(PWM_Value);
			SetZero_B();
			SetFloating_A();
			ADC_Change_Order(ADC_CHANNEL_A);
			Hall_Change_Active(ADC_CHANNEL_A);
		}else if(Step_Num == 2){
			SetPulse_CH(PWM_Value);
			SetFloating_B();
			SetZero_A();
			ADC_Change_Order(ADC_CHANNEL_B);
			Hall_Change_Active(ADC_CHANNEL_B);
		}else if(Step_Num == 3){
			SetFloating_C();
			SetPulse_BH(PWM_Value);
			SetZero_A();
			ADC_Change_Order(ADC_CHANNEL_C);
			Hall_Change_Active(ADC_CHANNEL_C);
		}else if(Step_Num == 4){
			SetZero_C();
			SetPulse_BH(PWM_Value);
			SetFloating_A();
			ADC_Change_Order(ADC_CHANNEL_A);
			Hall_Change_Active(ADC_CHANNEL_A);
		}else if(Step_Num == 5){
			SetZero_C();
			SetFloating_B();
			SetPulse_AH(PWM_Value);
			ADC_Change_Order(ADC_CHANNEL_B);
			Hall_Change_Active(ADC_CHANNEL_B);
		}else if(Step_Num == 6){
			SetFloating_C();
			SetZero_B();
			SetPulse_AH(PWM_Value);
			ADC_Change_Order(ADC_CHANNEL_C);
			Hall_Change_Active(ADC_CHANNEL_C);
		}
		Old_Step = Step_Num;
	}
}

uint16_t DupkoSin[360] = {553,559,564,569,574,579,584,588,592,597,600,604,608,611,614,617,620,623,625,627,629,631,633,634,635,637,637,638,639,639,639,639,639,638,637,637,635,634,633,631,629,627,625,623,620,617,614,611,608,604,600,597,592,588,584,579,574,569,564,559,553,559,564,569,574,579,584,588,592,597,600,604,608,611,614,617,620,623,625,627,629,631,633,634,635,637,637,638,639,639,639,639,639,638,637,637,635,634,633,631,629,627,625,623,620,617,614,611,608,604,600,597,592,588,584,579,574,569,564,559,553,548,542,536,530,523,517,510,504,497,490,482,475,467,460,452,444,436,428,419,411,402,393,385,376,367,357,348,339,329,320,310,300,290,280,270,260,250,239,229,219,208,197,187,176,165,155,144,133,122,111,100,89,78,67,56,45,33,22,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,22,33,45,56,67,78,89,100,111,122,133,144,155,165,176,187,197,208,219,229,239,250,260,270,280,290,300,310,319,329,339,348,357,367,376,385,393,402,411,419,428,436,444,452,460,467,475,482,490,497,504,510,517,523,530,536,542,548};
uint16_t OldAngle = 0xffff;

// ZMIANA PWM W POLOWIE CENTER ALIGN TO ZLY POMYSL !!!!

void DupkoSin_Block(uint16_t PWM_Value){
	uint16_t tmpAngle = 0;
	// REMEMBER THAT MAXIMUM PWM VALUE IS 640
	if(PWM_Value == 0){
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		Step_Num = 0;
		Old_Step = 0;
		OldAngle = 0xffff;
		return;
	}
	if(Angle != OldAngle){
		OldAngle = Angle;
		tmpAngle = Angle;

		SetPulse_CH((((uint32_t)DupkoSin[tmpAngle])*PWM_Value)/640);

		tmpAngle += 120;
		if(tmpAngle >= 360) tmpAngle -= 360;
		SetPulse_AH((((uint32_t)DupkoSin[tmpAngle])*PWM_Value)/640);

		tmpAngle += 120;
		if(tmpAngle >= 360) tmpAngle -= 360;
		SetPulse_BH((((uint32_t)DupkoSin[tmpAngle])*PWM_Value)/640);
	}
	Step_Num = (Angle/60)+1;
	if(Step_Num != Old_Step){
		if(Step_Num == 1){
			Hall_Change_Active(ADC_CHANNEL_A);
		}else if(Step_Num == 2){
			Hall_Change_Active(ADC_CHANNEL_B);
		}else if(Step_Num == 3){
			Hall_Change_Active(ADC_CHANNEL_C);
		}else if(Step_Num == 4){
			Hall_Change_Active(ADC_CHANNEL_A);
		}else if(Step_Num == 5){
			Hall_Change_Active(ADC_CHANNEL_B);
		}else if(Step_Num == 6){
			Hall_Change_Active(ADC_CHANNEL_C);
		}
		Old_Step = Step_Num;
	}
}

void Set_PWM(uint16_t value){
	PWM_Value = value;
}

inline void SetZero_A(){
	PWM_LA_GPIO_Port->CRL |= 0x00000080;	// Set to alternate push-pull
	PWM_HA_GPIO_Port->CRH |= 0x00000800;	// Set to alternate out push-pull
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_3, 0);
}

inline void SetZero_B(){
	PWM_LB_GPIO_Port->CRL |= 0x00000008;	// Set to alternate push-pull
	PWM_HB_GPIO_Port->CRH |= 0x00000080;	// Set to alternate push-pull
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_2, 0);
}

inline void SetZero_C(){
	PWM_LC_GPIO_Port->CRL |= 0x80000000;	// Set to alternate push-pull
	PWM_HC_GPIO_Port->CRH |= 0x00000008;	// Set to alternate push-pull
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_1, 0);
}
///////////////////
inline void SetPulse_AH(uint16_t value){
	PWM_LA_GPIO_Port->CRL |= 0x00000080;	// Set to alternate push-pull
	PWM_HA_GPIO_Port->CRH |= 0x00000800;	// Set to alternate out push-pull
	//if(value == 0) value = 1;
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_3, value);
}

inline void SetPulse_BH(uint16_t value){
	PWM_LB_GPIO_Port->CRL |= 0x00000008;	// Set to alternate push-pull
	PWM_HB_GPIO_Port->CRH |= 0x00000080;	// Set to alternate push-pull
	//if(value == 0) value = 1;
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_2, value);
}

inline void SetPulse_CH(uint16_t value){
	PWM_LC_GPIO_Port->CRL |= 0x80000000;	// Set to alternate push-pull
	PWM_HC_GPIO_Port->CRH |= 0x00000008;	// Set to alternate push-pull
	//if(value == 0) value = 1;
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_1, value);
}
///////////////////
inline void SetFloating_A(){
	PWM_LA_GPIO_Port->CRL &= ~0x000000C0;	// Set to general out push-pull
	PWM_HA_GPIO_Port->CRH &= ~0x00000C00;	// Set to general out push-pull
	//__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_3, 1);
	//HAL_GPIO_WritePin(PWM_AL_GPIO_Port, PWM_AL_Pin, 0);
	//__HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_2, PWM_COUNTER);
}

inline void SetFloating_B(){
	PWM_LB_GPIO_Port->CRL &= ~0x0000000C;	// Set to general out push-pull
	PWM_HB_GPIO_Port->CRH &= ~0x000000C0;	// Set to general out push-pull
	//__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_2, 1);
	//HAL_GPIO_WritePin(PWM_BL_GPIO_Port, PWM_BL_Pin, 0);
	//__HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_2, PWM_COUNTER);
}

inline void SetFloating_C(){
	PWM_LC_GPIO_Port->CRL &= ~0xC0000000;	// Set to general out push-pull
	PWM_HC_GPIO_Port->CRH &= ~0x0000000C;	// Set to general out push-pull
	//__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_1, 1);
	//HAL_GPIO_WritePin(PWM_CL_GPIO_Port, PWM_CL_Pin, 0);
	//__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_1, PWM_COUNTER);
}
