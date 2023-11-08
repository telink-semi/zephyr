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


int32_t BSP_NFCTAG_Init (uint32_t Instance)
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

  status = ST25DVxxKC_RegisterBusIO (&NfcTagObj, &IO);
  if(status != NFCTAG_OK)
    return NFCTAG_ERROR;

  Nfctag_Drv = (ST25DVxxKC_Drv_t *)(void *)&St25Dvxxkc_Drv;
  if(Nfctag_Drv->Init != NULL)
  {
    status = Nfctag_Drv->Init(&NfcTagObj);
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
void BSP_NFCTAG_DeInit( uint32_t Instance )
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
int32_t BSP_NFCTAG_ReadID( uint32_t Instance, uint8_t * const wai_id )
{
  UNUSED(Instance);
  if ( Nfctag_Drv->ReadID == NULL )
  {
    return NFCTAG_ERROR;
  }
  
  return Nfctag_Drv->ReadID(&NfcTagObj, wai_id );
}

/**
  * @brief  Check if the nfctag is available
  * @param  Trials : Number of trials
  * @retval NFCTAG enum status
  */
int32_t BSP_NFCTAG_IsDeviceReady( uint32_t Instance, const uint32_t Trials )
{
  UNUSED(Instance);
  if ( Nfctag_Drv->IsReady == NULL )
  {
    return NFCTAG_ERROR;
  }
  
  return Nfctag_Drv->IsReady(&NfcTagObj, Trials );
}

/**
  * @brief  Configure nfctag interrupt
  * @param  ITConfig : store interrupt to configure
  *                  - 0x01 => RF BUSY
  *                  - 0x02 => WIP
  * @retval NFCTAG enum status
  */
int32_t BSP_NFCTAG_ConfigIT( uint32_t Instance, const uint16_t ITConfig )
{
  UNUSED(Instance);
  if ( Nfctag_Drv->ConfigIT == NULL )
  {
    return NFCTAG_ERROR;
  }
  return Nfctag_Drv->ConfigIT(&NfcTagObj, ITConfig );
}

/**
  * @brief  Get nfctag interrupt configutration
  * @param  ITConfig : store interrupt configuration
  *                  - 0x01 => RF BUSY
  *                  - 0x02 => WIP
  * @retval NFCTAG enum status
  */
int32_t BSP_NFCTAG_GetITStatus(uint32_t Instance,  uint16_t * const ITConfig )
{
  UNUSED(Instance);
  if ( Nfctag_Drv->GetITStatus == NULL )
  {
    return NFCTAG_ERROR;
  }
  
  return Nfctag_Drv->GetITStatus(&NfcTagObj, ITConfig );
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
int32_t BSP_NFCTAG_ReadRegister( uint32_t Instance, uint8_t * const pData, const uint16_t TarAddr, const uint16_t Size )
{
  UNUSED(Instance);

  return ST25DVxxKC_ReadRegister(&NfcTagObj, pData, TarAddr, Size );
}

/**
  * @brief  Writes nfctag Register
  * @param  pData : pointer to the data to write
  * @param  TarAddr : I2C register address to write
  * @param  Size : Size in bytes of the value to be written
  * @retval NFCTAG enum status
  */
int32_t BSP_NFCTAG_WriteRegister( uint32_t Instance, const uint8_t * const pData, const uint16_t TarAddr, const uint16_t Size )
{
  UNUSED(Instance);
  int32_t ret_value;

  ret_value = ST25DVxxKC_WriteRegister(&NfcTagObj, pData, TarAddr, Size );
  if( ret_value == NFCTAG_OK )
  {
    while( Nfctag_Drv->IsReady(&NfcTagObj, 1 ) != NFCTAG_OK ) {};
      return NFCTAG_OK;
  }
  
  return ret_value;
}

/**
  * @brief  Return the size of the nfctag
  * @retval Size of the NFCtag in Bytes
  */
uint32_t BSP_NFCTAG_GetByteSize( uint32_t Instance )
{
  UNUSED(Instance);
  ST25DVxxKC_MEM_SIZE_t mem_size;
  ST25DVxxKC_ReadMemSize(&NfcTagObj, &mem_size );
  
  return (mem_size.BlockSize+1) * (mem_size.Mem_Size+1);
}

/**
  * @brief  Reads the ST25DV IC Revision.
  * @param  pICRev Pointer on the uint8_t used to return the ST25DV IC Revision number.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadICRev( uint32_t Instance, uint8_t * const pICRev )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadICRev(&NfcTagObj, pICRev);
}


/**
  * @brief  Reads the ST25DV ITtime duration for the GPO pulses.
  * @param  pITtime Pointer used to return the coefficient for the GPO Pulse duration (Pulse duration = 302,06 us - ITtime * 512 / fc).
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadITPulse(uint32_t Instance, ST25DVxxKC_PULSE_DURATION_E * const pITtime )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadITPulse(&NfcTagObj, pITtime);
}

/**
  * @brief    Configures the ST25DV ITtime duration for the GPO pulse.
  * @details  Needs the I2C Password presentation to be effective.
  * @param    ITtime Coefficient for the Pulse duration to be written (Pulse duration = 302,06 us - ITtime * 512 / fc)
  * @retval   int32_t enum status.
  */
int32_t BSP_NFCTAG_WriteITPulse( uint32_t Instance, const ST25DVxxKC_PULSE_DURATION_E ITtime )
{
  UNUSED(Instance);
  return ST25DVxxKC_WriteITPulse(&NfcTagObj, ITtime);
}

/**
  * @brief  Reads the ST25DV UID.
  * @param  pUid Pointer used to return the ST25DV UID value.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadUID( uint32_t Instance, ST25DVxxKC_UID_t * const pUid )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadUID(&NfcTagObj, pUid);
}

/**
  * @brief  Reads the ST25DV DSFID.
  * @param  pDsfid Pointer used to return the ST25DV DSFID value.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadDSFID( uint32_t Instance, uint8_t * const pDsfid )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadDSFID(&NfcTagObj, pDsfid);
}

/**
  * @brief  Reads the ST25DV DSFID RF Lock state.
  * @param  pLockDsfid Pointer on a ST25DVxxKC_LOCK_STATUS_E used to return the DSFID lock state.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadDsfidRFProtection( uint32_t Instance, ST25DVxxKC_LOCK_STATUS_E * const pLockDsfid )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadDsfidRFProtection(&NfcTagObj, pLockDsfid);
}

/**
  * @brief  Reads the ST25DV AFI.
  * @param  pAfi Pointer used to return the ST25DV AFI value.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadAFI( uint32_t Instance, uint8_t * const pAfi )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadAFI(&NfcTagObj, pAfi);
}

/**
  * @brief  Reads the AFI RF Lock state.
  * @param  pLockAfi Pointer on a ST25DVxxKC_LOCK_STATUS_E used to return the ASFID lock state.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadAfiRFProtection( uint32_t Instance, ST25DVxxKC_LOCK_STATUS_E * const pLockAfi )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadAfiRFProtection(&NfcTagObj, pLockAfi);
}

/**
  * @brief  Reads the I2C Protected Area state.
  * @param  pProtZone Pointer on a ST25DVxxKC_I2C_PROT_ZONE_t structure used to return the Protected Area state.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadI2CProtectZone( uint32_t Instance, ST25DVxxKC_I2C_PROT_ZONE_t * const pProtZone )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadI2CProtectZone(&NfcTagObj, pProtZone);
}

/**
  * @brief    Sets the I2C write-protected state to an EEPROM Area.
  * @details  Needs the I2C Password presentation to be effective.
  * @param    Zone                ST25DVxxKC_PROTECTION_ZONE_E value coresponding to the area to protect.
  * @param    ReadWriteProtection ST25DVxxKC_PROTECTION_CONF_E value corresponding to the protection to be set.
  * @return   int32_t enum status.
  */
int32_t BSP_NFCTAG_WriteI2CProtectZonex(uint32_t Instance, const ST25DVxxKC_PROTECTION_ZONE_E Zone,  const ST25DVxxKC_PROTECTION_CONF_E ReadWriteProtection )
{
  UNUSED(Instance);
  return ST25DVxxKC_WriteI2CProtectZonex(&NfcTagObj, Zone, ReadWriteProtection);
}

/**
  * @brief  Reads the CCile protection state.
  * @param  pLockCCFile Pointer on a ST25DVxxKC_LOCK_CCFILE_t value corresponding to the lock state of the CCFile.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadLockCCFile(uint32_t Instance, ST25DVxxKC_LOCK_CCFILE_t * const pLockCCFile )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadLockCCFile(&NfcTagObj, pLockCCFile);
}

/**
  * @brief  Locks the CCile to prevent any RF write access.
  * @details  Needs the I2C Password presentation to be effective.
  * @param  NbBlockCCFile ST25DVxxKC_CCFILE_BLOCK_E value corresponding to the number of blocks to be locked.
  * @param  LockCCFile    ST25DVxxKC_LOCK_CCFILE_t value corresponding to the lock state to apply on the CCFile.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_WriteLockCCFile(uint32_t Instance, const ST25DVxxKC_CCFILE_BLOCK_E NbBlockCCFile,  const ST25DVxxKC_LOCK_STATUS_E LockCCFile )
{
  UNUSED(Instance);
  return ST25DVxxKC_WriteLockCCFile(&NfcTagObj, NbBlockCCFile, LockCCFile);
}

/**
  * @brief  Reads the Cfg registers protection.
  * @param  pLockCfg Pointer on a ST25DVxxKC_LOCK_STATUS_E value corresponding to the Cfg registers lock state.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadLockCFG(uint32_t Instance, ST25DVxxKC_LOCK_STATUS_E * const pLockCfg )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadLockCFG(&NfcTagObj, pLockCfg);
}

/**
  * @brief  Lock/Unlock the Cfg registers, to prevent any RF write access.
  * @details  Needs the I2C Password presentation to be effective.
  * @param  LockCfg ST25DVxxKC_LOCK_STATUS_E value corresponding to the lock state to be written.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_WriteLockCFG(uint32_t Instance, const ST25DVxxKC_LOCK_STATUS_E LockCfg )
{
  UNUSED(Instance);
  return ST25DVxxKC_WriteLockCFG(&NfcTagObj, LockCfg);
}

/**
  * @brief  Presents I2C password, to authorize the I2C writes to protected areas.
  * @param  PassWord Password value on 32bits
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_PresentI2CPassword(uint32_t Instance, const ST25DVxxKC_PASSWD_t PassWord )
{
  UNUSED(Instance);
  return ST25DVxxKC_PresentI2CPassword(&NfcTagObj, PassWord);
}

/**
  * @brief  Writes a new I2C password.
  * @details  Needs the I2C Password presentation to be effective.
  * @param  PassWord New I2C PassWord value on 32bits.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_WriteI2CPassword(uint32_t Instance, const ST25DVxxKC_PASSWD_t PassWord )
{
  UNUSED(Instance);
  return ST25DVxxKC_WriteI2CPassword(&NfcTagObj, PassWord);
}

/**
  * @brief  Reads the RF Zone Security Status (defining the allowed RF accesses).
  * @param  Zone        ST25DVxxKC_PROTECTION_ZONE_E value coresponding to the protected area.
  * @param  pRfprotZone Pointer on a ST25DVxxKC_RF_PROT_ZONE_t value corresponding to the area protection state.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadRFZxSS(uint32_t Instance, const ST25DVxxKC_PROTECTION_ZONE_E Zone,  ST25DVxxKC_RF_PROT_ZONE_t * const pRfprotZone )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadRFZxSS(&NfcTagObj, Zone, pRfprotZone);
}

/**
  * @brief  Writes the RF Zone Security Status (defining the allowed RF accesses)
  * @details  Needs the I2C Password presentation to be effective.
  * @param  Zone        ST25DVxxKC_PROTECTION_ZONE_E value corresponding to the area on which to set the RF protection.
  * @param  RfProtZone  Pointer on a ST25DVxxKC_RF_PROT_ZONE_t value defininf the protection to be set on the area.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_WriteRFZxSS(uint32_t Instance, const ST25DVxxKC_PROTECTION_ZONE_E Zone,  const ST25DVxxKC_RF_PROT_ZONE_t RfProtZone )
{
  UNUSED(Instance);
  return ST25DVxxKC_WriteRFZxSS(&NfcTagObj, Zone, RfProtZone);
}

/**
  * @brief  Reads the value of the an area end address.
  * @param  EndZone ST25DVxxKC_END_ZONE_E value corresponding to an area end address.
  * @param  pEndZ   Pointer used to return the end address of the area.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadEndZonex(uint32_t Instance, const ST25DVxxKC_END_ZONE_E EndZone,  uint8_t * pEndZ )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadEndZonex(&NfcTagObj, EndZone, pEndZ);
}

/**
  * @brief    Sets the end address of an area.
  * @details  Needs the I2C Password presentation to be effective.
  * @note     The ST25DV answers a NACK when setting the EndZone2 & EndZone3 to same value than repectively EndZone1 & EndZone2.\n
  *           These NACKs are ok.
  * @param  EndZone ST25DVxxKC_END_ZONE_E value corresponding to an area.
  * @param  EndZ   End zone value to be written.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_WriteEndZonex(uint32_t Instance, const ST25DVxxKC_END_ZONE_E EndZone,  const uint8_t EndZ )
{
  UNUSED(Instance);
  return ST25DVxxKC_WriteEndZonex(&NfcTagObj, EndZone, EndZ);
}

/**
  * @brief  Initializes the end address of the ST25DV areas with their default values (end of memory).
  * @details  Needs the I2C Password presentation to be effective..
  *           The ST25DV answers a NACK when setting the EndZone2 & EndZone3 to same value than repectively EndZone1 & EndZone2.
  *           These NACKs are ok.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_InitEndZone(uint32_t Instance)
{
  UNUSED(Instance);
  return ST25DVxxKC_InitEndZone(&NfcTagObj);
}

/**
  * @brief  Creates user areas with defined lengths.
  * @details  Needs the I2C Password presentation to be effective.
  * @param  Zone1Length Length of area1 in bytes (32 to 8192, 0x20 to 0x2000)
  * @param  Zone2Length Length of area2 in bytes (0 to 8128, 0x00 to 0x1FC0)
  * @param  Zone3Length Length of area3 in bytes (0 to 8064, 0x00 to 0x1F80)
  * @param  Zone4Length Length of area4 in bytes (0 to 8000, 0x00 to 0x1F40)
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_CreateUserZone(uint32_t Instance, uint16_t Zone1Length,  uint16_t Zone2Length,  uint16_t Zone3Length,  uint16_t Zone4Length )
{
  UNUSED(Instance);
  return ST25DVxxKC_CreateUserZone(&NfcTagObj, Zone1Length, Zone2Length, Zone3Length, Zone4Length);
}

/**
  * @brief  Reads the ST25DV Memory Size.
  * @param  pSizeInfo Pointer on a ST25DVxxKC_MEM_SIZE_t structure used to return the Memory size information.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadMemSize(uint32_t Instance, ST25DVxxKC_MEM_SIZE_t * const pSizeInfo )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadMemSize(&NfcTagObj, pSizeInfo);
}

/**
  * @brief  Reads the Energy harvesting mode.
  * @param  pEH_mode Pointer on a ST25DVxxKC_EH_MODE_STATUS_E value corresponding to the Energy Harvesting state.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadEHMode(uint32_t Instance, ST25DVxxKC_EH_MODE_STATUS_E * const pEH_mode )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadEHMode(&NfcTagObj, pEH_mode);
}

/**
  * @brief  Sets the Energy harvesting mode.
  * @details  Needs the I2C Password presentation to be effective.
  * @param  EH_mode ST25DVxxKC_EH_MODE_STATUS_E value for the Energy harvesting mode to be set.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_WriteEHMode(uint32_t Instance, const ST25DVxxKC_EH_MODE_STATUS_E EH_mode )
{
  UNUSED(Instance);
  return ST25DVxxKC_WriteEHMode(&NfcTagObj, EH_mode);
}

/**
  * @brief  Reads the RF Management configuration.
  * @param  pRF_Mngt Pointer on a ST25DVxxKC_RF_MNGT_t structure used to return the RF Management configuration.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadRFMngt(uint32_t Instance, ST25DVxxKC_RF_MNGT_t * const pRF_Mngt )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadRFMngt(&NfcTagObj, pRF_Mngt);
}

/**
  * @brief  Sets the RF Management configuration.
  * @details  Needs the I2C Password presentation to be effective.
  * @param  Rfmngt Value of the RF Management configuration to be written.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_WriteRFMngt(uint32_t Instance, const uint8_t Rfmngt )
{
  UNUSED(Instance);
  return ST25DVxxKC_WriteRFMngt(&NfcTagObj, Rfmngt);
}

/**
  * @brief  Reads the RFDisable register information.
  * @param  pRFDisable Pointer on a ST25DVxxKC_EN_STATUS_E value corresponding to the RF Disable status.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_GetRFDisable(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pRFDisable )
{
  UNUSED(Instance);
  return ST25DVxxKC_GetRFDisable(&NfcTagObj, pRFDisable);
}

/**
  * @brief  Sets the RF Disable configuration.
  * @details  Needs the I2C Password presentation to be effective.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_SetRFDisable(uint32_t Instance)
{
  UNUSED(Instance);
  return ST25DVxxKC_SetRFDisable(&NfcTagObj);
}

/**
  * @brief  Resets the RF Disable configuration
  * @details  Needs the I2C Password presentation to be effective.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ResetRFDisable(uint32_t Instance)
{
  UNUSED(Instance);
  return ST25DVxxKC_ResetRFDisable(&NfcTagObj);
}

/**
  * @brief  Reads the RFSleep register information.
  * @param  pRFSleep Pointer on a ST25DVxxKC_EN_STATUS_E value corresponding to the RF Sleep status.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_GetRFSleep(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pRFSleep )
{
  UNUSED(Instance);
  return ST25DVxxKC_GetRFSleep(&NfcTagObj, pRFSleep);
}

/**
  * @brief  Sets the RF Sleep configuration.
  * @details  Needs the I2C Password presentation to be effective.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_SetRFSleep(uint32_t Instance)
{
  UNUSED(Instance);
  return ST25DVxxKC_SetRFSleep(&NfcTagObj);
}

/**
  * @brief  Resets the RF Sleep configuration.
  * @details  Needs the I2C Password presentation to be effective.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ResetRFSleep(uint32_t Instance)
{
  UNUSED(Instance);
  return ST25DVxxKC_ResetRFSleep(&NfcTagObj);
}

/**
  * @brief  Reads the Mailbox mode.
  * @param  pMB_mode Pointer on a ST25DVxxKC_EH_MODE_STATUS_E value used to return the Mailbox mode.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadMBMode(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pMB_mode )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadMBMode(&NfcTagObj, pMB_mode);
}

/**
  * @brief  Sets the Mailbox mode.
  * @details  Needs the I2C Password presentation to be effective.
  * @param  MB_mode ST25DVxxKC_EN_STATUS_E value corresponding to the Mailbox mode to be set.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_WriteMBMode(uint32_t Instance, const ST25DVxxKC_EN_STATUS_E MB_mode )
{
  UNUSED(Instance);
  return ST25DVxxKC_WriteMBMode(&NfcTagObj, MB_mode);
}

/**
  * @brief  Reads the Mailbox watchdog duration coefficient.
  * @param  pWdgDelay Pointer on a uint8_t used to return the watchdog duration coefficient.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadMBWDG(uint32_t Instance, uint8_t * const pWdgDelay )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadMBWDG(&NfcTagObj, pWdgDelay);
}

/**
  * @brief  Writes the Mailbox watchdog coefficient delay
  * @details  Needs the I2C Password presentation to be effective.
  * @param  WdgDelay Watchdog duration coefficient to be written (Watch dog duration = MB_WDG*30 ms +/- 6%).
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_WriteMBWDG(uint32_t Instance, const uint8_t WdgDelay )
{
  UNUSED(Instance);
  return ST25DVxxKC_WriteMBWDG(&NfcTagObj, WdgDelay);
}

/**
  * @brief  Reads N bytes of data from the Mailbox, starting at the specified byte offset.
  * @param  pData   Pointer on the buffer used to return the read data.
  * @param  Offset  Offset in the Mailbox memory, byte number to start the read.
  * @param  NbByte  Number of bytes to be read.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadMailboxData(uint32_t Instance, uint8_t * const pData,  const uint16_t TarAddr,  const uint16_t NbByte )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadMailboxData(&NfcTagObj, pData, TarAddr, NbByte);
}

/**
  * @brief  Writes N bytes of data in the Mailbox, starting from first Mailbox Address.
  * @param  pData   Pointer to the buffer containing the data to be written.
  * @param  NbByte  Number of bytes to be written.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_WriteMailboxData(uint32_t Instance, const uint8_t * const pData,  const uint16_t NbByte )
{
  UNUSED(Instance);
  return ST25DVxxKC_WriteMailboxData(&NfcTagObj, pData, NbByte);
}

/**
  * @brief  Reads N bytes from the mailbox registers, starting at the specified I2C address.
  * @param  pData   Pointer on the buffer used to return the data.
  * @param  TarAddr I2C memory address to be read.
  * @param  NbByte  Number of bytes to be read.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadMailboxRegister(uint32_t Instance, uint8_t * const pData,  const uint16_t TarAddr,  const uint16_t NbByte )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadMailboxRegister(&NfcTagObj, pData, TarAddr, NbByte);
}

/**
  * @brief  Writes N bytes to the specified mailbox register.
  * @param  pData   Pointer on the data to be written.
  * @param  TarAddr I2C register address to be written.
  * @param  NbByte  Number of bytes to be written.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_WriteMailboxRegister(uint32_t Instance, const uint8_t * const pData,  const uint16_t TarAddr,  const uint16_t NbByte )
{
  UNUSED(Instance);
  return ST25DVxxKC_WriteMailboxRegister(&NfcTagObj, pData, TarAddr, NbByte);
}

/**
  * @brief  Reads the status of the security session open register.
  * @param  pSession Pointer on a ST25DVxxKC_I2CSSO_STATUS_E value used to return the session status.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadI2CSecuritySession_Dyn(uint32_t Instance, ST25DVxxKC_I2CSSO_STATUS_E * const pSession )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadI2CSecuritySession_Dyn(&NfcTagObj, pSession);
}

/**
  * @brief  Reads the IT status register from the ST25DV.
  * @param  pITStatus Pointer on uint8_t, used to return the IT status, such as:
  *                       - RFUSERSTATE = 0x01
  *                       - RFBUSY = 0x02
  *                       - RFINTERRUPT = 0x04
  *                       - FIELDFALLING = 0x08
  *                       - FIELDRISING = 0x10
  *                       - RFPUTMSG = 0x20
  *                       - RFGETMSG = 0x40
  *                       - RFWRITE = 0x80
  *
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadITSTStatus_Dyn(uint32_t Instance, uint8_t * const pITStatus )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadITSTStatus_Dyn(&NfcTagObj, pITStatus);
}

/**
  * @brief  Read value of dynamic GPO register configuration.
  * @param  pGPO ST25DVxxKC_GPO_t pointer of the dynamic GPO configuration to store.
  * @retval NFCTAG enum status.
  */
int32_t BSP_NFCTAG_ReadGPO_Dyn(uint32_t Instance, uint8_t *GPOConfig )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadGPO_Dyn(&NfcTagObj, GPOConfig);
}

/**
  * @brief  Get dynamique GPO enable status
  * @param  pGPO_en ST25DVxxKC_EN_STATUS_E pointer of the GPO enable status to store
  * @retval NFCTAG enum status
  */
int32_t BSP_NFCTAG_GetGPO_en_Dyn(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pGPO_en )
{
  UNUSED(Instance);
  return ST25DVxxKC_GetGPO_en_Dyn(&NfcTagObj, pGPO_en);
}

/**
  * @brief  Set dynamique GPO enable configuration.
  * @param  None No parameters.
  * @retval NFCTAG enum status.
  */
int32_t BSP_NFCTAG_SetGPO_en_Dyn(uint32_t Instance)
{
  UNUSED(Instance);
  return ST25DVxxKC_SetGPO_en_Dyn(&NfcTagObj);
}

/**
  * @brief  Reset dynamique GPO enable configuration.
  * @param  None No parameters.
  * @retval NFCTAG enum status.
  */
int32_t BSP_NFCTAG_ResetGPO_en_Dyn(uint32_t Instance)
{
  UNUSED(Instance);
  return ST25DVxxKC_ResetGPO_en_Dyn(&NfcTagObj);
}

/**
  * @brief  Read value of dynamic EH Ctrl register configuration
  * @param  pEH_CTRL : ST25DVxxKC_EH_CTRL_t pointer of the dynamic EH Ctrl configuration to store
  * @retval NFCTAG enum status
  */
int32_t BSP_NFCTAG_ReadEHCtrl_Dyn(uint32_t Instance, ST25DVxxKC_EH_CTRL_t * const pEH_CTRL )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadEHCtrl_Dyn(&NfcTagObj, pEH_CTRL);
}

/**
  * @brief  Reads the Energy Harvesting dynamic status.
  * @param  pEH_Val Pointer on a ST25DVxxKC_EN_STATUS_E value used to return the Energy Harvesting dynamic status.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_GetEHENMode_Dyn(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pEH_Val )
{
  UNUSED(Instance);
  return ST25DVxxKC_GetEHENMode_Dyn(&NfcTagObj, pEH_Val);
}

/**
  * @brief  Dynamically sets the Energy Harvesting mode.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_SetEHENMode_Dyn(uint32_t Instance)
{
  UNUSED(Instance);
  return ST25DVxxKC_SetEHENMode_Dyn(&NfcTagObj);
}

/**
  * @brief  Dynamically unsets the Energy Harvesting mode.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ResetEHENMode_Dyn(uint32_t Instance)
{
  UNUSED(Instance);
  return ST25DVxxKC_ResetEHENMode_Dyn(&NfcTagObj);
}

/**
  * @brief  Reads the EH_ON status from the EH_CTRL_DYN register.
  * @param  pEHON Pointer on a ST25DVxxKC_EN_STATUS_E value used to return the EHON status.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_GetEHON_Dyn(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pEHON )
{
  UNUSED(Instance);
  return ST25DVxxKC_GetEHON_Dyn(&NfcTagObj, pEHON);
}

/**
  * @brief  Checks if RF Field is present in front of the ST25DV.
  * @param  pRF_Field Pointer on a ST25DVxxKC_FIELD_STATUS_E value used to return the field presence.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_GetRFField_Dyn(uint32_t Instance, ST25DVxxKC_FIELD_STATUS_E * const pRF_Field )
{
  UNUSED(Instance);
  return ST25DVxxKC_GetRFField_Dyn(&NfcTagObj, pRF_Field);
}

/**
  * @brief  Check if VCC is supplying the ST25DV.
  * @param  pVCC ST25DVxxKC_VCC_STATUS_E pointer of the VCC status to store
  * @retval NFCTAG enum status.
  */
int32_t BSP_NFCTAG_GetVCC_Dyn(uint32_t Instance, ST25DVxxKC_VCC_STATUS_E * const pVCC )
{
  UNUSED(Instance);
  return ST25DVxxKC_GetVCC_Dyn(&NfcTagObj, pVCC);
}

/**
  * @brief  Read value of dynamic RF Management configuration
  * @param  pRF_Mngt : ST25DVxxKC_RF_MNGT_t pointer of the dynamic RF Management configuration to store
  * @retval NFCTAG enum status
  */
int32_t BSP_NFCTAG_ReadRFMngt_Dyn(uint32_t Instance, ST25DVxxKC_RF_MNGT_t * const pRF_Mngt )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadRFMngt_Dyn(&NfcTagObj, pRF_Mngt);
}

/**
  * @brief  Writes a value to the RF Management dynamic register.
  * @param  RF_Mngt Value to be written to the RF Management dynamic register.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_WriteRFMngt_Dyn(uint32_t Instance, const uint8_t RF_Mngt )
{
  UNUSED(Instance);
  return ST25DVxxKC_WriteRFMngt_Dyn(&NfcTagObj, RF_Mngt);
}

/**
  * @brief  Reads the RFDisable dynamic register information.
  * @param  pRFDisable Pointer on a ST25DVxxKC_EN_STATUS_E value used to return the RF Disable state.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_GetRFDisable_Dyn(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pRFDisable )
{
  UNUSED(Instance);
  return ST25DVxxKC_GetRFDisable_Dyn(&NfcTagObj, pRFDisable);
}

/**
  * @brief  Sets the RF Disable dynamic configuration.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_SetRFDisable_Dyn(uint32_t Instance)
{
  UNUSED(Instance);
  return ST25DVxxKC_SetRFDisable_Dyn(&NfcTagObj);
}

/**
  * @brief  Unsets the RF Disable dynamic configuration.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ResetRFDisable_Dyn(uint32_t Instance)
{
  UNUSED(Instance);
  return ST25DVxxKC_ResetRFDisable_Dyn(&NfcTagObj);
}

/**
  * @brief  Reads the RFSleep dynamic register information.
  * @param  pRFSleep Pointer on a ST25DVxxKC_EN_STATUS_E values used to return the RF Sleep state.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_GetRFSleep_Dyn(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pRFSleep )
{
  UNUSED(Instance);
  return ST25DVxxKC_GetRFSleep_Dyn(&NfcTagObj, pRFSleep);
}

/**
  * @brief  Sets the RF Sleep dynamic configuration.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_SetRFSleep_Dyn(uint32_t Instance)
{
  UNUSED(Instance);
  return ST25DVxxKC_SetRFSleep_Dyn(&NfcTagObj);
}

/**
  * @brief  Unsets the RF Sleep dynamic configuration.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ResetRFSleep_Dyn(uint32_t Instance)
{
  UNUSED(Instance);
  return ST25DVxxKC_ResetRFSleep_Dyn(&NfcTagObj);
}

/**
  * @brief  Reads the Mailbox ctrl dynamic register.
  * @param  pCtrlStatus Pointer on a ST25DVxxKC_MB_CTRL_DYN_STATUS_t structure used to return the dynamic Mailbox ctrl information.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadMBCtrl_Dyn(uint32_t Instance, ST25DVxxKC_MB_CTRL_DYN_STATUS_t * const pCtrlStatus )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadMBCtrl_Dyn(&NfcTagObj, pCtrlStatus);
}

/**
  * @brief  Reads the Mailbox Enable dynamic configuration.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_GetMBEN_Dyn(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pMBEN )
{
  UNUSED(Instance);
  return ST25DVxxKC_GetMBEN_Dyn(&NfcTagObj, pMBEN);
}

/**
  * @brief  Sets the Mailbox Enable dynamic configuration.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_SetMBEN_Dyn(uint32_t Instance)
{
  UNUSED(Instance);
  return ST25DVxxKC_SetMBEN_Dyn(&NfcTagObj);
}

/**
  * @brief  Unsets the Mailbox Enable dynamic configuration.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ResetMBEN_Dyn(uint32_t Instance)
{
  UNUSED(Instance);
  return ST25DVxxKC_ResetMBEN_Dyn(&NfcTagObj);
}

/**
  * @brief  Reads the Mailbox message length dynamic register.
  * @param  pMBLength Pointer on a uint8_t used to return the Mailbox message length.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadMBLength_Dyn(uint32_t Instance, uint8_t * const pMBLength )
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadMBLength_Dyn(&NfcTagObj, pMBLength);
}

/**
  * @brief  Writes the I2C Slave mode.
  * @param  deviceCode Device Code to be set.
  * @param  E0 E0 to be set.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_WriteI2CSlaveMode(uint32_t Instance, const ST25DVxxKC_SLAVE_MODE_E slaveMode)
{
  UNUSED(Instance);
  return ST25DVxxKC_WriteI2CSlaveMode(&NfcTagObj, slaveMode);
}

/**
  * @brief  Reads the I2C Slave mode.
  * @param  deviceCode Pointer on a uint8_t used to return the Device Code.
  * @param  E0 Pointer on a uint8_t used to return the E0.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadI2CSlaveMode(uint32_t Instance, ST25DVxxKC_SLAVE_MODE_E *const slaveMode)
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadI2CSlaveMode(&NfcTagObj, slaveMode);
}

/**
  * @brief  Writes the I2C Slave address.
  * @param  deviceCode Device Code to be set.
  * @param  E0 E0 to be set.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_WriteI2CSlaveAddress(uint32_t Instance, const uint8_t deviceCode, const uint8_t E0)
{
  UNUSED(Instance);
  return ST25DVxxKC_WriteI2CSlaveAddress(&NfcTagObj, deviceCode, E0);
}

/**
  * @brief  Reads the I2C Slave address.
  * @param  deviceCode Pointer on a uint8_t used to return the Device Code.
  * @param  E0 Pointer on a uint8_t used to return the E0.
  * @return int32_t enum status.
  */
int32_t BSP_NFCTAG_ReadI2CSlaveAddress(uint32_t Instance, uint8_t *const deviceCode, uint8_t *const E0)
{
  UNUSED(Instance);
  return ST25DVxxKC_ReadI2CSlaveAddress(&NfcTagObj, deviceCode, E0);
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
