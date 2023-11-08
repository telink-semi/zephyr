/**
  ******************************************************************************
  * @file    st25dv-i2c_zephyr.h
  * @author  MMY Application Team
  * @brief   This file provides a set of functions needed to manage the I2C of
						 the ST25DV-I2C device.
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT� 2020 STMicroelectronics, all rights reserved
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_I2CM24SR_H
#define __DRV_I2CM24SR_H

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
int32_t NFC_IO_DeInit(void);
int32_t NFC_IO_Init(void);
int32_t NFC_IO_IsDeviceReady(uint16_t DevAddr, uint32_t Trials) ;
int32_t NFC_IO_WriteReg16(const struct device *dev, uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length) ;
int32_t  NFC_IO_ReadReg16(const struct device *dev, uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length) ;

#ifdef __cplusplus
 }
#endif

#endif 


/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/
