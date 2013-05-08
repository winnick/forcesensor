/**
  ******************************************************************************
  * @file main.c
  * @brief This file contains the main function for this template.
  * @author STMicroelectronics - MCD Application Team
  * @version V2.0.0
  * @date 15-March-2011
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_it.h"
#include "stm8s_adc1.h"
#include "stm8s_gpio.h"
#include "parameter.h"

/* Private defines -----------------------------------------------------------*/
#define CCR1_Val ((uint16_t)976)
/* Private function prototypes -----------------------------------------------*/
void UART_send_buf(uint8_t * buf, uint8_t len);
void UART_send_str(uint8_t * str);

uint16_t average_result(uint16_t *p, uint8_t smp) { 
	uint8_t i;
	uint16_t sum;
	
	for(i=0, sum= 0; i < smp; ++i)
		sum+= *p++;		
	return sum / smp;
}
/* Private functions ---------------------------------------------------------*/
/* Variables -----------------------------------------------------------------*/
	uint8_t  temp_AD_H;		// temporary registers for reading ADC result (MSB)
	uint8_t  temp_AD_L;             // temporary registers for reading ADC result (LSB)
	uint8_t	 ADInit;		// flag for ADC initialized
	uint8_t	 ADSampRdy;		// flag for filed of samples ready
	uint8_t  AD_samp;		// counter of stored samples
	uint16_t AD_sample[NUMB_SAMP];	// store samples field 
	uint16_t AD_avg_value;		// average of ADC result

void main(void)
{
/* Variables -----------------------------------------------------------------*/
uint16_t i;

  
/* Initializing --------------------------------------------------------------*/
CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

// UART1 CONFIGURATION
UART1_DeInit();
UART1_Init((uint32_t) 9600,
           UART1_WORDLENGTH_8D,
           UART1_STOPBITS_1,
           UART1_PARITY_NO,
           UART1_SYNCMODE_CLOCK_DISABLE,
           UART1_MODE_TX_ENABLE);
UART1_Cmd(ENABLE);

// GPIO CONFIGURATION (PC4, PD2, PD3) AS ANALOG INPUTS
GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);
GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT);
GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);


/**
  * ADC1 CONFIGURATION - skonfigurowac pomiar cykliczny wyzwalany timerem
  * co 0,5s
  */

TIM1->ARRH= (uint8_t)(AUTORELOAD >> 8);              // set autoreload register for trigger period
TIM1->ARRL= (uint8_t)(AUTORELOAD); 
TIM1->CCR1H= (uint8_t)((AUTORELOAD-AD_STAB) >> 8);   // set compare register for trigger period
TIM1->CCR1L= (uint8_t)(AUTORELOAD-AD_STAB);
TIM1->CR1|= TIM1_CR1_ARPE;		        // auto reload register is buferred
	
TIM1->CR2= (4<<4) & TIM1_CR2_MMS;	        // CC1REF is used as TRGO
TIM1->CCMR1= (6<<4) & TIM1_CCMR_OCM;	        // CC1REF in PWM 1 mode
TIM1->IER|= TIM1_IER_CC1IE;		        // CC1 interrupt enable
TIM1->CCER1|= TIM1_CCER1_CC1P;		        // CC1 negative polarity
TIM1->CCER1|= TIM1_CCER1_CC1E;		        // CC1 output enable
TIM1->BKR|= TIM1_BKR_MOE;												
	
TIM1->SMCR|=  TIM1_SMCR_MSM;		        // synchronization of TRGO with ADC
	
TIM1->CR1|= TIM1_CR1_CEN;		        // timer 1 enable

ADC1_DeInit();
ADC1_PrescalerConfig(ADC1_PRESSEL_FCPU_D8);
ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_2, ADC1_ALIGN_LEFT);
ADC1_SchmittTriggerConfig(ADC1_SCHMITTTRIG_CHANNEL2, DISABLE);
ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE);
ADC1_ExternalTriggerConfig(ADC1_EXTTRIG_TIM, ENABLE);
// init ADC variables
AD_samp= 0;                                     // number of stored samples 0
ADInit= TRUE;                                   // ADC initialized 
ADSampRdy= FALSE;                               // No sample

ADC1_Cmd(ENABLE);

enableInterrupts();

//UART_send_str("Dzien dobry \n");

i=1023;

UART_send_buf((uint8_t*)&i, sizeof(i)); // wysylanie dwoch bajtow danych


/* Infinite loop */
  while (1)
  {
       if (ADSampRdy == TRUE) {				   // field of ADC samples is ready?
	AD_avg_value= average_result(&AD_sample[0], AD_samp); // average of samples
			
	AD_samp= 0;    		                           // reinitalize ADC variables
	ADSampRdy= FALSE;
	
	UART_send_buf((uint8_t*) &average_result, sizeof(AD_avg_value)); 
        }
  }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
  }

#endif

void UART_send_str(uint8_t * str){
  int i = 0;
  
  while(str[i] != 0)
  {
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
    {
    }
    UART1_SendData8(str[i++]);
  }
}

void UART_send_buf(uint8_t * buf, uint8_t len){
    int i = 0;
    
    for(i = 0; i < len; i++){
      while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
      {
      }
      UART1_SendData8(buf[i]);
    }
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
