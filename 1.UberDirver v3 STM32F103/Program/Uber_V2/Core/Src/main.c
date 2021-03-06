/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "DRV8305.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define COLLECT_DATA	1	// 1 - Collection wil be performed, otherwise not.
#define SCOTTER_PROGRAM 0	// SCOTTER program

#define PHASE_A			1
#define PHASE_B			2
#define PHASE_C			3
#define STATE_LOW		0
#define STATE_HIGH		1
#define STATE_FLOATING	2
#define	STATE_F_LOW		2
#define	STATE_F_HIGH	3

#define SINGLE_DMA		1
#define MIN_TICKS  		0x0000000f
#define MAX_TICKS  		0x00ffffff
int IsDMA = 1;

#define SET_MOTOR_1_SPEED_FORWAD    	64   	// 0
#define SET_MOTOR_1_SPEED_BACK         	65   	// 1
#define	SET_MOTOR_1_SPEED_FORWAD_SEQ_1	66		// 2
#define SET_MOTOR_1_SPEED_BACK_SEQ_1	67		// 3
#define	SET_MOTOR_1_SPEED_FORWAD_SEQ_2	68		// 4
#define SET_MOTOR_1_SPEED_BACK_SEQ_2	69		// 5
#define	SET_MOTOR_1_SPEED_FORWAD_SEQ_3	70		// 6
#define SET_MOTOR_1_SPEED_BACK_SEQ_3	71		// 7
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
uint8_t buf[2] = {0, 0};
uint16_t ADC_data[4] = {0,0,0,0};	// try 16bit
uint16_t adc_val = 0;
uint16_t data[4096];	// try 16bit
uint8_t tx_buffer[3] = {'O','K','!'};
uint8_t rx_buffer[3] = {0,0,0};
//uint32_t ADC_A[128];
//uint32_t ADC_B[128];
//uint32_t ADC_C[128];
//uint32_t ADC_DC[128];
uint8_t tim_num = 0;
uint32_t TIM_A[64];
uint32_t TIM_B[64];
uint32_t TIM_C[64];
uint16_t data_num = 0;
TIM_OC_InitTypeDef TIM_LED = {0}, TIM_ADC = {0};
uint8_t ToSend[1] = {0};

uint8_t IsRunning = 0;
uint8_t IsBreaking = 0;
uint8_t Power = 0;
uint8_t Break = 0;
uint8_t Function = 0;

void StartPWM(){
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	//HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	//HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	//HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

	htim1.Instance->CNT = 0;
	htim2.Instance->CNT = 0;
	htim3.Instance->CNT = 0;
}

void SetZero_A(){
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	HAL_GPIO_WritePin(PWM_AL_GPIO_Port, PWM_AL_Pin, 1);
}

void SetZero_B(){
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	HAL_GPIO_WritePin(PWM_BL_GPIO_Port, PWM_BL_Pin, 1);
}

void SetZero_C(){
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
	HAL_GPIO_WritePin(PWM_CL_GPIO_Port, PWM_CL_Pin, 1);
}

void SetPulse_AH(uint8_t value){
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, value);
	HAL_GPIO_WritePin(PWM_AL_GPIO_Port, PWM_AL_Pin, 0);
}

void SetPulse_BH(uint8_t value){
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, value);
	HAL_GPIO_WritePin(PWM_BL_GPIO_Port, PWM_BL_Pin, 0);
}

void SetPulse_CH(uint8_t value){
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, value);
	HAL_GPIO_WritePin(PWM_CL_GPIO_Port, PWM_CL_Pin, 0);
}

void SetFloating_A(){
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	HAL_GPIO_WritePin(PWM_AL_GPIO_Port, PWM_AL_Pin, 0);
}

void SetFloating_B(){
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	HAL_GPIO_WritePin(PWM_BL_GPIO_Port, PWM_BL_Pin, 0);
}

void SetFloating_C(){
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
	HAL_GPIO_WritePin(PWM_CL_GPIO_Port, PWM_CL_Pin, 0);
}

void Delay_Tick(uint32_t val){
	__HAL_TIM_SET_COUNTER(&htim4,0);
	while(val > 0x00007fff){
		val -= 0x00007fff;
		while(__HAL_TIM_GET_COUNTER(&htim4) < 0x7fff);
		__HAL_TIM_SET_COUNTER(&htim4,0);
	}
	while(__HAL_TIM_GET_COUNTER(&htim4) < val);
}

void SixStep(uint32_t Speed, uint16_t Value){
	// 1
	SetPulse_AH(Value);
	SetZero_B();
	SetFloating_C();
	Delay_Tick(Speed);
	data[ data_num-1] = 0;
	//2
	SetPulse_AH(Value);
	SetFloating_B();
	SetZero_C();
	Delay_Tick(Speed);
	data[ data_num-1] = 0;
	//3
	SetFloating_A();
	SetPulse_BH(Value);
	SetZero_C();
	Delay_Tick(Speed);
	data[ data_num-1] = 0;
	//4
	SetZero_A();
	SetPulse_BH(Value);
	SetFloating_C();
	Delay_Tick(Speed);
	data[ data_num-1] = 0;
	//5
	SetZero_A();
	SetFloating_B();
	SetPulse_CH(Value);
	Delay_Tick(Speed);
	data[ data_num-1] = 0;
	//6
	SetFloating_A();
	SetZero_B();
	SetPulse_CH(Value);
	Delay_Tick(Speed);
	data[ data_num-1] = 0;
	ToSend[0]++;
}

void SixStep_rev(uint32_t Speed, uint16_t Value){
	// 1
	SetPulse_CH(Value);
	SetZero_B();
	SetFloating_A();
	Delay_Tick(Speed);
	data[ data_num-1] = 0;
	//2
	SetPulse_CH(Value);
	SetFloating_B();
	SetZero_A();
	Delay_Tick(Speed);
	data[ data_num-1] = 0;
	//3
	SetFloating_C();
	SetPulse_BH(Value);
	SetZero_A();
	Delay_Tick(Speed);
	data[ data_num-1] = 0;
	//4
	SetZero_C();
	SetPulse_BH(Value);
	SetFloating_A();
	Delay_Tick(Speed);
	data[ data_num-1] = 0;
	//5
	SetZero_C();
	SetFloating_B();
	SetPulse_AH(Value);
	Delay_Tick(Speed);
	data[ data_num-1] = 0;
	//6
	SetFloating_C();
	SetZero_B();
	SetPulse_AH(Value);
	Delay_Tick(Speed);
	data[ data_num-1] = 0;
	ToSend[0]++;
}

uint32_t WaitForCross(uint8_t num, uint8_t val){
	__HAL_TIM_SET_COUNTER(&htim4,0);
	uint32_t ret_val = 0;

	while(ADC_data[num] > val+ADC_data[3]/2){
		if (__HAL_TIM_GET_COUNTER(&htim4) > 0x7fff){
			ret_val += __HAL_TIM_GET_COUNTER(&htim4);
			__HAL_TIM_SET_COUNTER(&htim4,0);
		}
		if((ret_val + __HAL_TIM_GET_COUNTER(&htim4) ) > MAX_TICKS){
			break;
		}
	}
	/*while(1){
		if (__HAL_TIM_GET_COUNTER(&htim4) > 0x7fff){
			ret_val += __HAL_TIM_GET_COUNTER(&htim4);
			__HAL_TIM_SET_COUNTER(&htim4,0);
		}
		if (IsNewVal == 1){
			if(ADC_data[num] < val+ADC_data[3]/2){

			}
			IsNewVal = 0;
		}
	}*/

	ret_val += __HAL_TIM_GET_COUNTER(&htim4);
	return ret_val;
}

uint32_t WaitForCross2(uint8_t num, uint8_t val){
	__HAL_TIM_SET_COUNTER(&htim4,0);
	uint32_t ret_val = 0;

	while(ADC_data[num] < val+ADC_data[3]/2){
		if (__HAL_TIM_GET_COUNTER(&htim4) > 0x7fff){
			ret_val += __HAL_TIM_GET_COUNTER(&htim4);
			__HAL_TIM_SET_COUNTER(&htim4,0);
		}
		if((ret_val + __HAL_TIM_GET_COUNTER(&htim4) ) > MAX_TICKS){
			break;
		}
	}

	ret_val += __HAL_TIM_GET_COUNTER(&htim4);
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
	data[ data_num-1] = 0;
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
	data[ data_num-1] = 0;
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
	data[ data_num-1] = 0;
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
	data[ data_num-1] = 0;
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
	data[ data_num-1] = 0;
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
	data[ data_num-1] = 0;
	ToSend[0]++;
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
	data[ data_num-1] = 0;
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
	data[ data_num-1] = 0;
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
	data[ data_num-1] = 0;
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
	data[ data_num-1] = 0;
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
	data[ data_num-1] = 0;
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
	data[ data_num-1] = 0;
	ToSend[0]++;
	return ticks;
}

uint32_t BEMF_SixStep_TEST(uint16_t Value, uint16_t LastTicks){
	float ticks = 0;
	float div = 6;//1.5;
	////////////////////////////////////////////////////////// 1
	if(rx_buffer[0] == 0) return;
	SetPulse_AH(Value);
	SetZero_B();
	SetFloating_C();
	ticks = LastTicks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross(2,0);
	Delay_Tick(ticks/div);

	data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 2
	if(rx_buffer[0] == 0) return;
	SetPulse_AH(Value);
	SetFloating_B();
	SetZero_C();
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross2(1,0);	// 1/2
	Delay_Tick(ticks/div);

	data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 3
	if(rx_buffer[0] == 0) return;
	SetFloating_A();
	SetPulse_BH(Value);
	SetZero_C();
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross(0,0);
	Delay_Tick(ticks/div);

	data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 4
	if(rx_buffer[0] == 0) return;
	SetZero_A();
	SetPulse_BH(Value);
	SetFloating_C();
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross2(2,0);
	Delay_Tick(ticks/div);

	data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 5
	if(rx_buffer[0] == 0) return;
	SetZero_A();
	SetFloating_B();
	SetPulse_CH(Value);
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross(1,0);
	Delay_Tick(ticks/div);

	data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 6
	if(rx_buffer[0] == 0) return;
	SetFloating_A();
	SetZero_B();
	SetPulse_CH(Value);
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross2(0,0);
	Delay_Tick(ticks/div);

	data[ data_num-1] = 0;
	ToSend[0]++;
	return ticks;
}

uint32_t BEMF_SixStep_TEST_rev(uint16_t Value, uint16_t LastTicks){
	float ticks = 0;
	float div = 6;//1.5;
	////////////////////////////////////////////////////////// 1
	if(rx_buffer[0] == 0) return;
	SetPulse_CH(Value);
	SetZero_B();
	SetFloating_A();
	ticks = LastTicks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross(0,0);
	Delay_Tick(ticks/div);

	data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 2
	if(rx_buffer[0] == 0) return;
	SetPulse_CH(Value);
	SetFloating_B();
	SetZero_A();
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross2(1,0);	// 1/2
	Delay_Tick(ticks/div);

	data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 3
	if(rx_buffer[0] == 0) return;
	SetFloating_C();
	SetPulse_BH(Value);
	SetZero_A();
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross(2,0);
	Delay_Tick(ticks/div);

	data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 4
	if(rx_buffer[0] == 0) return;
	SetZero_C();
	SetPulse_BH(Value);
	SetFloating_A();
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross2(0,0);
	Delay_Tick(ticks/div);

	data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 5
	if(rx_buffer[0] == 0) return;
	SetZero_C();
	SetFloating_B();
	SetPulse_AH(Value);
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross(1,0);
	Delay_Tick(ticks/div);

	data[ data_num-1] = 0;
	////////////////////////////////////////////////////////// 6
	if(rx_buffer[0] == 0) return;
	SetFloating_C();
	SetZero_B();
	SetPulse_AH(Value);
	ticks = ticks/2;	// 1/4
	Delay_Tick(ticks);
	ticks += WaitForCross2(2,0);
	Delay_Tick(ticks/div);

	data[ data_num-1] = 0;
	ToSend[0]++;
	return ticks;
}

uint8_t test = 0;
void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim){
	test = __HAL_TIM_GET_COUNTER(&htim1);
	if(htim->Instance == TIM1){
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3){
			test = __HAL_TIM_GET_COUNTER(&htim1);
			test = __HAL_TIM_GET_COUNTER(&htim1);
		}
	}
}
uint8_t IsNewVal = 0;
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
int cnt = 0;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	// Collect data
	uint8_t div = 1;
	if(COLLECT_DATA == 1){
		if((data_num < (4096-4)) && cnt >= div){

			data[data_num] = ADC_data[0];
			data_num++;
			data[data_num] = ADC_data[1];
			data_num++;
			data[data_num] = ADC_data[2];
			data_num++;
			data[data_num] = ADC_data[3];
			data_num++;
			cnt = 0;
		}
		if((tim_num < (64-4))){
			TIM_B[tim_num] = __HAL_TIM_GET_COUNTER(&htim3);
			TIM_C[tim_num] = __HAL_TIM_GET_COUNTER(&htim1);
			TIM_A[tim_num] = __HAL_TIM_GET_COUNTER(&htim2);
			tim_num++;
		}
	}
	IsNewVal = 1;
	cnt ++;
}

// Po odebraniu danych z UART + DMA wyowulje sie przerwanie
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	HAL_UART_Receive_DMA(&huart1, rx_buffer, 2);		// Chcemy obierac dalej
	HAL_UART_Transmit_DMA(&huart1, ToSend, 1);			// Odsylamy warrtosc obrotow
	ToSend[0] = 0;										// resetujemy zmienna obrotow
	if ((rx_buffer[0] == 0) || rx_buffer[1] ==0){		// Zatrzymanie awaryjne
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM4_Init(void);
static void MX_ADC2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*

*/
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 0);
  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, 1);

  DRV_Init(hspi1);

  HAL_TIM_Base_Start(&htim4);

  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);		// LED

  StartPWM();

  HAL_UART_Receive_DMA(&huart1, rx_buffer, 2);

  //HAL_UART_Transmit_DMA(&huart1, tx_buffer, 3);
  HAL_Delay(1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */



  uint16_t ValueArray[10] = {0x0002,0x0003,0x0003,0x0003,0x0003,0x0003,0x0003,0x0003,0x0003,0x0003};
  uint16_t SpeedArray[10] = {0xff00,0xf800,0xf000,0x8000,0x0fff,0x0ff0,0x0f00,0x0800,0x00ff,0x00f0};
  uint8_t Presc = 8;
  uint16_t Value = Presc;
  uint32_t Speed = 0x0007ffff;

  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1);
  HAL_Delay(500);
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
  HAL_Delay(3000);

  HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 1);
  HAL_ADC_Start_DMA(&hadc1, ADC_data, 4);		// Po konwersji ADC, DMA zapisuje odczyty
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 8);
  HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_3);	// Pulse wyzwala ADC

// WSTAWKA 0.25A
  /*
	uint8_t send1[2] = {0x78,(5 << 3) + 0x3};			// Read command, MSB is shifted in and out first
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, 0);
	uint8_t res = HAL_SPI_TransmitReceive(&hspi1, send1, buf, 2, 1000);
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, 1);

	Delay_Tick(0x0f);

	uint8_t send2[2] = {0x78,(6 << 3) + 0x3};
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, 0);
	res = HAL_SPI_TransmitReceive(&hspi1, send2, buf, 2, 1000);
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, 1);
*/

	/*
	  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);

	SetFloating_A();
	SetFloating_B();
	SetFloating_C();
	  HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 0);

*/
	//Speed = Speed - 0x0ff;
		  //Speed = SpeedArray[a];
		  //Value = ValueArray[a];
  //}
  IsDMA = 1;
__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 3);
uint16_t ticks = MIN_TICKS;

///////////////////////
if(COLLECT_DATA == 1){
	HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 1);
	int c = 0;
	int val = 8;
	Speed = 0x00070000;
	for (; Speed > 0x00030000; Speed -= 0x00001fff){		// 1
		SixStep(Speed, val);
		++c;
		if (c == 4){
			++val;
			c = 0;
		}
	}
	// Reset Stored Values
	data_num = 0;
	cnt = 0;
	tim_num = 0;
	for (; Speed > 0x00006000; Speed = Speed - 0x00000fff){		// 1
		SixStep(Speed, val);
	}
	val+=8;
while(1)SixStep(Speed, val);

	uint16_t New_data[4096];
	for (int a = 0; a < 4096; a ++){
		New_data[a] = data[a];
	}

	HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 0);
	SetFloating_A();
	SetFloating_B();
	SetFloating_C();
	IsRunning = 0;
}

///////////////////////
ticks = MIN_TICKS;

while (1){


	if(SCOTTER_PROGRAM){
		Break = 0;
		if (HAL_GPIO_ReadPin(D1_INPUT_GPIO_Port, D1_INPUT_Pin) == 1){
			Break += 1;
		}
		if (HAL_GPIO_ReadPin(D2_INPUT_GPIO_Port, D2_INPUT_Pin) == 1){
			Break += 1;
		}
		if (Break > 0){
			IsBreaking = 1;
			Power = 0;
			ticks = MIN_TICKS;
			HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 1);
			if(IsRunning) {			// Gdy hamujemy z jazdy to synchronizujący delay
				IsRunning = 0;
				SetFloating_A();
				SetFloating_B();
				SetFloating_C();
				HAL_Delay(10);
			}
			SetZero_A();
			SetZero_B();
			SetZero_C();
			HAL_Delay(Break);
			SetFloating_A();
			SetFloating_B();
			SetFloating_C();
			HAL_Delay(4-Break);
			continue;
		}

		if(IsBreaking) {
			SetFloating_A();
			SetFloating_B();
			SetFloating_C();
			HAL_Delay(10);
			IsBreaking = 0;
		}
		HAL_ADC_Start(&hadc2); // start the adc
		HAL_ADC_PollForConversion(&hadc2, 100); // poll for conversion
		rx_buffer[0] = rx_buffer[1] = Power = adc_val = HAL_ADC_GetValue(&hadc2)/32; // get the adc value
		HAL_ADC_Stop(&hadc2); // stop adc
		if (Power < 28) Power = 0;
		if (Power > 128-28) Power = 128;
		if (Power > 0){
			// Kręcimy normalnie								// Kręcimy do przodu
			HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 1);
			ticks = BEMF_SixStep_TEST(Power, ticks);
			IsRunning = 1;
		}else {
			HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 0);
			SetFloating_A();
			SetFloating_B();
			SetFloating_C();
			IsRunning = 0;
			ticks = MIN_TICKS;
		}
		continue;
	}
	Power = rx_buffer[1];
	//HAL_Delay(1);
	if(Power == 0){								// wartosc = 0, stop
		HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 0);
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		IsRunning = 0;
		ticks = MIN_TICKS;
		Function = rx_buffer[0];				// Zmiana funkcji jest mzliwa jedynie gdy silnik stoi
		if (SCOTTER_PROGRAM) HAL_Delay(10);		// Synchronizujący delay
	}else if(Function >= 64){					// Jezeli to funkcja z jakas wartoscia
		HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 1);
		if (Power < 28) Power = 28;				// Zabezpieczenie by nie dac sygnalo krotszego niz obsluguje sterownik
		else if (Power > 128-28) Power = 128;	// Zabezpieczenie by nie dac sygnalo krotszego niz obsluguje sterownik

		if (IsRunning == 0){					// Startujemy
			int Speed;
			int cnt = 0;
			int val = 8;
			switch (Function){
			case SET_MOTOR_1_SPEED_FORWAD_SEQ_1: {
				// Sekwencja rozruchowa dla silnika: TAROT_SZYBKI
				Speed = 0x00070000;
				for (; Speed > 0x00030000; Speed -= 0x00001fff){		// 1
					SixStep(Speed, val);
					++cnt;
					if (cnt == 4){
						++val;
						cnt = 0;
					}
				}

				for (; Speed > 0x00006000; Speed = Speed - 0x00000fff){		// 1
					SixStep(Speed, val);
				}
				break;
			}
			case SET_MOTOR_1_SPEED_BACK_SEQ_1: {
				// Sekwencja rozruchowa dla silnika: TAROT_SZYBKI
				Speed = 0x00070000;
				for (; Speed > 0x00030000; Speed -= 0x00001fff){		// 1
					SixStep_rev(Speed, val);
					++cnt;
					if (cnt == 4){
						++val;
						cnt = 0;
					}
				}

				for (; Speed > 0x00006000; Speed = Speed - 0x00000fff){		// 1
					SixStep_rev(Speed, val);
				}
				break;
			}
			case SET_MOTOR_1_SPEED_FORWAD_SEQ_2: {
				break;
			}
			case SET_MOTOR_1_SPEED_BACK_SEQ_2: {
				break;
			}
			case SET_MOTOR_1_SPEED_FORWAD_SEQ_3: {
				break;
			}
			case SET_MOTOR_1_SPEED_BACK_SEQ_3: {
				break;
			}
			default: {
				// Błąd
			}
			}
		}
		// Kręcimy normalnie
		if(Function % 2 == 1){					// Kręcimy do tylu
			ticks = BEMF_SixStep_TEST_rev(Power, ticks);

		}else{									// Kręcimy do przodu
			ticks = BEMF_SixStep_TEST(Power, ticks);
		}

		IsRunning = 1;
	} else {								// Nie funkcja z wartoscia -> Bład
		HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 0);
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		IsRunning = 0;
		ticks = MIN_TICKS;
	}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  //PWMCL.Pulse =  PWMCL.Pulse - 1;
	  //HAL_TIM_PWM_ConfigChannel(&htim1, &PWMCL, TIM_CHANNEL_1);
	  //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);


	  //uint8_t test[2] = {0,2};
	  //DRV_Write(IC_OPERATION, test);
/*
	  for(int a = 1; a < 7; a++){
		  //HAL_Delay(1);
		  //DRV_Read(a, buf);

		  	uint8_t send[2] = {0,(a << 3) + 0x80};			// Read command, MSB is shifted in and out first
		  	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, 0);
		  	//HAL_Delay(1);
		  	//Delay_Tick(0x0f);
		  	uint8_t res = HAL_SPI_TransmitReceive(&hspi1, send, buf, 2, 1000);
		  	//HAL_Delay(1);
		  	//Delay_Tick(0x0f);
		  	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, 1);
		  	//HAL_Delay(1);

	  }
	  */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL14;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC3;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 4;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */
  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = PWM_PRESC;
  htim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  htim1.Init.Period = PWM_PERIOD;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC3REF;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = PWM_PRESC;
  htim2.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  htim2.Init.Period = PWM_PERIOD;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = PWM_PRESC;
  htim3.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  htim3.Init.Period = PWM_PERIOD;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SPI_CS_Pin|PWM_AL_Pin|PWM_BL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PWM_CL_Pin|LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : ENGATE_Pin */
  GPIO_InitStruct.Pin = ENGATE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ENGATE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : NFAULT_Pin */
  GPIO_InitStruct.Pin = NFAULT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(NFAULT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI_CS_Pin PWM_AL_Pin PWM_BL_Pin */
  GPIO_InitStruct.Pin = SPI_CS_Pin|PWM_AL_Pin|PWM_BL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PWM_CL_Pin LED_Pin */
  GPIO_InitStruct.Pin = PWM_CL_Pin|LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : D2_INPUT_Pin D1_INPUT_Pin */
  GPIO_InitStruct.Pin = D2_INPUT_Pin|D1_INPUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure peripheral I/O remapping */
  __HAL_AFIO_REMAP_PD01_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
