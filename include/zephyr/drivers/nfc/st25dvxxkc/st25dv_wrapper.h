/**
  ******************************************************************************
  * @file    st25dv_wrapper.h
  * @author  MMY Application Team
  * @brief   This file wraps API from ST25DV to ST25DVxxKC
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2022 STMicroelectronics, all rights reserved
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
#ifndef __ST25DV_WRAPPER_H
#define __ST25DV_WRAPPER_H

#include "st25dvxxkc.h"
#include "st25dv_reg_wrapper.h"

#define ST25DV_ENABLE                                           ST25DVXXKC_ENABLE
#define ST25DV_DISABLE                                          ST25DVXXKC_DISABLE
#define ST25DV_EN_STATUS                                        ST25DVxxKC_EN_STATUS_E

#define ST25DV_EH_ON_DEMAND                                     ST25DVXXKC_EH_ON_DEMAND
#define ST25DV_EH_ACTIVE_AFTER_BOOT                             ST25DVXXKC_EH_ACTIVE_AFTER_BOOT
#define ST25DV_EH_MODE_STATUS                                   ST25DVxxKC_EH_MODE_STATUS_E

#define ST25DV_FIELD_STATUS                                     ST25DVxxKC_FIELD_STATUS_E
#define ST25DV_FIELD_ON                                         ST25DVXXKC_FIELD_ON
#define ST25DV_FIELD_OFF                                        ST25DVXXKC_FIELD_OFF

#define ST25DV_VCC_STATUS                                       ST25DVxxKC_VCC_STATUS_E
#define ST25DV_VCC_ON                                           ST25DVXXKC_VCC_ON
#define ST25DV_VCC_OFF                                          ST25DVXXKC_VCC_OFF

#define ST25DV_NO_PROT                                          ST25DVXXKC_NO_PROT
#define ST25DV_WRITE_PROT                                       ST25DVXXKC_WRITE_PROT
#define ST25DV_READ_PROT                                        ST25DVXXKC_READ_PROT
#define ST25DV_READWRITE_PROT                                   ST25DVXXKC_READWRITE_PROT
#define ST25DV_PROTECTION_CONF                                  ST25DVxxKC_PROTECTION_CONF_E

#define ST25DV_PROT_ZONE4                                       ST25DVXXKC_PROT_ZONE4
#define ST25DV_PROT_ZONE3                                       ST25DVXXKC_PROT_ZONE3
#define ST25DV_PROT_ZONE2                                       ST25DVXXKC_PROT_ZONE2
#define ST25DV_PROT_ZONE1                                       ST25DVXXKC_PROT_ZONE1
#define ST25DV_PROTECTION_ZONE                                  ST25DVxxKC_PROTECTION_ZONE_E

#define ST25DV_NOT_PROTECTED                                    ST25DVXXKC_NOT_PROTECTED
#define ST25DV_PROT_PASSWD3                                     ST25DVXXKC_PROT_PASSWD3
#define ST25DV_PROT_PASSWD2                                     ST25DVXXKC_PROT_PASSWD2
#define ST25DV_PROT_PASSWD1                                     ST25DVXXKC_PROT_PASSWD1
#define ST25DV_PASSWD_PROT_STATUS                               ST25DVxxKC_PASSWD_PROT_STATUS_E

#define ST25DV_UNLOCKED                                         ST25DVXXKC_UNLOCKED
#define ST25DV_LOCKED                                           ST25DVXXKC_LOCKED
#define ST25DV_LOCK_STATUS                                      ST25DVxxKC_LOCK_STATUS_E

#define ST25DV_CCFILE_BLOCK                                     ST25DVxxKC_CCFILE_BLOCK_E
#define ST25DV_CCFILE_2BLCK                                     ST25DVXXKC_CCFILE_2BLCK
#define ST25DV_CCFILE_1BLCK                                     ST25DVXXKC_CCFILE_1BLCK

#define ST25DV_SESSION_OPEN                                     ST25DVXXKC_SESSION_OPEN
#define ST25DV_SESSION_CLOSED                                   ST25DVXXKC_SESSION_CLOSED
#define ST25DV_I2CSSO_STATUS                                    ST25DVxxKC_I2CSSO_STATUS_E

#define ST25DV_ZONE_END3                                        ST25DVXXKC_ZONE_END3
#define ST25DV_ZONE_END2                                        ST25DVXXKC_ZONE_END2
#define ST25DV_ZONE_END1                                        ST25DVXXKC_ZONE_END1
#define ST25DV_END_ZONE                                         ST25DVxxKC_END_ZONE_E

#define ST25DV_75_US                                            ST25DVXXKC_75_US
#define ST25DV_37_US                                            ST25DVXXKC_37_US
#define ST25DV_302_US                                           ST25DVXXKC_302_US
#define ST25DV_264_US                                           ST25DVXXKC_264_US
#define ST25DV_226_US                                           ST25DVXXKC_226_US
#define ST25DV_188_US                                           ST25DVXXKC_188_US
#define ST25DV_151_US                                           ST25DVXXKC_151_US
#define ST25DV_113_US                                           ST25DVXXKC_113_US
#define ST25DV_PULSE_DURATION                                   ST25DVxxKC_PULSE_DURATION_E

#define ST25DV_NO_MSG                                           ST25DVXXKC_NO_MSG
#define ST25DV_HOST_MSG                                         ST25DVXXKC_HOST_MSG
#define ST25DV_RF_MSG                                           ST25DVXXKC_RF_MSG
#define ST25DV_CURRENT_MSG                                      ST25DVxxKC_CURRENT_MSG_E

#define ST25DV_EH_CTRL                                          ST25DVxxKC_EH_CTRL_t

#define ST25DV_GPO                                              ST25DVxxKC_GPO_t

#define ST25DV_RF_MNGT                                          ST25DVxxKC_RF_MNGT_t

#define ST25DV_RF_PROT_ZONE                                     ST25DVxxKC_RF_PROT_ZONE_t

#define ST25DV_I2C_PROT_ZONE                                    ST25DVxxKC_I2C_PROT_ZONE_t

#define ST25DV_MB_CTRL_DYN_STATUS                               ST25DVxxKC_MB_CTRL_DYN_STATUS_t

#define ST25DV_LOCK_CCFILE                                      ST25DVxxKC_LOCK_CCFILE_t

#define ST25DV_MEM_SIZE                                         ST25DVxxKC_MEM_SIZE_t

#define ST25DV_UID                                              ST25DVxxKC_UID_t

#define ST25DV_PASSWD                                           ST25DVxxKC_PASSWD_t

#define ST25DV_Init_Func                                        ST25DVxxKC_Init_Func
#define ST25DV_DeInit_Func                                      ST25DVxxKC_DeInit_Func
#define ST25DV_GetTick_Func                                     ST25DVxxKC_GetTick_Func
#define ST25DV_Write_Func                                       ST25DVxxKC_Write_Func
#define ST25DV_Read_Func                                        ST25DVxxKC_Read_Func
#define ST25DV_IsReady_Func                                     ST25DVxxKC_IsReady_Func

#define ST25DV_IO_t                                             ST25DVxxKC_IO_t

#define ST25DV_Object_t                                         ST25DVxxKC_Object_t

#define NFCTAG_DrvTypeDef                                       ST25DVxxKC_Drv_t
#define ST25DV_Drv_t                                            ST25DVxxKC_Drv_t

#define ST25DV_AM_I_OPEN_DRAIN                                  ST25DVXXKC_AM_I_OPEN_DRAIN
#define ST25DV_AM_I_CMOS                                        ST25DVXXKC_AM_I_CMOS

#define ST25DV_ADDR_DATA_I2C                                    ST25DVXXKC_ADDR_DATA_I2C
#define ST25DV_ADDR_SYST_I2C                                    (ST25DVXXKC_ADDR_DATA_I2C | 0x08)

#define ST25DV_WRITE_TIMEOUT                                    ST25DVXXKC_WRITE_TIMEOUT

#define ST25DV_MAX_WRITE_BYTE                                   ST25DVXXKC_MAX_WRITE_BYTE
#define ST25DV_MAX_MAILBOX_LENGTH                               ST25DVXXKC_MAX_MAILBOX_LENGTH

#define ST25DV_IS_DYNAMIC_REGISTER                              ST25DVXXKC_IS_DYNAMIC_REGISTER

#define St25Dv_Drv                                              St25Dvxxkc_Drv

#define ST25DV_ReadRegister                                     ST25DVxxKC_ReadRegister
#define ST25DV_WriteRegister                                    ST25DVxxKC_WriteRegister
#define ST25DV_RegisterBusIO                                    ST25DVxxKC_RegisterBusIO 
#define ST25DV_ReadMemSize                                      ST25DVxxKC_ReadMemSize
#define ST25DV_ReadICRev                                        ST25DVxxKC_ReadICRev
#define ST25DV_ReadITPulse                                      ST25DVxxKC_ReadITPulse
#define ST25DV_WriteITPulse                                     ST25DVxxKC_WriteITPulse
#define ST25DV_ReadUID                                          ST25DVxxKC_ReadUID
#define ST25DV_ReadDSFID                                        ST25DVxxKC_ReadDSFID
#define ST25DV_ReadDsfidRFProtection                            ST25DVxxKC_ReadDsfidRFProtection
#define ST25DV_ReadAFI                                          ST25DVxxKC_ReadAFI
#define ST25DV_ReadAfiRFProtection                              ST25DVxxKC_ReadAfiRFProtection
#define ST25DV_ReadI2CProtectZone                               ST25DVxxKC_ReadI2CProtectZone
#define ST25DV_WriteI2CProtectZonex                             ST25DVxxKC_WriteI2CProtectZonex
#define ST25DV_ReadLockCCFile                                   ST25DVxxKC_ReadLockCCFile
#define ST25DV_WriteLockCCFile                                  ST25DVxxKC_WriteLockCCFile
#define ST25DV_ReadLockCFG                                      ST25DVxxKC_ReadLockCFG
#define ST25DV_WriteLockCFG                                     ST25DVxxKC_WriteLockCFG
#define ST25DV_PresentI2CPassword                               ST25DVxxKC_PresentI2CPassword
#define ST25DV_WriteI2CPassword                                 ST25DVxxKC_WriteI2CPassword
#define ST25DV_ReadRFZxSS                                       ST25DVxxKC_ReadRFZxSS
#define ST25DV_WriteRFZxSS                                      ST25DVxxKC_WriteRFZxSS
#define ST25DV_ReadEndZonex                                     ST25DVxxKC_ReadEndZonex
#define ST25DV_WriteEndZonex                                    ST25DVxxKC_WriteEndZonex
#define ST25DV_InitEndZone                                      ST25DVxxKC_InitEndZone
#define ST25DV_CreateUserZone                                   ST25DVxxKC_CreateUserZone
#define ST25DV_ReadMemSize                                      ST25DVxxKC_ReadMemSize
#define ST25DV_ReadEHMode                                       ST25DVxxKC_ReadEHMode
#define ST25DV_WriteEHMode                                      ST25DVxxKC_WriteEHMode
#define ST25DV_ReadRFMngt                                       ST25DVxxKC_ReadRFMngt
#define ST25DV_WriteRFMngt                                      ST25DVxxKC_WriteRFMngt
#define ST25DV_GetRFDisable                                     ST25DVxxKC_GetRFDisable
#define ST25DV_SetRFDisable                                     ST25DVxxKC_SetRFDisable
#define ST25DV_ResetRFDisable                                   ST25DVxxKC_ResetRFDisable
#define ST25DV_GetRFSleep                                       ST25DVxxKC_GetRFSleep
#define ST25DV_SetRFSleep                                       ST25DVxxKC_SetRFSleep
#define ST25DV_ResetRFSleep                                     ST25DVxxKC_ResetRFSleep
#define ST25DV_ReadMBMode                                       ST25DVxxKC_ReadMBMode
#define ST25DV_WriteMBMode                                      ST25DVxxKC_WriteMBMode
#define ST25DV_ReadMBWDG                                        ST25DVxxKC_ReadMBWDG
#define ST25DV_WriteMBWDG                                       ST25DVxxKC_WriteMBWDG
#define ST25DV_ReadMailboxData                                  ST25DVxxKC_ReadMailboxData
#define ST25DV_WriteMailboxData                                 ST25DVxxKC_WriteMailboxData
#define ST25DV_ReadMailboxRegister                              ST25DVxxKC_ReadMailboxRegister
#define ST25DV_WriteMailboxRegister                             ST25DVxxKC_WriteMailboxRegister
#define ST25DV_ReadI2CSecuritySession_Dyn                       ST25DVxxKC_ReadI2CSecuritySession_Dyn
#define ST25DV_ReadITSTStatus_Dyn                               ST25DVxxKC_ReadITSTStatus_Dyn
#define ST25DV_ReadGPO_Dyn                                      ST25DVxxKC_ReadGPO_Dyn
#define ST25DV_GetGPO_en_Dyn                                    ST25DVxxKC_GetGPO_en_Dyn
#define ST25DV_SetGPO_en_Dyn                                    ST25DVxxKC_SetGPO_en_Dyn
#define ST25DV_ResetGPO_en_Dyn                                  ST25DVxxKC_ResetGPO_en_Dyn
#define ST25DV_ReadEHCtrl_Dyn                                   ST25DVxxKC_ReadEHCtrl_Dyn
#define ST25DV_GetEHENMode_Dyn                                  ST25DVxxKC_GetEHENMode_Dyn
#define ST25DV_SetEHENMode_Dyn                                  ST25DVxxKC_SetEHENMode_Dyn
#define ST25DV_ResetEHENMode_Dyn                                ST25DVxxKC_ResetEHENMode_Dyn
#define ST25DV_GetEHON_Dyn                                      ST25DVxxKC_GetEHON_Dyn
#define ST25DV_GetRFField_Dyn                                   ST25DVxxKC_GetRFField_Dyn
#define ST25DV_GetVCC_Dyn                                       ST25DVxxKC_GetVCC_Dyn
#define ST25DV_ReadRFMngt_Dyn                                   ST25DVxxKC_ReadRFMngt_Dyn
#define ST25DV_WriteRFMngt_Dyn                                  ST25DVxxKC_WriteRFMngt_Dyn
#define ST25DV_GetRFDisable_Dyn                                 ST25DVxxKC_GetRFDisable_Dyn
#define ST25DV_SetRFDisable_Dyn                                 ST25DVxxKC_SetRFDisable_Dyn
#define ST25DV_ResetRFDisable_Dyn                               ST25DVxxKC_ResetRFDisable_Dyn
#define ST25DV_GetRFSleep_Dyn                                   ST25DVxxKC_GetRFSleep_Dyn
#define ST25DV_SetRFSleep_Dyn                                   ST25DVxxKC_SetRFSleep_Dyn
#define ST25DV_ResetRFSleep_Dyn                                 ST25DVxxKC_ResetRFSleep_Dyn
#define ST25DV_ReadMBCtrl_Dyn                                   ST25DVxxKC_ReadMBCtrl_Dyn
#define ST25DV_GetMBEN_Dyn                                      ST25DVxxKC_GetMBEN_Dyn
#define ST25DV_SetMBEN_Dyn                                      ST25DVxxKC_SetMBEN_Dyn
#define ST25DV_ResetMBEN_Dyn                                    ST25DVxxKC_ResetMBEN_Dyn
#define ST25DV_ReadMBLength_Dyn                                 ST25DVxxKC_ReadMBLength_Dyn


#endif  /*__ST25DV_WRAPPER_H*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
