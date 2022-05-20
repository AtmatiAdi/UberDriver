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
uint8_t tx_buffer[1] = {0};
uint8_t Function = 0;
uint16_t Power = 0;
uint32_t Speed = 0x0007ffff;
uint8_t Break = 0;
uint8_t IsRunning = 0;
uint8_t IsBreaking = 0;
uint16_t ticks = MIN_TICKS;
uint8_t step_delay = 255;	// (255+1)/32 = 8 times shorter t3 = (t1+t2)/8
uint8_t MessageRecived = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *_huart){
	//HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	MessageRecived = 1;
}

void Application_Init(UART_HandleTypeDef *_huart){
	huart = _huart;
	HAL_UART_Receive_DMA(huart, rx_buffer, 3);
}


void NormalControl(){
	if(MessageRecived){
		MessageRecived = 0;
		Function = rx_buffer[0];
		Power = (rx_buffer[1] >> 1);	// zalezy od max period
		step_delay = rx_buffer[2];
		if((Function == SET_MOTOR_1_SPEED_FORWAD) && (Power > 0)){					// Jezeli to funkcja z jakas wartoscia
			HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 1);
			Set_PWM(Power + 128);	// zalezy od max period
			Set_Observer_Div(step_delay/8);
		}else if(Function == SCAN_ONE_VARIABLE){
			EnableScan();
		}else{
			Set_PWM(0);
			SetFloating_A();
			SetFloating_B();
			SetFloating_C();
			HAL_GPIO_WritePin(ENGATE_GPIO_Port, ENGATE_Pin, 0);
		}
		HAL_UART_Receive_DMA(huart, rx_buffer, 3);		// Chcemy obierac dalej
		HAL_UART_Transmit_DMA(huart, tx_buffer, 1);			// Odsylamy warrtosc obrotow
	}
	if (IsScanReady() == 1){
		HAL_UART_Transmit_DMA(huart, GetScanData(), SCAN_SIZE);
	}
}

void Application_Update(){
	NormalControl();
//	ADC_Change_Order(ADC_CHANNEL_A);//2
//	HAL_Delay(100);
//	ADC_Change_Order(ADC_CHANNEL_B);//1
//	HAL_Delay(100);
//	ADC_Change_Order(ADC_CHANNEL_C);//3
//	HAL_Delay(100);
}
