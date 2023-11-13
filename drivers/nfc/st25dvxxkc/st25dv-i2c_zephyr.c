/**
  ******************************************************************************
  * @file    st25dv-i2c_linux.c
  * @author  MMY Application Team
  * @brief   This file provides a set of functions needed to manage the I2C of
  *          the ST25DV-I2C device.
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

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/nfc/st25dvxxkc/st25dv-i2c_zephyr.h>
#include <zephyr/drivers/nfc/st25dvxxkc/st25dvxxkc.h>
#include <zephyr/drivers/nfc/st25dv.h>

uint32_t i2c_cfg = I2C_SPEED_SET(I2C_SPEED_STANDARD) | I2C_MODE_CONTROLLER;

/** @addtogroup ST25DV_I2C_BSP
  * @{
  *	@brief  <b>This file contains the BSP IO layer for the ST25DV-I2C</b> 
  */

#define REVERSE16(x) ((X >> 8) | ((X & 0xFF) << 8)) 

/** @defgroup ST25DV_I2C_BSP_IO_Private_Functions
 *  @{
 */

/**
  * @}
  */


/** @defgroup ST25DV_I2C_BSP_IO_Public_Functions
  * @{
  */



/******************************** LINK NFC ********************************/

/**
  * @brief  Tick function used in NFC device low level driver.
  * @retval Current Tick
  */
int32_t NFC_IO_Tick(void)
{
  return k_uptime_get_32();
}


/**
  * @brief  DeInitializes Sensors low level.
  * @retval Status Success (0), Error (1)
  */
int32_t NFC_IO_DeInit(const struct device *dev)
{
  return 0;
}

/**
  * @brief  Initializes Sensors low level.
  * @retval Status Success (0), Error (1)
  */
int32_t NFC_IO_Init(const struct device *dev)
{
  struct st25dvxxkc_data *data = (struct st25dvxxkc_data *) dev->data;
  
  if (data->dev_i2c == NULL) {
    return -ENODEV;
  }

  if (!device_is_ready(data->dev_i2c)) {
		printk("I2C device is not ready\n");
		return ERR_IO;
	}
  if (i2c_configure(data->dev_i2c, i2c_cfg)) {
		printk("I2C config failed\n");
		return ERR_IO;
	}
  
  return 0;
}

/**
  * @brief  Checks if target device is ready for communication.
  * @param  Addr  NFC device I2C address
  * @param  Trials  Number of trials
  * @retval Status Success (0), Timeout (1)
  */
int32_t NFC_IO_IsDeviceReady(const struct device *dev, uint16_t DevAddr, uint32_t Trials) 
{
  int ret = 0;
  uint8_t regAddr[2] = {0,0};
  uint8_t pData[1];
  struct st25dvxxkc_data *data = (struct st25dvxxkc_data *) dev->data;
  
  if (data->dev_i2c == NULL) {
    return -ENODEV;
  }
  
  ret = i2c_write_read(data->dev_i2c, DevAddr >> 1, regAddr, 2, pData, 1);

  if (ret)
  {
    return NFC_I2C_ERROR_TIMEOUT;
  } 

  return NFC_I2C_STATUS_SUCCESS;
}

/**

  * @brief  Write a value in a register of the device through BUS.
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to write

  * @param  pData  Pointer to data buffer to write
  * @param  Length Data Length (max is 256)
  * @retval Status Success (0), Linux errno 
  */

int32_t NFC_IO_WriteReg16(const struct device *dev, uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length) 
{
  int8_t ret = 0;
  uint8_t buffer[258];
  struct st25dvxxkc_data *data = (struct st25dvxxkc_data *) dev->data;

  if (data->dev_i2c == NULL) {
    return -ENODEV;
  }

  if((pData == NULL) && (Length > 0))
  {
    printk("ERROR NFC_IO_WriteReg16 Data buffer is NULL but length is not 0 (%x,%d)\r\n",(uint32_t)pData, Length);
    return ERR_PARAM;
  }

  if(Length > 256)
  {
    printk("ERROR NFC_IO_WriteReg16 Length is too big %d\r\n",Length);
    return ERR_PARAM;
  }

  /* Prepare tx buffer */
  buffer[0] = Reg>>8;
  buffer[1] = Reg & 0xFF;
  if((pData != NULL) && (Length > 0)) {
    memcpy(&buffer[2],pData,Length);
  }

  ret = i2c_write(data->dev_i2c, buffer, (Length || (pData != NULL)) ? Length + 2 : 0, DevAddr >> 1);

  if (ret)
  {
    return NFC_I2C_ERROR_TIMEOUT;
  } 

  return NFC_I2C_STATUS_SUCCESS;
}

/**
  * @brief  Read registers through a bus (16 bits)
  * @param  DevAddr: Device address on BUS
  * @param  Reg: The target register address to read
  * @param  Length Data Length
  * @retval Status Success (0), Error (1)
  */
int32_t  NFC_IO_ReadReg16(const struct device *dev, uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length) 
{
  int8_t ret = 0;
  uint8_t regAddr[2];
  regAddr[0] = Reg>>8;
  regAddr[1] = Reg & 0xFF;

  struct st25dvxxkc_data *data = (struct st25dvxxkc_data *) dev->data;
  
  if (data->dev_i2c == NULL) {
    return -ENODEV;
  }

  if((pData == NULL) && (Length > 0))
  {
    printk("ERROR NFC_IO_ReadReg16 Data buffer is NULL but length is not 0 (%x,%d)\r\n",(uint32_t)pData, Length);
    return ERR_PARAM;
  }

  ret = i2c_write_read(data->dev_i2c, DevAddr >> 1, regAddr, 2, pData, Length);
  if (ret) {
      printk("\r\nError %d while reading @%X (devAddr=%X)\r\n", ret,Reg,DevAddr);
      return ret;
  }

  return NFC_I2C_STATUS_SUCCESS;

}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

