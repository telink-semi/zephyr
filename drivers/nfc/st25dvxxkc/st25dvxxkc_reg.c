/**
  ******************************************************************************
  * @file           : st25dvxxkc_reg.c
  * @author         : MMY Ecosystem Team
  * @brief          : ST25DVXXKC register file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "st25dvxxkc_reg.h"

/** @addtogroup BSP
  * @{
  */

/** @defgroup ST25DVxxKC ST25DVxxKC_reg driver
  * @brief    This module implements the functions to access the ST25DVxxKC registers.
  * @{
  */
  
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
int32_t ST25DVxxKC_ReadReg(const ST25DVxxKC_Ctx_t *const ctx, const uint16_t Reg, uint8_t *const Data, const uint16_t len);
int32_t ST25DVxxKC_WriteReg(const ST25DVxxKC_Ctx_t *const ctx, const uint16_t Reg, const uint8_t *const Data, const uint16_t len);
/* Public functions ---------------------------------------------------------*/
/**
 * @brief  Read register from component
 * @param[in] ctx structure containing context driver
 * @param[in] Reg register to read
 * @param[out] Data pointer to store register content
 * @param[out] len length of data
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_ReadReg(const ST25DVxxKC_Ctx_t *const ctx, const uint16_t Reg, uint8_t *const Data, const uint16_t len)
{
  return ctx->ReadReg(ctx->handle, Reg, Data, len);
}

/**
 * @brief  Write register to component
 * @param[in] ctx structure containing context driver
 * @param[in] Reg register to write
 * @param[in] Data data pointer to write to register
 * @param[out] len length of data
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_WriteReg(const ST25DVxxKC_Ctx_t *const ctx, const uint16_t Reg, const uint8_t *const Data, const uint16_t len)
{
  return ctx->WriteReg(ctx->handle, Reg, Data, len);
}

/**
 * @brief  Read IC Ref register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetICREF(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_ICREF_REG), value, 1);
}

/**
 * @brief  Read ENDA1 register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetENDA1(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_ENDA1_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Write ENDA1 register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetENDA1(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  return ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_ENDA1_REG), value, 1);
}

/**
 * @brief  Read ENDA2 register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetENDA2(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_ENDA2_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Write ENDA2 register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetENDA2(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  return ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_ENDA2_REG), value, 1);
}

/**
 * @brief  Read ENDA3 register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetENDA3(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_ENDA3_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Write ENDA3 register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetENDA3(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  return ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_ENDA3_REG), value, 1);
}

/**
 * @brief  Read DSFID register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetDSFID(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_DSFID_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Read AFI register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetAFI(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_AFI_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Read MEM_SIZE_MSB register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetMEM_SIZE_MSB(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_MEM_SIZE_MSB_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Read BLK_SIZE register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetBLK_SIZE(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_BLK_SIZE_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Read MEM_SIZE_LSB register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetMEM_SIZE_LSB(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_MEM_SIZE_LSB_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Read ICREV register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetICREV(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_ICREV_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Read UID register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetUID(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_UID_REG), (uint8_t *)value, 8);
}

/**
 * @brief  Read I2CPASSWD register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetI2CPASSWD(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_I2CPASSWD_REG), (uint8_t *)value, 8);
}

/**
 * @brief  Write I2CPASSWD register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetI2CPASSWD(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  return ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_I2CPASSWD_REG), value, 1);
}

/**
 * @brief  Read LOCKDSFI register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetLOCKDSFID(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_LOCKDSFID_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Read LOCKAFI register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetLOCKAFI(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_LOCKAFI_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Read FTM_RW register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetFTM_MBMODE(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_FTM_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {  
    *value &= (ST25DVXXKC_FTM_MBMODE_MASK);
    *value = *value >> (ST25DVXXKC_FTM_MBMODE_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write FTM_RW register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetFTM_MBMODE(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_FTM_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_FTM_MBMODE_SHIFT)) & (ST25DVXXKC_FTM_MBMODE_MASK)) |
                  (reg_value & ~(ST25DVXXKC_FTM_MBMODE_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_FTM_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read FTM register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetFTM_MBWDG(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_FTM_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_FTM_MBWDG_MASK);
    *value = *value >> (ST25DVXXKC_FTM_MBWDG_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write FTM register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetFTM_MBWDG(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_FTM_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_FTM_MBWDG_SHIFT)) & (ST25DVXXKC_FTM_MBWDG_MASK)) |
                  (reg_value & ~(ST25DVXXKC_FTM_MBWDG_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_FTM_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read MBLEN_DYN_MBLEN register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetMBLEN_DYN_MBLEN(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_MBLEN_DYN_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Read MB_CTRL_DYN_MBEN register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetMB_CTRL_DYN_MBEN(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_MB_CTRL_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_MB_CTRL_DYN_MBEN_MASK);
    *value = *value >> (ST25DVXXKC_MB_CTRL_DYN_MBEN_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write MB_CTRL_DYN_MBEN register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetMB_CTRL_DYN_MBEN(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_MB_CTRL_DYN_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_MB_CTRL_DYN_MBEN_SHIFT)) & (ST25DVXXKC_MB_CTRL_DYN_MBEN_MASK)) |
                  (reg_value & ~(ST25DVXXKC_MB_CTRL_DYN_MBEN_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_MB_CTRL_DYN_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read MB_CTRL_DYN_HOSTPUTMSG register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetMB_CTRL_DYN_HOSTPUTMSG(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_MB_CTRL_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_MB_CTRL_DYN_HOSTPUTMSG_MASK);
    *value = *value >> (ST25DVXXKC_MB_CTRL_DYN_HOSTPUTMSG_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read MB_CTRL_DYN_RFPUTMSG register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetMB_CTRL_DYN_RFPUTMSG(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_MB_CTRL_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_MB_CTRL_DYN_RFPUTMSG_MASK);
    *value = *value >> (ST25DVXXKC_MB_CTRL_DYN_RFPUTMSG_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read MB_CTRL_DYN_STRESERVED register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetMB_CTRL_DYN_STRESERVED(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_MB_CTRL_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_MB_CTRL_DYN_STRESERVED_MASK);
    *value = *value >> (ST25DVXXKC_MB_CTRL_DYN_STRESERVED_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read MB_CTRL_DYN_HOSTMISSMSG register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetMB_CTRL_DYN_HOSTMISSMSG(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_MB_CTRL_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_MB_CTRL_DYN_HOSTMISSMSG_MASK);
    *value = *value >> (ST25DVXXKC_MB_CTRL_DYN_HOSTMISSMSG_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read MB_CTRL_DYN_RFMISSMSG register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetMB_CTRL_DYN_RFMISSMSG(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_MB_CTRL_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_MB_CTRL_DYN_RFMISSMSG_MASK);
    *value = *value >> (ST25DVXXKC_MB_CTRL_DYN_RFMISSMSG_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read MB_CTRL_DYN_CURRENTMSG register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetMB_CTRL_DYN_CURRENTMSG(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_MB_CTRL_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_MB_CTRL_DYN_CURRENTMSG_MASK);
    *value = *value >> (ST25DVXXKC_MB_CTRL_DYN_CURRENTMSG_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read MB_CTRL_DYN_ALL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetMB_CTRL_DYN_ALL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_MB_CTRL_DYN_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Read I2CCFG_DEVICECODE register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetI2CCFG_DEVICECODE(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_I2CCFG_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_I2CCFG_DEVICECODE_MASK);
    *value = *value >> (ST25DVXXKC_I2CCFG_DEVICECODE_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write I2CCFG_DEVICECODE register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetI2CCFG_DEVICECODE(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_I2CCFG_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_I2CCFG_DEVICECODE_SHIFT)) & (ST25DVXXKC_I2CCFG_DEVICECODE_MASK)) |
                  (reg_value & ~(ST25DVXXKC_I2CCFG_DEVICECODE_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_I2CCFG_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read I2CCFG_E0 register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetI2CCFG_E0(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_I2CCFG_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_I2CCFG_E0_MASK);
    *value = *value >> (ST25DVXXKC_I2CCFG_E0_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write I2CCFG_E0 register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetI2CCFG_E0(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_I2CCFG_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_I2CCFG_E0_SHIFT)) & (ST25DVXXKC_I2CCFG_E0_MASK)) |
                  (reg_value & ~(ST25DVXXKC_I2CCFG_E0_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_I2CCFG_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read I2CCFG_RFSWITCHOFF register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetI2CCFG_RFSWITCHOFF(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_I2CCFG_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_I2CCFG_RFSWITCHOFF_MASK);
    *value = *value >> (ST25DVXXKC_I2CCFG_RFSWITCHOFF_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write I2CCFG_RFSWITCHOFF register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetI2CCFG_RFSWITCHOFF(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_I2CCFG_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_I2CCFG_RFSWITCHOFF_SHIFT)) & (ST25DVXXKC_I2CCFG_RFSWITCHOFF_MASK)) |
                  (reg_value & ~(ST25DVXXKC_I2CCFG_RFSWITCHOFF_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_I2CCFG_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read GPO1_ENABLE register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetGPO1_ENABLE(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO1_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_GPO1_ENABLE_MASK);
    *value = *value >> (ST25DVXXKC_GPO1_ENABLE_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write GPO1_ENABLE register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetGPO1_ENABLE(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO1_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_GPO1_ENABLE_SHIFT)) & (ST25DVXXKC_GPO1_ENABLE_MASK)) |
                  (reg_value & ~(ST25DVXXKC_GPO1_ENABLE_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_GPO1_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read GPO1_RFUSERSTATE register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetGPO1_RFUSERSTATE(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO1_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_GPO1_RFUSERSTATE_MASK);
    *value = *value >> (ST25DVXXKC_GPO1_RFUSERSTATE_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write GPO1_RFUSERSTATE register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetGPO1_RFUSERSTATE(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO1_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_GPO1_RFUSERSTATE_SHIFT)) & (ST25DVXXKC_GPO1_RFUSERSTATE_MASK)) |
                  (reg_value & ~(ST25DVXXKC_GPO1_RFUSERSTATE_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_GPO1_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read GPO1_RFACTIVITY register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetGPO1_RFACTIVITY(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO1_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_GPO1_RFACTIVITY_MASK);
    *value = *value >> (ST25DVXXKC_GPO1_RFACTIVITY_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write GPO1_RFACTIVITY register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetGPO1_RFACTIVITY(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO1_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_GPO1_RFACTIVITY_SHIFT)) & (ST25DVXXKC_GPO1_RFACTIVITY_MASK)) |
                  (reg_value & ~(ST25DVXXKC_GPO1_RFACTIVITY_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_GPO1_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read GPO1_RFINTERRUPT register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetGPO1_RFINTERRUPT(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO1_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_GPO1_RFINTERRUPT_MASK);
    *value = *value >> (ST25DVXXKC_GPO1_RFINTERRUPT_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write GPO1_RFINTERRUPT register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetGPO1_RFINTERRUPT(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO1_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_GPO1_RFINTERRUPT_SHIFT)) & (ST25DVXXKC_GPO1_RFINTERRUPT_MASK)) |
                  (reg_value & ~(ST25DVXXKC_GPO1_RFINTERRUPT_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_GPO1_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read GPO1_FIELDCHANGE register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetGPO1_FIELDCHANGE(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO1_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_GPO1_FIELDCHANGE_MASK);
    *value = *value >> (ST25DVXXKC_GPO1_FIELDCHANGE_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write GPO1_FIELDCHANGE register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetGPO1_FIELDCHANGE(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO1_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_GPO1_FIELDCHANGE_SHIFT)) & (ST25DVXXKC_GPO1_FIELDCHANGE_MASK)) |
                  (reg_value & ~(ST25DVXXKC_GPO1_FIELDCHANGE_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_GPO1_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read GPO1_RFPUTMSG register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetGPO1_RFPUTMSG(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO1_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_GPO1_RFPUTMSG_MASK);
    *value = *value >> (ST25DVXXKC_GPO1_RFPUTMSG_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write GPO1_RFPUTMSG register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetGPO1_RFPUTMSG(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO1_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_GPO1_RFPUTMSG_SHIFT)) & (ST25DVXXKC_GPO1_RFPUTMSG_MASK)) |
                  (reg_value & ~(ST25DVXXKC_GPO1_RFPUTMSG_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_GPO1_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read GPO1_RFGETMSG register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetGPO1_RFGETMSG(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO1_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_GPO1_RFGETMSG_MASK);
    *value = *value >> (ST25DVXXKC_GPO1_RFGETMSG_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write GPO1_RFGETMSG register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetGPO1_RFGETMSG(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO1_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_GPO1_RFGETMSG_SHIFT)) & (ST25DVXXKC_GPO1_RFGETMSG_MASK)) |
                  (reg_value & ~(ST25DVXXKC_GPO1_RFGETMSG_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_GPO1_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read GPO1_RFWRITE register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetGPO1_RFWRITE(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO1_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_GPO1_RFWRITE_MASK);
    *value = *value >> (ST25DVXXKC_GPO1_RFWRITE_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write GPO1_RFWRITE register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetGPO1_RFWRITE(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO1_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_GPO1_RFWRITE_SHIFT)) & (ST25DVXXKC_GPO1_RFWRITE_MASK)) |
                  (reg_value & ~(ST25DVXXKC_GPO1_RFWRITE_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_GPO1_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read GPO1_ALL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetGPO1_ALL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO1_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Write GPO1_ALL register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetGPO1_ALL(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  return ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_GPO1_REG), value, 1);
}

/**
 * @brief  Read GPO2_I2CWRITEENABLE register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetGPO2_I2CWRITEENABLE(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO2_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_GPO2_I2CWRITEENABLE_MASK);
    *value = *value >> (ST25DVXXKC_GPO2_I2CWRITEENABLE_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write GPO2_I2CWRITEENABLE register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetGPO2_I2CWRITEENABLE(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO2_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_GPO2_I2CWRITEENABLE_SHIFT)) & (ST25DVXXKC_GPO2_I2CWRITEENABLE_MASK)) |
                  (reg_value & ~(ST25DVXXKC_GPO2_I2CWRITEENABLE_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_GPO2_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read GPO2_RFOFF register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetGPO2_RFOFF(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO2_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {  
    *value &= (ST25DVXXKC_GPO2_RFOFF_MASK);
    *value = *value >> (ST25DVXXKC_GPO2_RFOFF_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write GPO2_RFOFF register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetGPO2_RFOFF(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO2_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_GPO2_RFOFF_SHIFT)) & (ST25DVXXKC_GPO2_RFOFF_MASK)) |
                  (reg_value & ~(ST25DVXXKC_GPO2_RFOFF_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_GPO2_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read GPO2_ITTIME register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetGPO2_ITTIME(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO2_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_GPO2_ITTIME_MASK);
    *value = *value >> (ST25DVXXKC_GPO2_ITTIME_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write GPO2_ITTIME register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetGPO2_ITTIME(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO2_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_GPO2_ITTIME_SHIFT)) & (ST25DVXXKC_GPO2_ITTIME_MASK)) |
                  (reg_value & ~(ST25DVXXKC_GPO2_ITTIME_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_GPO2_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read GPO2_ALL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetGPO2_ALL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO2_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_GPO2_ALL_MASK);
    *value = *value >> (ST25DVXXKC_GPO2_ALL_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write GPO2_ALL register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetGPO2_ALL(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO2_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_GPO2_ALL_SHIFT)) & (ST25DVXXKC_GPO2_ALL_MASK)) |
                  (reg_value & ~(ST25DVXXKC_GPO2_ALL_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_GPO2_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read GPO_DYN_ENABLE register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetGPO_DYN_ENABLE(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_GPO_DYN_ENABLE_MASK);
    *value = *value >> (ST25DVXXKC_GPO_DYN_ENABLE_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write GPO_DYN_ENABLE register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetGPO_DYN_ENABLE(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO_DYN_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_GPO_DYN_ENABLE_SHIFT)) & (ST25DVXXKC_GPO_DYN_ENABLE_MASK)) |
                  (reg_value & ~(ST25DVXXKC_GPO_DYN_ENABLE_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_GPO_DYN_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read GPO_DYN_ALL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetGPO_DYN_ALL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_GPO_DYN_ALL_MASK);
    *value = *value >> (ST25DVXXKC_GPO_DYN_ALL_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write GPO_DYN_ALL register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetGPO_DYN_ALL(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_GPO_DYN_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_GPO_DYN_ALL_SHIFT)) & (ST25DVXXKC_GPO_DYN_ALL_MASK)) |
                  (reg_value & ~(ST25DVXXKC_GPO_DYN_ALL_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_GPO_DYN_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read ITTIME_DELAY register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetITTIME_DELAY(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_GetGPO2_ITTIME(ctx, value);
}

/**
 * @brief  Write ITTIME_DELAY register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetITTIME_DELAY(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  return ST25DVxxKC_SetGPO2_ITTIME(ctx, value);
}

/**
 * @brief  Read ITSTS_DYN_RFUSERSTATE register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetITSTS_DYN_RFUSERSTATE(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_ITSTS_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_ITSTS_DYN_RFUSERSTATE_MASK);
    *value = *value >> (ST25DVXXKC_ITSTS_DYN_RFUSERSTATE_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read ITSTS_DYN_RFACTIVITY register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetITSTS_DYN_RFACTIVITY(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_ITSTS_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_ITSTS_DYN_RFACTIVITY_MASK);
    *value = *value >> (ST25DVXXKC_ITSTS_DYN_RFACTIVITY_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read ITSTS_DYN_RFINTERRUPT register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetITSTS_DYN_RFINTERRUPT(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_ITSTS_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_ITSTS_DYN_RFINTERRUPT_MASK);
    *value = *value >> (ST25DVXXKC_ITSTS_DYN_RFINTERRUPT_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read ITSTS_DYN_FIELDFALLING register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetITSTS_DYN_FIELDFALLING(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_ITSTS_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_ITSTS_DYN_FIELDFALLING_MASK);
    *value = *value >> (ST25DVXXKC_ITSTS_DYN_FIELDFALLING_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read ITSTS_DYN_FIELDRISING register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetITSTS_DYN_FIELDRISING(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_ITSTS_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_ITSTS_DYN_FIELDRISING_MASK);
    *value = *value >> (ST25DVXXKC_ITSTS_DYN_FIELDRISING_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read ITSTS_DYN_RFPUTMSG register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetITSTS_DYN_RFPUTMSG(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_ITSTS_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_ITSTS_DYN_RFPUTMSG_MASK);
    *value = *value >> (ST25DVXXKC_ITSTS_DYN_RFPUTMSG_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read ITSTS_DYN_RFGETMSG register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetITSTS_DYN_RFGETMSG(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_ITSTS_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_ITSTS_DYN_RFGETMSG_MASK);
    *value = *value >> (ST25DVXXKC_ITSTS_DYN_RFGETMSG_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read ITSTS_DYN_RFWRITE register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetITSTS_DYN_RFWRITE(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_ITSTS_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_ITSTS_DYN_RFWRITE_MASK);
    *value = *value >> (ST25DVXXKC_ITSTS_DYN_RFWRITE_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read ITSTS_DYN_ALL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetITSTS_DYN_ALL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_ITSTS_DYN_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Read EH_MODE register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetEH_MODE(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_EH_MODE_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_EH_MODE_MASK);
    *value = *value >> (ST25DVXXKC_EH_MODE_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write EH_MODE register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetEH_MODE(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_EH_MODE_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_EH_MODE_SHIFT)) & (ST25DVXXKC_EH_MODE_MASK)) |
                  (reg_value & ~(ST25DVXXKC_EH_MODE_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_EH_MODE_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read EH_CTRL_DYN_EH_EN register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetEH_CTRL_DYN_EH_EN(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_EH_CTRL_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_EH_CTRL_DYN_EH_EN_MASK);
    *value = *value >> (ST25DVXXKC_EH_CTRL_DYN_EH_EN_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write EH_CTRL_DYN_EH_EN register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetEH_CTRL_DYN_EH_EN(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_EH_CTRL_DYN_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_EH_CTRL_DYN_EH_EN_SHIFT)) & (ST25DVXXKC_EH_CTRL_DYN_EH_EN_MASK)) |
                  (reg_value & ~(ST25DVXXKC_EH_CTRL_DYN_EH_EN_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_EH_CTRL_DYN_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read EH_CTRL_DYN_EH_ON register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetEH_CTRL_DYN_EH_ON(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_EH_CTRL_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_EH_CTRL_DYN_EH_ON_MASK);
    *value = *value >> (ST25DVXXKC_EH_CTRL_DYN_EH_ON_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read EH_CTRL_DYN_FIELD_ON register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetEH_CTRL_DYN_FIELD_ON(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_EH_CTRL_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_EH_CTRL_DYN_FIELD_ON_MASK);
    *value = *value >> (ST25DVXXKC_EH_CTRL_DYN_FIELD_ON_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read EH_CTRL_DYN_VCC_ON register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetEH_CTRL_DYN_VCC_ON(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_EH_CTRL_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_EH_CTRL_DYN_VCC_ON_MASK);
    *value = *value >> (ST25DVXXKC_EH_CTRL_DYN_VCC_ON_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read EH_CTRL_DYN_ALL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetEH_CTRL_DYN_ALL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_EH_CTRL_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_EH_CTRL_DYN_ALL_MASK);
    *value = *value >> (ST25DVXXKC_EH_CTRL_DYN_ALL_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read RF_MNGT_RFDIS register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRF_MNGT_RFDIS(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RF_MNGT_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RF_MNGT_RFDIS_MASK);
    *value = *value >> (ST25DVXXKC_RF_MNGT_RFDIS_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RF_MNGT_RFDIS register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRF_MNGT_RFDIS(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RF_MNGT_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RF_MNGT_RFDIS_SHIFT)) & (ST25DVXXKC_RF_MNGT_RFDIS_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RF_MNGT_RFDIS_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RF_MNGT_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RF_MNGT_RFSLEEP register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRF_MNGT_RFSLEEP(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RF_MNGT_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RF_MNGT_RFSLEEP_MASK);
    *value = *value >> (ST25DVXXKC_RF_MNGT_RFSLEEP_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RF_MNGT_RFSLEEP register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRF_MNGT_RFSLEEP(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RF_MNGT_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RF_MNGT_RFSLEEP_SHIFT)) & (ST25DVXXKC_RF_MNGT_RFSLEEP_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RF_MNGT_RFSLEEP_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RF_MNGT_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RF_MNGT_ALL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRF_MNGT_ALL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RF_MNGT_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RF_MNGT_ALL_MASK);
    *value = *value >> (ST25DVXXKC_RF_MNGT_ALL_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RF_MNGT_ALL register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRF_MNGT_ALL(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RF_MNGT_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RF_MNGT_ALL_SHIFT)) & (ST25DVXXKC_RF_MNGT_ALL_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RF_MNGT_ALL_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RF_MNGT_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RF_MNGT_DYN_RFDIS register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRF_MNGT_DYN_RFDIS(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RF_MNGT_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RF_MNGT_DYN_RFDIS_MASK);
    *value = *value >> (ST25DVXXKC_RF_MNGT_DYN_RFDIS_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RF_MNGT_DYN_RFDIS register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRF_MNGT_DYN_RFDIS(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RF_MNGT_DYN_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RF_MNGT_DYN_RFDIS_SHIFT)) & (ST25DVXXKC_RF_MNGT_DYN_RFDIS_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RF_MNGT_DYN_RFDIS_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RF_MNGT_DYN_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RF_MNGT_DYN_RFSLEEP register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRF_MNGT_DYN_RFSLEEP(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RF_MNGT_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RF_MNGT_DYN_RFSLEEP_MASK);
    *value = *value >> (ST25DVXXKC_RF_MNGT_DYN_RFSLEEP_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RF_MNGT_DYN_RFSLEEP register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRF_MNGT_DYN_RFSLEEP(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RF_MNGT_DYN_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RF_MNGT_DYN_RFSLEEP_SHIFT)) & (ST25DVXXKC_RF_MNGT_DYN_RFSLEEP_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RF_MNGT_DYN_RFSLEEP_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RF_MNGT_DYN_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RF_MNGT_DYN_RFOFF register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRF_MNGT_DYN_RFOFF(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RF_MNGT_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RF_MNGT_DYN_RFOFF_MASK);
    *value = *value >> (ST25DVXXKC_RF_MNGT_DYN_RFOFF_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RF_MNGT_DYN_RFOFF register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRF_MNGT_DYN_RFOFF(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
   
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RF_MNGT_DYN_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RF_MNGT_DYN_RFOFF_SHIFT)) & (ST25DVXXKC_RF_MNGT_DYN_RFOFF_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RF_MNGT_DYN_RFOFF_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RF_MNGT_DYN_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RF_MNGT_DYN_ALL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRF_MNGT_DYN_ALL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RF_MNGT_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RF_MNGT_DYN_ALL_MASK);
    *value = *value >> (ST25DVXXKC_RF_MNGT_DYN_ALL_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RF_MNGT_DYN_ALL register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRF_MNGT_DYN_ALL(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RF_MNGT_DYN_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RF_MNGT_DYN_ALL_SHIFT)) & (ST25DVXXKC_RF_MNGT_DYN_ALL_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RF_MNGT_DYN_ALL_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RF_MNGT_DYN_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RFA1SS_PWDCTRL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRFA1SS_PWDCTRL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA1SS_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RFA1SS_PWDCTRL_MASK);
    *value = *value >> (ST25DVXXKC_RFA1SS_PWDCTRL_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RFA1SS_PWDCTRL register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRFA1SS_PWDCTRL(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA1SS_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RFA1SS_PWDCTRL_SHIFT)) & (ST25DVXXKC_RFA1SS_PWDCTRL_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RFA1SS_PWDCTRL_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RFA1SS_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RFA1SS_RWPROT register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRFA1SS_RWPROT(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA1SS_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RFA1SS_RWPROT_MASK);
    *value = *value >> (ST25DVXXKC_RFA1SS_RWPROT_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RFA1SS_RWPROT register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRFA1SS_RWPROT(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA1SS_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RFA1SS_RWPROT_SHIFT)) & (ST25DVXXKC_RFA1SS_RWPROT_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RFA1SS_RWPROT_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RFA1SS_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RFA1SS_ALL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRFA1SS_ALL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA1SS_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RFA1SS_ALL_MASK);
    *value = *value >> (ST25DVXXKC_RFA1SS_ALL_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RFA1SS_ALL register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRFA1SS_ALL(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA1SS_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RFA1SS_ALL_SHIFT)) & (ST25DVXXKC_RFA1SS_ALL_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RFA1SS_ALL_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RFA1SS_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RFA2SS_PWDCTRL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRFA2SS_PWDCTRL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA2SS_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RFA2SS_PWDCTRL_MASK);
    *value = *value >> (ST25DVXXKC_RFA2SS_PWDCTRL_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RFA2SS_PWDCTRL register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRFA2SS_PWDCTRL(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA2SS_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RFA2SS_PWDCTRL_SHIFT)) & (ST25DVXXKC_RFA2SS_PWDCTRL_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RFA2SS_PWDCTRL_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RFA2SS_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RFA2SS_RWPROT register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRFA2SS_RWPROT(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA2SS_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RFA2SS_RWPROT_MASK);
    *value = *value >> (ST25DVXXKC_RFA2SS_RWPROT_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RFA2SS_RWPROT register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRFA2SS_RWPROT(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA2SS_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RFA2SS_RWPROT_SHIFT)) & (ST25DVXXKC_RFA2SS_RWPROT_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RFA2SS_RWPROT_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RFA2SS_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RFA2SS_ALL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRFA2SS_ALL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA2SS_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RFA2SS_ALL_MASK);
    *value = *value >> (ST25DVXXKC_RFA2SS_ALL_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RFA2SS_ALL register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRFA2SS_ALL(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA2SS_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RFA2SS_ALL_SHIFT)) & (ST25DVXXKC_RFA2SS_ALL_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RFA2SS_ALL_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RFA2SS_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RFA3SS_PWDCTRL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRFA3SS_PWDCTRL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA3SS_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RFA3SS_PWDCTRL_MASK);
    *value = *value >> (ST25DVXXKC_RFA3SS_PWDCTRL_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RFA3SS_PWDCTRL register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRFA3SS_PWDCTRL(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA3SS_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RFA3SS_PWDCTRL_SHIFT)) & (ST25DVXXKC_RFA3SS_PWDCTRL_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RFA3SS_PWDCTRL_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RFA3SS_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RFA3SS_RWPROT register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRFA3SS_RWPROT(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA3SS_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RFA3SS_RWPROT_MASK);
    *value = *value >> (ST25DVXXKC_RFA3SS_RWPROT_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RFA3SS_RWPROT register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRFA3SS_RWPROT(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA3SS_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RFA3SS_RWPROT_SHIFT)) & (ST25DVXXKC_RFA3SS_RWPROT_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RFA3SS_RWPROT_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RFA3SS_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RFA3SS_ALL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRFA3SS_ALL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA3SS_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RFA3SS_ALL_MASK);
    *value = *value >> (ST25DVXXKC_RFA3SS_ALL_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RFA3SS_ALL register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRFA3SS_ALL(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA3SS_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RFA3SS_ALL_SHIFT)) & (ST25DVXXKC_RFA3SS_ALL_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RFA3SS_ALL_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RFA3SS_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RFA4SS_PWDCTRL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRFA4SS_PWDCTRL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA4SS_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RFA4SS_PWDCTRL_MASK);
    *value = *value >> (ST25DVXXKC_RFA4SS_PWDCTRL_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RFA4SS_PWDCTRL register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRFA4SS_PWDCTRL(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA4SS_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RFA4SS_PWDCTRL_SHIFT)) & (ST25DVXXKC_RFA4SS_PWDCTRL_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RFA4SS_PWDCTRL_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RFA4SS_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RFA4SS_RWPROT register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRFA4SS_RWPROT(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA4SS_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RFA4SS_RWPROT_MASK);
    *value = *value >> (ST25DVXXKC_RFA4SS_RWPROT_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RFA4SS_RWPROT register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRFA4SS_RWPROT(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA4SS_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RFA4SS_RWPROT_SHIFT)) & (ST25DVXXKC_RFA4SS_RWPROT_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RFA4SS_RWPROT_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RFA4SS_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RFA4SS_ALL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRFA4SS_ALL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA4SS_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RFA4SS_ALL_MASK);
    *value = *value >> (ST25DVXXKC_RFA4SS_ALL_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RFA4SS_ALL register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRFA4SS_ALL(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_RFA4SS_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RFA4SS_ALL_SHIFT)) & (ST25DVXXKC_RFA4SS_ALL_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RFA4SS_ALL_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_RFA4SS_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read I2CSS_PZ1 register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetI2CSS_PZ1(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_I2CSS_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_I2CSS_PZ1_MASK);
    *value = *value >> (ST25DVXXKC_I2CSS_PZ1_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write I2CSS_PZ1 register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetI2CSS_PZ1(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_I2CSS_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_I2CSS_PZ1_SHIFT)) & (ST25DVXXKC_I2CSS_PZ1_MASK)) |
                  (reg_value & ~(ST25DVXXKC_I2CSS_PZ1_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_I2CSS_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read I2CSS_PZ2 register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetI2CSS_PZ2(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_I2CSS_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_I2CSS_PZ2_MASK);
    *value = *value >> (ST25DVXXKC_I2CSS_PZ2_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write I2CSS_PZ2 register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetI2CSS_PZ2(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_I2CSS_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_I2CSS_PZ2_SHIFT)) & (ST25DVXXKC_I2CSS_PZ2_MASK)) |
                  (reg_value & ~(ST25DVXXKC_I2CSS_PZ2_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_I2CSS_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read I2CSS_PZ3 register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetI2CSS_PZ3(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_I2CSS_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_I2CSS_PZ3_MASK);
    *value = *value >> (ST25DVXXKC_I2CSS_PZ3_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write I2CSS_PZ3 register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetI2CSS_PZ3(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_I2CSS_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_I2CSS_PZ3_SHIFT)) & (ST25DVXXKC_I2CSS_PZ3_MASK)) |
                  (reg_value & ~(ST25DVXXKC_I2CSS_PZ3_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_I2CSS_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read I2CSS_PZ4 register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetI2CSS_PZ4(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_I2CSS_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_I2CSS_PZ4_MASK);
    *value = *value >> (ST25DVXXKC_I2CSS_PZ4_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write I2CSS_PZ4 register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetI2CSS_PZ4(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_I2CSS_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_I2CSS_PZ4_SHIFT)) & (ST25DVXXKC_I2CSS_PZ4_MASK)) |
                  (reg_value & ~(ST25DVXXKC_I2CSS_PZ4_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_I2CSS_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read I2CSS_ALL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetI2CSS_ALL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_I2CSS_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Write I2CSS_ALL register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetI2CSS_ALL(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  return ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_I2CSS_REG), value, 1);
}

/**
 * @brief  Read LOCKCCFILE_BLCK0 register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetLOCKCCFILE_BLCK0(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_LOCKCCFILE_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_LOCKCCFILE_BLCK0_MASK);
    *value = *value >> (ST25DVXXKC_LOCKCCFILE_BLCK0_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write LOCKCCFILE_BLCK0 register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetLOCKCCFILE_BLCK0(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_LOCKCCFILE_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_LOCKCCFILE_BLCK0_SHIFT)) & (ST25DVXXKC_LOCKCCFILE_BLCK0_MASK)) |
                  (reg_value & ~(ST25DVXXKC_LOCKCCFILE_BLCK0_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_LOCKCCFILE_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read LOCKCCFILE_BLCK1 register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetLOCKCCFILE_BLCK1(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_LOCKCCFILE_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_LOCKCCFILE_BLCK1_MASK);
    *value = *value >> (ST25DVXXKC_LOCKCCFILE_BLCK1_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write LOCKCCFILE_BLCK1 register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetLOCKCCFILE_BLCK1(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_LOCKCCFILE_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_LOCKCCFILE_BLCK1_SHIFT)) & (ST25DVXXKC_LOCKCCFILE_BLCK1_MASK)) |
                  (reg_value & ~(ST25DVXXKC_LOCKCCFILE_BLCK1_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_LOCKCCFILE_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read LOCKCCFILE_ALL register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetLOCKCCFILE_ALL(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_LOCKCCFILE_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_LOCKCCFILE_ALL_MASK);
    *value = *value >> (ST25DVXXKC_LOCKCCFILE_ALL_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write LOCKCCFILE_ALL register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetLOCKCCFILE_ALL(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_LOCKCCFILE_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_LOCKCCFILE_ALL_SHIFT)) & (ST25DVXXKC_LOCKCCFILE_ALL_MASK)) |
                  (reg_value & ~(ST25DVXXKC_LOCKCCFILE_ALL_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_LOCKCCFILE_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read LOCKCFG_B0 register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetLOCKCFG_B0(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_LOCKCFG_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_LOCKCFG_B0_MASK);
    *value = *value >> (ST25DVXXKC_LOCKCFG_B0_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write LOCKCFG_B0 register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetLOCKCFG_B0(const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_LOCKCFG_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_LOCKCFG_B0_SHIFT)) & (ST25DVXXKC_LOCKCFG_B0_MASK)) |
                  (reg_value & ~(ST25DVXXKC_LOCKCFG_B0_MASK));

    ret = ST25DVxxKC_WriteReg(ctx, (ST25DVXXKC_LOCKCFG_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read I2C_SSO_DYN_I2CSSO register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetI2C_SSO_DYN_I2CSSO(const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(ctx, (ST25DVXXKC_I2C_SSO_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_I2C_SSO_DYN_I2CSSO_MASK);
    *value = *value >> (ST25DVXXKC_I2C_SSO_DYN_I2CSSO_SHIFT);
  }
  
  return ret;
}

/**
  * @}
  */

/**
  * @}
  */ 
