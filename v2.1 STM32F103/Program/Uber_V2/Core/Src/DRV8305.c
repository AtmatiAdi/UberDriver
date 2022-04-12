#include "DRV8305.h"


/*
 * DRV8305.c
 *
 *  Created on: Sep 3, 2020
 *      Author: atmat
 */
static SPI_HandleTypeDef *DRV_spi;

void DRV_Init(SPI_HandleTypeDef *hspi){
	//Copy SPI handle variable
	DRV_spi = hspi;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
	HAL_Delay(1);
}

uint8_t DRV_Read(uint8_t addr, uint8_t *buf){
	uint8_t data[2] = {0,(addr << 3) + 0x80};
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
	HAL_Delay(1);
	uint8_t res = HAL_SPI_TransmitReceive(DRV_spi, data, buf, 2, 100);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
	return res;
}

uint8_t DRV_Write(uint8_t addr, uint16_t data){
	uint8_t buf[2] = {(uint8_t)(data), ( addr << 3) + (uint8_t)(data >> 8)};
	uint8_t ret[2];
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
	HAL_Delay(1);
	uint8_t res = HAL_SPI_TransmitReceive(DRV_spi, buf, ret, 1, 100);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
	return res;
}
