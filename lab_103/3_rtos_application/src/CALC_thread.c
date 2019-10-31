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

// include the shu bsp libraries for the stm32f7 discovery board
#include "pinmappings.h"
#include "clock.h"
#include "random_numbers.h"
#include "gpio.h"

// RTOS DEFINES

// declare the thread function prototypes, thread id, and priority
void CALC_thread(void const *argument);
osThreadId tid_CALC_thread;
osThreadDef(CALC_thread, osPriorityNormal, 1, 0);

extern osMailQId mail_box_1;

// set up the mail queues
osMailQDef(mail_box_2, 16, calc_mail_t);
osMailQId  mail_box_2;

osMailQDef(mail_box_3, 16, alarm_mail_t);
osMailQId  mail_box_3;

// THREAD INITIALISATION

// create the data generation thread
int init_CALC_thread(void)
{  
  // create the mailboxs
  mail_box_2 = osMailCreate(osMailQ(mail_box_2), NULL);
	mail_box_3 = osMailCreate(osMailQ(mail_box_3), NULL);
	
  // create the thread and get its task id
  tid_CALC_thread = osThreadCreate(osThread(CALC_thread), NULL);
  
  // check if everything worked ...
  if(!tid_CALC_thread)
  {
    return(-1);
  }
  return(0);
}

// ACTUAL THREADS

// data generation thread - create some random data and stuff it in a mail 
// queue
void CALC_thread(void const *argument)
{  	
	uint16_t light_low = 0;
	uint16_t temp_low = 0;
	
  while(1)
  {
		// get the data ...
    osEvent evt = osMailGet(mail_box_1, osWaitForever);
    if(evt.status == osEventMail)
    {
      daq_mail_t *daq_mail = (daq_mail_t*)evt.value.p;		
			
			// create our mail (i.e. the message container)   
			calc_mail_t* calc_mail = (calc_mail_t*) osMailAlloc(mail_box_2, osWaitForever);  
      calc_mail->LDR_Percentage = (100*(daq_mail->LDR_ADC))/4095.0;
      calc_mail->TDR_Temperature = ((((daq_mail->TDR_ADC)*3.3)/4095.0)-0.5) * 100;

			alarm_mail_t* alarm_mail = (alarm_mail_t*) osMailAlloc(mail_box_3, osWaitForever);
			
			if(calc_mail->LDR_Percentage <= 50)
				light_low = 1;
			else
				light_low = 0;
			
			if(calc_mail->TDR_Temperature <= 18)
				temp_low = 1;
			else
				temp_low = 0;
			
			alarm_mail->Light_Alarm = light_low;
			alarm_mail->Temp_Alarm = temp_low;
			
			
			// put the data in the mail boxs and wait for 100ms
			osMailPut(mail_box_2, calc_mail);			
			osMailPut(mail_box_3, alarm_mail);
			
			// free the daq mail box
      osMailFree(mail_box_1, daq_mail);	
    }
	}
} 
