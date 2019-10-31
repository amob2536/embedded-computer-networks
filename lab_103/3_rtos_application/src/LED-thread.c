/* 
 * blinky_thread.c - adapted
 *
 * simple thread to handle blinking some leds on the discovery board - updated
 * for the stm32f7xx hal libraries
 *
 * author:    Dr. Alex Shenfield
 * date:      01/09/2018
 * purpose:   55-604481 embedded computer networks : lab 103
 */

#include "main.h"

// include the basic headers for the hal drivers
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"

// include the shu bsp libraries for the stm32f7 discovery board
#include "clock.h"
#include "gpio.h"
#include "pinmappings.h"

// HARDWARE DEFINES

// the mailbox we are pulling data from is declared elsewhere ...
extern osMailQId mail_box_3;

// specify some leds
gpio_pin_t ledb1 = {PA_8, GPIOA, GPIO_PIN_8};
gpio_pin_t ledb2 = {PA_15, GPIOA, GPIO_PIN_15};
gpio_pin_t ledb3 = {PI_2, GPIOI, GPIO_PIN_2};

// RTOS DEFINES

// declare the thread function prototypes
void led_1_thread(void const *argument);
void led_2_thread(void const *argument);
void led_3_thread(void const *argument);

// declare the thread ids
osThreadId tid_led_1_thread;
osThreadId tid_led_2_thread;
osThreadId tid_led_3_thread;

// define thread priorities
osThreadDef(led_1_thread, osPriorityNormal, 1, 0);

// OTHER FUNCTIONS

// function prototype for our dumb delay function
void dumb_delay(uint32_t delay);

// THREAD INITIALISATION

// create the threads
int init_LED_thread(void)
{
  // initialize peripherals here
  init_gpio(ledb1, OUTPUT);
  init_gpio(ledb2, OUTPUT);
  init_gpio(ledb3, OUTPUT);
  
  // create the thread and get its taks id
  tid_led_1_thread = osThreadCreate(osThread(led_1_thread), NULL);
  
  // check if everything worked ...
  if(!(tid_led_1_thread))
  {
    return(-1);
  }
  return(0);
}

// ACTUAL THREADS

// blink led 1
void led_1_thread(void const *argument)
{
  while(1)
  {
		// get the data ...
    osEvent evt = osMailGet(mail_box_3, osWaitForever);
    if(evt.status == osEventMail)
		{			
      alarm_mail_t *alarm_mail = (alarm_mail_t*)evt.value.p;
			
			if(alarm_mail->Light_Alarm == 1)
				write_gpio(ledb1, HIGH);
			else
				write_gpio(ledb1, LOW);
			
			if(alarm_mail->Temp_Alarm == 1)
				write_gpio(ledb2, HIGH);
			else
				write_gpio(ledb2, LOW);
			
			if((alarm_mail->Light_Alarm == 1)&&(alarm_mail->Temp_Alarm == 1))
				write_gpio(ledb3, HIGH);
			else
				write_gpio(ledb3, LOW);
			
			osMailFree(mail_box_3, alarm_mail);
		}
  }
}

// OTHER FUNCTIONS

// dumb delay function
void dumb_delay(uint32_t delay)
{
  // just spin through processor cycles to introduce a delay 
  long delaytime;
  for(delaytime = 0; delaytime < (delay * 10000); delaytime++)
  {
    __nop();
  }
}
