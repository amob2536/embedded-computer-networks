/*
 * b_led_dice.c
 *
 * this is the main application skeleton for the student exercises in task 3
 *
 */

// include the hal drivers
#include "stm32f7xx_hal.h"

// include the shu bsp libraries for the stm32f7 discovery board
#include "pinmappings.h"
#include "clock.h"
#include "gpio.h"
#include "random_numbers.h"

void display(int);

// map the led to gpio pins
gpio_pin_t led1 = {PI_2,  GPIOI, GPIO_PIN_2};
gpio_pin_t led2 = {PA_15,  GPIOA, GPIO_PIN_15};
gpio_pin_t led3 = {PA_8,  GPIOA, GPIO_PIN_8};
gpio_pin_t led4 = {PB_15,  GPIOB, GPIO_PIN_15};
gpio_pin_t led5 = {PB_14,  GPIOB, GPIO_PIN_14};
gpio_pin_t led6 = {PI_1,  GPIOI, GPIO_PIN_1};


// this is the main method
int main()
{
  // we need to initialise the hal library and set up the SystemCoreClock 
  // properly
  HAL_Init();
  init_sysclk_216MHz();
	
	// Init random number generator
	init_random();
	
	 // initialise the gpio pins
  init_gpio(led1, OUTPUT);
	init_gpio(led2, OUTPUT);
  init_gpio(led3, OUTPUT);
	init_gpio(led4, OUTPUT);
  init_gpio(led5, OUTPUT);
  init_gpio(led6, OUTPUT);
	
	while(1)
	{		
		//Generate Random Number
		uint32_t rnd = (get_random_int() % 6) + 1;
		
		display(rnd);
		
		// wait for 1 second
    HAL_Delay(1000);
		
	}
}

void display(int rnd)
{
			write_gpio(led1, LOW);
	    write_gpio(led2, LOW);
	    write_gpio(led3, LOW);
	    write_gpio(led4, LOW);
	    write_gpio(led5, LOW);
	    write_gpio(led6, LOW);
	
			switch(rnd)
	  	{
				case 1:
					toggle_gpio(led1); 
				break;
				case 2: 
					toggle_gpio(led1);
					toggle_gpio(led2);
				break;
				case 3: 
					toggle_gpio(led1);
					toggle_gpio(led2);
					toggle_gpio(led3);
				break;
				case 4: 
					toggle_gpio(led1);
					toggle_gpio(led2);
					toggle_gpio(led3);
					toggle_gpio(led4);			
				break;
				case 5: 
					toggle_gpio(led1);
					toggle_gpio(led2);
					toggle_gpio(led3);
					toggle_gpio(led4);
					toggle_gpio(led5);			
				break;
				case 6: 
					toggle_gpio(led1);
					toggle_gpio(led2);
					toggle_gpio(led3);
					toggle_gpio(led4);
					toggle_gpio(led5);
					toggle_gpio(led6);			
				break;
				default:; 
				break;
		 }
}
