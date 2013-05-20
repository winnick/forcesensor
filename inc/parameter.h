/**
  ******************************************************************************
  * @file parameter.h
  * @brief This file contains the parameters for ADC.
  * @author STMicroelectronics - MCD Application Team
  * @version  V2.0.0
  * @date     15-March-2011
  ******************************************************************************
  *
  * THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

#define AUTORELOAD	0x7A00			//	sampling period definition (trigg period)
#define PRESCALER       512
#define NUMB_SAMP	8						//	number of samples for SW filtering
#define NUMB_CHAN       4
#define PEAK_FILTER	8						//  floating spot fall speed filter
#define AD_STAB		20						//  AD stabilization [TIM1 increments] (~10us)

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

	
	/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
