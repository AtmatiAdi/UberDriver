/*
 * application.h
 *
 *  Created on: Mar 1, 2022
 *      Author: atmat
 */

#ifndef INC_APPLICATION_H_
#define INC_APPLICATION_H_

#include "main.h"
#include "control.h"

// COMMANDS DEFINITION FOR COMMUNICATIONS
#define SET_MOTOR_1_SPEED_FORWAD    	64   	// 0
#define SET_MOTOR_1_SPEED_BACK         	65   	// 1
#define	SET_MOTOR_1_SPEED_FORWAD_SEQ_1	66		// 2
#define SET_MOTOR_1_SPEED_BACK_SEQ_1	67		// 3
#define	SET_MOTOR_1_SPEED_FORWAD_SEQ_2	68		// 4
#define SET_MOTOR_1_SPEED_BACK_SEQ_2	69		// 5
#define	SET_MOTOR_1_SPEED_FORWAD_SEQ_3	70		// 6
#define SET_MOTOR_1_SPEED_BACK_SEQ_3	71		// 7

#define SCAN_ONE_VARIABLE               80    // 16

void Application_Init(UART_HandleTypeDef *_huart);
void CollectData();
void ScotterProgram();
void NormalControl();
void Application_Update();
void Test_Sequence_01();

#endif /* INC_APPLICATION_H_ */
