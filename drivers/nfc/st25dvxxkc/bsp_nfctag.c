/**
  ******************************************************************************
  * @file    bsp_nfctag.c
  * @author  MCD Application Team
  * @brief   Linux support package
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
#include <zephyr/drivers/nfc/st25dvxxkc/bsp_nfctag.h>

/** @addtogroup BSP
 * @{
 */

/** @addtogroup STM32L4S5I_IOT01
 * @{
 */

/** @defgroup STM32L4S5I_IOT01_NFCTAG
 * @{
 */
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/** @defgroup STM32L4S5I_IOT01_NFCTAG_Private_Defines
 * @{
 */
#ifndef NULL
#define NULL      (void *) 0
#endif
/**
 * @}
 */

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 
/* Global variables ----------------------------------------------------------*/
/** @defgroup STM32L4S5I_IOT01_NFCTAG_Private_Variables
 * @{
 */
static ST25DVxxKC_Drv_t *Nfctag_Drv = NULL;
static uint8_t NfctagInitialized = 0;
static ST25DVxxKC_Object_t NfcTagObj;

/**
 * @}
 */
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/** @defgroup STM32L4S5I_IOT01_NFCTAG_Public_Functions
 * @{
 */


int32_t BSP_NFCTAG_Init (const struct device *dev, uint32_t Instance)
{
  int32_t status;
  ST25DVxxKC_IO_t IO;
  UNUSED(Instance);

  /* Configure the component */
  IO.Init         = NFC_IO_Init;
  IO.DeInit       = NFC_IO_DeInit;
  IO.IsReady      = NFC_IO_IsDeviceReady;
  IO.Read         = NFC_IO_ReadReg16;
  IO.Write        = (ST25DVxxKC_Write_Func)NFC_IO_WriteReg16;
  IO.GetTick      = NFC_IO_Tick;

  status = ST25DVxxKC_RegisterBusIO (dev, &NfcTagObj, &IO);
  if(status != NFCTAG_OK)
    return NFCTAG_ERROR;

  Nfctag_Drv = (ST25DVxxKC_Drv_t *)(void *)&St25Dvxxkc_Drv;
  if(Nfctag_Drv->Init != NULL)
  {
    status = Nfctag_Drv->Init(dev, &NfcTagObj);
    if(status != NFCTAG_OK)
    {
      Nfctag_Drv = NULL;
      return NFCTAG_ERROR;
    }
  } else {
    Nfctag_Drv = NULL;
    return NFCTAG_ERROR;
  }
  return NFCTAG_OK;
}


/**
  * @brief  Deinitializes peripherals used by the I2C NFCTAG driver
  * @param  None
  * @retval None
  */
void BSP_NFCTAG_DeInit(const struct device *dev, uint32_t Instance )
{ 
  UNUSED(Instance);

  if(Nfctag_Drv != NULL)
  {
    Nfctag_Drv = NULL;
    NfctagInitialized = 0;
  }
}

/**
  * @brief  Check if the nfctag is initialized
  * @param  None
  * @retval 0 if the nfctag is not initialized, 1 if the nfctag is already initialized
  */
uint8_t BSP_NFCTAG_isInitialized( uint32_t Instance )
{
  UNUSED(Instance);
  return NfctagInitialized;
}

/**
  * @brief  Read the ID of the nfctag
  * @param  wai_id : the pointer where the who_am_i of the device is stored
  * @retval NFCTAG enum status
  */
int32_t BSP_NFCTAG_ReadID(const struct device *dev, uint32_t Instance, uint8_t * const wai_id )
{
  UNUSED(Instance);
  if ( Nfctag_Drv->ReadID == NULL )
  {
    return NFCTAG_ERROR;
  }
  
  return Nfctag_Drv->ReadID(dev, &NfcTagObj, wai_id);
}

/**
  * @brief  Check if the nfctag is available
  * @param  Trials : Number of trials
  * @retval NFCTAG enum status
  */
int32_t BSP_NFCTAG_IsDeviceReady(const struct device *dev, uint32_t Instance, const uint32_t Trials )
{
  UNUSED(Instance);
  if ( Nfctag_Drv->IsReady == NULL )
  {
    return NFCTAG_ERROR;
  }
  
  return Nfctag_Drv->IsReady(dev, &NfcTagObj, Trials );
}

/**
  * @brief  Reads data in the nfctag at specific address
  * @param  pData : pointer to store read data
  * @param  TarAddr : I2C data memory address to read
  * @param  Size : Size in bytes of the value to be read
  * @retval NFCTAG enum status
  */
int32_t BSP_NFCTAG_ReadData(const struct device *dev, uint32_t Instance, uint8_t * const pData, const uint16_t TarAddr, const uint16_t Size )
{
  UNUSED(Instance);
  if ( Nfctag_Drv->ReadData == NULL )
  {
    return NFCTAG_ERROR;
  }
  
  return Nfctag_Drv->ReadData(dev, &NfcTagObj, pData, TarAddr, Size );
}

/**
  * @brief  Writes data in the nfctag at specific address
  * @param  pData : pointer to the data to write
  * @param  TarAddr : I2C data memory address to write
  * @param  Size : Size in bytes of the value to be written
  * @retval NFCTAG enum status
  */
int32_t BSP_NFCTAG_WriteData(const struct device *dev, uint32_t Instance, const uint8_t * const pData, const uint16_t TarAddr, const uint16_t Size )
{
  UNUSED(Instance);
  if ( Nfctag_Drv->WriteData == NULL )
  {
    return NFCTAG_ERROR;
  }
  
  return Nfctag_Drv->WriteData(dev, &NfcTagObj, pData, TarAddr, Size );
}

/**
  * @brief  Reads nfctag Register
  * @param  pData : pointer to store read data
  * @param  TarAddr : I2C register address to read
  * @param  Size : Size in bytes of the value to be read
  * @retval NFCTAG enum status
  */
int32_t BSP_NFCTAG_ReadRegister(const struct device *dev, uint32_t Instance, uint8_t * const pData, const uint16_t TarAddr, const uint16_t Size )
{
  UNUSED(Instance);

  return ST25DVxxKC_ReadRegister(dev, &NfcTagObj, pData, TarAddr, Size );
}

/**
  * @brief  Writes nfctag Register
  * @param  pData : pointer to the data to write
  * @param  TarAddr : I2C register address to write
  * @param  Size : Size in bytes of the value to be written
  * @retval NFCTAG enum status
  */
int32_t BSP_NFCTAG_WriteRegister(const struct device *dev, uint32_t Instance, const uint8_t * const pData, const uint16_t TarAddr, const uint16_t Size )
{
  UNUSED(Instance);
  int32_t ret_value;

  ret_value = ST25DVxxKC_WriteRegister(dev, &NfcTagObj, pData, TarAddr, Size );
  if( ret_value == NFCTAG_OK )
  {
    while( Nfctag_Drv->IsReady(dev, &NfcTagObj, 1 ) != NFCTAG_OK ) {};
      return NFCTAG_OK;
  }
  
  return ret_value;
}

/**
  * @brief  Return the size of the nfctag
  * @retval Size of the NFCtag in Bytes
  */
uint32_t BSP_NFCTAG_GetByteSize(const struct device *dev, uint32_t Instance )
{
  UNUSED(Instance);
  ST25DVxxKC_MEM_SIZE_t mem_size;
  ST25DVxxKC_ReadMemSize(dev, &NfcTagObj, &mem_size );
  
  return (mem_size.BlockSize+1) * (mem_size.Mem_Size+1);
}

/**
  * @brief  Reads the ST25DV Memory Size.
  * @param  pSizeInfo Pointer on a ST25DVxxKC_MEM_SIZE_t structure used to return the Memory size information.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadMemSize(const struct device *dev, uint32_t Instance, ST25DVxxKC_MEM_SIZE_t * const pSizeInfo )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadMemSize(dev, &NfcTagObj, pSizeInfo);
}

/**
  * @brief  Reads the RFSleep dynamic register information.
  * @param  pRFSleep Pointer on a ST25DVxxKC_EN_STATUS_E values used to return the RF Sleep state.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_GetRFSleep_Dyn(const struct device *dev, uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pRFSleep )
{
  UNUSED(Instance);
  return ST25DVxxKC_GetRFSleep_Dyn(dev, &NfcTagObj, pRFSleep);
}

/**
  * @brief  Sets the RF Sleep dynamic configuration.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_SetRFSleep_Dyn(const struct device *dev, uint32_t Instance)
{
  UNUSED(Instance);
  return ST25DVxxKC_SetRFSleep_Dyn(dev, &NfcTagObj);
}

/**
  * @brief  Unsets the RF Sleep dynamic configuration.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ResetRFSleep_Dyn(const struct device *dev, uint32_t Instance)
{
  UNUSED(Instance);
  return ST25DVxxKC_ResetRFSleep_Dyn(dev, &NfcTagObj);
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

/**
 * @}
 */

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/
