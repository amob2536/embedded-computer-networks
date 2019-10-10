/*
 * main.c
 *
 * this is the skeleton for task 3
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

	gpio_pin_t TDR = {PA_0, GPIOA, GPIO_PIN_0};
	init_adc(TDR);
	
	float volt = 0;
	float temp = 0;
	float adc_val = 0;
	
	while(1)
	{
		adc_val = read_adc(TDR);
		
	  // format a string based around the adc value and print to lcd
    char str[24];
    sprintf(str, "ADC = %.0f", adc_val);
    BSP_LCD_DisplayStringAtLine(1, (uint8_t *)str);
		
		volt = ((adc_val*3)/4096.0);
		temp = ((volt*1000)-500)/100.0;
		
		char str2[24];
    sprintf(str2, "Voltage = %.2f V", volt);
    BSP_LCD_DisplayStringAtLine(2, (uint8_t *)str2);
		
		char str3[24];
    sprintf(str3, "Temp = %6.1f C", temp);
    BSP_LCD_DisplayStringAtLine(3, (uint8_t *)str3);
		
		HAL_Delay(100);
	}
}
