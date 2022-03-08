/*
 * application.c
 *
 *  Created on: Mar 1, 2022
 *      Author: atmat
 */
#include <application.h>

#define COLLECT_DATA  		0
#define SCOTTER_PROGRAM  	0
#define NORMAL_CONTROL	 	1

static UART_HandleTypeDef *huart;
uint8_t rx_buffer[3] = {0,0,0};
uint8_t Function = 0;
uint8_t Power = 0;
uint32_t Speed = 0x0007ffff;
uint8_t Break = 0;
uint8_t IsRunning = 0;
uint8_t IsBreaking = 0;
uint16_t ticks = MIN_TICKS;
uint8_t Rotations[1] = {0};
float step_delay = 255;	// (255+1)/32 = 8 times shorter t3 = (t1+t2)/8

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *_huart){
	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	HAL_UART_Receive_DMA(huart, rx_buffer, 3);		// Chcemy obierac dalej
	HAL_UART_Transmit_DMA(huart, Rotations, 1);			// Odsylamy warrtosc obrotow
	Rotations[0] = 0;										// resetujemy zmienna obrotow
	// DONT UPDATE FUNCTION WHEN WE GET SPECIAL FUNCTION
	if(rx_buffer[0] == SCAN_ONE_VARIABLE){
		EnableScan();
	} else
	{
		Function = rx_buffer[0];
		Power = rx_buffer[1];
		step_delay = rx_buffer[2];
	}
	if ((Function == 0) || Power == 0){		// Zatrzymanie awaryjne
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
	}
}

void Application_Init(UART_HandleTypeDef *_huart){
	//memcpy(&huart, &_huart, sizeof(_huart)); // This shit does not work
	huart = _huart;
	HAL_UART_Receive_DMA(huart, rx_buffer, 3);
}

void ScotterProgram(){
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
		return;
	}

	if(IsBreaking) {
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		HAL_Delay(10);
		IsBreaking = 0;
	}
	//HAL_ADC_Start(&hadc2); // start the adc
	//HAL_ADC_PollForConversion(&hadc2, 100); // poll for conversion
	//rx_buffer[0] = rx_buffer[1] = Power = adc_val = HAL_ADC_GetValue(&hadc2)/32; // get the adc value
	//HAL_ADC_Stop(&hadc2); // stop adc
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
	return;
}

void CollectData(){
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
	//data_num = 0;
	//cnt = 0;
	//tim_num = 0;
	for (; Speed > 0x00006000; Speed = Speed - 0x00000fff){		// 1
		SixStep(Speed, val);
	}
	val+=8;
	while(1)SixStep(Speed, val);

	//uint16_t New_data[4096];
	for (int a = 0; a < 4096; a ++){
		//New_data[a] = data[a];
	}

	HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 0);
	SetFloating_A();
	SetFloating_B();
	SetFloating_C();
	IsRunning = 0;
}

void NormalControl(){
	if(Power == 0){								// wartosc = 0, stop
		HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 0);
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		IsRunning = 0;
		ticks = MIN_TICKS;
		//Function = rx_buffer[0];				// Zmiana funkcji jest mzliwa jedynie gdy silnik stoi
	}else if(Function >= 64){					// Jezeli to funkcja z jakas wartoscia
		HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 1);
		uint16_t pwm = Power*3;
		if (pwm < 8) pwm = 8;				// Zabezpieczenie by nie dac sygnalo krotszego niz obsluguje sterownik
		else if (pwm > (640-1)-8) pwm = (640-1);	// Zabezpieczenie by nie dac sygnalo krotszego niz obsluguje sterownik

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
			ticks = BEMF_SixStep_TEST_3_rev(pwm, ticks,step_delay/16);

		}else{									// Kręcimy do przodu
			ticks = BEMF_SixStep_TEST_3_rev(pwm, ticks,step_delay/16);
		}
		Rotations[0] += 1;

		IsRunning = 1;
	}else {								// Nie funkcja z wartoscia -> Bład
		HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 0);
		SetFloating_A();
		SetFloating_B();
		SetFloating_C();
		IsRunning = 0;
		ticks = MIN_TICKS;
	}
	if (IsScanReady() == 1){
		HAL_UART_Transmit_DMA(huart, GetScanData(), 4096);
	}
}

void Application_Update(){

	if(COLLECT_DATA ){
		CollectData();
	}
	else if(SCOTTER_PROGRAM){
		ScotterProgram();
	}
	else if(NORMAL_CONTROL){
		NormalControl();
	}
	else {

		HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 1);
		SetZero_A();
		SetZero_B();
		SetZero_C();
	}
}

void Test_Sequence_01(){
	int cnt = 0;
	int val = 8;
	int Speed;
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
}
