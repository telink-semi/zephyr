/**
  ******************************************************************************
  * @file    stm32l4s5i_iot01.h
  * @author  MCD Application Team
  * @brief   STM32L4S5I IOT01 board support package
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L4S5I_IOT01_H
#define __STM32L4S5I_IOT01_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

	 
/* Flags ---------------------------------------------------------------------*/
	 
/* macro function ------------------------------------------------------------*/	 
 

/*!< constant Unsigned integer types  */
/* 	I2C config	------------------------------------------------------------------------------*/
//#define ST25DV_I2C_TIMEOUT   	(0x3FFFF) /*!< I2C Time out */
#define ST25DV_I2C_TIMEOUT   	(40) /*!< I2C Time out */
#define ST25DV_I2C_POLLING  		0x0FFF /* Nb attempt of the polling */
#define ST25DV_ADDR           	0xAC   /*!< M24SR address */

	
/* error code ---------------------------------------------------------------------------------*/
#define ST25DV_STATUS_SUCCESS 0
#define ST25DV_ERROR_DEFAULT 1


#define NFC_I2C_STATUS_SUCCESS 0
#define NFC_I2C_ERROR_TIMEOUT 1
#define NFC_I2C_ERROR_GPIO 2
#define ERR_NONE (0)
#define ERR_IO (1)
#define ERR_PARAM (2)
    
/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will 
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define ST25DV_FLAG_TIMEOUT         ((uint16_t)0xF000)
#define ST25DV_LONG_TIMEOUT         ((uint16_t)( ST25DV_FLAG_TIMEOUT))    
     

/*  public function	--------------------------------------------------------------------------*/

int32_t NFC_IO_Tick(void);
int32_t NFC_IO_DeInit(const struct device *dev);
int32_t NFC_IO_Init(const struct device *dev);
int32_t NFC_IO_IsDeviceReady(const struct device *dev, uint16_t DevAddr, uint32_t Trials) ;
int32_t NFC_IO_WriteReg16(const struct device *dev, uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length) ;
int32_t  NFC_IO_ReadReg16(const struct device *dev, uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length) ;

#ifdef __cplusplus
}
#endif

#endif /* __STM32L4S5I_IOT01_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/