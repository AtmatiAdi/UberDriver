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
//	  sConfigInjected_1.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	  sConfigInjected_2.InjectedSamplingTime = ADC_SAMPLETIME_28CYCLES_5;
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
//	  sConfigInjected_1.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	  sConfigInjected_2.InjectedSamplingTime = ADC_SAMPLETIME_28CYCLES_5;
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

__attribute__( ( section(".data") ) )
void ADC_Change_Order(uint32_t channel){
	// Stop timer what triggers ADC conversions
	//HAL_TIM_PWM_Stop_IT(htim1, TIM_CHANNEL_4);
	__HAL_TIM_DISABLE_IT(htim1, TIM_IT_CC4);
	// Change order of conversions
	sConfigInjected_1.InjectedChannel = channel;

	//HAL_ADCEx_InjectedConfigChannel(hadc1, &sConfigInjected_1);
	hadc1->Instance->JSQR = channel << ADC_SQR3_SQ4_Pos;

	// Start timer what triggers ADC conversions
	//HAL_TIM_PWM_Start_IT(htim1, TIM_CHANNEL_4);
	__HAL_TIM_ENABLE_IT(htim1, TIM_IT_CC4);
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_4, 1);
}

__attribute__( ( section(".data") ) )
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
int16_t PWM_Value = 0;
int16_t PWM_Value_irr = 0;
__attribute__( ( section(".data") ) )
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc){
	//BEMF_Observer_Block();
	HALL_Observer_Block();
	//FAST_HALL_Observer_Block();

	//_Six_Step_Block(PWM_Value_irr);	// Bemf
	//Six_Step_Block(PWM_Value_irr);// Hall
	DupkoSin_Block(PWM_Value_irr);
	// GOOD COMBO 	-> PWM(div 1) + FAST_HALL_Observer_Block(); + Six_Step_Block(PWM_Value);
	//				-> PWM(div 4) + HALL_Observer_Block(); + DupkoSin_Block(PWM_Value);
	//				-> PWM(div 1) + BEMF_Observer_Block(); + Six_Step_Block(PWM_Value);
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

uint16_t BEMF_delay = 0;
uint16_t Angle = 0;	// form 0 to 384 for fast calc
uint8_t Diode_Is_Conducting = 0 ;
//uint16_t Ticks_Diff = 0;
uint16_t V_DC = 0;
uint16_t V_DC_iir = 0;
uint16_t V_Floating = 0;
uint16_t V_Floating_iir = 0;
uint16_t Half_V_DC = 0;
uint8_t Bemf_Is_running = 0;
uint8_t Block = 0;
__attribute__( ( section(".data") ) )
void BEMF_Observer_Block(){
	//GPIOD->BSRR |= (1<<1);		// SET DEBUG_PIN HIGH
	// Input Block
	V_Floating = hadc1->Instance->JDR1;
	V_DC = hadc2->Instance->JDR1;
	// IRR Filtration
	if (V_DC > V_DC_iir) V_DC_iir += (V_DC - V_DC_iir) >> 2;
	else V_DC_iir -= (V_DC_iir - V_DC) >> 2;


	Half_V_DC = V_DC_iir >> 1;
	// Do nothing if pwm is ste to 0
	if(PWM_Value == 0){
		Old_Cross = 0;
		Cross = 0;
		BEMF_cnt_sign = 0;
		BEMF_time_cnt = 1;
		BEMF_Angle = 330;
		V_Floating_Old = 0;
		BEMF_delay = 0;
		Bemf_Is_running = 0;
		//Block = 0;

		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		return;
	}
	if (Bemf_Is_running == 0){
		//if (Block) return;
		// Start sequence
		// 300 deg
		SetFloating_C();
		SetZero_B();
		SetPulse_AH(255);

		BEMF_time_cnt ++ ;
		if (BEMF_time_cnt > 64) {
			//Block = 1;
			// 360/0 deg
			SetPulse_CH(255);
			SetZero_B();
			SetFloating_A();

			Old_Step = (Angle>>6)+1;
			PWM_Value_irr = 255;

			Bemf_Is_running = 1;
			BEMF_time_cnt = 1;
		}
		return;
	}

	// 0 Cross Detection Block
	if((Cross == 0) && (BEMF_cnt_sign == 1) && (BEMF_time_cnt > BEMF_delay)){
		if((Step_Num == 1) || (Step_Num == 3) || (Step_Num == 5)){
			// BEMF voltage will be decreasing -> '\'
			if (V_Floating > 0) {	// If Voltage is more than 0V
				if (V_Floating < Half_V_DC ) {
					Cross = 1;
				}
			}
		}else if((Step_Num == 2) || (Step_Num == 4) || (Step_Num == 6)){
			// Bemf voltage will be increasing -> '/'
			if (V_Floating < V_DC_iir){	// If Voltage is less than VDC
				if (V_Floating > Half_V_DC ) {
					Cross = 1;
				}
			}
		}
	}
	// 0 Cross Counter Block
	// Change counting sign when 0-cross is detected
	if (Cross == 1) {
		BEMF_cnt_sign = 0;
		BEMF_delay = 0;
		Cross = 0;
		Angle += 32;
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
		Angle += 32;
		BEMF_cnt_sign = 1;
		BEMF_time_cnt = 0;
	}
	// Counter buffor reset before overflow
	if(BEMF_time_cnt >= 4096){
		BEMF_delay = BEMF_time_cnt/4;
		Angle += 64;
		BEMF_cnt_sign = 1;
		BEMF_time_cnt = 0;
	}
	//Old_Step = Step_Num;
	if (Angle >= 384) Angle = 0;
	//Angle = BEMF_Angle;

	if (Scan_Is_enabled > 0){
		Log_Scan(V_Floating/8,
				0,
				Step_Num * 16,
				BEMF_time_cnt);
	}

	//GPIOD->BSRR |= (1<<17);		// SET DEBUG_PIN LOW
}
// 1. ZMIENIC DZIELNIK NAPIECIA TAK BY NIE DZIELIC VDC
// 2. PRZENIESC DO RAMU
uint8_t BEMF_slope  = 0;	// 0 means decreasing, 1 means increasing voltage
void FAST_BEMF_Observer_Block(){
	// INPUT BLOCK
	V_Floating	= hadc1->Instance->JDR1;
	V_DC 		= hadc2->Instance->JDR1;
	// 0 CROSS DETECTION BLOCK
	if((BEMF_cnt_sign) && (BEMF_time_cnt > BEMF_delay)){
		if(BEMF_slope){
			// Bemf voltage will be increasing -> '/'
			if (V_Floating > V_DC/2 ) {
				BEMF_cnt_sign = 0;
				BEMF_delay = BEMF_time_cnt >> 2;	// Fast divison by 4
				BEMF_Treshold = BEMF_time_cnt/Div;
			}
		}else {
			// BEMF voltage will be decreasing -> '\'
			if (V_Floating < V_DC/2 ) {
				BEMF_cnt_sign = 0;
				BEMF_delay = BEMF_time_cnt >> 2;	// Fast divison by 4
				BEMF_Treshold = BEMF_time_cnt/Div;
			}
		}
	}
	// Counter
	if (BEMF_cnt_sign) {
		BEMF_time_cnt ++;
	}else {
		if (BEMF_time_cnt > 0) BEMF_time_cnt --;
	}
	// When Counter reached TRESHOLD then change sign and update BEMF_Angle
	if((BEMF_time_cnt <= BEMF_Treshold) && (BEMF_cnt_sign == 0)){
		BEMF_Angle += 60;
		BEMF_cnt_sign = 1;
		BEMF_time_cnt = 0;
	}
	// Counter buffor reset before overflow
	if(BEMF_time_cnt >= 1024){
		BEMF_delay = BEMF_time_cnt >> 2;	// Fast divison by 4
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

__attribute__( ( section(".data") ) )
void HALL_Observer_Block(){
	//GPIOD->BSRR |= (1<<1);		// SET DEBUG_PIN HIGH

	if(PWM_Value == 0){
		HALL_Is_Running = 0;
		OldHall = 0xff;
		HALL_cnt_sign = 1;
		HALL_time_cnt = 0;
		First_Half_Upper = 0;
		HALL_Angle = 0;	// UWAZAC Z GOWNO PRZESUNIECIAMI
		Six_Step_Block(PWM_Value);
		return;
	}

	// Initail state reading
	if (HALL_Is_Running == 0){
		HALL_Is_Running = 1;

		if ((HALL_A_GPIO_Port->IDR & HALL_A_Pin) != 0) HALL_Start_Pos = 1;
		else HALL_Start_Pos = 0;
		if ((HALL_B_GPIO_Port->IDR & HALL_B_Pin) != 0) HALL_Start_Pos += 2;
		if ((HALL_C_GPIO_Port->IDR & HALL_C_Pin) != 0) HALL_Start_Pos += 4;

		HALL_Angle = HALL_Angles[HALL_Start_Pos];
		Angle = HALL_Angle;
		HALL_cnt_sign = 1;
		SetHallPin();	// it sets up hall pin
		if ((Hall_GPIO_Port->IDR & Hall_GPIO_Pin) != 0) OldHall = 1;
		else OldHall = 0;
		Step_Num = 0;
	}
	// Input Block
	if ((Hall_GPIO_Port->IDR & Hall_GPIO_Pin) != 0) Hall = 1;
	else Hall = 0;

// 0 Cross Counter Block
	// Change counting sign when 0-cross is detected
	if (Hall != OldHall){
		HALL_cnt_sign = 0;
		HALL_Angle  += 32;
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
		if (HALL_time_cnt > HALL_Treshold) HALL_time_cnt--;
	}

	if(First_Half_Upper){
		if(HALL_cnt_sign){
			// cnt is increasing
			Approx_Angle = (HALL_time_cnt<<5)/First_Half_Upper;
		}else{
			// cnt is decreasing
			Approx_Angle = ((HALL_time_cnt-HALL_Treshold)<<5)/Second_Half_Upper;
			Approx_Angle = 32 - Approx_Angle;
		}
		if(Approx_Angle >= 32 ) Approx_Angle = 31;
	}

	// When Counter reached TRESHOLD then change sign and update BEMF_Angle
	if((HALL_time_cnt <= HALL_Treshold) && (HALL_cnt_sign == 0)){
		HALL_Angle += 32;
		HALL_cnt_sign = 1;
		HALL_time_cnt = 0;
	}
	// Counter buffor reset before overflow
	if(HALL_time_cnt == 1024){
//		HALL_Angle += 64;
		HALL_cnt_sign = 1;
		HALL_time_cnt = 0;
		First_Half_Upper = 0;
		HALL_Is_Running = 0;
		return;
	}


	if(HALL_Angle >= 384) HALL_Angle = 0;
	Angle = HALL_Angle + Approx_Angle;

	if (Scan_Is_enabled > 0){
		Log_Scan(Angle/2,
				(uint8_t)(Meas[0]/16),
				(uint8_t)(Meas[1]/16),
				0);
	}

	//GPIOD->BSRR |= (1<<17);		// SET DEBUG_PIN LOW
}

__attribute__( ( section(".data") ) )
void FAST_HALL_Observer_Block(){
	//GPIOD->BSRR |= (1<<1);		// SET DEBUG_PIN HIGH

	if(PWM_Value == 0){
		HALL_Is_Running = 0;
		OldHall = 0xff;
		HALL_cnt_sign = 1;
		HALL_time_cnt = 0;
		First_Half_Upper = 0;
		HALL_Angle = 0;	// UWAZAC Z GOWNO PRZESUNIECIAMI
		Six_Step_Block(PWM_Value);
		return;
	}

	// Initail state reading
	if (HALL_Is_Running == 0){
		HALL_Is_Running = 1;

		if ((HALL_A_GPIO_Port->IDR & HALL_A_Pin) != 0) HALL_Start_Pos = 1;
		else HALL_Start_Pos = 0;
		if ((HALL_B_GPIO_Port->IDR & HALL_B_Pin) != 0) HALL_Start_Pos += 2;
		if ((HALL_C_GPIO_Port->IDR & HALL_C_Pin) != 0) HALL_Start_Pos += 4;

		HALL_Angle = HALL_Angles[HALL_Start_Pos];
		Angle = HALL_Angle;
		HALL_cnt_sign = 1;
		SetHallPin();	// it sets up hall pin
		if ((Hall_GPIO_Port->IDR & Hall_GPIO_Pin) != 0) OldHall = 1;
		else OldHall = 0;
		Step_Num = 0;
	}
	// Input Block
	if ((Hall_GPIO_Port->IDR & Hall_GPIO_Pin) != 0) Hall = 1;
	else Hall = 0;

// 0 Cross Counter Block
	// Change counting sign when 0-cross is detected
	if (Hall != OldHall){
		HALL_cnt_sign = 0;
		HALL_Angle  += 32;
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
		HALL_Angle += 32;
		HALL_cnt_sign = 1;
		HALL_time_cnt = 0;
	}
	// Counter buffor reset before overflow
	if(HALL_time_cnt == 1024){
//		HALL_Angle += 64;
		HALL_cnt_sign = 1;
		HALL_time_cnt = 0;
		First_Half_Upper = 0;
		HALL_Is_Running = 0;
		return;
	}

	if(HALL_Angle >= 384) HALL_Angle = 0;
	Angle = HALL_Angle;

	//GPIOD->BSRR |= (1<<17);		// SET DEBUG_PIN LOW
}
__attribute__( ( section(".data") ) )
inline void Log_Scan(uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4){
	Scan_iter += trace_num * 4;

	Scan_Data[Scan_iter] = data1;
	Scan_iter ++ ;
	Scan_Data[Scan_iter] = data2;
	Scan_iter ++ ;
	Scan_Data[Scan_iter] = data3;
	Scan_iter ++ ;
	Scan_Data[Scan_iter] = data4;
	Scan_iter ++ ;

	Scan_iter += (MORE_TRACES-trace_num) * 4;
	if (Scan_iter >= SCAN_SIZE) {
		trace_num++;
		if(trace_num > MORE_TRACES){
			Scan_Is_enabled = 0;
		} else Scan_iter = 0;
	}
}
__attribute__( ( section(".data") ) )
inline void SetHallPin(){
	Step_Num = (Angle/60)+1;
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
}

__attribute__( ( section(".data") ) )
inline void Six_Step_Block(uint16_t PWM_Value){
	//GPIOD->BSRR |= (1<<1);		// SET DEBUG_PIN HIGH
	// Do nothing if pwm is ste to 0
	if(PWM_Value == 0){
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		Step_Num = 0;
		Old_Step = 0;
		return;
	}

	Step_Num = (Angle>>6)+1;

	if(Step_Num != Old_Step){
		if(Step_Num == 1){
			SetPulse_CH(PWM_Value);
			SetZero_B();
			SetFloating_A();
			//ADC_Change_Order(ADC_CHANNEL_A);
			Hall_Change_Active(ADC_CHANNEL_A);
		}else if(Step_Num == 2){
			SetPulse_CH(PWM_Value);
			SetFloating_B();
			SetZero_A();
			//ADC_Change_Order(ADC_CHANNEL_B);
			Hall_Change_Active(ADC_CHANNEL_B);
		}else if(Step_Num == 3){
			SetFloating_C();
			SetPulse_BH(PWM_Value);
			SetZero_A();
			//ADC_Change_Order(ADC_CHANNEL_C);
			Hall_Change_Active(ADC_CHANNEL_C);
		}else if(Step_Num == 4){
			SetZero_C();
			SetPulse_BH(PWM_Value);
			SetFloating_A();
			//ADC_Change_Order(ADC_CHANNEL_A);
			Hall_Change_Active(ADC_CHANNEL_A);
		}else if(Step_Num == 5){
			SetZero_C();
			SetFloating_B();
			SetPulse_AH(PWM_Value);
			//ADC_Change_Order(ADC_CHANNEL_B);
			Hall_Change_Active(ADC_CHANNEL_B);
		}else if(Step_Num == 6){
			SetFloating_C();
			SetZero_B();
			SetPulse_AH(PWM_Value);
			//ADC_Change_Order(ADC_CHANNEL_C);
			Hall_Change_Active(ADC_CHANNEL_C);
		}
		Old_Step = Step_Num;
	}
	//GPIOD->BSRR |= (1<<17);		// SET DEBUG_PIN LOW
}

__attribute__( ( section(".data") ) )
inline void _Six_Step_Block(uint16_t PWM_Value){
	//GPIOD->BSRR |= (1<<1);		// SET DEBUG_PIN HIGH
	// Do nothing if pwm is ste to 0
	if(PWM_Value == 0){
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		Step_Num = 0;
		Old_Step = 0;
		return;
	}

	Step_Num = (Angle>>6)+1;

	if(Step_Num != Old_Step){
		if(Step_Num == 1){
			SetPulse_CH(PWM_Value);
			SetZero_B();
			SetFloating_A();
			ADC_Change_Order(ADC_CHANNEL_A);
			//Hall_Change_Active(ADC_CHANNEL_A);
		}else if(Step_Num == 2){
			SetPulse_CH(PWM_Value);
			SetFloating_B();
			SetZero_A();
			ADC_Change_Order(ADC_CHANNEL_B);
			//Hall_Change_Active(ADC_CHANNEL_B);
		}else if(Step_Num == 3){
			SetFloating_C();
			SetPulse_BH(PWM_Value);
			SetZero_A();
			ADC_Change_Order(ADC_CHANNEL_C);
			//Hall_Change_Active(ADC_CHANNEL_C);
		}else if(Step_Num == 4){
			SetZero_C();
			SetPulse_BH(PWM_Value);
			SetFloating_A();
			ADC_Change_Order(ADC_CHANNEL_A);
			//Hall_Change_Active(ADC_CHANNEL_A);
		}else if(Step_Num == 5){
			SetZero_C();
			SetFloating_B();
			SetPulse_AH(PWM_Value);
			ADC_Change_Order(ADC_CHANNEL_B);
			//Hall_Change_Active(ADC_CHANNEL_B);
		}else if(Step_Num == 6){
			SetFloating_C();
			SetZero_B();
			SetPulse_AH(PWM_Value);
			ADC_Change_Order(ADC_CHANNEL_C);
			//Hall_Change_Active(ADC_CHANNEL_C);
		}
		Old_Step = Step_Num;
	}
	//GPIOD->BSRR |= (1<<17);		// SET DEBUG_PIN LOW
}

uint8_t DupkoSin_384[384] = {221,223,225,227,229,231,232,234,236,237,239,240,241,243,244,245,246,247,248,249,250,251,252,252,253,253,254,254,254,255,255,255,255,255,255,255,254,254,254,253,253,252,252,251,250,249,248,247,246,245,244,243,241,240,239,237,236,234,232,231,229,227,225,223,221,223,225,227,229,231,232,234,236,237,239,240,241,243,244,245,246,247,248,249,250,251,252,252,253,253,254,254,254,255,255,255,255,255,255,255,254,254,254,253,253,252,252,251,250,249,248,247,246,245,244,243,241,240,239,237,236,234,232,231,229,227,225,223,221,219,217,214,212,210,207,205,202,200,197,194,192,189,186,183,180,177,174,171,168,165,162,159,155,152,149,145,142,138,135,131,128,124,120,117,113,109,105,101,98,94,90,86,82,78,74,70,66,62,58,54,50,46,42,37,33,29,25,21,17,13,8,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,8,13,17,21,25,29,33,37,42,46,50,54,58,62,66,70,74,78,82,86,90,94,98,101,105,109,113,117,120,124,128,131,135,138,142,145,149,152,155,159,162,165,168,171,174,177,180,183,186,189,192,194,197,200,202,205,207,210,212,214,217,219};
uint8_t DupkoSin_48[48] = {221,236,246,253,255,253,246,236,221,236,246,253,255,253,246,236,221,202,180,155,128,98,66,33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,33,66,98,128,155,180,202};
uint8_t DupkoSin_48_plus15[48] = {180,202,221,236,246,253,255,253,246,236,221,236,246,253,255,253,246,236,221,202,180,155,128,98,66,33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,33,66,98,128,155};
uint8_t DupkoSin_48_minus15[48] = {246,253,255,253,246,236,221,236,246,253,255,253,246,236,221,202,180,155,128,98,66,33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,33,66,98,128,155,180,202,221,236};
uint8_t DupkoSin_48_minus7_5[48] = {236,246,253,255,253,246,236,221,236,246,253,255,253,246,236,221,202,180,155,128,98,66,33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,33,66,98,128,155,180,202,221};

uint16_t OldAngle = 0xffff;

// ZMIANA PWM W POLOWIE CENTER ALIGN TO ZLY POMYSL !!!!
__attribute__( ( section(".data") ) )
void DupkoSin_Block(uint16_t PWM_Value){
	//GPIOD->BSRR |= (1<<1);		// SET DEBUG_PIN HIGH
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

		SetPulse_CH((((uint32_t)DupkoSin_48_minus7_5[tmpAngle>>3])*(PWM_Value+1))>>8);// Rapid divison by 256

		tmpAngle += 128;
		if(tmpAngle >= 384) tmpAngle -= 384;
		SetPulse_AH((((uint32_t)DupkoSin_48_minus7_5[tmpAngle>>3])*(PWM_Value+1))>>8);// Rapid divison by 256

		tmpAngle += 128;
		if(tmpAngle >= 384) tmpAngle -= 384;
		SetPulse_BH((((uint32_t)DupkoSin_48_minus7_5[tmpAngle>>3])*(PWM_Value+1))>>8);// Rapid divison by 256
	}
	Step_Num = (Angle>>6)+1;
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
	//GPIOD->BSRR |= (1<<17);		// SET DEBUG_PIN LOW
}

void Set_PWM(int16_t value){
	PWM_Value = value;
	PWM_Value_irr += (PWM_Value - PWM_Value_irr) >> 1;
	Old_Step = 0;
	OldAngle = 0xffff;
}
__attribute__( ( section(".data") ) )
inline void SetZero_A(){
	PWM_LA_GPIO_Port->CRL |= 0x00000080;	// Set to alternate push-pull
	PWM_HA_GPIO_Port->CRH |= 0x00000800;	// Set to alternate out push-pull
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_3, 0);
}
__attribute__( ( section(".data") ) )
inline void SetZero_B(){
	PWM_LB_GPIO_Port->CRL |= 0x00000008;	// Set to alternate push-pull
	PWM_HB_GPIO_Port->CRH |= 0x00000080;	// Set to alternate push-pull
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_2, 0);
}
__attribute__( ( section(".data") ) )
inline void SetZero_C(){
	PWM_LC_GPIO_Port->CRL |= 0x80000000;	// Set to alternate push-pull
	PWM_HC_GPIO_Port->CRH |= 0x00000008;	// Set to alternate push-pull
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_1, 0);
}
///////////////////
__attribute__( ( section(".data") ) )
inline void SetPulse_AH(uint16_t value){
	PWM_LA_GPIO_Port->CRL |= 0x00000080;	// Set to alternate push-pull
	PWM_HA_GPIO_Port->CRH |= 0x00000800;	// Set to alternate out push-pull
	//if(value == 0) value = 1;
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_3, value);
}
__attribute__( ( section(".data") ) )
inline void SetPulse_BH(uint16_t value){
	PWM_LB_GPIO_Port->CRL |= 0x00000008;	// Set to alternate push-pull
	PWM_HB_GPIO_Port->CRH |= 0x00000080;	// Set to alternate push-pull
	//if(value == 0) value = 1;
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_2, value);
}
__attribute__( ( section(".data") ) )
inline void SetPulse_CH(uint16_t value){
	PWM_LC_GPIO_Port->CRL |= 0x80000000;	// Set to alternate push-pull
	PWM_HC_GPIO_Port->CRH |= 0x00000008;	// Set to alternate push-pull
	//if(value == 0) value = 1;
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_1, value);
}
///////////////////
__attribute__( ( section(".data") ) )
inline void SetFloating_A(){
	PWM_LA_GPIO_Port->CRL &= ~0x000000C0;	// Set to general out push-pull
	PWM_HA_GPIO_Port->CRH &= ~0x00000C00;	// Set to general out push-pull
	//__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_3, 1);
	//HAL_GPIO_WritePin(PWM_AL_GPIO_Port, PWM_AL_Pin, 0);
	//__HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_2, PWM_COUNTER);
}
__attribute__( ( section(".data") ) )
inline void SetFloating_B(){
	PWM_LB_GPIO_Port->CRL &= ~0x0000000C;	// Set to general out push-pull
	PWM_HB_GPIO_Port->CRH &= ~0x000000C0;	// Set to general out push-pull
	//__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_2, 1);
	//HAL_GPIO_WritePin(PWM_BL_GPIO_Port, PWM_BL_Pin, 0);
	//__HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_2, PWM_COUNTER);
}
__attribute__( ( section(".data") ) )
inline void SetFloating_C(){
	PWM_LC_GPIO_Port->CRL &= ~0xC0000000;	// Set to general out push-pull
	PWM_HC_GPIO_Port->CRH &= ~0x0000000C;	// Set to general out push-pull
	//__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_1, 1);
	//HAL_GPIO_WritePin(PWM_CL_GPIO_Port, PWM_CL_Pin, 0);
	//__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_1, PWM_COUNTER);
}
