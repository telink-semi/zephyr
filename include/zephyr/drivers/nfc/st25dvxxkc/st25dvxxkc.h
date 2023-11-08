/**
  ******************************************************************************
  * @file    st25dvxxkc.h
  * @author  MMY Application Team
  * @brief   This file provides set of driver functions to manage communication 
  * @brief   between MCU and ST25DVxxKC chip 
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2021 STMicroelectronics, all rights reserved
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
#ifndef ST25DVXXKC_H
#define ST25DVXXKC_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "st25dvxxkc_reg.h"
#include <zephyr/device.h>

/** @addtogroup BSP
  * @{
  */ 
 

/** @addtogroup ST25DVXXKC
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/**
 * @brief  ST25DVxxKC Enable Disable enumerator definition.
 */
typedef enum
{
  ST25DVXXKC_DISABLE = 0,
  ST25DVXXKC_ENABLE
} ST25DVxxKC_EN_STATUS_E;

/**
 * @brief  ST25DVxxKC Energy Harvesting mode enumerator definition.
 */
typedef enum
{
  ST25DVXXKC_EH_ACTIVE_AFTER_BOOT = 0,
  ST25DVXXKC_EH_ON_DEMAND
} ST25DVxxKC_EH_MODE_STATUS_E;

/**
 * @brief  ST25DVxxKC FIELD status enumerator definition.
 */
typedef enum
{
  ST25DVXXKC_FIELD_OFF = 0,
  ST25DVXXKC_FIELD_ON
} ST25DVxxKC_FIELD_STATUS_E;

/**
 * @brief  ST25DVxxKC VCC status enumerator definition
 */
typedef enum
{
  ST25DVXXKC_VCC_OFF = 0,
  ST25DVXXKC_VCC_ON
} ST25DVxxKC_VCC_STATUS_E;

/**
 * @brief  ST25DVxxKC protection status enumerator definition
 */
typedef enum
{
  ST25DVXXKC_NO_PROT = 0,
  ST25DVXXKC_WRITE_PROT,
  ST25DVXXKC_READ_PROT,
  ST25DVXXKC_READWRITE_PROT
} ST25DVxxKC_PROTECTION_CONF_E;

/**
 * @brief  ST25DVxxKC area protection enumerator definition.
 */
typedef enum
{
  ST25DVXXKC_PROT_ZONE1 = 0,
  ST25DVXXKC_PROT_ZONE2,
  ST25DVXXKC_PROT_ZONE3,
  ST25DVXXKC_PROT_ZONE4
} ST25DVxxKC_PROTECTION_ZONE_E;

/**
 * @brief  ST25DVxxKC password protection status enumerator definition.
 */
typedef enum
{
  ST25DVXXKC_NOT_PROTECTED = 0,
  ST25DVXXKC_PROT_PASSWD1,
  ST25DVXXKC_PROT_PASSWD2,
  ST25DVXXKC_PROT_PASSWD3
} ST25DVxxKC_PASSWD_PROT_STATUS_E;

/**
 * @brief  ST25DVxxKC lock status enumerator definition.
 */
typedef enum
{
  ST25DVXXKC_UNLOCKED = 0,
  ST25DVXXKC_LOCKED
} ST25DVxxKC_LOCK_STATUS_E;

/**
 * @brief  ST25DVxxKC Number of Blocks for the CCFile enumerator definition.
 */
typedef enum
{
  ST25DVXXKC_CCFILE_1BLCK = 0,
  ST25DVXXKC_CCFILE_2BLCK
} ST25DVxxKC_CCFILE_BLOCK_E;

/**
 * @brief  ST25DVxxKC session status enumerator definition.
 */
typedef enum
{
  ST25DVXXKC_SESSION_CLOSED = 0,
  ST25DVXXKC_SESSION_OPEN
} ST25DVxxKC_I2CSSO_STATUS_E;

/**
 * @brief  ST25DVxxKC area end address enumerator definition.
 */
typedef enum
{
  ST25DVXXKC_ZONE_END1 = 0,
  ST25DVXXKC_ZONE_END2,
  ST25DVXXKC_ZONE_END3
} ST25DVxxKC_END_ZONE_E;

/**
 * @brief  ST25DVxxKC IT pulse duration enumerator definition.
 */
typedef enum
{
  ST25DVXXKC_302_US = 0,
  ST25DVXXKC_264_US,
  ST25DVXXKC_226_US,
  ST25DVXXKC_188_US,
  ST25DVXXKC_151_US,
  ST25DVXXKC_113_US,
  ST25DVXXKC_75_US,
  ST25DVXXKC_37_US  
} ST25DVxxKC_PULSE_DURATION_E;

/**
 * @brief  ST25DVxxKC Mailbox Current Message enumerator definition
 */
typedef enum
{
  ST25DVXXKC_NO_MSG = 0,
  ST25DVXXKC_HOST_MSG,
  ST25DVXXKC_RF_MSG  
} ST25DVxxKC_CURRENT_MSG_E;

/**
 * @brief  ST25DVxxKC I2C Slave modes enumerator definition
 */
typedef enum
{
  ST25DVXXKC_SLAVE_MODE_NORMAL = 0,
  ST25DVXXKC_SLAVE_MODE_RFOFF,
  ST25DVXXKC_SLAVE_MODE_RFON  
} ST25DVxxKC_SLAVE_MODE_E;

/**
 * @brief  ST25DVxxKC EH Ctrl structure definition
 */
typedef struct
{
  ST25DVxxKC_EN_STATUS_E EH_EN_Mode;
  ST25DVxxKC_EN_STATUS_E EH_on;
  ST25DVxxKC_EN_STATUS_E Field_on;
  ST25DVxxKC_EN_STATUS_E VCC_on;
} ST25DVxxKC_EH_CTRL_t;

/**
 * @brief  ST25DVxxKC GPO structure definition
 */
typedef struct
{
  ST25DVxxKC_EN_STATUS_E GPO_RFUser_en;
  ST25DVxxKC_EN_STATUS_E GPO_RFActivity_en;
  ST25DVxxKC_EN_STATUS_E GPO_RFInterrupt_en;
  ST25DVxxKC_EN_STATUS_E GPO_FieldChange_en;
  ST25DVxxKC_EN_STATUS_E GPO_RFPutMsg_en;
  ST25DVxxKC_EN_STATUS_E GPO_RFGetMsg_en;
  ST25DVxxKC_EN_STATUS_E GPO_RFWrite_en;
  ST25DVxxKC_EN_STATUS_E GPO_Enable;
} ST25DVxxKC_GPO_t;

/**
 * @brief  ST25DVxxKC RF Management structure definition.
 */
typedef struct
{
  ST25DVxxKC_EN_STATUS_E RfDisable;
  ST25DVxxKC_EN_STATUS_E RfSleep;
} ST25DVxxKC_RF_MNGT_t;

/**
 * @brief  ST25DVxxKC RF Area protection structure definition.
 */
typedef struct
{
  ST25DVxxKC_PASSWD_PROT_STATUS_E PasswdCtrl;
  ST25DVxxKC_PROTECTION_CONF_E RWprotection;
} ST25DVxxKC_RF_PROT_ZONE_t;

/**
 * @brief  ST25DVxxKC I2C Area protection structure definition.
 */
typedef struct
{
  ST25DVxxKC_PROTECTION_CONF_E ProtectZone1;
  ST25DVxxKC_PROTECTION_CONF_E ProtectZone2;
  ST25DVxxKC_PROTECTION_CONF_E ProtectZone3;
  ST25DVxxKC_PROTECTION_CONF_E ProtectZone4;
} ST25DVxxKC_I2C_PROT_ZONE_t;

/**
 * @brief  ST25DVxxKC MB_CTRL_DYN register structure definition.
 */
typedef struct
{
  uint8_t MbEnable;
  uint8_t HostPutMsg;
  uint8_t RfPutMsg;
  uint8_t HostMissMsg;
  uint8_t RFMissMsg;
  ST25DVxxKC_CURRENT_MSG_E CurrentMsg;
} ST25DVxxKC_MB_CTRL_DYN_STATUS_t;

/**
 * @brief  ST25DVxxKC Lock CCFile structure definition.
 */
typedef struct
{
  ST25DVxxKC_LOCK_STATUS_E LckBck0;
  ST25DVxxKC_LOCK_STATUS_E LckBck1;
} ST25DVxxKC_LOCK_CCFILE_t;

/**
 * @brief  ST25DVxxKC Memory size structure definition.
 */
typedef struct
{
  uint8_t BlockSize;
  uint16_t Mem_Size;
} ST25DVxxKC_MEM_SIZE_t;

/**
 * @brief  ST25DVxxKC UID information structure definition.
 */
typedef struct
{
  uint32_t MsbUid;
  uint32_t LsbUid;
} ST25DVxxKC_UID_t;

/**
 * @brief  ST25DVxxKC Password structure definition.
 */
typedef struct
{
  uint32_t MsbPasswd;
  uint32_t LsbPasswd;
} ST25DVxxKC_PASSWD_t;


typedef int32_t (*ST25DVxxKC_Init_Func) (void);
typedef int32_t (*ST25DVxxKC_DeInit_Func) (void);
typedef int32_t (*ST25DVxxKC_GetTick_Func) (void);
typedef int32_t (*ST25DVxxKC_Write_Func)(const struct device *dev, const uint16_t, const uint16_t, const uint8_t *const, const uint16_t);
typedef int32_t (*ST25DVxxKC_Read_Func) (const struct device *dev, const uint16_t, const uint16_t, uint8_t *const, const uint16_t);
// typedef int32_t (*ST25DVxxKC_Write_Func)(const uint16_t, const uint16_t, const uint8_t *const, const uint16_t);
// typedef int32_t (*ST25DVxxKC_Read_Func) (const uint16_t, const uint16_t, uint8_t *const, const uint16_t);

typedef int32_t (*ST25DVxxKC_IsReady_Func) (const uint16_t, const uint32_t);

/**
 * @brief  ST25DVxxKC IO API structure definition.
 */
typedef struct
{
  ST25DVxxKC_Init_Func    Init;
  ST25DVxxKC_DeInit_Func  DeInit;
  ST25DVxxKC_IsReady_Func IsReady;
  ST25DVxxKC_Write_Func   Write;
  ST25DVxxKC_Read_Func    Read;
  ST25DVxxKC_GetTick_Func GetTick;
  uint16_t                DeviceAddress;
} ST25DVxxKC_IO_t;

/**
 * @brief  ST25DVxxKC device structure definition.
 */
typedef struct
{
  ST25DVxxKC_IO_t IO ;
  ST25DVxxKC_Ctx_t Ctx;
  uint32_t IsInitialized;
} ST25DVxxKC_Object_t;


#ifndef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief  ST25DVxxKC standard driver API structure definition.
 */
typedef struct
{
  int32_t       (*Init)(ST25DVxxKC_Object_t *const);
  int32_t       (*ReadID)(const ST25DVxxKC_Object_t *const, uint8_t *const);
  int32_t       (*IsReady)(const ST25DVxxKC_Object_t *const, const uint32_t);
  int32_t       (*GetITStatus)(const ST25DVxxKC_Object_t *const, uint16_t *const);
  int32_t       (*ConfigIT)(const ST25DVxxKC_Object_t *const, const uint16_t);
  int32_t       (*ReadData)(const struct device *dev, const ST25DVxxKC_Object_t *const, uint8_t *const, const uint16_t, const uint16_t);
  int32_t       (*WriteData)(const struct device *dev, const ST25DVxxKC_Object_t *const, const uint8_t *const, const uint16_t, const uint16_t);
} ST25DVxxKC_Drv_t;


#endif

/* Exported constants --------------------------------------------------------*/
/** @brief ST25DVxxKC 4Kbits ICref */
#define I_AM_ST25DV04KC                        0x50U
/** @brief ST25DVxxKC 16/64Kbits ICref */
#define I_AM_ST25DV64KC                        0x51U

/** @brief Check ST25DVxxKC Open Drain version */
#define ST25DVXXKC_AM_I_OPEN_DRAIN(x)     (((x) == 0x50U) || ((x) == 0x51U))
/** @brief Check ST25DVxxKC CMOS version */
#define ST25DVXXKC_AM_I_CMOS(x)           (((x) == 0x52U) || ((x) == 0x53U))


#ifndef NULL
#define NULL      (void *) 0
#endif

/** @brief Default I2C address to be used for ST25DVxxKC Data accesses. */
#define ST25DVXXKC_ADDR_DATA_I2C                (uint8_t)0xA6
/** @brief System memory bit on I2C slave device address */
#define ST25DVXXKC_ADDR_SYSTEMMEMORY_BIT_I2C    (uint8_t)0x08
/** @brief RF Switch bit on I2C slave device address */
#define ST25DVXXKC_ADDR_RFSWITCH_BIT_I2C        (uint8_t)0x08
/** @brief System memory bit on I2C slave device address */
#define ST25DVXXKC_ADDR_MODE_BIT_I2C            (uint8_t)0x04

#define ST25DVXXKC_ADDR_DEVICECODE_SHIFT        (uint8_t)(4)
#define ST25DVXXKC_ADDR_DEVICECODE_MASK         (uint8_t)0xF0
#define ST25DVXXKC_ADDR_E0_SHIFT                (uint8_t)(1)
#define ST25DVXXKC_ADDR_E0_MASK                 (uint8_t)0x02


/** @brief I2C Time out (ms), max value : tw + tw * (Max write bytes) / (Internal page write) :  5 + 5*(256/16). */
#define ST25DVXXKC_WRITE_TIMEOUT                 85U

/** @brief Size of the ST25DVxxKC write buffer. */
#define ST25DVXXKC_MAX_WRITE_BYTE                256U
/** @brief Size of the ST25DVxxKC Mailbox memory. */
#define ST25DVXXKC_MAX_MAILBOX_LENGTH            256U

/** @brief Offset of ST25DVxxKC dynamic registers. */
#define ST25DVXXKC_IS_DYNAMIC_REGISTER          (uint16_t)0x2000

  
/* External variables --------------------------------------------------------*/
/* ST25DVxxKC driver structure */
extern ST25DVxxKC_Drv_t St25Dvxxkc_Drv;

/* Exported macro ------------------------------------------------------------*/
/* Imported functions ------------------------------------------------------- */

/* Exported functions ------------------------------------------------------- */
extern int32_t ST25DVxxKC_ReadRegister(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pData, const uint16_t TarAddr, \
                                                                                      const uint16_t NbByte);
extern int32_t ST25DVxxKC_WriteRegister(ST25DVxxKC_Object_t *const pObj, const uint8_t *const pData, \
                                                                        const uint16_t TarAddr, const uint16_t NbByte);
int32_t ST25DVxxKC_RegisterBusIO(ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_IO_t *const pIO);
int32_t ST25DVxxKC_ReadICRev(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pICRev);
int32_t ST25DVxxKC_ReadITPulse(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_PULSE_DURATION_E *const pITtime);
int32_t ST25DVxxKC_WriteITPulse(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_PULSE_DURATION_E ITtime);
int32_t ST25DVxxKC_ReadUID(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_UID_t *const pUid);
int32_t ST25DVxxKC_ReadDSFID(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pDsfid);
int32_t ST25DVxxKC_ReadDsfidRFProtection(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_LOCK_STATUS_E *const pLockDsfid);
int32_t ST25DVxxKC_ReadAFI(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pAfi);
int32_t ST25DVxxKC_ReadAfiRFProtection(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_LOCK_STATUS_E *const pLockAfi);
int32_t ST25DVxxKC_ReadI2CProtectZone(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_I2C_PROT_ZONE_t *const pProtZone);
int32_t ST25DVxxKC_WriteI2CProtectZonex(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_PROTECTION_ZONE_E Zone, \
                                                          const ST25DVxxKC_PROTECTION_CONF_E ReadWriteProtection);
int32_t ST25DVxxKC_ReadLockCCFile(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_LOCK_CCFILE_t *const pLockCCFile);
int32_t ST25DVxxKC_WriteLockCCFile(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_CCFILE_BLOCK_E NbBlockCCFile, \
                                                          const ST25DVxxKC_LOCK_STATUS_E LockCCFile);
int32_t ST25DVxxKC_ReadLockCFG(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_LOCK_STATUS_E *const pLockCfg);
int32_t ST25DVxxKC_WriteLockCFG(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_LOCK_STATUS_E LockCfg);
int32_t ST25DVxxKC_PresentI2CPassword(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_PASSWD_t PassWord);
int32_t ST25DVxxKC_WriteI2CPassword(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_PASSWD_t PassWord);
int32_t ST25DVxxKC_ReadRFZxSS(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_PROTECTION_ZONE_E Zone, \
                                                          ST25DVxxKC_RF_PROT_ZONE_t *const pRfprotZone);
int32_t ST25DVxxKC_WriteRFZxSS(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_PROTECTION_ZONE_E Zone, \
                                                          const ST25DVxxKC_RF_PROT_ZONE_t RfProtZone);
int32_t ST25DVxxKC_ReadEndZonex(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_END_ZONE_E EndZone, uint8_t * pEndZ);
int32_t ST25DVxxKC_WriteEndZonex(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_END_ZONE_E EndZone, const uint8_t EndZ);
int32_t ST25DVxxKC_InitEndZone(const ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_CreateUserZone(const ST25DVxxKC_Object_t *const pObj, uint16_t Zone1Length, uint16_t Zone2Length, \
                                                          uint16_t Zone3Length, uint16_t Zone4Length);
int32_t ST25DVxxKC_ReadMemSize(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_MEM_SIZE_t *const pSizeInfo);
int32_t ST25DVxxKC_ReadEHMode(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EH_MODE_STATUS_E *const pEH_mode);
int32_t ST25DVxxKC_WriteEHMode(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_EH_MODE_STATUS_E EH_mode);
int32_t ST25DVxxKC_ReadRFMngt(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_RF_MNGT_t *const pRF_Mngt);
int32_t ST25DVxxKC_WriteRFMngt(const ST25DVxxKC_Object_t *const pObj, const uint8_t Rfmngt);
int32_t ST25DVxxKC_GetRFDisable(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pRFDisable);
int32_t ST25DVxxKC_SetRFDisable(const ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_ResetRFDisable(const ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_GetRFSleep(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pRFSleep);
int32_t ST25DVxxKC_SetRFSleep(const ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_ResetRFSleep(const ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_ReadMBMode(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pMB_mode);
int32_t ST25DVxxKC_WriteMBMode(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_EN_STATUS_E MB_mode);
int32_t ST25DVxxKC_ReadMBWDG(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pWdgDelay);
int32_t ST25DVxxKC_WriteMBWDG(const ST25DVxxKC_Object_t *const pObj, const uint8_t WdgDelay);
int32_t ST25DVxxKC_ReadMailboxData(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pData, const uint16_t TarAddr, \
                                                          const uint16_t NbByte);
int32_t ST25DVxxKC_WriteMailboxData(const ST25DVxxKC_Object_t *const pObj, const uint8_t *const pData, const uint16_t NbByte);
int32_t ST25DVxxKC_ReadMailboxRegister(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pData, const uint16_t TarAddr, \
                                                          const uint16_t NbByte);
int32_t ST25DVxxKC_WriteMailboxRegister(const ST25DVxxKC_Object_t *const pObj, const uint8_t *const pData, \
                                                          const uint16_t TarAddr, const uint16_t NbByte);
int32_t ST25DVxxKC_ReadI2CSecuritySession_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_I2CSSO_STATUS_E *const pSession);
int32_t ST25DVxxKC_ReadITSTStatus_Dyn(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pITStatus);
int32_t ST25DVxxKC_ReadGPO_Dyn(const ST25DVxxKC_Object_t *const pObj, uint8_t *GPOConfig);
int32_t ST25DVxxKC_GetGPO_en_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pGPO_en);
int32_t ST25DVxxKC_SetGPO_en_Dyn(const ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_ResetGPO_en_Dyn(const ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_ReadEHCtrl_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EH_CTRL_t *const pEH_CTRL);
int32_t ST25DVxxKC_GetEHENMode_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pEH_Val);
int32_t ST25DVxxKC_SetEHENMode_Dyn(const ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_ResetEHENMode_Dyn(const ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_GetEHON_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pEHON);
int32_t ST25DVxxKC_GetRFField_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_FIELD_STATUS_E *const pRF_Field);
int32_t ST25DVxxKC_GetVCC_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_VCC_STATUS_E *const pVCC);
int32_t ST25DVxxKC_ReadRFMngt_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_RF_MNGT_t *const pRF_Mngt);
int32_t ST25DVxxKC_WriteRFMngt_Dyn(const ST25DVxxKC_Object_t *const pObj, const uint8_t RF_Mngt);
int32_t ST25DVxxKC_GetRFDisable_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pRFDisable);
int32_t ST25DVxxKC_SetRFDisable_Dyn(const ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_ResetRFDisable_Dyn(const ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_GetRFSleep_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pRFSleep);
int32_t ST25DVxxKC_SetRFSleep_Dyn(const ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_ResetRFSleep_Dyn(const ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_GetRFOff_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pRFSleep);
int32_t ST25DVxxKC_SetRFOff_Dyn(const ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_ResetRFOff_Dyn(const ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_ReadMBCtrl_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_MB_CTRL_DYN_STATUS_t *const pCtrlStatus);
int32_t ST25DVxxKC_GetMBEN_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pMBEN);
int32_t ST25DVxxKC_SetMBEN_Dyn(const ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_ResetMBEN_Dyn(const ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_ReadMBLength_Dyn(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pMBLength);
int32_t ST25DVxxKC_WriteI2CSlaveMode(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_SLAVE_MODE_E slave_mode);
int32_t ST25DVxxKC_ReadI2CSlaveMode(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_SLAVE_MODE_E *const slave_mode);
int32_t ST25DVxxKC_WriteI2CSlaveAddress(const ST25DVxxKC_Object_t *const pObj, const uint8_t deviceCode, const uint8_t E0);
int32_t ST25DVxxKC_ReadI2CSlaveAddress(const ST25DVxxKC_Object_t *const pObj, uint8_t *const deviceCode, uint8_t *const E0);

/**
  * @}
  */

/**
  * @}
  */ 
  

#ifdef __cplusplus
  }
#endif
#endif /* ST25DVXXKC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
