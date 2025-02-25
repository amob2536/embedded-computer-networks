/*
 * main.h
 *
 * this is the header for the main rtos mail queue application
 *
 * author:    Dr. Alex Shenfield
 * date:      01/09/2017
 * purpose:   55-604481 embedded computer networks : lab 103
 */

// define to prevent recursive inclusion
#ifndef __MAIN_H
#define __MAIN_H

// include the basic headers for the hal drivers and the rtos library
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"

// set up our mailbox

// mail data structures

typedef struct 
{
  uint16_t   LDR_ADC;
	uint16_t   TDR_ADC;
} 
daq_mail_t;

typedef struct
{
	uint16_t		Temp_Alarm;
	uint16_t		Light_Alarm;
}
alarm_mail_t;

typedef struct
{
	float				LDR_Percentage;
	float				TDR_Temperature;
} 
calc_mail_t;

#endif // MAIN_H
