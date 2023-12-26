/**
  ******************************************************************************
  * @file           : st25dvxxkc.c
  * @author         : MMY Ecosystem Team
  * @brief          : This file provides set of driver functions to manage communication 
  *          between BSP and ST25DVxxKC chip.
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
#include "st25dvxxkc.h"
#include <zephyr/drivers/nfc/st25dv.h>
#include <zephyr/kernel.h>

/** @addtogroup BSP
  * @{
  */

/** @defgroup ST25DVxxKC ST25DVxxKC driver
  * @brief    This module implements the functions to drive the ST25DVxxKC NFC dynamic tag.
  * @details  As recommended by the STM32 Cube methodology, this driver provides a standard structure to expose 
  *           the NFC tag standard API.\n It also provides an extended API through its extended driver structure.\n
  *           To be usable on any MCU, this driver calls several IOBus functions.
  *           The IOBus functions are implemented outside this driver, and are in charge of accessing 
  *           the MCU peripherals used for the communication with the tag.
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/** @brief This component driver only supports 1 instance of the component */
#define ST25DVXXKC_MAX_INSTANCE         1


/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static int32_t ReadRegWrap(const struct device *dev, const void *const Handle, const uint16_t Reg, uint8_t *const pData, const uint16_t Length);
static int32_t WriteRegWrap(const struct device *dev, const void *Handle, const uint16_t Reg, const uint8_t *const pData, const uint16_t Length);

int32_t ST25DVxxKC_Init(const struct device *dev, ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_ReadID(const struct device *dev, const ST25DVxxKC_Object_t *const pObj, uint8_t *const pICRef);
int32_t ST25DVxxKC_IsDeviceReady(const struct device *dev, const ST25DVxxKC_Object_t *const pObj, const uint32_t Trials);
int32_t ST25DVxxKC_ReadData(const struct device *dev, const ST25DVxxKC_Object_t *const pObj, uint8_t *const pData, const uint16_t TarAddr, \
                                                                                  const uint16_t NbByte);
int32_t ST25DVxxKC_WriteData(const struct device *dev, const ST25DVxxKC_Object_t *const pObj, const uint8_t *const pData, \
                                                                     const uint16_t TarAddr, const uint16_t NbByte);

/* Global variables ---------------------------------------------------------*/

/**
  * @brief    Standard NFC tag driver API for the ST25DVxxKC.
  * @details  Provides a generic way to access the ST25DVxxKC implementation of the NFC tag standard driver functions.
  */
ST25DVxxKC_Drv_t St25Dvxxkc_Drv =
{
  ST25DVxxKC_Init,
  ST25DVxxKC_ReadID,
  ST25DVxxKC_IsDeviceReady,
  ST25DVxxKC_ReadData,
  ST25DVxxKC_WriteData
};


/* Public functions ---------------------------------------------------------*/

/**
  * @brief  Register Component Bus IO operations.
  * @param[out] pObj pointer to the device structure object.
  * @param[in] pIO pointer to the IO APIs structure.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_RegisterBusIO(const struct device *dev, ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_IO_t *const pIO)
{
  int32_t ret;
  const struct st25dvxxkc_cfg *cfg = (const struct st25dvxxkc_cfg *) dev->config;

  if (pObj == NULL)
  {
    ret = NFCTAG_ERROR;
  }
  else
  {
    pObj->IO.Init           = pIO->Init;
    pObj->IO.DeInit         = pIO->DeInit;
    pObj->IO.Write          = pIO->Write;
    pObj->IO.Read           = pIO->Read;
    pObj->IO.IsReady        = pIO->IsReady;
    pObj->IO.GetTick        = pIO->GetTick;
    pObj->IO.DeviceAddress  = cfg->i2c.addr;

    pObj->Ctx.ReadReg  = ReadRegWrap;
    pObj->Ctx.WriteReg = WriteRegWrap;
    pObj->Ctx.handle   = pObj;

    if (pObj->IO.Init == NULL)
    {
      ret = NFCTAG_ERROR;
    }
    else
    {
      ret = (pObj->IO.Init(dev) == 0) ? NFCTAG_OK : NFCTAG_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  ST25DVxxKC nfctag Initialization.
  * @param[in,out] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_Init(const struct device *dev, ST25DVxxKC_Object_t *const pObj)
{
  int32_t ret = NFCTAG_OK;
  
  if (pObj->IsInitialized == 0U)
  {
    uint8_t nfctag_id;
    ret = ST25DVxxKC_ReadID(dev, pObj,&nfctag_id);
    if (ret == NFCTAG_OK)
    {
        if((nfctag_id != I_AM_ST25DV04KC) && (nfctag_id != I_AM_ST25DV64KC))
        {
          ret = NFCTAG_ERROR;
        }
    }
  }

  if (ret == NFCTAG_OK)
  {
    pObj->IsInitialized = 1U;
  }
  
  return ret;
}


/**
  * @brief  Reads the ST25DVxxKC ID.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pICRef Pointer on an uint8_t used to return the ST25DVxxKC ID.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadID(const struct device *dev, const ST25DVxxKC_Object_t *const pObj, uint8_t *const pICRef)
{
  /* Read ICRef on device */
  return ST25DVxxKC_GetICREF(dev, &(pObj->Ctx), pICRef);
}

/**
  * @brief    Checks the ST25DVxxKC availability.
  * @details  The ST25DVxxKC I2C is NACKed when a RF communication is on-going.
  *           This function determines if the ST25DVxxKC is ready to answer an I2C request.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] Trials Max number of tentative.
  * @retval int32_t enum status.
  */
int32_t ST25DVxxKC_IsDeviceReady(const struct device *dev, const ST25DVxxKC_Object_t *const pObj, const uint32_t Trials)
{
  /* Test communication with device */
  return pObj->IO.IsReady(dev, pObj->IO.DeviceAddress, Trials);
}

/**
  * @brief  Reads N bytes of Data, starting from the specified I2C address.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pData   Pointer used to return the read data.
  * @param[in] TarAddr I2C data memory address to read.
  * @param[in] NbByte  Number of bytes to be read.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadData(const struct device *dev, const ST25DVxxKC_Object_t *const pObj, uint8_t *const pData, const uint16_t TarAddr, \
                                                                                  const uint16_t NbByte)
{
  /* Read Data in user memory */
  return pObj->IO.Read(dev, pObj->IO.DeviceAddress, TarAddr, pData, NbByte);
}

/**
  * @brief  Writes N bytes of Data starting from the specified I2C Address.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] pData   Pointer on the data to be written.
  * @param[in] TarAddr I2C data memory address to be written.
  * @param[in] NbByte  Number of bytes to be written.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_WriteData(const struct device *dev, const ST25DVxxKC_Object_t *const pObj, const uint8_t *const pData, \
                                                                        const uint16_t TarAddr, const uint16_t NbByte)
{
  int32_t ret;
  uint16_t split_data_nb;
  const uint8_t *pdata_index = (const uint8_t *)pData;
  uint16_t bytes_to_write = NbByte;
  uint16_t mem_addr = TarAddr;
  
  /* ST25DVxxKC can write a maximum of 256 bytes in EEPROM per i2c communication */
  do
  {
    /* Split write if data to write is superior of max write bytes for ST25DVxxKC */
    if(bytes_to_write > ST25DVXXKC_MAX_WRITE_BYTE)
    {
      /* DataSize higher than max page write, copy data by page */
      split_data_nb = ST25DVXXKC_MAX_WRITE_BYTE;
    }
    else
    {
      /* DataSize lower or equal to max page write, copy only last bytes */
      split_data_nb = bytes_to_write;
    }
    /* Write split_data_nb bytes in memory */
    ret = pObj->IO.Write(dev, pObj->IO.DeviceAddress, mem_addr, pdata_index, split_data_nb);
    if(ret == NFCTAG_OK)
    {
      /* Sleep until EEPROM is available */
      k_sleep(K_MSEC(5U + 5U*split_data_nb/16));
      int32_t pollstatus = pObj->IO.IsReady(dev, pObj->IO.DeviceAddress, 1);
      if(pollstatus != NFCTAG_OK)
      {
        ret = NFCTAG_TIMEOUT;
      }
    }

    /* update index, dest address, size for next write */
    pdata_index = &pdata_index[split_data_nb];
    mem_addr += split_data_nb;
    bytes_to_write -= split_data_nb;
  }
  while((bytes_to_write > 0U) && (ret == NFCTAG_OK));
  
  return ret;
}

/**
  * @brief  Reads N bytes from Registers, starting at the specified I2C address.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pData Pointer used to return the read data.
  * @param[in] TarAddr I2C memory address to be read.
  * @param[in] NbByte  Number of bytes to be read.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadRegister(const struct device *dev, const ST25DVxxKC_Object_t *const pObj, uint8_t * pData, const uint16_t TarAddr, \
                                                                                      const uint16_t NbByte)
{
  /* Read Data in system memory */
  return pObj->IO.Read(dev, pObj->IO.DeviceAddress | ST25DVXXKC_ADDR_SYSTEMMEMORY_BIT_I2C | ST25DVXXKC_ADDR_MODE_BIT_I2C, TarAddr, pData, NbByte);
}

/**
  * @brief    Writes N bytes to the specified register.
  * @details  Needs the I2C Password presentation to be effective.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] pData   Pointer on the data to be written.
  * @param[in] TarAddr I2C register address to be written.
  * @param[in] NbByte  Number of bytes to be written.
  * @return   int32_t enum status.
  */
int32_t ST25DVxxKC_WriteRegister(const struct device *dev, ST25DVxxKC_Object_t *const pObj, const uint8_t *const pData, const uint16_t TarAddr, const uint16_t NbByte)
{ 
  int32_t ret;
  uint16_t split_data_nb;
  uint16_t bytes_to_write = NbByte;
  uint16_t mem_addr = TarAddr;
  const uint8_t *pdata_index = (const uint8_t *)pData;
  
  /* ST25DVxxKC can write a maximum of 256 bytes in EEPROM per i2c communication */
  do
  {
    /* Split write if data to write is superior of max write bytes for ST25DVxxKC */
    if(bytes_to_write > ST25DVXXKC_MAX_WRITE_BYTE)
    {
      /* DataSize higher than max page write, copy data by page */
      split_data_nb = ST25DVXXKC_MAX_WRITE_BYTE;
    }
    else
    {
      /* DataSize lower or equal to max page write, copy only last bytes */
      split_data_nb = bytes_to_write;
    }
    /* Write split_data_nb bytes in register */
    ret = pObj->IO.Write(dev, pObj->IO.DeviceAddress | ST25DVXXKC_ADDR_SYSTEMMEMORY_BIT_I2C | ST25DVXXKC_ADDR_MODE_BIT_I2C, mem_addr, pdata_index, split_data_nb);
    if(ret == NFCTAG_OK)
    {
      int32_t pollstatus;
      /* Poll until EEPROM is available */
      uint32_t tickstart = pObj->IO.GetTick();

      // Special case for ST25DVXXKC_I2CCFG_REG: align IO.DeviceAddress with register content
      if ((mem_addr <= ST25DVXXKC_I2CCFG_REG) && (mem_addr + split_data_nb >= ST25DVXXKC_I2CCFG_REG)) {
        uint8_t deviceCode = (pdata_index[ST25DVXXKC_I2CCFG_REG - mem_addr] & ST25DVXXKC_I2CCFG_DEVICECODE_MASK) >> ST25DVXXKC_I2CCFG_DEVICECODE_SHIFT;
        uint8_t E0 = (pdata_index[ST25DVXXKC_I2CCFG_REG - mem_addr] & ST25DVXXKC_I2CCFG_E0_MASK) >> ST25DVXXKC_I2CCFG_E0_SHIFT;

        pObj->IO.DeviceAddress = (pObj->IO.DeviceAddress & ~(ST25DVXXKC_ADDR_DEVICECODE_MASK)) | ((deviceCode << ST25DVXXKC_ADDR_DEVICECODE_SHIFT) & ST25DVXXKC_ADDR_DEVICECODE_MASK);
        pObj->IO.DeviceAddress = (pObj->IO.DeviceAddress & ~(ST25DVXXKC_ADDR_E0_MASK)) | ((E0 << ST25DVXXKC_ADDR_E0_SHIFT) & ST25DVXXKC_ADDR_E0_MASK);
      }

      /* Wait until ST25DVxxKC is ready or timeout occurs */
      do
      {
        pollstatus = pObj->IO.IsReady(dev, pObj->IO.DeviceAddress, 1);
      } while(   ((uint32_t)pObj->IO.GetTick() < tickstart + ST25DVXXKC_WRITE_TIMEOUT)
              && (pollstatus != NFCTAG_OK) );
      
      if(pollstatus != NFCTAG_OK)
      {
        ret = NFCTAG_TIMEOUT;
      }
    }

    /* update index, dest address, size for next write */
    pdata_index = &pdata_index[split_data_nb];
    mem_addr += split_data_nb;
    bytes_to_write -= split_data_nb;
  }
  while((bytes_to_write > 0U) && (ret == NFCTAG_OK));
  
  return ret;
}

/**
  * @brief  Reads the ST25DVxxKC Memory Size.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pSizeInfo Pointer on a ST25DVxxKC_MEM_SIZE structure used to return the Memory size information.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadMemSize(const struct device *dev, const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_MEM_SIZE_t *const pSizeInfo)
{
  uint8_t memsize_msb;
  uint8_t memsize_lsb;
  int32_t status;
  
  pSizeInfo->Mem_Size = 0;

  /* Read actual value of MEM_SIZE register */
  status = ST25DVxxKC_GetMEM_SIZE_LSB(dev, &(pObj->Ctx), &memsize_lsb);
  if(status == NFCTAG_OK)
  {
    status = ST25DVxxKC_GetMEM_SIZE_MSB(dev, &(pObj->Ctx), &memsize_msb);
    if(status == NFCTAG_OK)
    {
      status = ST25DVxxKC_GetBLK_SIZE(dev, &(pObj->Ctx), &(pSizeInfo->BlockSize));
      if(status == NFCTAG_OK)
      {
        /* Extract Memory information */
        pSizeInfo->Mem_Size = memsize_msb;
        pSizeInfo->Mem_Size = (pSizeInfo->Mem_Size << 8) |memsize_lsb;
      }
    }
  }
  return status;
}

/**
  * @brief  Reads the RFSleep dynamic register information.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pRFSleep Pointer on a ST25DVxxKC_EN_STATUS values used to return the RF Sleep state.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_GetRFSleep_Dyn(const struct device *dev, const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pRFSleep)
{
  int32_t status;
  uint8_t reg_value = 0;
  
  *pRFSleep = ST25DVXXKC_DISABLE;

  /* Read actual value of RF_MNGT_DYN register */
  status = ST25DVxxKC_GetRF_MNGT_DYN_RFSLEEP(dev, &(pObj->Ctx), &reg_value);
  
  /* Extract RFSleep information */
  if(status == NFCTAG_OK)
  {
    if(reg_value != 0U)
    {
      *pRFSleep = ST25DVXXKC_ENABLE;
    }
    else
    {
      *pRFSleep = ST25DVXXKC_DISABLE;
    }
  }
  
  return status;
}

/**
  * @brief  Sets the RF Sleep dynamic configuration.
  * @param[in] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_SetRFSleep_Dyn(const struct device *dev, const ST25DVxxKC_Object_t *const pObj)
{
  uint8_t reg_value = 1U;
  
  return ST25DVxxKC_SetRF_MNGT_DYN_RFSLEEP(dev, &(pObj->Ctx), &reg_value);
}

/**
  * @brief  Unsets the RF Sleep dynamic configuration.
  * @param[in] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ResetRFSleep_Dyn(const struct device *dev, const ST25DVxxKC_Object_t *const pObj)
{
  uint8_t reg_value = 0U;
  
  return ST25DVxxKC_SetRF_MNGT_DYN_RFSLEEP(dev, &(pObj->Ctx), &reg_value);
}

/**
  * @brief Reads register on the ST25DVxxKC device.
  * @param[in] handle pointer to the context device structure object.
  * @param[in] Reg register address to read.
  * @param[out] pData pointer used to return the read data.
  * @param[in] len length of the data to be read.
  * @return int32_t enum status.
  */
static int32_t ReadRegWrap(const struct device *dev, const void *const handle, const uint16_t Reg, uint8_t *const pData, const uint16_t len)
{
  int32_t ret;
  ST25DVxxKC_Object_t *pObj = (ST25DVxxKC_Object_t *)handle;
  
  if((Reg & (ST25DVXXKC_IS_DYNAMIC_REGISTER)) == ST25DVXXKC_IS_DYNAMIC_REGISTER)
  {
    ret = pObj->IO.Read(dev, pObj->IO.DeviceAddress, Reg, pData, len);
  }
  else
  {
    ret = pObj->IO.Read(dev, pObj->IO.DeviceAddress | ST25DVXXKC_ADDR_SYSTEMMEMORY_BIT_I2C | ST25DVXXKC_ADDR_MODE_BIT_I2C, Reg, pData, len);
  }
  
  return ret;
}

/**
  * @brief  Writes register on the ST25DVxxKC device.
  * @param[in] handle pointer to the context device structure object.
  * @param[in] Reg register address to write.
  * @param[in] pData pointer used to store data to write.
  * @param[in] len length of the data to be written.
  * @return int32_t enum status.
  */
static int32_t WriteRegWrap(const struct device *dev, const void *const handle, const uint16_t Reg, const uint8_t *const pData, \
                                                                const uint16_t len)
{
  int32_t ret;
  ST25DVxxKC_Object_t *pObj = (ST25DVxxKC_Object_t *)handle;
  
  if((Reg & (ST25DVXXKC_IS_DYNAMIC_REGISTER)) == ST25DVXXKC_IS_DYNAMIC_REGISTER)
  {
    ret = pObj->IO.Write(dev, pObj->IO.DeviceAddress, Reg, pData, len);
  }
  else
  {
    ret = pObj->IO.Write(dev, pObj->IO.DeviceAddress | ST25DVXXKC_ADDR_SYSTEMMEMORY_BIT_I2C | ST25DVXXKC_ADDR_MODE_BIT_I2C, Reg, pData, len);
    if(ret == NFCTAG_OK)
    {
      int32_t pollstatus;
      /* Poll until EEPROM is available */
      uint32_t tickstart = pObj->IO.GetTick();

      // Special case for ST25DVXXKC_I2CCFG_REG: align IO.DeviceAddress with register content
      if (Reg == ST25DVXXKC_I2CCFG_REG) {
        uint8_t deviceCode = (*pData & ST25DVXXKC_I2CCFG_DEVICECODE_MASK) >> ST25DVXXKC_I2CCFG_DEVICECODE_SHIFT;
        uint8_t E0 = (*pData & ST25DVXXKC_I2CCFG_E0_MASK) >> ST25DVXXKC_I2CCFG_E0_SHIFT;

        pObj->IO.DeviceAddress = (pObj->IO.DeviceAddress & ~(ST25DVXXKC_ADDR_DEVICECODE_MASK)) | ((deviceCode << ST25DVXXKC_ADDR_DEVICECODE_SHIFT) & ST25DVXXKC_ADDR_DEVICECODE_MASK);
        pObj->IO.DeviceAddress = (pObj->IO.DeviceAddress & ~(ST25DVXXKC_ADDR_E0_MASK)) | ((E0 << ST25DVXXKC_ADDR_E0_SHIFT) & ST25DVXXKC_ADDR_E0_MASK);
      }

      /* Wait until ST25DVxxKC is ready or timeout occurs */
      do
      {
        pollstatus = pObj->IO.IsReady(dev, pObj->IO.DeviceAddress | ST25DVXXKC_ADDR_SYSTEMMEMORY_BIT_I2C | ST25DVXXKC_ADDR_MODE_BIT_I2C, 1);
      } while(   ((uint32_t)pObj->IO.GetTick() < tickstart + ST25DVXXKC_WRITE_TIMEOUT)
              && (pollstatus != NFCTAG_OK) );
    
      if(pollstatus != NFCTAG_OK)
      {
        ret = NFCTAG_TIMEOUT;
      }
    }
  }

  return ret;
}

/**
 * @}
 */

/**
 * @}
 */

