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
#include "stdio.h"
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
        
	uint8_t	 ADInit;		// flag for ADC initialized
	uint8_t	 ADSampRdy;		// flag for filed of samples ready
	uint8_t  AD_samp;		// counter of stored samples
	uint16_t AD_sample[NUMB_SAMP];	// store samples field 
	uint16_t AD_avg_value;		// average of ADC result
        
        uint16_t AD_ch2_value;
        uint16_t AD_ch3_value;
        uint16_t AD_ch4_value;
        
        uint8_t Stored_Data [20];
        
void main(void)
{
/* Variables -----------------------------------------------------------------*/

  
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
TIM2_DeInit();
TIM2_TimeBaseInit(TIM2_PRESCALER_512, AUTORELOAD);

TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);



ADC1_DeInit();
ADC1_PrescalerConfig(ADC1_PRESSEL_FCPU_D8);
ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_4, ADC1_ALIGN_LEFT);
ADC1_SchmittTriggerConfig(ADC1_SCHMITTTRIG_CHANNEL4, DISABLE);
ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE);

ADC1_ScanModeCmd(ENABLE);
ADC1_DataBufferCmd(ENABLE);



// init ADC variables
AD_samp = 2;                                     // number of stored samples 0
ADInit = TRUE;                                   // ADC initialized 
ADSampRdy= FALSE;                               // No sample

enableInterrupts();
ADC1_Cmd(ENABLE);
TIM2_Cmd(ENABLE);

/* Infinite loop */
  while (1)
  {
    
       if (ADSampRdy == TRUE) {				   // field of ADC samples is ready?
//      AD_avg_value= average_result(&AD_sample[0], AD_samp); // average of samples
	AD_samp= 2;   		                           // reinitalize ADC variables
	AD_ch2_value = AD_sample[2];
        AD_ch3_value = AD_sample[3];
        AD_ch4_value = AD_sample[4];
        UART_send_str(" | probka:1 ");
        UART_send_buf((uint8_t*) &AD_ch2_value, sizeof(AD_ch2_value));
        UART_send_str(" | probka:2 ");
        UART_send_buf((uint8_t*) &AD_ch3_value, sizeof(AD_ch3_value));
        UART_send_str(" | probka:3 ");
        UART_send_buf((uint8_t*) &AD_ch4_value, sizeof(AD_ch4_value));
        ADSampRdy= FALSE;
        
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
