/*
 * main.c
 *
 * this is the skeleton for task 4
 *
 * author:    Dr. Alex Shenfield
 * date:      04/09/2018
 * purpose:   55-604481 embedded computer networks : lab 102
 */

// include the basic headers and hal drivers
#include "stm32f7xx_hal.h"

// include the shu bsp libraries for the stm32f7 discovery board
#include "pinmappings.h"
#include "clock.h"
#include "adc.h"
#include "stm32746g_discovery_lcd.h"

// LCD DEFINES

// CODE

// this is the main method
int main()
{
  // we need to initialise the hal library and set up the SystemCoreClock 
  // properly
  HAL_Init();
  init_sysclk_216MHz();
	
  // initialise the lcd
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, SDRAM_DEVICE_ADDR);
  BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
	
	// set the background colour to blue and clear the lcd
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_Clear(LCD_COLOR_BLUE);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  
  // set the font to use
  BSP_LCD_SetFont(&Font24); 

	gpio_pin_t LDR = {PF_8, GPIOF, GPIO_PIN_8};
	init_adc(LDR);
	gpio_pin_t TDR = {PA_0, GPIOA, GPIO_PIN_0};
	init_adc(TDR);
	
	uint16_t ldr_val = 0;
	float ldr_max = 0;
	float ldr_min = 0;
	float LDR_percentage = 0;
	uint16_t tdr_val = 0;
	float tdr_volt = 0;
	float Temperature = 0;
	int Calibration_Time = HAL_GetTick();
	int Calibration_Duration = Calibration_Time + 10000;
	
	char str1[24];
  sprintf(str1, "Calibration in Progress...");
  BSP_LCD_DisplayStringAtLine(1, (uint8_t *)str1);
	
	while (HAL_GetTick() < Calibration_Duration)
	{
		ldr_val = read_adc(LDR);
		
		if (ldr_val > ldr_max)
		  ldr_max = ldr_val;
			
		if(ldr_val < ldr_min)
			ldr_min = ldr_val;
	}
	
	BSP_LCD_Clear(LCD_COLOR_BLUE);
	
	while(1)
	{
		ldr_val = read_adc(LDR);
		tdr_val = read_adc(TDR);
		
	  // format a string based around the adc value and print to lcd
    char str1[24];
    sprintf(str1, "LDR = %04d", ldr_val);
    BSP_LCD_DisplayStringAtLine(1, (uint8_t *)str1);
		
		char str2[24];
    sprintf(str2, "TDR = %04d", tdr_val);
    BSP_LCD_DisplayStringAtLine(2, (uint8_t *)str2);
		
		LDR_percentage = ((ldr_val-ldr_min)*100.0)/(ldr_max-ldr_min);
		
		char str3[24];
    sprintf(str3, "LDR Percentage = %6.0f %%", LDR_percentage);
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *)str3);
		
		tdr_volt = ((tdr_val*3.3)/4077.0);
		Temperature = ((tdr_volt*1000.0)-500.0)/10.0;
		
		char str4[24];
    sprintf(str4, "Temperature = %6.1f C", Temperature);
    BSP_LCD_DisplayStringAtLine(5, (uint8_t *)str4);
		
		HAL_Delay(100);
	}
}
