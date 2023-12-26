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
int32_t ST25DVxxKC_ReadReg(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, const uint16_t Reg, uint8_t *const Data, const uint16_t len);
int32_t ST25DVxxKC_WriteReg(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, const uint16_t Reg, const uint8_t *const Data, const uint16_t len);
/* Public functions ---------------------------------------------------------*/
/**
 * @brief  Read register from component
 * @param[in] ctx structure containing context driver
 * @param[in] Reg register to read
 * @param[out] Data pointer to store register content
 * @param[out] len length of data
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_ReadReg(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, const uint16_t Reg, uint8_t *const Data, const uint16_t len)
{
  return ctx->ReadReg(dev, ctx->handle, Reg, Data, len);
}

/**
 * @brief  Write register to component
 * @param[in] ctx structure containing context driver
 * @param[in] Reg register to write
 * @param[in] Data data pointer to write to register
 * @param[out] len length of data
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_WriteReg(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, const uint16_t Reg, const uint8_t *const Data, const uint16_t len)
{
  return ctx->WriteReg(dev, ctx->handle, Reg, Data, len);
}

/**
 * @brief  Read IC Ref register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetICREF(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(dev, ctx, (ST25DVXXKC_ICREF_REG), value, 1);
}

/**
 * @brief  Read MEM_SIZE_MSB register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetMEM_SIZE_MSB(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(dev, ctx, (ST25DVXXKC_MEM_SIZE_MSB_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Read BLK_SIZE register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetBLK_SIZE(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(dev, ctx, (ST25DVXXKC_BLK_SIZE_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Read MEM_SIZE_LSB register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetMEM_SIZE_LSB(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  return ST25DVxxKC_ReadReg(dev, ctx, (ST25DVXXKC_MEM_SIZE_LSB_REG), (uint8_t *)value, 1);
}

/**
 * @brief  Read RF_MNGT_RFDIS register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRF_MNGT_RFDIS(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(dev, ctx, (ST25DVXXKC_RF_MNGT_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RF_MNGT_RFDIS_MASK);
    *value = *value >> (ST25DVXXKC_RF_MNGT_RFDIS_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Write RF_MNGT_RFSLEEP register
 * @param[in] ctx structure containing context driver
 * @param[in] value data pointer to write to register
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_SetRF_MNGT_RFSLEEP(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(dev, ctx, (ST25DVXXKC_RF_MNGT_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RF_MNGT_RFSLEEP_SHIFT)) & (ST25DVXXKC_RF_MNGT_RFSLEEP_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RF_MNGT_RFSLEEP_MASK));

    ret = ST25DVxxKC_WriteReg(dev, ctx, (ST25DVXXKC_RF_MNGT_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
 * @brief  Read RF_MNGT_DYN_RFDIS register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRF_MNGT_DYN_RFDIS(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(dev, ctx, (ST25DVXXKC_RF_MNGT_DYN_REG), (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= (ST25DVXXKC_RF_MNGT_DYN_RFDIS_MASK);
    *value = *value >> (ST25DVXXKC_RF_MNGT_DYN_RFDIS_SHIFT);
  }
  
  return ret;
}

/**
 * @brief  Read RF_MNGT_DYN_RFSLEEP register
 * @param[in] ctx structure containing context driver
 * @param[out] value data pointer to store register content
 * @return 0 in case of success, an error code otherwise
 */
int32_t ST25DVxxKC_GetRF_MNGT_DYN_RFSLEEP(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value)
{
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(dev, ctx, (ST25DVXXKC_RF_MNGT_DYN_REG), (uint8_t *)value, 1);
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
int32_t ST25DVxxKC_SetRF_MNGT_DYN_RFSLEEP(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value)
{
  uint8_t reg_value;
  int32_t ret;
  
  ret = ST25DVxxKC_ReadReg(dev, ctx, (ST25DVXXKC_RF_MNGT_DYN_REG), &reg_value, 1);
  if(ret == 0)
  {
    reg_value = ((*value << (ST25DVXXKC_RF_MNGT_DYN_RFSLEEP_SHIFT)) & (ST25DVXXKC_RF_MNGT_DYN_RFSLEEP_MASK)) |
                  (reg_value & ~(ST25DVXXKC_RF_MNGT_DYN_RFSLEEP_MASK));

    ret = ST25DVxxKC_WriteReg(dev, ctx, (ST25DVXXKC_RF_MNGT_DYN_REG), &reg_value, 1);
  }
  
  return ret;
}

/**
  * @}
  */

/**
  * @}
  */ 
