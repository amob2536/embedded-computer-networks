/*
 * main.c
 *
 * this is the skeleton for task 2
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
#include "gpio.h"
#include "stm32746g_discovery_lcd.h"

// LCD DEFINES

// define a message boarder (note the lcd is 28 characters wide using Font24)
#define BOARDER     "****************************"

// specify a welcome message
const char * welcome_message[2] = 
{
  "*     Hello LCD World!     *",
  "*      Welcome to SHU      *"
};

//LED Init
gpio_pin_t led1 = {PB_14, GPIOB, GPIO_PIN_14};
gpio_pin_t led2 = {PB_15, GPIOB, GPIO_PIN_15};

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

	//Init LED
  init_gpio(led1, OUTPUT);
	init_gpio(led2, OUTPUT);
	
  // set the background colour to blue and clear the lcd
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_Clear(LCD_COLOR_BLUE);
  
  // set the font to use
  BSP_LCD_SetFont(&Font24); 
  
  // print the welcome message ...
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAtLine(0, (uint8_t *)BOARDER);
  BSP_LCD_DisplayStringAtLine(1, (uint8_t *)welcome_message[0]);
  BSP_LCD_DisplayStringAtLine(2, (uint8_t *)welcome_message[1]);
  BSP_LCD_DisplayStringAtLine(3, (uint8_t *)BOARDER); 
	
	//ADC Init
	int adc_per = 0;
	int adc_width = 0;
	gpio_pin_t pot = {PA_0, GPIOA, GPIO_PIN_0};
	init_adc(pot);
  
	while(1)
	{
		uint16_t adc_val = read_adc(pot);
		
	  // format a string based around the adc value and print to lcd
    char str[12];
    sprintf(str, "ADC = %4d", adc_val);
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *)str);
		
		//Convert to percentage
		adc_per = (adc_val*100)/(4096); 
		
		char str2[24];
    sprintf(str2, "ADC Percentage = %4d%%", adc_per);
    BSP_LCD_DisplayStringAtLine(5, (uint8_t *)str2);
		
		//Percentage Bar - Screen 480x272	
		adc_width = (adc_per * 480)/100;
		
		BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
		BSP_LCD_FillRect(adc_width, 150, 480-adc_width, 25);
		
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_FillRect(0, 150, adc_width, 25);
		
		if(adc_per >= 25)
		{
			write_gpio(led1, HIGH);
			
			if(adc_per >= 75)
			{
				write_gpio(led2, HIGH);
			}
			else
			{
				write_gpio(led2, LOW);
			}
		}
		else
		{
			write_gpio(led1, LOW);
			write_gpio(led2, LOW);
		}
		
		char str3[24];
    sprintf(str3, "LED 1 = %s", adc_per >= 25 ? "ON " : "OFF");
    BSP_LCD_DisplayStringAtLine(8, (uint8_t *)str3);
		
		char str4[24];
    sprintf(str4, "LED 2 = %s", adc_per >= 75 ? "ON " : "OFF");
    BSP_LCD_DisplayStringAtLine(9, (uint8_t *)str4);

		//Delay
		HAL_Delay(100);
	}
}
