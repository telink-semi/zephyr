/**
  ******************************************************************************
  * @file    bsp_nfctag.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BSP_NFCTAG_H
#define BSP_NFCTAG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
//#include "stm32l4s5i_iot01.h"
#include "st25dvxxkc.h"
#include "st25dv-i2c_zephyr.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L4S5I_IOT01
  * @{
  */
   
/** @addtogroup STM32L4S5I_IOT01_NFCTAG
  * @{
  */
/* Exported types ------------------------------------------------------------*/
/**
 * @brief  NFCTAG status enumerator definition.
 */


/* Exported constants --------------------------------------------------------*/
#define NFCTAG_4K_SIZE            ((uint32_t) 0x200)
#define NFCTAG_16K_SIZE           ((uint32_t) 0x800)
#define NFCTAG_64K_SIZE           ((uint32_t) 0x2000)
   
/**
 * @brief  NFCTAG status enumerator definition.
 */
#define NFCTAG_OK      (0)
#define NFCTAG_ERROR   (-1)
#define NFCTAG_BUSY    (-2)
#define NFCTAG_TIMEOUT (-3)
#define NFCTAG_NACK    (-102)

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

/* External variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported function	--------------------------------------------------------*/
/** @defgroup STM32L4S5I_IOT01_NFCTAG_Exported_Functions
  * @{
  */
int32_t BSP_NFCTAG_Init( uint32_t Instance );
void BSP_NFCTAG_DeInit( uint32_t Instance );
uint8_t BSP_NFCTAG_isInitialized( uint32_t Instance );
int32_t BSP_NFCTAG_ReadID( uint32_t Instance, uint8_t * const wai_id );
int32_t BSP_NFCTAG_ConfigIT( uint32_t Instance, const uint16_t ITConfig );
int32_t BSP_NFCTAG_GetITStatus( uint32_t Instance, uint16_t * const ITConfig );
int32_t BSP_NFCTAG_ReadData(const struct device *dev, uint32_t Instance, uint8_t * const pData, const uint16_t TarAddr, const uint16_t Size );
int32_t BSP_NFCTAG_WriteData(const struct device *dev, uint32_t Instance, const uint8_t * const pData, const uint16_t TarAddr, const uint16_t Size );
int32_t BSP_NFCTAG_ReadRegister( uint32_t Instance, uint8_t * const pData, const uint16_t TarAddr, const uint16_t Size );
int32_t BSP_NFCTAG_WriteRegister( uint32_t Instance, const uint8_t * const pData, const uint16_t TarAddr, const uint16_t Size );
int32_t BSP_NFCTAG_IsDeviceReady( uint32_t Instance,const uint32_t Trials );

uint32_t BSP_NFCTAG_GetByteSize( uint32_t Instance );
int32_t BSP_NFCTAG_ReadICRev( uint32_t Instance, uint8_t * const pICRev );
int32_t BSP_NFCTAG_ReadITPulse( uint32_t Instance, ST25DVxxKC_PULSE_DURATION_E * const pITtime );
int32_t BSP_NFCTAG_WriteITPulse( uint32_t Instance, const ST25DVxxKC_PULSE_DURATION_E ITtime );
int32_t BSP_NFCTAG_ReadUID( uint32_t Instance, ST25DVxxKC_UID_t * const pUid );
int32_t BSP_NFCTAG_ReadDSFID( uint32_t Instance, uint8_t * const pDsfid );
int32_t BSP_NFCTAG_ReadDsfidRFProtection( uint32_t Instance, ST25DVxxKC_LOCK_STATUS_E * const pLockDsfid );
int32_t BSP_NFCTAG_ReadAFI( uint32_t Instance, uint8_t * const pAfi );
int32_t BSP_NFCTAG_ReadAfiRFProtection( uint32_t Instance, ST25DVxxKC_LOCK_STATUS_E * const pLockAfi );
int32_t BSP_NFCTAG_ReadI2CProtectZone( uint32_t Instance, ST25DVxxKC_I2C_PROT_ZONE_t * const pProtZone );
int32_t BSP_NFCTAG_WriteI2CProtectZonex(uint32_t Instance, const ST25DVxxKC_PROTECTION_ZONE_E Zone,  const ST25DVxxKC_PROTECTION_CONF_E ReadWriteProtection );
int32_t BSP_NFCTAG_ReadLockCCFile(uint32_t Instance, ST25DVxxKC_LOCK_CCFILE_t * const pLockCCFile );
int32_t BSP_NFCTAG_WriteLockCCFile(uint32_t Instance, const ST25DVxxKC_CCFILE_BLOCK_E NbBlockCCFile,  const ST25DVxxKC_LOCK_STATUS_E LockCCFile );
int32_t BSP_NFCTAG_ReadLockCFG(uint32_t Instance, ST25DVxxKC_LOCK_STATUS_E * const pLockCfg );
int32_t BSP_NFCTAG_WriteLockCFG(uint32_t Instance, const ST25DVxxKC_LOCK_STATUS_E LockCfg );
int32_t BSP_NFCTAG_PresentI2CPassword(uint32_t Instance, const ST25DVxxKC_PASSWD_t PassWord );
int32_t BSP_NFCTAG_WriteI2CPassword(uint32_t Instance, const ST25DVxxKC_PASSWD_t PassWord );
int32_t BSP_NFCTAG_ReadRFZxSS(uint32_t Instance, const ST25DVxxKC_PROTECTION_ZONE_E Zone,  ST25DVxxKC_RF_PROT_ZONE_t * const pRfprotZone );
int32_t BSP_NFCTAG_WriteRFZxSS(uint32_t Instance, const ST25DVxxKC_PROTECTION_ZONE_E Zone,  const ST25DVxxKC_RF_PROT_ZONE_t RfProtZone );
int32_t BSP_NFCTAG_ReadEndZonex(uint32_t Instance, const ST25DVxxKC_END_ZONE_E EndZone,  uint8_t * pEndZ );
int32_t BSP_NFCTAG_WriteEndZonex(uint32_t Instance, const ST25DVxxKC_END_ZONE_E EndZone,  const uint8_t EndZ );
int32_t BSP_NFCTAG_InitEndZone(uint32_t Instance);
int32_t BSP_NFCTAG_CreateUserZone(uint32_t Instance, uint16_t Zone1Length,  uint16_t Zone2Length,  uint16_t Zone3Length,  uint16_t Zone4Length );
int32_t BSP_NFCTAG_ReadMemSize(uint32_t Instance, ST25DVxxKC_MEM_SIZE_t * const pSizeInfo );
int32_t BSP_NFCTAG_ReadEHMode(uint32_t Instance, ST25DVxxKC_EH_MODE_STATUS_E * const pEH_mode );
int32_t BSP_NFCTAG_WriteEHMode(uint32_t Instance, const ST25DVxxKC_EH_MODE_STATUS_E EH_mode );
int32_t BSP_NFCTAG_ReadRFMngt(uint32_t Instance, ST25DVxxKC_RF_MNGT_t * const pRF_Mngt );
int32_t BSP_NFCTAG_WriteRFMngt(uint32_t Instance, const uint8_t Rfmngt );
int32_t BSP_NFCTAG_GetRFDisable(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pRFDisable );
int32_t BSP_NFCTAG_SetRFDisable(uint32_t Instance);
int32_t BSP_NFCTAG_ResetRFDisable(uint32_t Instance);
int32_t BSP_NFCTAG_GetRFSleep(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pRFSleep );
int32_t BSP_NFCTAG_SetRFSleep(uint32_t Instance);
int32_t BSP_NFCTAG_ResetRFSleep(uint32_t Instance);
int32_t BSP_NFCTAG_ReadMBMode(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pMB_mode );
int32_t BSP_NFCTAG_WriteMBMode(uint32_t Instance, const ST25DVxxKC_EN_STATUS_E MB_mode );
int32_t BSP_NFCTAG_ReadMBWDG(uint32_t Instance, uint8_t * const pWdgDelay );
int32_t BSP_NFCTAG_WriteMBWDG(uint32_t Instance, const uint8_t WdgDelay );
int32_t BSP_NFCTAG_ReadMailboxData(uint32_t Instance, uint8_t * const pData,  const uint16_t TarAddr,  const uint16_t NbByte );
int32_t BSP_NFCTAG_WriteMailboxData(uint32_t Instance, const uint8_t * const pData,  const uint16_t NbByte );
int32_t BSP_NFCTAG_ReadMailboxRegister(uint32_t Instance, uint8_t * const pData,  const uint16_t TarAddr,  const uint16_t NbByte );
int32_t BSP_NFCTAG_WriteMailboxRegister(uint32_t Instance, const uint8_t * const pData,  const uint16_t TarAddr,  const uint16_t NbByte );
int32_t BSP_NFCTAG_ReadI2CSecuritySession_Dyn(uint32_t Instance, ST25DVxxKC_I2CSSO_STATUS_E * const pSession );
int32_t BSP_NFCTAG_ReadITSTStatus_Dyn(uint32_t Instance, uint8_t * const pITStatus );
int32_t BSP_NFCTAG_ReadGPO_Dyn(uint32_t Instance, uint8_t *GPOConfig );
int32_t BSP_NFCTAG_GetGPO_en_Dyn(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pGPO_en );
int32_t BSP_NFCTAG_SetGPO_en_Dyn(uint32_t Instance);
int32_t BSP_NFCTAG_ResetGPO_en_Dyn(uint32_t Instance);
int32_t BSP_NFCTAG_ReadEHCtrl_Dyn(uint32_t Instance, ST25DVxxKC_EH_CTRL_t * const pEH_CTRL );
int32_t BSP_NFCTAG_GetEHENMode_Dyn(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pEH_Val );
int32_t BSP_NFCTAG_SetEHENMode_Dyn(uint32_t Instance);
int32_t BSP_NFCTAG_ResetEHENMode_Dyn(uint32_t Instance);
int32_t BSP_NFCTAG_GetEHON_Dyn(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pEHON );
int32_t BSP_NFCTAG_GetRFField_Dyn(uint32_t Instance, ST25DVxxKC_FIELD_STATUS_E * const pRF_Field );
int32_t BSP_NFCTAG_GetVCC_Dyn(uint32_t Instance, ST25DVxxKC_VCC_STATUS_E * const pVCC );
int32_t BSP_NFCTAG_ReadRFMngt_Dyn(uint32_t Instance, ST25DVxxKC_RF_MNGT_t * const pRF_Mngt );
int32_t BSP_NFCTAG_WriteRFMngt_Dyn(uint32_t Instance, const uint8_t RF_Mngt );
int32_t BSP_NFCTAG_GetRFDisable_Dyn(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pRFDisable );
int32_t BSP_NFCTAG_SetRFDisable_Dyn(uint32_t Instance);
int32_t BSP_NFCTAG_ResetRFDisable_Dyn(uint32_t Instance);
int32_t BSP_NFCTAG_GetRFSleep_Dyn(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pRFSleep );
int32_t BSP_NFCTAG_SetRFSleep_Dyn(uint32_t Instance);
int32_t BSP_NFCTAG_ResetRFSleep_Dyn(uint32_t Instance);
int32_t BSP_NFCTAG_ReadMBCtrl_Dyn(uint32_t Instance, ST25DVxxKC_MB_CTRL_DYN_STATUS_t * const pCtrlStatus );
int32_t BSP_NFCTAG_GetMBEN_Dyn(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pMBEN );
int32_t BSP_NFCTAG_SetMBEN_Dyn(uint32_t Instance);
int32_t BSP_NFCTAG_ResetMBEN_Dyn(uint32_t Instance);
int32_t BSP_NFCTAG_ReadMBLength_Dyn(uint32_t Instance, uint8_t * const pMBLength );
int32_t BSP_NFCTAG_WriteI2CSlaveMode(uint32_t Instance, const ST25DVxxKC_SLAVE_MODE_E slaveMode);
int32_t BSP_NFCTAG_ReadI2CSlaveMode(uint32_t Instance, ST25DVxxKC_SLAVE_MODE_E *const slaveMode);
int32_t BSP_NFCTAG_WriteI2CSlaveAddress(uint32_t Instance, const uint8_t deviceCode, const uint8_t E0);
int32_t BSP_NFCTAG_ReadI2CSlaveAddress(uint32_t Instance, uint8_t *const deviceCode, uint8_t *const E0);



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

#endif /* BSP_NFCTAG_H */

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/
