/*
 * DRV8305.h
 *
 *  Created on: Sep 3, 2020
 *      Author: atmat
 */

#ifndef INC_DRV8305_H_
#define INC_DRV8305_H_

#include "stm32f1xx_hal.h"   //** Change this according to your STM32 series **//

#define WARNINGS_WATCHDOG_RESET 	0x1
	#define FAULT						0x400	// Fault indication
	#define TEMP_FLAG4					0x100	// Temperature flag setting for approximately 175°C
	#define	PVDD_UVFL					0x80	// PVDD undervoltage flag warning
	#define	PVDD_OVFL 					0x40	// PVDD overvoltage flag warning
	#define	VDS_STATUS					0x20	// Real time OR of all VDS overcurrent monitors
	#define	VCHP_UVFL					0x10	// Charge pump undervoltage flag warning
	#define	TEMP_FLAG1					0x8		// Temperature flag setting for approximately 105°C
	#define	TEMP_FLAG2					0x4		// Temperature flag setting for approximately 125°C
	#define	TEMP_FLAG3					0x2		// Temperature flag setting for approximately 135°C
	#define	OTW							0x1		// Overtemperature warning
#define OV_VDS_FAULTS				0x2
	#define VDS_HA						0x400	// VDS overcurrent fault for high-side MOSFET A
	#define VDS_LA						0x200	// VDS overcurrent fault for low-side MOSFET A
	#define VDS_HB						0x100	// VDS overcurrent fault for high-side MOSFET B
	#define	VDS_LB						0x80	// VDS overcurrent fault for low-side MOSFET B
	#define	VDS_HC 						0x40	// VDS overcurrent fault for high-side MOSFET C
	#define	VDS_LC						0x20	// VDS overcurrent fault for low-side MOSFET C
	#define	SNS_C_OCP					0x4		// Sense C overcurrent fault
	#define	SNS_B_OCP					0x2		// Sense B overcurrent fault
	#define	SNS_A_OCP					0x1		// Sense A overcurrent fault
#define IC_FAULTS					0x3
	#define PVDD_UVLO2					0x400	// PVDD undervoltage 2 fault
	#define WD_FAULT					0x200	// Watchdog fault
	#define OTSD						0x100	// Overtemperature fault
	#define	VREG_UV 					0x40	// VREG undervoltage fault
	#define	AVDD_UVLO					0x20	// AVDD undervoltage fault
	#define	VCP_LSD_UVLO2				0x10	// Low-side gate supply fault
	#define	VCPH_UVLO2					0x4		// High-side charge pump undervoltage 2 fault
	#define	VCPH_OVLO					0x2		// High-side charge pump overvoltage fault
	#define	VCPH_OVLO_ABS				0x1		// High-side charge pump overvoltage ABS fault
#define VGS_FAULTS					0x4
	#define VGS_HA						0x400	// VGS gate drive fault for high-side MOSFET A
	#define VGS_LA						0x200	// VGS gate drive fault for low-side MOSFET A
	#define VGS_HB						0x100	// VGS gate drive fault for high-side MOSFET B
	#define	VGS_LB						0x80	// VGS gate drive fault for low-side MOSFET B
	#define	VGS_HC 						0x40	// VGS gate drive fault for high-side MOSFET C
	#define	VGS_LC						0x20	// VGS gate drive fault for low-side MOSFET C
#define HS_GATE_DRIVE_CONTROL		0x5
	#define TDRIVEN						0x300	// High-side gate driver peak source time
												// b'00 - 220 ns
												// b'01 - 440 ns
												// b'10 - 880 ns
												// B'11_-_1780_NS
	#define	IDRIVEN_HS					0xF0	// High-side gate driver peak sink current
												// b'0000 - 20 mA	b'0001 - 30 mA	b'0010 - 40 mA	b'0011 - 50 mA
												// B'0100_-_60_MA	b'0101 - 70 mA	b'0110 - 80 mA	b'0111 - 0.25 A
												// b'1000 - 0.50 A	b'1001 - 0.75 A	b'1010 - 1.00 A	b'1011 - 1.25 A
												// b'1100 - 60 mA	b'1101 - 60 mA	b'1110 - 60 mA	b'1111 - 60 mA
	#define	IDRIVEP_HS					0xF	// High-side gate driver peak source current
												// b'0000 - 10 mA	b'0001 - 20 mA	b'0010 - 30 mA	b'0011 - 40 mA
												// B'0100_-_50_MA	b'0101 - 60 mA	b'0110 - 70 mA	b'0111 - 0.125 A
												// b'1000 - 0.25 A	b'1001 - 0.50 A	b'1010 - 0.75 A	b'1011 - 1.00 A
												// b'1100 - 50 mA	b'1101 - 50 mA	b'1110 - 50 mA	b'1111 - 50 mA
#define LS_GATE_DRIVE_CONTROL		0x6
	#define TDRIVEN						0x300	// Low-side gate driver peak source time
												// b'00 - 220 ns
												// b'01 - 440 ns
												// b'10 - 880 ns
												// B'11_-_1780_NS
	#define	IDRIVEN_HS					0xF0	// Low-side gate driver peak sink current
												// b'0000 - 20 mA	b'0001 - 30 mA	b'0010 - 40 mA	b'0011 - 50 mA
												// B'0100_-_60_MA	b'0101 - 70 mA	b'0110 - 80 mA	b'0111 - 0.25 A
												// b'1000 - 0.50 A	b'1001 - 0.75 A	b'1010 - 1.00 A	b'1011 - 1.25 A
												// b'1100 - 60 mA	b'1101 - 60 mA	b'1110 - 60 mA	b'1111 - 60 mA
	#define	IDRIVEP_HS					0xF		// Low-side gate driver peak source current
												// b'0000 - 10 mA	b'0001 - 20 mA	b'0010 - 30 mA	b'0011 - 40 mA
												// B'0100_-_50_MA	b'0101 - 60 mA	b'0110 - 70 mA	b'0111 - 0.125 A
												// b'1000 - 0.25 A	b'1001 - 0.50 A	b'1010 - 0.75 A	b'1011 - 1.00 A
												// b'1100 - 50 mA	b'1101 - 50 mA	b'1110 - 50 mA	b'1111 - 50 mA

#define GATE_DRIVE_CONTROL			0x7
	#define COMM_OPTION					0x200	// Rectification control (PWM_MODE = b'10 only)
												// b'0 - diode freewheeling
												// B'1_-_ACTIVE_FREEWHEELING
	#define PWM_MODE					0x180	// PWM Mode
												// B'00_-_PWM_WITH_6_INDEPENDENT_INPUTS
												// b'01 - PWM with 3 independent inputs
												// b'10 - PWM with one input
												// b'11 - PWM with 6 independent inputs
	#define	DEAD_TIME 					0x70	// Dead time
												// b'000 - 35 ns	B'001_-_52_NS	b'010 - 88 ns
												// b'011 - 440 ns	b'100 - 880 ns	b'101 - 1760 ns
												// b'110 - 3520 ns	b'111 - 5280 ns
	#define	TBLANK						0xC		// VDS sense blanking
												// b'00 - 0 µs
												// B'01_-_1.75_µS
												// b'10 - 3.5 µs
												// b'11 - 7 µs
	#define	TVDS						0x3		// VDS sense deglitch
												// b'00 - 0 µs
												// b'01 - 1.75 µs
												// B'10_-_3.5_µS
												// b'11 - 7 µs
#define IC_OPERATION				0x9
	#define FLIP_OTSD					0x400	// Enable OTSD
												// b'0 - Disable OTSD
												// B'1_-_Enable_OTSD
	#define DIS_PVDD_UVLO2				0x200	// Disable PVDD_UVLO2 fault and reporting
												// B'0_-_PVDD_UVLO2_ENABLED
												// b'1 - PVDD_UVLO2 disabled
	#define DIS_GDRV_FAULT				0x100	// Disable gate drive fault and reporting
												// B'0_-_GATE_DRIVER_FAULT_ENABLED
												// b'1 - Gate driver fault disabled
	#define	EN_SNS_CLAMP				0x80	//  Enable sense amplifier clamp
												// B'0_-_SENSE_AMPLIFIER_CLAMP_ID_NOT_ENABLED
												// b'1 - Sense amplifier clamp is enabled, limiting output to ~3.3 V
	#define	WD_DLY 						0x60	// Watchdog delay
												// b'00 - 10 ms
												// B'01_-_20_MS
												// b'10 - 50 ms
												// b'11 - 100 ms
	#define	DIS_SNS_OCP					0x10	// Disable SNS overcurrent protection fault and reporting
												// B'0_-_SNS_OCP_ENABLED
												// b'1 - SNS OCP disabled
	#define	WD_EN						0x8		// Watchdog enable
												// B'0_-_WATCH_DOG_DISABLED
												// b'1 - Watch dog enabled
	#define	SLEEP						0x4		// Put device into sleep mode
												// B'0_-_DEVICE_AWAKE
												// b'1 - Device asleep
	#define	CLR_FLTS					0x2		// Clear faults
												// B'0_-_NORMAL_OPERATION
												// b'1 - Clear faults
	#define	SET_VCPH_UV					0x1		// Set charge pump undervoltage threshold level
												// B'0_-_4.9_V
												// b'1 - 4.6 V
#define SHUNT_AMPLIFIER_CONTROL		0xA
	#define DC_CAL_CH3					0x400	// DC calibration of CS amplifier 3
												// B'0_-_NORMAL_OPERATION
												// b'1 - DC calibration mode
	#define DC_CAL_CH2					0x200	// DC calibration of CS amplifier 2
												// B'0_-_NORMAL_OPERATION
												// b'1 - DC calibration mode
	#define DC_CAL_CH1					0x100	// DC calibration of CS amplifier 1
												// B'0_-_NORMAL_OPERATION
												// b'1 - DC calibration mode
	#define	CS_BLANK					0xC0	// Current shunt amplifier blanking time
												// B'00_-_0_NS
												// b'01 - 500 ns
												// b'10 - 2.5 µs
												// b'11 - 10 µs
	#define	GAIN_CS3					0x30	// Gain of CS amplifier 3
												// B'00_-_10_V/V
												// b'01 - 20 V/V
												// b'10 - 40 V/V
												// b'11 - 80 V/V
	#define	GAIN_CS2					0xC		// Gain of CS amplifier 2
												// B'00_-_10_V/V
												// b'01 - 20 V/V
												// b'10 - 40 V/V
												// b'11 - 80 V/V
	#define	GAIN_CS1					0x3		// Gain of CS amplifier 1
												// B'00_-_10_V/V
												// b'01 - 20 V/V
												// b'10 - 40 V/V
												// b'11 - 80 V/V
#define VOLTAGE_REGULATOR_CONTROL	0xB
	#define VREF_SCALE					0x300	// VREF Scaling
												// b'00 - RSVD
												// B'01_-_K_=_2
												// b'10 - k = 4
												// b'11 - RSVD
	#define	SLEEP_DLY					0x18	//Delay to power down VREG after SLEEP
												// b'00 - 0 µs
												// B'01_-_10_µS
												// b'10 - 50 µs
												// b'11 - 1 ms
	#define	DIS_VREG_PWRGD				0x4		// Disable VREG undervoltage fault and reporting
												// B'0_-_VREG_UV_ENABLED
												// b'1 - VREG_UV disabled

	#define	VREG_UV_LEVEL				0x3		// VREG undervoltage set point
												// b'00 - VREG x 0.9
												// b'01 - VREG x 0.8
												// B'10_-_VREG_x_0.7
												// b'11 - VREG x 0.7
#define VDS_SENSE_CONTROL			0xC
	#define	VDS_LEVEL					0xF8	// VDS comparator threshold
												// b'00000 - 0.060 V	b'00001 - 0.068 V	b'00010 - 0.076 V	b'00011 - 0.086 V
												// b'00100 - 0.097 V	b'00101 - 0.109 V	b'00110 - 0.123 V	b'00111 - 0.138 V
												// b'01000 - 0.155 V	b'01001 - 0.175 V	b'01010 - 0.197V	b'01011 - 0.222 V
												// b'01100 - 0.250 V	b'01101 - 0.282 V	b'01110 - 0.317 V	b'01111 - 0.358 V
												// b'10000 - 0.403 V	b'10001 - 0.454 V	b'10010 - 0.511 V	b'10011 - 0.576 V
												// b'10100 - 0.648 V	b'10101 - 0.730 V	b'10110 - 0.822 V	b'10111 - 0.926 V
												// b'11000 - 1.043 V	B'11001_-_1.175_V	b'11010 - 1.324 V	b'11011 - 1.491 V
												// b'11100 - 1.679 V	b'11101 - 1.892 V	b'11110 - 2.131 V	b'11111 - 2.131 V
	#define	VDS_MODE					0x7		// VDS mode
												// B'000_-_LATCHED_SHUT_DOWN_WHEN_OVER-CURRENT_DETECTED
												// b'001 - Report only when over current detected
												// b'010 - VDS protection disabled (no overcurrent sensing or reporting)

void DRV_Init(SPI_HandleTypeDef *hspi);
uint8_t DRV_Read(uint8_t addr, uint8_t *buf);
uint8_t DRV_Write(uint8_t addr, uint16_t data);

#endif /* INC_DRV8305_H_ */
