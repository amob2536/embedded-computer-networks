/*
 * data_generation_thread.c - adapted
 *
 * this is a thread that periodically generates some data and puts it in a 
 * mail queue
 *
 * author:    Dr. Alex Shenfield
 * date:      06/09/2018
 * purpose:   55-604481 embedded computer networks : lab 103
 */
 
// include cmsis_os for the rtos api
#include "cmsis_os.h"

// include main.h (this is where we initialise the mail type and queue)
#include "main.h"

// include the basic headers and hal drivers
#include "stm32f7xx_hal.h"

// include the shu bsp libraries for the stm32f7 discovery board
#include "pinmappings.h"
#include "clock.h"
#include "gpio.h"
#include "adc.h"

// RTOS DEFINES

// declare the thread function prototypes, thread id, and priority
void data_thread(void const *argument);
osThreadId tid_DAQ_thread;
osThreadDef(data_thread, osPriorityNormal, 1, 0);

// set up the mail queue
osMailQDef(mail_box_1, 16, daq_mail_t);
osMailQId  mail_box_1;

// HARDWARE DEFINES
gpio_pin_t LDR = {PF_8, GPIOF, GPIO_PIN_8};
gpio_pin_t TDR = {PA_0, GPIOA, GPIO_PIN_0};

// led is on PI 1 (this is the inbuilt led)
gpio_pin_t led1 = {PI_1, GPIOI, GPIO_PIN_1};

// THREAD INITIALISATION

// create the data generation thread
int init_DAQ_thread(void)
{
  // initialize peripherals (i.e. the led and random number generator) here
  init_gpio(led1, OUTPUT);
	init_adc(LDR);
	init_adc(TDR);
  
  // create the mailbox
  mail_box_1 = osMailCreate(osMailQ(mail_box_1), NULL);
  
  // create the thread and get its task id
  tid_DAQ_thread = osThreadCreate(osThread(data_thread), NULL);
  
  // check if everything worked ...
  if(!tid_DAQ_thread)
  {
    return(-1);
  }
  return(0);
}

// ACTUAL THREADS

// data generation thread - create some random data and stuff it in a mail 
// queue
void data_thread(void const *argument)
{ 
  while(1)
  {
		//DAQ
		int ldr_val = read_adc(LDR);
		int tdr_val = read_adc(TDR);
		
    // create our mail (i.e. the message container)   
    daq_mail_t* mail = (daq_mail_t*) osMailAlloc(mail_box_1, osWaitForever);    
    
    // mail our data
    mail->LDR_ADC = ldr_val;
		mail->TDR_ADC = tdr_val;
    
    // put the data in the mail box and wait for 100ms
    osMailPut(mail_box_1, mail);
    osDelay(100);
  }
} 
