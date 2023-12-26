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
static int32_t ReadRegWrap(const void *const Handle, const uint16_t Reg, uint8_t *const pData, const uint16_t Length);
static int32_t WriteRegWrap(const void *Handle, const uint16_t Reg, const uint8_t *const pData, const uint16_t Length);

int32_t ST25DVxxKC_Init(ST25DVxxKC_Object_t *const pObj);
int32_t ST25DVxxKC_ReadID(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pICRef);
int32_t ST25DVxxKC_IsDeviceReady(const ST25DVxxKC_Object_t *const pObj, const uint32_t Trials);
int32_t ST25DVxxKC_GetGPOStatus(const ST25DVxxKC_Object_t *const pObj, uint16_t *const pGPOStatus);
int32_t ST25DVxxKC_ConfigureGPO(const ST25DVxxKC_Object_t *const pObj, const uint16_t ITConf);
int32_t ST25DVxxKC_ReadData(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pData, const uint16_t TarAddr, \
                                                                                  const uint16_t NbByte);
int32_t ST25DVxxKC_WriteData(const ST25DVxxKC_Object_t *const pObj, const uint8_t *const pData, \
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
  ST25DVxxKC_GetGPOStatus,
  ST25DVxxKC_ConfigureGPO,
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
int32_t ST25DVxxKC_RegisterBusIO(ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_IO_t *const pIO)
{
  int32_t ret;

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
    pObj->IO.DeviceAddress  = ST25DVXXKC_ADDR_DATA_I2C;

    pObj->Ctx.ReadReg  = ReadRegWrap;
    pObj->Ctx.WriteReg = WriteRegWrap;
    pObj->Ctx.handle   = pObj;

    if (pObj->IO.Init == NULL)
    {
      ret = NFCTAG_ERROR;
    }
    else
    {
      ret = (pObj->IO.Init() == 0) ? NFCTAG_OK : NFCTAG_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  ST25DVxxKC nfctag Initialization.
  * @param[in,out] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_Init(ST25DVxxKC_Object_t *const pObj)
{
  int32_t ret = NFCTAG_OK;
  
  if (pObj->IsInitialized == 0U)
  {
    uint8_t nfctag_id;
    ret = ST25DVxxKC_ReadID(pObj,&nfctag_id);
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
int32_t ST25DVxxKC_ReadID(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pICRef)
{
  /* Read ICRef on device */
  return ST25DVxxKC_GetICREF(&(pObj->Ctx), pICRef);
}

/**
  * @brief  Reads the ST25DVxxKC IC Revision.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pICRev Pointer on the uint8_t used to return the ST25DVxxKC IC Revision number.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadICRev(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pICRev)
{
  /* Read ICRev on device */
  return ST25DVxxKC_GetICREV(&(pObj->Ctx), pICRev);
}

/**
  * @brief    Checks the ST25DVxxKC availability.
  * @details  The ST25DVxxKC I2C is NACKed when a RF communication is on-going.
  *           This function determines if the ST25DVxxKC is ready to answer an I2C request.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] Trials Max number of tentative.
  * @retval int32_t enum status.
  */
int32_t ST25DVxxKC_IsDeviceReady(const ST25DVxxKC_Object_t *const pObj, const uint32_t Trials)
{
  /* Test communication with device */
  return pObj->IO.IsReady(pObj->IO.DeviceAddress, Trials);
}

/**
  * @brief  Reads the ST25DVxxKC GPO configuration.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pGPOStatus  Pointer on a uint16_t used to return the current GPO consiguration, as:
  *                     - RFUSERSTATE = 0x0001
  *                     - RFBUSY = 0x0002
  *                     - RFINTERRUPT = 0x0004
  *                     - FIELDFALLING = 0x0008
  *                     - FIELDRISING = 0x0010
  *                     - RFPUTMSG = 0x0020
  *                     - RFGETMSG = 0x0040
  *                     - RFWRITE = 0x0080
  *                     - I2CWRITEENABLE = 0x0100
  *                     - RFOFF = 0x0200
  *
  * @retval int32_t enum status.
  */
int32_t ST25DVxxKC_GetGPOStatus(const ST25DVxxKC_Object_t *const pObj, uint16_t *const pGPOStatus)
{
  uint8_t reg_value;
  int32_t status;

  *pGPOStatus = 0x0000;
  
  /* Read value of GPO register */
  status = ST25DVxxKC_GetGPO2_ALL(&(pObj->Ctx), &reg_value);
  if(status == NFCTAG_OK)
  {
    /* Extract GPO configuration */
    *pGPOStatus = ((uint16_t)0x0003 & (uint16_t)reg_value) << 8;
    
    /* Read value of GPO register */
    status = ST25DVxxKC_GetGPO1_ALL(&(pObj->Ctx), &reg_value);
    if(status == NFCTAG_OK)
    {
      /* Extract GPO configuration */
      *pGPOStatus |= ((uint16_t)0x00FF & (uint16_t)reg_value);
    }
  }
  return status;
}

/**
  * @brief    Configures the ST25DVxxKC GPO.
  * @details  Needs the I2C Password presentation to be effective.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] ITConf Provides the GPO configuration to apply:
  *           - RFUSERSTATE = 0x01
  *           - RFBUSY = 0x02
  *           - RFINTERRUPT = 0x04
  *           - FIELDFALLING = 0x08
  *           - FIELDRISING = 0x10
  *           - RFPUTMSG = 0x20
  *           - RFGETMSG = 0x40
  *           - RFWRITE = 0x80
  *
  * @retval int32_t enum status.
  */
int32_t ST25DVxxKC_ConfigureGPO(const ST25DVxxKC_Object_t *const pObj, const uint16_t ITConf)
{
  uint8_t reg_value;
  int32_t status;
  
  /* Write GPO1 configuration to register */
  status = ST25DVxxKC_SetGPO1_ALL(&(pObj->Ctx), (const uint8_t *)&ITConf);
  if(status == NFCTAG_OK)
  {
    status = ST25DVxxKC_GetGPO2_ALL(&(pObj->Ctx), &reg_value);
    if(status == NFCTAG_OK)
    {
      /* Write GPO2 configuration to register */
      reg_value &= (uint8_t)0xFC;
      reg_value |= (uint8_t)(ITConf >> 8) & ST25DVXXKC_GPO2_I2CWRITEENABLE_MASK;
      reg_value |= (uint8_t)(ITConf >> 8) & ST25DVXXKC_GPO2_RFOFF_MASK;
      status = ST25DVxxKC_SetGPO2_ALL(&(pObj->Ctx), &reg_value);
    }
  }
  
  return status;
}

/**
  * @brief  Reads the ST25DVxxKC ITtime duration for the GPO pulses.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pITtime Pointer used to return the coefficient for the GPO Pulse duration
  *         (Pulse duration = 302,06 us - ITtime * 512 / fc).
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadITPulse(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_PULSE_DURATION_E *const pITtime)
{
  ST25DVxxKC_PULSE_DURATION_E reg_value;
  int32_t status;
  
  *pITtime = ST25DVXXKC_302_US;

  /* Read ITtime register value */
  status = ST25DVxxKC_GetITTIME_DELAY(&(pObj->Ctx), (uint8_t *)&reg_value);
  if(status == NFCTAG_OK)
  {
    /* Extract delay coefficient value */
    *pITtime = reg_value;
  }
  
  return status;
}

/**
  * @brief    Configures the ST25DVxxKC ITtime duration for the GPO pulse.
  * @details  Needs the I2C Password presentation to be effective.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] ITtime Coefficient for the Pulse duration to be written (Pulse duration = 302,06 us - ITtime * 512 / fc)
  * @retval int32_t enum status.
  */
int32_t ST25DVxxKC_WriteITPulse(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_PULSE_DURATION_E ITtime)
{
  uint8_t reg_value;
  
  /* prepare data to write */
  reg_value = (uint8_t)ITtime;
  
  /* Write value for ITtime register */
  return ST25DVxxKC_SetITTIME_DELAY(&(pObj->Ctx), &reg_value);
}

/**
  * @brief  Reads N bytes of Data, starting from the specified I2C address.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pData   Pointer used to return the read data.
  * @param[in] TarAddr I2C data memory address to read.
  * @param[in] NbByte  Number of bytes to be read.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadData(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pData, const uint16_t TarAddr, \
                                                                                  const uint16_t NbByte)
{
  /* Read Data in user memory */
  return pObj->IO.Read(pObj->IO.DeviceAddress, TarAddr, pData, NbByte);
}

/**
  * @brief  Writes N bytes of Data starting from the specified I2C Address.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] pData   Pointer on the data to be written.
  * @param[in] TarAddr I2C data memory address to be written.
  * @param[in] NbByte  Number of bytes to be written.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_WriteData(const ST25DVxxKC_Object_t *const pObj, const uint8_t *const pData, \
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
    ret = pObj->IO.Write(pObj->IO.DeviceAddress, mem_addr, pdata_index, split_data_nb);
    if(ret == NFCTAG_OK)
    {
      int32_t pollstatus;
      /* Poll until EEPROM is available */
      uint32_t tickstart = pObj->IO.GetTick();
      /* Wait until ST25DVxxKC is ready or timeout occurs */
      do
      {
        pollstatus = pObj->IO.IsReady(pObj->IO.DeviceAddress, 1);
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
  * @brief  Reads N bytes from Registers, starting at the specified I2C address.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pData Pointer used to return the read data.
  * @param[in] TarAddr I2C memory address to be read.
  * @param[in] NbByte  Number of bytes to be read.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadRegister(const ST25DVxxKC_Object_t *const pObj, uint8_t * pData, const uint16_t TarAddr, \
                                                                                      const uint16_t NbByte)
{
  /* Read Data in system memory */
  return pObj->IO.Read(pObj->IO.DeviceAddress | ST25DVXXKC_ADDR_SYSTEMMEMORY_BIT_I2C | ST25DVXXKC_ADDR_MODE_BIT_I2C, TarAddr, pData, NbByte);
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
int32_t ST25DVxxKC_WriteRegister(ST25DVxxKC_Object_t *const pObj, const uint8_t *const pData, const uint16_t TarAddr, const uint16_t NbByte)
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
    ret = pObj->IO.Write(pObj->IO.DeviceAddress | ST25DVXXKC_ADDR_SYSTEMMEMORY_BIT_I2C | ST25DVXXKC_ADDR_MODE_BIT_I2C, mem_addr, pdata_index, split_data_nb);
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
        pollstatus = pObj->IO.IsReady(pObj->IO.DeviceAddress, 1);
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
  * @brief  Reads the ST25DVxxKC UID.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pUid Pointer used to return the ST25DVxxKC UID value.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadUID(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_UID_t *const pUid)
{
  uint8_t reg_value[8];
  uint8_t i;
  int32_t status;

  pUid->MsbUid = 0x00000000;
  pUid->LsbUid = 0x00000000;
  
  /* Read value of UID registers */
  status = ST25DVxxKC_GetUID(&(pObj->Ctx), reg_value);
  if(status == NFCTAG_OK)
  {
    /* Store information in 2 WORD */
    pUid->MsbUid = 0;
    
    for(i = 0U; i < 4U; i++)
    {
      pUid->MsbUid = (pUid->MsbUid << 8U) | reg_value[7U - i];
    }
    
    pUid->LsbUid = 0U;
    
    for(i = 0U; i < 4U; i++)
    {
      pUid->LsbUid = (pUid->LsbUid << 8U) | reg_value[3U - i];
    }
  }
  return status;
}

/**
  * @brief  Reads the ST25DVxxKC DSFID.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pDsfid Pointer used to return the ST25DVxxKC DSFID value.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadDSFID(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pDsfid)
{
  /* Read DSFID register */
  return ST25DVxxKC_GetDSFID(&(pObj->Ctx), pDsfid);
}

/**
  * @brief  Reads the ST25DVxxKC DSFID RF Lock state.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pLockDsfid Pointer on a ST25DVxxKC_LOCK_STATUS used to return the DSFID lock state.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadDsfidRFProtection(const ST25DVxxKC_Object_t *const pObj, \
                                                          ST25DVxxKC_LOCK_STATUS_E *const pLockDsfid)
{
  uint8_t reg_value;
  int32_t status;

  *pLockDsfid = ST25DVXXKC_LOCKED;
  
  /* Read register */
  status = ST25DVxxKC_GetLOCKDSFID(&(pObj->Ctx), &reg_value);
  if(status == NFCTAG_OK)
  {
    /* Extract Lock Status */
    if(reg_value == 0U)
    {
      *pLockDsfid = ST25DVXXKC_UNLOCKED;
    }
    else
    {
      *pLockDsfid = ST25DVXXKC_LOCKED;
    }
  }
  return status;
}

/**
  * @brief  Reads the ST25DVxxKC AFI.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pAfi Pointer used to return the ST25DVxxKC AFI value.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadAFI(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pAfi)
{
  /* Read AFI register */
  return ST25DVxxKC_GetAFI(&(pObj->Ctx), pAfi);
}

/**
  * @brief  Reads the AFI RF Lock state.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pLockAfi Pointer on a ST25DVxxKC_LOCK_STATUS used to return the ASFID lock state.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadAfiRFProtection(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_LOCK_STATUS_E *const pLockAfi)
{
  uint8_t reg_value;
  int32_t status;

  *pLockAfi = ST25DVXXKC_LOCKED;
  
  /* Read register */
  status = ST25DVxxKC_GetLOCKAFI(&(pObj->Ctx), &reg_value);
  if(status == NFCTAG_OK)
  {
    /* Extract Lock Status */
    if(reg_value == 0U)
    {
      *pLockAfi = ST25DVXXKC_UNLOCKED;
    }
    else
    {
      *pLockAfi = ST25DVXXKC_LOCKED;
    }
  }
  return status;
}

/**
  * @brief  Reads the I2C Protected Area state.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pProtZone Pointer on a ST25DVxxKC_I2C_PROT_ZONE structure used to return the Protected Area state.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadI2CProtectZone(const ST25DVxxKC_Object_t *const pObj, \
                                                            ST25DVxxKC_I2C_PROT_ZONE_t *const pProtZone)
{
  uint8_t reg_value;
  int32_t status;

  pProtZone->ProtectZone1 = ST25DVXXKC_NO_PROT;
  pProtZone->ProtectZone2 = ST25DVXXKC_NO_PROT;
  pProtZone->ProtectZone3 = ST25DVXXKC_NO_PROT;
  pProtZone->ProtectZone4 = ST25DVXXKC_NO_PROT;
  
  /* Read value of I2c Protected Zone register */
  status = ST25DVxxKC_GetI2CSS_ALL(&(pObj->Ctx), &reg_value);
  if(status == NFCTAG_OK)
  {
    /* Dispatch information to corresponding struct member */
    switch((reg_value & ST25DVXXKC_I2CSS_PZ1_MASK) >> ST25DVXXKC_I2CSS_PZ1_SHIFT)
    {
      case ST25DVXXKC_NO_PROT:
        pProtZone->ProtectZone1 = ST25DVXXKC_NO_PROT;
        break;
        
      case ST25DVXXKC_WRITE_PROT:
        pProtZone->ProtectZone1 = ST25DVXXKC_WRITE_PROT;
        break;
        
      case ST25DVXXKC_READ_PROT:
        pProtZone->ProtectZone1 = ST25DVXXKC_READ_PROT;
        break;
        
      case ST25DVXXKC_READWRITE_PROT:
        pProtZone->ProtectZone1 = ST25DVXXKC_READWRITE_PROT;
        break;
        
      default:
        status = NFCTAG_ERROR;
        break;
    }
    
    switch((reg_value & ST25DVXXKC_I2CSS_PZ2_MASK) >> ST25DVXXKC_I2CSS_PZ2_SHIFT)
    {
      case ST25DVXXKC_NO_PROT:
        pProtZone->ProtectZone2 = ST25DVXXKC_NO_PROT;
        break;
        
      case ST25DVXXKC_WRITE_PROT:
        pProtZone->ProtectZone2 = ST25DVXXKC_WRITE_PROT;
        break;
        
      case ST25DVXXKC_READ_PROT:
        pProtZone->ProtectZone2 = ST25DVXXKC_READ_PROT;
        break;
        
      case ST25DVXXKC_READWRITE_PROT:
        pProtZone->ProtectZone2 = ST25DVXXKC_READWRITE_PROT;
        break;
        
      default:
        status = NFCTAG_ERROR;
        break;
    }

    switch((reg_value & ST25DVXXKC_I2CSS_PZ3_MASK) >> ST25DVXXKC_I2CSS_PZ3_SHIFT)
    {
      case ST25DVXXKC_NO_PROT:
        pProtZone->ProtectZone3 = ST25DVXXKC_NO_PROT;
        break;
        
      case ST25DVXXKC_WRITE_PROT:
        pProtZone->ProtectZone3 = ST25DVXXKC_WRITE_PROT;
        break;
        
      case ST25DVXXKC_READ_PROT:
        pProtZone->ProtectZone3 = ST25DVXXKC_READ_PROT;
        break;
        
      case ST25DVXXKC_READWRITE_PROT:
        pProtZone->ProtectZone3 = ST25DVXXKC_READWRITE_PROT;
        break;
        
      default:
        status = NFCTAG_ERROR;
        break;
    }

    switch((reg_value & ST25DVXXKC_I2CSS_PZ4_MASK) >> ST25DVXXKC_I2CSS_PZ4_SHIFT)
    {
      case ST25DVXXKC_NO_PROT:
        pProtZone->ProtectZone4 = ST25DVXXKC_NO_PROT;
        break;
        
      case ST25DVXXKC_WRITE_PROT:
        pProtZone->ProtectZone4 = ST25DVXXKC_WRITE_PROT;
        break;
        
      case ST25DVXXKC_READ_PROT:
        pProtZone->ProtectZone4 = ST25DVXXKC_READ_PROT;
        break;
        
      case ST25DVXXKC_READWRITE_PROT:
        pProtZone->ProtectZone4 = ST25DVXXKC_READWRITE_PROT;
        break;
        
      default:
        status = NFCTAG_ERROR;
        break;
    }
  }
  return status;
}

/**
  * @brief    Sets the I2C write-protected state to an EEPROM Area.
  * @details  Needs the I2C Password presentation to be effective.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] Zone                ST25DVxxKC_PROTECTION_ZONE value coresponding to the area to protect.
  * @param[in] ReadWriteProtection ST25DVxxKC_PROTECTION_CONF value corresponding to the protection to be set.
  * @return   int32_t enum status.
  */
int32_t ST25DVxxKC_WriteI2CProtectZonex(const ST25DVxxKC_Object_t *const pObj, \
                     const ST25DVxxKC_PROTECTION_ZONE_E Zone, const ST25DVxxKC_PROTECTION_CONF_E ReadWriteProtection)
{
  int32_t status;
  uint8_t reg_value;
  
  /* Compute and update new i2c Zone Security Status */
  switch(Zone)
  {
    case ST25DVXXKC_PROT_ZONE1:
      /* Read protection is not allowed for Zone 1 */
      reg_value = ((uint8_t)ReadWriteProtection & (uint8_t)0x01);
      status = ST25DVxxKC_SetI2CSS_PZ1(&(pObj->Ctx), &reg_value);
      break;
    case ST25DVXXKC_PROT_ZONE2:
      reg_value = (uint8_t)ReadWriteProtection;
      status = ST25DVxxKC_SetI2CSS_PZ2(&(pObj->Ctx), &reg_value);
      break;
    case ST25DVXXKC_PROT_ZONE3:
      reg_value = (uint8_t)ReadWriteProtection;
      status = ST25DVxxKC_SetI2CSS_PZ3(&(pObj->Ctx), &reg_value);
      break;
    case ST25DVXXKC_PROT_ZONE4:
      reg_value = (uint8_t)ReadWriteProtection;
      status = ST25DVxxKC_SetI2CSS_PZ4(&(pObj->Ctx), &reg_value);
      break;
    
    default:
      status = NFCTAG_ERROR;
      break;
  }
  
  /* Write I2CZSS register */
  return status;
}

/**
  * @brief  Reads the CCile protection state.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pLockCCFile Pointer on a ST25DVxxKC_LOCK_CCFILE value corresponding to the lock state of the CCFile.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadLockCCFile(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_LOCK_CCFILE_t *const pLockCCFile)
{
  uint8_t reg_value;
  int32_t status;
  
  pLockCCFile->LckBck0 = ST25DVXXKC_UNLOCKED;
  pLockCCFile->LckBck1 = ST25DVXXKC_UNLOCKED;

  /* Get actual LOCKCCFILE register value */
  status = ST25DVxxKC_GetLOCKCCFILE_ALL(&(pObj->Ctx), &reg_value);
  if(status == NFCTAG_OK)
  {
    /* Extract CCFile block information */
    if((reg_value & ST25DVXXKC_LOCKCCFILE_BLCK0_MASK) == ST25DVXXKC_LOCKCCFILE_BLCK0_MASK)
    {
      pLockCCFile->LckBck0 = ST25DVXXKC_LOCKED;
    }
    else
    {
      pLockCCFile->LckBck0 = ST25DVXXKC_UNLOCKED;
    }
    
    if((reg_value & ST25DVXXKC_LOCKCCFILE_BLCK1_MASK) == ST25DVXXKC_LOCKCCFILE_BLCK1_MASK)
    {
      pLockCCFile->LckBck1 = ST25DVXXKC_LOCKED;
    }
    else
    {
      pLockCCFile->LckBck1 = ST25DVXXKC_UNLOCKED;
    }
  }
  return status;
}

/**
  * @brief  Locks the CCile to prevent any RF write access.
  * @details  Needs the I2C Password presentation to be effective.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] NbBlockCCFile ST25DVxxKC_CCFILE_BLOCK value corresponding to the number of blocks to be locked.
  * @param[in] LockCCFile ST25DVxxKC_LOCK_CCFILE value corresponding to the lock state to apply on the CCFile.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_WriteLockCCFile(const ST25DVxxKC_Object_t *const pObj, \
                            const ST25DVxxKC_CCFILE_BLOCK_E NbBlockCCFile, const ST25DVxxKC_LOCK_STATUS_E LockCCFile)
{
  uint8_t reg_value;
  
  /* Configure value to write on register */
  if(NbBlockCCFile == ST25DVXXKC_CCFILE_1BLCK)
  {
    if(LockCCFile == ST25DVXXKC_LOCKED)
    {
      reg_value = ST25DVXXKC_LOCKCCFILE_BLCK0_MASK;
    }
    else
    {
      reg_value = 0x00;
    }
  }
  else
  {
    if(LockCCFile == ST25DVXXKC_LOCKED)
    {
      reg_value = ST25DVXXKC_LOCKCCFILE_BLCK0_MASK | ST25DVXXKC_LOCKCCFILE_BLCK1_MASK;
    }
    else
    {
      reg_value = 0x00;
    }
  }
  
  /* Write LOCKCCFILE register */
  return ST25DVxxKC_SetLOCKCCFILE_ALL(&(pObj->Ctx), &reg_value);
}

/**
  * @brief  Reads the Cfg registers protection.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pLockCfg Pointer on a ST25DVxxKC_LOCK_STATUS value corresponding to the Cfg registers lock state.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadLockCFG(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_LOCK_STATUS_E *const pLockCfg)
{
  uint8_t reg_value;
  int32_t status;
  
  *pLockCfg = ST25DVXXKC_UNLOCKED;

  /* Get actual LOCKCCFILE register value */
  status = ST25DVxxKC_GetLOCKCFG_B0(&(pObj->Ctx), &reg_value);
  if(status == NFCTAG_OK)
  {
    /* Extract LOCKCFG block information */
    if((reg_value & ST25DVXXKC_LOCKCFG_B0_MASK) == ST25DVXXKC_LOCKCFG_B0_MASK)
    {
      *pLockCfg = ST25DVXXKC_LOCKED;
    }
    else
    {
      *pLockCfg = ST25DVXXKC_UNLOCKED;
    }
  }
  return status;
}

/**
  * @brief  Lock/Unlock the Cfg registers, to prevent any RF write access.
  * @details  Needs the I2C Password presentation to be effective.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] LockCfg ST25DVxxKC_LOCK_STATUS value corresponding to the lock state to be written.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_WriteLockCFG(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_LOCK_STATUS_E LockCfg)
{
  uint8_t reg_value;
  
  /* Configure value to write on register */
  reg_value = (uint8_t)LockCfg;
  
  /* Write LOCKCFG register */
  return ST25DVxxKC_SetLOCKCFG_B0(&(pObj->Ctx), &reg_value);
}

/**
  * @brief  Presents I2C password, to authorize the I2C writes to protected areas.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] PassWord Password value on 32bits
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_PresentI2CPassword(ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_PASSWD_t PassWord)
{
  uint8_t ai2c_message[17] = {0};
  uint8_t i;
  
  /* Build I2C Message with Password + Validation code 0x09 + Password */
  ai2c_message[8] = 0x09U;
  for(i = 0U; i < 4U; i++)
  {
    ai2c_message[i] = (uint8_t)(PassWord.MsbPasswd >> ((3U - i) * 8U)) & (uint8_t)0xFF;
    ai2c_message[i + 4U] = (uint8_t)(PassWord.LsbPasswd >> ((3U - i) * 8U)) & (uint8_t)0xFFU;
    ai2c_message[i + 9U] = ai2c_message[i];
    ai2c_message[i + 13U] = ai2c_message[i + 4U];
  };
  
  /* Present password to ST25DVxxKC */
  return ST25DVxxKC_WriteRegister(pObj, ai2c_message, ST25DVXXKC_I2CPASSWD_REG, 17);
}

/**
  * @brief  Writes a new I2C password.
  * @details  Needs the I2C Password presentation to be effective.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] PassWord New I2C PassWord value on 32bits.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_WriteI2CPassword(ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_PASSWD_t PassWord)
{
  uint8_t ai2c_message[17] = {0};
  uint8_t i;
  
  /* Build I2C Message with Password + Validation code 0x07 + Password */
  ai2c_message[8] = 0x07U;

  for(i = 0U; i < 4U; i++)
  {
    ai2c_message[i] = (uint8_t)(PassWord.MsbPasswd >> ((3U - i) * 8U)) & (uint8_t)0xFF;
    ai2c_message[i + 4U] = (uint8_t)(PassWord.LsbPasswd >> ((3U - i) * 8U)) & (uint8_t)0xFF;
    ai2c_message[i + 9U] = ai2c_message[i];
    ai2c_message[i + 13U] = ai2c_message[i + 4U];
  };
  
  /* Write new password in I2CPASSWD register */
  return ST25DVxxKC_WriteRegister(pObj, ai2c_message, ST25DVXXKC_I2CPASSWD_REG, 17);
}

/**
  * @brief  Reads the RF Zone Security Status (defining the allowed RF accesses).
  * @param[in] pObj pointer to the device structure object.
  * @param[in] Zone ST25DVxxKC_PROTECTION_ZONE value coresponding to the protected area.
  * @param[out] pRfprotZone Pointer on a ST25DVxxKC_RF_PROT_ZONE value corresponding to the area protection state.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadRFZxSS(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_PROTECTION_ZONE_E Zone, \
                                                                ST25DVxxKC_RF_PROT_ZONE_t *const pRfprotZone)
{
  uint8_t reg_value = 0;
  int32_t status;

  pRfprotZone->PasswdCtrl = ST25DVXXKC_NOT_PROTECTED;
  pRfprotZone->RWprotection = ST25DVXXKC_NO_PROT;
  
  /* Read actual value of Sector Security Status register */
  switch(Zone)
  {
    case ST25DVXXKC_PROT_ZONE1:
      status = ST25DVxxKC_GetRFA1SS_ALL(&(pObj->Ctx), &reg_value);
      break;
    case ST25DVXXKC_PROT_ZONE2:
      status = ST25DVxxKC_GetRFA2SS_ALL(&(pObj->Ctx), &reg_value);
      break;
    case ST25DVXXKC_PROT_ZONE3:
      status = ST25DVxxKC_GetRFA3SS_ALL(&(pObj->Ctx), &reg_value);
      break;
    case ST25DVXXKC_PROT_ZONE4:
      status = ST25DVxxKC_GetRFA4SS_ALL(&(pObj->Ctx), &reg_value);
      break;
    
    default:
      status = NFCTAG_ERROR;
      break;
  }
  
  if(status == NFCTAG_OK)
  {
    /* Extract Sector Security Status configuration */
    switch((reg_value & ST25DVXXKC_RFA1SS_PWDCTRL_MASK) >> ST25DVXXKC_RFA1SS_PWDCTRL_SHIFT)
    {
      case ST25DVXXKC_NOT_PROTECTED:
        pRfprotZone->PasswdCtrl = ST25DVXXKC_NOT_PROTECTED;
        break;
      case ST25DVXXKC_PROT_PASSWD1:
        pRfprotZone->PasswdCtrl = ST25DVXXKC_PROT_PASSWD1;
        break;
      case ST25DVXXKC_PROT_PASSWD2:
        pRfprotZone->PasswdCtrl = ST25DVXXKC_PROT_PASSWD2;
        break;
      case ST25DVXXKC_PROT_PASSWD3:
        pRfprotZone->PasswdCtrl = ST25DVXXKC_PROT_PASSWD3;
        break;
      
      default:
        status = NFCTAG_ERROR;
        break;
    }

    switch((reg_value & ST25DVXXKC_RFA1SS_PWDCTRL_MASK) >> ST25DVXXKC_RFA1SS_PWDCTRL_SHIFT)
    {
      case ST25DVXXKC_NO_PROT:
        pRfprotZone->RWprotection = ST25DVXXKC_NO_PROT;
        break;
      case ST25DVXXKC_WRITE_PROT:
        pRfprotZone->RWprotection = ST25DVXXKC_WRITE_PROT;
        break;
      case ST25DVXXKC_READ_PROT:
        pRfprotZone->RWprotection = ST25DVXXKC_READ_PROT;
        break;
      case ST25DVXXKC_READWRITE_PROT:
        pRfprotZone->RWprotection = ST25DVXXKC_READWRITE_PROT;
        break;
      
      default:
        status = NFCTAG_ERROR;
        break;
    }
  }
  return status;
}

/**
  * @brief  Writes the RF Zone Security Status (defining the allowed RF accesses)
  * @details  Needs the I2C Password presentation to be effective.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] Zone ST25DVxxKC_PROTECTION_ZONE value corresponding to the area on which to set the RF protection.
  * @param[in] RfProtZone  Pointer on a ST25DVxxKC_RF_PROT_ZONE value defininf the protection to be set on the area.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_WriteRFZxSS(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_PROTECTION_ZONE_E Zone, \
                                                                const ST25DVxxKC_RF_PROT_ZONE_t RfProtZone)
{
  uint8_t reg_value;
  int32_t status;
  

  /* Update Sector Security Status */
  reg_value = ((uint8_t)RfProtZone.RWprotection << ST25DVXXKC_RFA1SS_RWPROT_SHIFT) & ST25DVXXKC_RFA1SS_RWPROT_MASK;
  reg_value |= (((uint8_t)RfProtZone.PasswdCtrl << ST25DVXXKC_RFA1SS_PWDCTRL_SHIFT) & ST25DVXXKC_RFA1SS_PWDCTRL_MASK);
  
  /* Write Sector Security register */
  switch(Zone)
  {
    case ST25DVXXKC_PROT_ZONE1:
      status = ST25DVxxKC_SetRFA1SS_ALL(&(pObj->Ctx), &reg_value);
      break;
    case ST25DVXXKC_PROT_ZONE2:
      status = ST25DVxxKC_SetRFA2SS_ALL(&(pObj->Ctx), &reg_value);
      break;
    case ST25DVXXKC_PROT_ZONE3:
      status = ST25DVxxKC_SetRFA3SS_ALL(&(pObj->Ctx), &reg_value);
      break;
    case ST25DVXXKC_PROT_ZONE4:
      status = ST25DVxxKC_SetRFA4SS_ALL(&(pObj->Ctx), &reg_value);
      break;
    
    default:
      status = NFCTAG_ERROR;
      break;
  }
    return status; 
}

/**
  * @brief  Reads the value of the an area end address.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] EndZone ST25DVxxKC_END_ZONE value corresponding to an area end address.
  * @param[out] pEndZ Pointer used to return the end address of the area.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadEndZonex(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_END_ZONE_E EndZone, \
                                                                                                uint8_t * pEndZ)
{
  int32_t status;

  *pEndZ = 0x00;

  /* Read the corresponding End zone */ 
  switch(EndZone)
  {
    case ST25DVXXKC_ZONE_END1:
      status = ST25DVxxKC_GetENDA1(&(pObj->Ctx),pEndZ);
      break;
    case ST25DVXXKC_ZONE_END2:
      status = ST25DVxxKC_GetENDA2(&(pObj->Ctx),pEndZ);
      break;
    case ST25DVXXKC_ZONE_END3:
      status = ST25DVxxKC_GetENDA3(&(pObj->Ctx),pEndZ);
      break;
    
    default:
      status = NFCTAG_ERROR;
      break;
  }
  
  return status;
}

/**
  * @brief    Sets the end address of an area.
  * @details  Needs the I2C Password presentation to be effective.
  * @note     The ST25DVxxKC answers a NACK when setting the EndZone2 & EndZone3 to same value than repectively 
  *           EndZone1 & EndZone2.\n These NACKs are ok.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] EndZone ST25DVxxKC_END_ZONE value corresponding to an area.
  * @param[in] EndZ End zone value to be written.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_WriteEndZonex(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_END_ZONE_E EndZone, \
                                                                                                const uint8_t EndZ)
{
  int32_t status;
  
  /* Write the corresponding End zone value in register */  
  switch(EndZone)
  {
    case ST25DVXXKC_ZONE_END1:
      status = ST25DVxxKC_SetENDA1(&(pObj->Ctx),&EndZ);
      break;
    case ST25DVXXKC_ZONE_END2:
      status = ST25DVxxKC_SetENDA2(&(pObj->Ctx),&EndZ);
      break;
    case ST25DVXXKC_ZONE_END3:
      status = ST25DVxxKC_SetENDA3(&(pObj->Ctx),&EndZ);
      break;
    
    default:
      status = NFCTAG_ERROR;
      break;
  }

  return status;
}

/**
  * @brief  Initializes the end address of the ST25DVxxKC areas with their default values (end of memory).
  * @details  Needs the I2C Password presentation to be effective..
  *           The ST25DVxxKC answers a NACK when setting the EndZone2 & EndZone3 to same value than repectively 
  *           EndZone1 & EndZone2. These NACKs are ok.
  * @param[in] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_InitEndZone(const ST25DVxxKC_Object_t *const pObj)
{
  uint8_t endval;
  uint32_t maxmemlength;
  ST25DVxxKC_MEM_SIZE_t memsize;
  int32_t ret;
  
  memsize.Mem_Size = 0;
  memsize.BlockSize = 0;

  /* Get EEPROM mem size */
  (void)ST25DVxxKC_ReadMemSize(pObj, &memsize);
  maxmemlength = ((uint32_t)memsize.Mem_Size + 1U) * ((uint32_t)memsize.BlockSize + 1U);
  
  /* Compute Max value for endzone register */
  endval = (uint8_t)((maxmemlength / 32U) - 1U);
  
  (void)ST25DVxxKC_WriteEndZonex(pObj, ST25DVXXKC_ZONE_END1, 0x00U);
  (void)ST25DVxxKC_WriteEndZonex(pObj, ST25DVXXKC_ZONE_END2, 0x01U);
  
  /* Write EndZone value to ST25DVxxKC registers */
  ret = ST25DVxxKC_WriteEndZonex(pObj, ST25DVXXKC_ZONE_END3, endval);
  if((ret == NFCTAG_OK) || (ret == NFCTAG_NACK))
  {
    ret = ST25DVxxKC_WriteEndZonex(pObj, ST25DVXXKC_ZONE_END2, endval);
  }
  
  if((ret == NFCTAG_OK) || (ret == NFCTAG_NACK))
  {
    ret = ST25DVxxKC_WriteEndZonex(pObj, ST25DVXXKC_ZONE_END1, endval);
  }

  if((ret == NFCTAG_OK) || (ret == NFCTAG_NACK))
  {
    ret = NFCTAG_OK;
  }
  
  return ret;
}

/**
  * @brief  Creates user areas with defined lengths.
  * @details  Needs the I2C Password presentation to be effective.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] Zone1Length Length of area1 in bytes (32 to 8192, 0x20 to 0x2000)
  * @param[in] Zone2Length Length of area2 in bytes (0 to 8128, 0x00 to 0x1FC0)
  * @param[in] Zone3Length Length of area3 in bytes (0 to 8064, 0x00 to 0x1F80)
  * @param[in] Zone4Length Length of area4 in bytes (0 to 8000, 0x00 to 0x1F40)
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_CreateUserZone(const ST25DVxxKC_Object_t *const pObj, const uint16_t Zone1Length, \
                                   const uint16_t Zone2Length, const uint16_t Zone3Length, const uint16_t Zone4Length)
{
  uint8_t EndVal;
  uint16_t Zone1Length_tmp = Zone1Length;
  uint16_t Zone2Length_tmp = Zone2Length;
  uint16_t Zone3Length_tmp = Zone3Length;
  uint16_t maxmemlength;
  int32_t ret = NFCTAG_OK;
  ST25DVxxKC_MEM_SIZE_t memsize;
  
  
  memsize.Mem_Size = 0;
  memsize.BlockSize = 0;

  (void)ST25DVxxKC_ReadMemSize(pObj, &memsize);
  
  maxmemlength = ((uint16_t)memsize.Mem_Size + 1U) * ((uint16_t)memsize.BlockSize + 1U);
  
  /* Checks that values of different zones are in bounds */
  if((Zone1Length < 32U) || (Zone1Length > maxmemlength) || (Zone2Length > (maxmemlength - 32U))
      || (Zone3Length > (maxmemlength - 64U)) || (Zone4Length > (maxmemlength - 96U)))
  {
    ret = NFCTAG_ERROR;
  }

  /* Checks that the total is less than the authorised maximum */
  if((Zone1Length + Zone2Length + Zone3Length + Zone4Length) > maxmemlength)
  {
    ret = NFCTAG_ERROR;
  }
  
  /* if the value for each Length is not a multiple of 64 correct it. */
  if((Zone1Length % 32U) != 0U)
  {
    Zone1Length_tmp = Zone1Length - (Zone1Length % 32U);
  }
  
  if((Zone2Length % 32U) != 0U)
  {
    Zone2Length_tmp = Zone2Length - (Zone2Length % 32U);
  }
  
  if((Zone3Length % 32U) != 0U)
  {
    Zone3Length_tmp = Zone3Length - (Zone3Length % 32U);
  }
  
  if(ret == NFCTAG_OK)
  {
    /* First right 0xFF in each Endx value */
    ret = ST25DVxxKC_InitEndZone(pObj);
  }
  
  if((ret == NFCTAG_OK) || (ret == NFCTAG_NACK))
  {
    /* Then Write corresponding value for each zone */
    EndVal = (uint8_t)((Zone1Length_tmp / 32U) - 1U);
    ret = ST25DVxxKC_WriteEndZonex(pObj, ST25DVXXKC_ZONE_END1, EndVal);
  }

  if((ret == NFCTAG_OK) || (ret == NFCTAG_NACK))
  {  
    EndVal = (uint8_t)(((Zone1Length_tmp + Zone2Length_tmp) / 32U) - 1U);
    ret = ST25DVxxKC_WriteEndZonex(pObj, ST25DVXXKC_ZONE_END2, EndVal);
  }

  if((ret == NFCTAG_OK) || (ret == NFCTAG_NACK))
  { 
    EndVal = (uint8_t)(((Zone1Length_tmp + Zone2Length_tmp + Zone3Length_tmp) / 32U) - 1U);
    ret = ST25DVxxKC_WriteEndZonex(pObj, ST25DVXXKC_ZONE_END3, EndVal);
  }

  if((ret == NFCTAG_OK) || (ret == NFCTAG_NACK))
  {
    ret = NFCTAG_OK;
  }
  
  return ret;
}

/**
  * @brief  Reads the ST25DVxxKC Memory Size.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pSizeInfo Pointer on a ST25DVxxKC_MEM_SIZE structure used to return the Memory size information.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadMemSize(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_MEM_SIZE_t *const pSizeInfo)
{
  uint8_t memsize_msb;
  uint8_t memsize_lsb;
  int32_t status;
  
  pSizeInfo->Mem_Size = 0;

  /* Read actual value of MEM_SIZE register */
  status = ST25DVxxKC_GetMEM_SIZE_LSB(&(pObj->Ctx), &memsize_lsb);
  if(status == NFCTAG_OK)
  {
    status = ST25DVxxKC_GetMEM_SIZE_MSB(&(pObj->Ctx), &memsize_msb);
    if(status == NFCTAG_OK)
    {
      status = ST25DVxxKC_GetBLK_SIZE(&(pObj->Ctx), &(pSizeInfo->BlockSize));
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
  * @brief  Reads the Energy harvesting mode.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pEH_mode Pointer on a ST25DVxxKC_EH_MODE_STATUS value corresponding to the Energy Harvesting state.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadEHMode(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EH_MODE_STATUS_E *const pEH_mode)
{
  uint8_t reg_value;
  int32_t status;
  
  *pEH_mode = ST25DVXXKC_EH_ACTIVE_AFTER_BOOT;

  /* Read actual value of EH_MODE register */
  status = ST25DVxxKC_GetEH_MODE(&(pObj->Ctx), &reg_value);
  if(status == NFCTAG_OK)
  {
    /* Extract EH_mode configuration */
    if(reg_value != 0U)
    {
      *pEH_mode = ST25DVXXKC_EH_ON_DEMAND;
    }
    else
    {
      *pEH_mode = ST25DVXXKC_EH_ACTIVE_AFTER_BOOT;
    }
  }
  return status;
}

/**
  * @brief  Sets the Energy harvesting mode.
  * @details  Needs the I2C Password presentation to be effective.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] EH_mode ST25DVxxKC_EH_MODE_STATUS value for the Energy harvesting mode to be set.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_WriteEHMode(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_EH_MODE_STATUS_E EH_mode)
{
  uint8_t reg_value;
  
  /* Update EH_mode */
  reg_value = (uint8_t)EH_mode;
  
  /* Write EH_MODE register */
  return ST25DVxxKC_SetEH_MODE(&(pObj->Ctx), &reg_value);
}

/**
  * @brief  Reads the RF Management configuration.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pRF_Mngt Pointer on a ST25DVxxKC_RF_MNGT structure used to return the RF Management configuration.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadRFMngt(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_RF_MNGT_t *const pRF_Mngt)
{
  int32_t status;
  uint8_t reg_value = 0;
  
  pRF_Mngt->RfDisable = ST25DVXXKC_DISABLE;
  pRF_Mngt->RfSleep = ST25DVXXKC_DISABLE;

  /* Read actual value of RF_MNGT register */
  status = ST25DVxxKC_GetRF_MNGT_ALL(&(pObj->Ctx), &reg_value);

  if(status == NFCTAG_OK)
  {
    /* Extract RF Disable information */
    if((reg_value & ST25DVXXKC_RF_MNGT_RFDIS_MASK) == ST25DVXXKC_RF_MNGT_RFDIS_MASK)
    {
      pRF_Mngt->RfDisable = ST25DVXXKC_ENABLE;
    }
    else
    {
      pRF_Mngt->RfDisable = ST25DVXXKC_DISABLE;
    }
    
    /* Extract RF Sleep information */
    if((reg_value & ST25DVXXKC_RF_MNGT_RFSLEEP_MASK) == ST25DVXXKC_RF_MNGT_RFSLEEP_MASK)
    {
      pRF_Mngt->RfSleep = ST25DVXXKC_ENABLE;
    }
    else
    {
      pRF_Mngt->RfSleep = ST25DVXXKC_DISABLE;
    }
  }
  
  return status;
}

/**
  * @brief  Sets the RF Management configuration.
  * @details  Needs the I2C Password presentation to be effective.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] Rfmngt Value of the RF Management configuration to be written.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_WriteRFMngt(const ST25DVxxKC_Object_t *const pObj, const uint8_t Rfmngt)
{
  /* Write RF_MNGT register */
  return ST25DVxxKC_SetRF_MNGT_ALL(&(pObj->Ctx), &Rfmngt);
}

/**
  * @brief  Reads the RFDisable register information.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pRFDisable Pointer on a ST25DVxxKC_EN_STATUS value corresponding to the RF Disable status.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_GetRFDisable(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pRFDisable)
{
  int32_t status;
  uint8_t reg_value = 0;
  
  *pRFDisable = ST25DVXXKC_DISABLE;

  /* Read actual value of RF_MNGT register */
  status = ST25DVxxKC_GetRF_MNGT_RFDIS(&(pObj->Ctx), &reg_value);
  
  /* Extract RFDisable information */
  if(status == NFCTAG_OK)
  {
    if(reg_value != 0U)
    {
      *pRFDisable = ST25DVXXKC_ENABLE;
    }
    else
    {
      *pRFDisable = ST25DVXXKC_DISABLE;
    }
  }
  
  return status;
}

/**
  * @brief  Sets the RF Disable configuration.
  * @details  Needs the I2C Password presentation to be effective.
  * @param[in] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_SetRFDisable(const ST25DVxxKC_Object_t *const pObj)
{
  uint8_t reg_value = 1;
  
  /* Write RF_MNGT register */  
  return ST25DVxxKC_SetRF_MNGT_RFDIS(&(pObj->Ctx), &reg_value);
}

/**
  * @brief  Resets the RF Disable configuration
  * @details  Needs the I2C Password presentation to be effective.
  * @param[in] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ResetRFDisable(const ST25DVxxKC_Object_t *const pObj)
{
  uint8_t reg_value = 0;
  
  /* Write RF_MNGT register */
  return ST25DVxxKC_SetRF_MNGT_RFDIS(&(pObj->Ctx), &reg_value);
}

/**
  * @brief  Reads the RFSleep register information.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pRFSleep Pointer on a ST25DVxxKC_EN_STATUS value corresponding to the RF Sleep status.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_GetRFSleep(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pRFSleep)
{
  int32_t status;
  uint8_t reg_value = 0;
  
  *pRFSleep = ST25DVXXKC_DISABLE;
  
  /* Read actual value of RF_MNGT register */
  status = ST25DVxxKC_GetRF_MNGT_RFSLEEP(&(pObj->Ctx), &reg_value);
  
  /* Extract RFDisable information */
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
  * @brief  Sets the RF Sleep configuration.
  * @details  Needs the I2C Password presentation to be effective.
  * @param[in] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_SetRFSleep(const ST25DVxxKC_Object_t *const pObj)
{
  uint8_t reg_value = 1U;
  
  /* Write RF_MNGT register */
  return ST25DVxxKC_SetRF_MNGT_RFSLEEP(&(pObj->Ctx), &reg_value);
}

/**
  * @brief  Resets the RF Sleep configuration.
  * @details  Needs the I2C Password presentation to be effective.
  * @param[in] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ResetRFSleep(const ST25DVxxKC_Object_t *const pObj)
{
  uint8_t reg_value = 0U;
  
  /* Write RF_MNGT register */
  return ST25DVxxKC_SetRF_MNGT_RFSLEEP(&(pObj->Ctx), &reg_value);
}

/**
  * @brief  Reads the Mailbox mode.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pMB_mode Pointer on a ST25DVxxKC_EH_MODE_STATUS value used to return the Mailbox mode.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadMBMode(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pMB_mode)
{
  uint8_t reg_value;
  int32_t status;
  
  *pMB_mode = ST25DVXXKC_DISABLE;

  /* Read actual value of FTM register */
  status = ST25DVxxKC_GetFTM_MBMODE(&(pObj->Ctx), &reg_value);
  if(status == NFCTAG_OK)
  {
    /* Extract Mailbox mode status */
    if(reg_value != 0U)
    {
      *pMB_mode = ST25DVXXKC_ENABLE;
    }
    else
    {
      *pMB_mode = ST25DVXXKC_DISABLE;
    }
  }
  return status;
}

/**
  * @brief  Sets the Mailbox mode.
  * @details  Needs the I2C Password presentation to be effective.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] MB_mode ST25DVxxKC_EN_STATUS value corresponding to the Mailbox mode to be set.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_WriteMBMode(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_EN_STATUS_E MB_mode)
{
  uint8_t reg_value;
  int32_t status;
  /* Update Mailbox mode status */
  reg_value = (uint8_t)MB_mode;
  
  /* Write FTM register */
  status = ST25DVxxKC_SetFTM_MBMODE(&(pObj->Ctx), &reg_value);

  return status;
}

/**
  * @brief  Reads the Mailbox watchdog duration coefficient.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pWdgDelay Pointer on a uint8_t used to return the watchdog duration coefficient.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadMBWDG(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pWdgDelay)
{
  int32_t status;
  
  /* Read actual value of MB_WDG register */
  status = ST25DVxxKC_GetFTM_MBWDG(&(pObj->Ctx), pWdgDelay);
  
  return status;
}

/**
  * @brief  Writes the Mailbox watchdog coefficient delay.
  * @details  Needs the I2C Password presentation to be effective.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] WdgDelay Watchdog duration coefficient to be written (Watch dog duration = MB_WDG*30 ms +/- 6%).
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_WriteMBWDG(const ST25DVxxKC_Object_t *const pObj, const uint8_t WdgDelay)
{
   /* Write MB_WDG register */
  return ST25DVxxKC_SetFTM_MBWDG(&(pObj->Ctx), &WdgDelay);
}

/**
  * @brief  Reads N bytes of data from the Mailbox, starting at the specified byte offset.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pData Pointer on the buffer used to return the read data.
  * @param[in] Offset Offset in the Mailbox memory, byte number to start the read.
  * @param[in] NbByte Number of bytes to be read.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadMailboxData(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pData, const uint16_t Offset, \
                                                                                        const uint16_t NbByte)
{
  int32_t ret;
  
  if(Offset > ST25DVXXKC_MAX_MAILBOX_LENGTH)
  {
    ret = NFCTAG_ERROR;
  }
  else
  {
    /* Read Data in user memory */
    ret = pObj->IO.Read(pObj->IO.DeviceAddress, ST25DVXXKC_MAILBOX_RAM_REG + Offset, pData, NbByte);
  }
  
  return ret;
}

/**
  * @brief  Writes N bytes of data in the Mailbox, starting from first Mailbox Address.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] pData Pointer to the buffer containing the data to be written.
  * @param[in] NbByte Number of bytes to be written.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_WriteMailboxData(const ST25DVxxKC_Object_t *const pObj, const uint8_t *const pData, \
                                                                                        const uint16_t NbByte)
{
  int32_t status;
  
  /* ST25DVxxKC can write a maximum of 256 bytes in Mailbox */
  if(NbByte <= ST25DVXXKC_MAX_MAILBOX_LENGTH)
  {
    /* Write NbByte data in memory */
    status =  pObj->IO.Write(pObj->IO.DeviceAddress, ST25DVXXKC_MAILBOX_RAM_REG, pData, NbByte);
  }
  else
  {
    status = NFCTAG_ERROR;
  }
  
  return status;
}

/**
  * @brief  Reads N bytes from the mailbox registers, starting at the specified I2C address.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pData Pointer on the buffer used to return the data.
  * @param[in] TarAddr I2C memory address to be read.
  * @param[in] NbByte Number of bytes to be read.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadMailboxRegister(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pData, \
                                                                       const uint16_t TarAddr, const uint16_t NbByte)
{
  int32_t ret;
  
  if((TarAddr < ST25DVXXKC_GPO_DYN_REG) || (TarAddr > ST25DVXXKC_MBLEN_DYN_REG))
  {
    ret = NFCTAG_ERROR;
  }
  else
  {
    ret = pObj->IO.Read(pObj->IO.DeviceAddress, TarAddr,pData, NbByte);
  }
  
  return ret;
}

/**
  * @brief  Writes N bytes to the specified mailbox register.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] pData Pointer on the data to be written.
  * @param[in] TarAddr I2C register address to be written.
  * @param[in] NbByte Number of bytes to be written.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_WriteMailboxRegister(const ST25DVxxKC_Object_t *const pObj, const uint8_t *const pData, \
                                                      const uint16_t TarAddr, const uint16_t NbByte)
{
  int32_t ret;
  
  if((TarAddr < ST25DVXXKC_GPO_DYN_REG) || (TarAddr > ST25DVXXKC_MBLEN_DYN_REG))
  {
    ret = NFCTAG_ERROR;
  }
  else
  {
    /* Write NbByte data in memory */
    ret = pObj->IO.Write(pObj->IO.DeviceAddress, TarAddr,pData, NbByte);
  }
  
  return ret;
}

/**
  * @brief  Reads the status of the security session open register.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pSession Pointer on a ST25DVxxKC_I2CSSO_STATUS value used to return the session status.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadI2CSecuritySession_Dyn(const ST25DVxxKC_Object_t *const pObj, \
                                                            ST25DVxxKC_I2CSSO_STATUS_E *const pSession)
{
  uint8_t reg_value;
  int32_t status;

  *pSession = ST25DVXXKC_SESSION_CLOSED;

  /* Read actual value of I2C_SSO_DYN register */
  status = ST25DVxxKC_GetI2C_SSO_DYN_I2CSSO(&(pObj->Ctx), &reg_value);
  if(status == NFCTAG_OK)
  {
    /* Extract Open session information */
    if(reg_value != 0U)
    {
      *pSession = ST25DVXXKC_SESSION_OPEN;
    }
    else
    {
      *pSession = ST25DVXXKC_SESSION_CLOSED;
    }
  }
  return status;
}

/**
  * @brief  Reads the IT status register from the ST25DVxxKC.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pITStatus Pointer on uint8_t, used to return the IT status, such as:
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
int32_t ST25DVxxKC_ReadITSTStatus_Dyn(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pITStatus)
{
  /* Read value of ITStatus register */
  return ST25DVxxKC_GetITSTS_DYN_ALL(&(pObj->Ctx), pITStatus);
}

/**
  * @brief  Read value of dynamic GPO register configuration.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pGPO ST25DVxxKC_GPO pointer of the dynamic GPO configuration to store.
  * @retval NFCTAG enum status.
  */
int32_t ST25DVxxKC_ReadGPO_Dyn(const ST25DVxxKC_Object_t *const pObj, uint8_t *GPOConfig)
{
  /* Read actual value of ST25DVxxKC_GPO_DYN_REG register */
  return ST25DVxxKC_GetGPO_DYN_ALL(&(pObj->Ctx), GPOConfig);
}

/**
  * @brief  Get dynamique GPO enable status.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pGPO_en ST25DVxxKC_EN_STATUS pointer of the GPO enable status to store.
  * @retval NFCTAG enum status
  */
int32_t ST25DVxxKC_GetGPO_en_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pGPO_en)
{
  uint8_t reg_value;
  int32_t status;
  
  *pGPO_en = ST25DVXXKC_DISABLE;

  /* Read actual value of GPO_DYN register */
  status = ST25DVxxKC_GetGPO_DYN_ENABLE(&(pObj->Ctx), &reg_value);
  if(status == NFCTAG_OK)
  {
    /* Extract GPO enable status information */
    if(reg_value != 0U)
    {
      *pGPO_en = ST25DVXXKC_ENABLE;
    }
    else
    {
      *pGPO_en = ST25DVXXKC_DISABLE;
    }
  }
  return status;
}

/**
  * @brief  Set dynamique GPO enable configuration.
  * @param[in] pObj pointer to the device structure object.
  * @retval NFCTAG enum status.
  */
int32_t ST25DVxxKC_SetGPO_en_Dyn(const ST25DVxxKC_Object_t *const pObj)
{
  uint8_t reg_value = 1;
  
  /* Write GPO_DYN Register */
  return ST25DVxxKC_SetGPO_DYN_ENABLE(&(pObj->Ctx), &reg_value);
}

/**
  * @brief  Reset dynamique GPO enable configuration.
  * @param[in] pObj pointer to the device structure object.
  * @retval NFCTAG enum status.
  */
int32_t ST25DVxxKC_ResetGPO_en_Dyn(const ST25DVxxKC_Object_t *const pObj)
{
  uint8_t reg_value = 0;
  
  /* Write GPO_DYN Register */
  return ST25DVxxKC_SetGPO_DYN_ENABLE(&(pObj->Ctx), &reg_value);
}

/**
  * @brief  Read value of dynamic EH Ctrl register configuration.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pEH_CTRL : ST25DVxxKC_EH_CTRL pointer of the dynamic EH Ctrl configuration to store.
  * @retval NFCTAG enum status
  */
int32_t ST25DVxxKC_ReadEHCtrl_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EH_CTRL_t *const pEH_CTRL)
{
  int32_t status;
  uint8_t reg_value = 0;
  
  pEH_CTRL->EH_EN_Mode = ST25DVXXKC_DISABLE;
  pEH_CTRL->EH_on = ST25DVXXKC_DISABLE;
  pEH_CTRL->Field_on = ST25DVXXKC_DISABLE;
  pEH_CTRL->VCC_on = ST25DVXXKC_DISABLE;

  /* Read actual value of ST25DVxxKC_EH_CTRL_DYN_REG register */
  status = ST25DVxxKC_GetEH_CTRL_DYN_ALL(&(pObj->Ctx), &reg_value);
  
  if(status == NFCTAG_OK)
  {
    /* Extract EH EN Mode configuration */
    if((reg_value & ST25DVXXKC_EH_CTRL_DYN_EH_EN_MASK) == ST25DVXXKC_EH_CTRL_DYN_EH_EN_MASK)
    {
      pEH_CTRL->EH_EN_Mode = ST25DVXXKC_ENABLE;
    }
    else
    {
      pEH_CTRL->EH_EN_Mode = ST25DVXXKC_DISABLE;
    }
    
    /* Extract EH_ON configuration */
    if((reg_value & ST25DVXXKC_EH_CTRL_DYN_EH_ON_MASK) == ST25DVXXKC_EH_CTRL_DYN_EH_ON_MASK)
    {
      pEH_CTRL->EH_on = ST25DVXXKC_ENABLE;
    }
    else
    {
      pEH_CTRL->EH_on = ST25DVXXKC_DISABLE;
    }
    
    /* Extract FIELD_ON configuration */
    if((reg_value & ST25DVXXKC_EH_CTRL_DYN_FIELD_ON_MASK) == ST25DVXXKC_EH_CTRL_DYN_FIELD_ON_MASK)
    {
      pEH_CTRL->Field_on = ST25DVXXKC_ENABLE;
    }
    else
    {
      pEH_CTRL->Field_on = ST25DVXXKC_DISABLE;
    }
    
    /* Extract VCC_ON configuration */
    if((reg_value & ST25DVXXKC_EH_CTRL_DYN_VCC_ON_MASK) == ST25DVXXKC_EH_CTRL_DYN_VCC_ON_MASK)
    {
      pEH_CTRL->VCC_on = ST25DVXXKC_ENABLE;
    }
    else
    {
      pEH_CTRL->VCC_on = ST25DVXXKC_DISABLE;
    }
  }
  
  return status;
}

/**
  * @brief  Reads the Energy Harvesting dynamic status.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pEH_Val Pointer on a ST25DVxxKC_EN_STATUS value used to return the Energy Harvesting dynamic status.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_GetEHENMode_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pEH_Val)
{
  uint8_t reg_value;
  int32_t status;
  
  *pEH_Val = ST25DVXXKC_DISABLE;

  /* Read actual value of EH_CTRL_DYN register */
  status = ST25DVxxKC_GetEH_CTRL_DYN_EH_EN(&(pObj->Ctx), &reg_value);
  if(status == NFCTAG_OK)
  {
    /* Extract Energy Harvesting status information */
    if(reg_value != 0U)
    {
      *pEH_Val = ST25DVXXKC_ENABLE;
    }
    else
    {
      *pEH_Val = ST25DVXXKC_DISABLE;
    }
  }
  return status;
}

/**
  * @brief  Dynamically sets the Energy Harvesting mode.
  * @param[in] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_SetEHENMode_Dyn(const ST25DVxxKC_Object_t *const pObj)
{
  uint8_t reg_value = 1;
  
  /* Write EH_CTRL_DYN Register */
  return ST25DVxxKC_SetEH_CTRL_DYN_EH_EN(&(pObj->Ctx), &reg_value);
}

/**
  * @brief  Dynamically unsets the Energy Harvesting mode.
  * @param[in] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ResetEHENMode_Dyn(const ST25DVxxKC_Object_t *const pObj)
{
  uint8_t reg_value = 0;
  
  /* Write EH_CTRL_DYN Register */
  return ST25DVxxKC_SetEH_CTRL_DYN_EH_EN(&(pObj->Ctx), &reg_value);
}

/**
  * @brief  Reads the EH_ON status from the EH_CTRL_DYN register.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pEHON Pointer on a ST25DVxxKC_EN_STATUS value used to return the EHON status.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_GetEHON_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pEHON)
{
  int32_t status;
  uint8_t reg_value = 0;
  
  *pEHON = ST25DVXXKC_DISABLE;

  /* Read actual value of EH_CTRL_DYN register */
  status = ST25DVxxKC_GetEH_CTRL_DYN_EH_ON(&(pObj->Ctx), &reg_value);
  
  /* Extract RF Field information */
  if(status == NFCTAG_OK)
  {
    if(reg_value != 0U)
    {
      *pEHON = ST25DVXXKC_ENABLE;
    }
    else
    {
      *pEHON = ST25DVXXKC_DISABLE;
    }
  }
  
  return status;
}

/**
  * @brief  Checks if RF Field is present in front of the ST25DVxxKC.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pRF_Field Pointer on a ST25DVxxKC_FIELD_STATUS value used to return the field presence.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_GetRFField_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_FIELD_STATUS_E *const pRF_Field)
{
  int32_t status;
  uint8_t reg_value = 0;
  
  *pRF_Field = ST25DVXXKC_FIELD_OFF;

  /* Read actual value of EH_CTRL_DYN register */
  status = ST25DVxxKC_GetEH_CTRL_DYN_FIELD_ON(&(pObj->Ctx), &reg_value);
  
  /* Extract RF Field information */
  if(status == NFCTAG_OK)
  {
    if(reg_value != 0U)
    {
      *pRF_Field = ST25DVXXKC_FIELD_ON;
    }
    else
    {
      *pRF_Field = ST25DVXXKC_FIELD_OFF;
    }
  }
  
  return status;
}

/**
  * @brief  Check if VCC is supplying the ST25DVxxKC.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pVCC ST25DVxxKC_VCC_STATUS pointer of the VCC status to store.
  * @retval NFCTAG enum status.
  */
int32_t ST25DVxxKC_GetVCC_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_VCC_STATUS_E *const pVCC)
{
  int32_t status;
  uint8_t reg_value = 0;
  
  *pVCC = ST25DVXXKC_VCC_OFF;

  /* Read actual value of EH_CTRL_DYN register */
  status = ST25DVxxKC_GetEH_CTRL_DYN_VCC_ON(&(pObj->Ctx), &reg_value);
  
  /* Extract VCC information */
  if(status == NFCTAG_OK)
  {
    if(reg_value != 0U)
    {
      *pVCC = ST25DVXXKC_VCC_ON;
    }
    else
    {
      *pVCC = ST25DVXXKC_VCC_OFF;
    }
  }
  
  return status;
}

/**
  * @brief  Read value of dynamic RF Management configuration.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pRF_Mngt ST25DVxxKC_RF_MNGT pointer of the dynamic RF Management configuration to store
  * @retval NFCTAG enum status
  */
int32_t ST25DVxxKC_ReadRFMngt_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_RF_MNGT_t *const pRF_Mngt)
{
  int32_t status;
  uint8_t reg_value = 0;
  
  pRF_Mngt->RfDisable = ST25DVXXKC_DISABLE;
  pRF_Mngt->RfSleep = ST25DVXXKC_DISABLE;

  /* Read actual value of RF_MNGT_DYN register */
  status = ST25DVxxKC_GetRF_MNGT_DYN_ALL(&(pObj->Ctx), &reg_value);
  
  if(status == NFCTAG_OK)
  {
    /* Extract RF Disable configuration */
    if((reg_value & ST25DVXXKC_RF_MNGT_DYN_RFDIS_MASK) == ST25DVXXKC_RF_MNGT_DYN_RFDIS_MASK)
    {
      pRF_Mngt->RfDisable = ST25DVXXKC_ENABLE;
    }
    else
    {
      pRF_Mngt->RfDisable = ST25DVXXKC_DISABLE;
    }
    
    /* Extract RF Sleep configuration */
    if((reg_value & ST25DVXXKC_RF_MNGT_DYN_RFSLEEP_MASK) == ST25DVXXKC_RF_MNGT_DYN_RFSLEEP_MASK)
    {
      pRF_Mngt->RfSleep = ST25DVXXKC_ENABLE;
    }
    else
    {
      pRF_Mngt->RfSleep = ST25DVXXKC_DISABLE;
    }
  }
  
  return status;
}

/**
  * @brief  Writes a value to the RF Management dynamic register.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] RF_Mngt Value to be written to the RF Management dynamic register.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_WriteRFMngt_Dyn(const ST25DVxxKC_Object_t *const pObj, const uint8_t RF_Mngt)
{
  /* Write value to RF_MNGT_DYN register */
  return ST25DVxxKC_SetRF_MNGT_DYN_ALL(&(pObj->Ctx), &RF_Mngt);
}

/**
  * @brief  Reads the RFDisable dynamic register information.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pRFDisable Pointer on a ST25DVxxKC_EN_STATUS value used to return the RF Disable state.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_GetRFDisable_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pRFDisable)
{
  int32_t status;
  uint8_t reg_value = 0;
  
  *pRFDisable = ST25DVXXKC_DISABLE;

  /* Read actual value of RF_MNGT_DYN register */
  status = ST25DVxxKC_GetRF_MNGT_DYN_RFDIS(&(pObj->Ctx), &reg_value);
  
  /* Extract RFDisable information */
  if(status == NFCTAG_OK)
  {
    if(reg_value != 0U)
    {
      *pRFDisable = ST25DVXXKC_ENABLE;
    }
    else
    {
      *pRFDisable = ST25DVXXKC_DISABLE;
    }
  }
  
  return status;
}

/**
  * @brief  Sets the RF Disable dynamic configuration.
  * @param[in] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_SetRFDisable_Dyn(const ST25DVxxKC_Object_t *const pObj)
{
  uint8_t reg_value = 1;
  
  return ST25DVxxKC_SetRF_MNGT_DYN_RFDIS(&(pObj->Ctx), &reg_value);
}

/**
  * @brief  Unsets the RF Disable dynamic configuration.
  * @param[in] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ResetRFDisable_Dyn(const ST25DVxxKC_Object_t *const pObj)
{
  uint8_t reg_value = 0;
  
  return ST25DVxxKC_SetRF_MNGT_DYN_RFDIS(&(pObj->Ctx), &reg_value);
}

/**
  * @brief  Reads the RFSleep dynamic register information.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pRFSleep Pointer on a ST25DVxxKC_EN_STATUS values used to return the RF Sleep state.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_GetRFSleep_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pRFSleep)
{
  int32_t status;
  uint8_t reg_value = 0;
  
  *pRFSleep = ST25DVXXKC_DISABLE;

  /* Read actual value of RF_MNGT_DYN register */
  status = ST25DVxxKC_GetRF_MNGT_DYN_RFSLEEP(&(pObj->Ctx), &reg_value);
  
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
int32_t ST25DVxxKC_SetRFSleep_Dyn(const ST25DVxxKC_Object_t *const pObj)
{
  uint8_t reg_value = 1U;
  
  return ST25DVxxKC_SetRF_MNGT_DYN_RFSLEEP(&(pObj->Ctx), &reg_value);
}

/**
  * @brief  Unsets the RF Sleep dynamic configuration.
  * @param[in] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ResetRFSleep_Dyn(const ST25DVxxKC_Object_t *const pObj)
{
  uint8_t reg_value = 0U;
  
  return ST25DVxxKC_SetRF_MNGT_DYN_RFSLEEP(&(pObj->Ctx), &reg_value);
}

/**
  * @brief  Reads the RFOff dynamic register information.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pRFSleep Pointer on a ST25DVxxKC_EN_STATUS values used to return the RF Off state.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_GetRFOff_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pRFSleep)
{
  int32_t status;
  uint8_t reg_value = 0;
  
  *pRFSleep = ST25DVXXKC_DISABLE;

  /* Read actual value of RF_MNGT_DYN register */
  status = ST25DVxxKC_GetRF_MNGT_DYN_RFOFF(&(pObj->Ctx), &reg_value);
  
  /* Extract RFOff information */
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
  * @brief  Sets the RF Off dynamic configuration.
  * @param[in] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_SetRFOff_Dyn(const ST25DVxxKC_Object_t *const pObj)
{
  return ST25DVxxKC_WriteI2CSlaveMode(pObj, ST25DVXXKC_SLAVE_MODE_RFOFF);
}

/**
  * @brief  Unsets the RF Off dynamic configuration.
  * @param[in] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ResetRFOff_Dyn(const ST25DVxxKC_Object_t *const pObj)
{
  return ST25DVxxKC_WriteI2CSlaveMode(pObj, ST25DVXXKC_SLAVE_MODE_RFON);
}

/**
  * @brief  Reads the Mailbox ctrl dynamic register.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pCtrlStatus Pointer on a ST25DVxxKC_MB_CTRL_DYN_STATUS structure used to return
  *         the dynamic Mailbox ctrl information.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadMBCtrl_Dyn(const ST25DVxxKC_Object_t *const pObj, \
                                                  ST25DVxxKC_MB_CTRL_DYN_STATUS_t *const pCtrlStatus)
{
  uint8_t reg_value;
  int32_t status;

  pCtrlStatus->MbEnable    = 0x00;
  pCtrlStatus->HostPutMsg  = 0x00;
  pCtrlStatus->RfPutMsg    = 0x00;
  pCtrlStatus->HostMissMsg = 0x00;
  pCtrlStatus->RFMissMsg   = 0x00;
  pCtrlStatus->CurrentMsg = ST25DVXXKC_NO_MSG;
  
  /* Read MB_CTRL_DYN register */
  status = ST25DVxxKC_GetMB_CTRL_DYN_ALL(&(pObj->Ctx), &reg_value);
  if(status == NFCTAG_OK)
  {
    /* Extract Mailbox ctrl information */
    pCtrlStatus->MbEnable = (reg_value & ST25DVXXKC_MB_CTRL_DYN_MBEN_MASK) >> ST25DVXXKC_MB_CTRL_DYN_MBEN_SHIFT;
    pCtrlStatus->HostPutMsg = (reg_value & ST25DVXXKC_MB_CTRL_DYN_HOSTPUTMSG_MASK) >> \
                                                                  ST25DVXXKC_MB_CTRL_DYN_HOSTPUTMSG_SHIFT;
    pCtrlStatus->RfPutMsg = (reg_value & ST25DVXXKC_MB_CTRL_DYN_RFPUTMSG_MASK) >> ST25DVXXKC_MB_CTRL_DYN_RFPUTMSG_SHIFT;
    pCtrlStatus->HostMissMsg = (reg_value & ST25DVXXKC_MB_CTRL_DYN_HOSTMISSMSG_MASK) >> \
                                                                  ST25DVXXKC_MB_CTRL_DYN_HOSTMISSMSG_SHIFT;
    pCtrlStatus->RFMissMsg = (reg_value & ST25DVXXKC_MB_CTRL_DYN_RFMISSMSG_MASK) >> \
                                                                  ST25DVXXKC_MB_CTRL_DYN_RFMISSMSG_SHIFT;

    switch((reg_value & ST25DVXXKC_MB_CTRL_DYN_CURRENTMSG_MASK) >> ST25DVXXKC_MB_CTRL_DYN_CURRENTMSG_SHIFT)
    {
      case ST25DVXXKC_NO_MSG:
        pCtrlStatus->CurrentMsg = ST25DVXXKC_NO_MSG;
        break;
        
      case ST25DVXXKC_HOST_MSG:
        pCtrlStatus->CurrentMsg = ST25DVXXKC_HOST_MSG;
        break;
        
      case ST25DVXXKC_RF_MSG:
        pCtrlStatus->CurrentMsg = ST25DVXXKC_RF_MSG;
        break;
        
      default:
        status = NFCTAG_ERROR;
        break;
    }
  }
  return status;
}

/**
  * @brief  Reads the Mailbox Enable dynamic configuration.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pMBEN pointer on ST25DVxxKC_EN_STATUS_E values used to return the Mailbox enable state.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_GetMBEN_Dyn(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_EN_STATUS_E *const pMBEN)
{
  uint8_t reg_value;
  int32_t status;
  
  *pMBEN = ST25DVXXKC_DISABLE;

  /* Read MB_CTRL_DYN register */
  status = ST25DVxxKC_GetMB_CTRL_DYN_MBEN(&(pObj->Ctx),&reg_value);
  if(status == NFCTAG_OK)
  {
    if(reg_value != 0U)
    {
      *pMBEN = ST25DVXXKC_ENABLE;
    }
    else
    {
      *pMBEN = ST25DVXXKC_DISABLE;
    }
  }
  
  return status;
}

/**
  * @brief  Sets the Mailbox Enable dynamic configuration.
  * @param[in] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_SetMBEN_Dyn(const ST25DVxxKC_Object_t *const pObj)
{
  uint8_t reg_value = 1U;
    
  /* Write MB_CTRL_DYN register */
  return ST25DVxxKC_SetMB_CTRL_DYN_MBEN(&(pObj->Ctx),&reg_value);
}

/**
  * @brief  Unsets the Mailbox Enable dynamic configuration.
  * @param[in] pObj pointer to the device structure object.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ResetMBEN_Dyn(const ST25DVxxKC_Object_t *const pObj)
{
  uint8_t reg_value = 0U;
  
  /* Write MB_CTRL_DYN register */
  return ST25DVxxKC_SetMB_CTRL_DYN_MBEN(&(pObj->Ctx),&reg_value);
}

/**
  * @brief  Reads the Mailbox message length dynamic register.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] pMBLength Pointer on a uint8_t used to return the Mailbox message length.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadMBLength_Dyn(const ST25DVxxKC_Object_t *const pObj, uint8_t *const pMBLength)
{
  /* Read actual value of MBLEN_DYN register */
  return ST25DVxxKC_GetMBLEN_DYN_MBLEN(&(pObj->Ctx),pMBLength);
}

/**
  * @brief Reads register on the ST25DVxxKC device.
  * @param[in] handle pointer to the context device structure object.
  * @param[in] Reg register address to read.
  * @param[out] pData pointer used to return the read data.
  * @param[in] len length of the data to be read.
  * @return int32_t enum status.
  */
static int32_t ReadRegWrap(const void *const handle, const uint16_t Reg, uint8_t *const pData, const uint16_t len)
{
  int32_t ret;
  ST25DVxxKC_Object_t *pObj = (ST25DVxxKC_Object_t *)handle;
  
  if((Reg & (ST25DVXXKC_IS_DYNAMIC_REGISTER)) == ST25DVXXKC_IS_DYNAMIC_REGISTER)
  {
    ret = pObj->IO.Read(pObj->IO.DeviceAddress, Reg, pData, len);
  }
  else
  {
    ret = pObj->IO.Read(pObj->IO.DeviceAddress | ST25DVXXKC_ADDR_SYSTEMMEMORY_BIT_I2C | ST25DVXXKC_ADDR_MODE_BIT_I2C, Reg, pData, len);
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
static int32_t WriteRegWrap(const void *const handle, const uint16_t Reg, const uint8_t *const pData, \
                                                                const uint16_t len)
{
  int32_t ret;
  ST25DVxxKC_Object_t *pObj = (ST25DVxxKC_Object_t *)handle;
  
  if((Reg & (ST25DVXXKC_IS_DYNAMIC_REGISTER)) == ST25DVXXKC_IS_DYNAMIC_REGISTER)
  {
    ret = pObj->IO.Write(pObj->IO.DeviceAddress, Reg, pData, len);
  }
  else
  {
    ret = pObj->IO.Write(pObj->IO.DeviceAddress | ST25DVXXKC_ADDR_SYSTEMMEMORY_BIT_I2C | ST25DVXXKC_ADDR_MODE_BIT_I2C, Reg, pData, len);
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
        pollstatus = pObj->IO.IsReady(pObj->IO.DeviceAddress | ST25DVXXKC_ADDR_SYSTEMMEMORY_BIT_I2C | ST25DVXXKC_ADDR_MODE_BIT_I2C, 1);
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
  * @brief  Sets the I2C slave mode.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] slaveMode slave mode to be used with subsequent I2C communications.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_WriteI2CSlaveMode(const ST25DVxxKC_Object_t *const pObj, const ST25DVxxKC_SLAVE_MODE_E slaveMode)
{
  int32_t ret;
  uint8_t reg_value;

  if (slaveMode == ST25DVXXKC_SLAVE_MODE_RFOFF) {
      reg_value = 0x01;
      ret = ST25DVxxKC_SetI2CCFG_RFSWITCHOFF(&(pObj->Ctx), &reg_value);
      if (ret == NFCTAG_OK) {
        // artificially writes to ICREF_REG
        // Caution: DVC DataSheet up to version 3 invertedRFSwitchOff/ON bits: b3=1b means RFSwitchOFF
        ret = pObj->IO.Write((pObj->IO.DeviceAddress | ST25DVXXKC_ADDR_RFSWITCH_BIT_I2C) & ~ST25DVXXKC_ADDR_MODE_BIT_I2C, ST25DVXXKC_ICREF_REG, NULL, 0);
      }
  }
  else if (slaveMode == ST25DVXXKC_SLAVE_MODE_RFON) {
      reg_value = 0x01;
      ret = ST25DVxxKC_SetI2CCFG_RFSWITCHOFF(&(pObj->Ctx), &reg_value);
      if (ret == NFCTAG_OK) {
        // artificially writes to ICREF_REG
        // Caution: DVC DataSheet up to version 3 invertedRFSwitchOff/ON bits: b3=0b means RFSwitchON
        ret = pObj->IO.Write(pObj->IO.DeviceAddress & ~(ST25DVXXKC_ADDR_RFSWITCH_BIT_I2C | ST25DVXXKC_ADDR_MODE_BIT_I2C), ST25DVXXKC_ICREF_REG, NULL, 0);
      }
  }
  else {
      // ST25DVXXKC_SLAVE_MODE_NORMAL
      reg_value = 0x00;
      ret = ST25DVxxKC_SetI2CCFG_RFSWITCHOFF(&(pObj->Ctx), &reg_value);
  }
  return ret;
}

/**
  * @brief  Gets the I2C slave mode.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] slaveMode pointer on slave mode being used with I2C communications.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadI2CSlaveMode(const ST25DVxxKC_Object_t *const pObj, ST25DVxxKC_SLAVE_MODE_E *const slaveMode)
{
  uint8_t reg_value;
  int32_t ret;

  *slaveMode = ST25DVXXKC_SLAVE_MODE_NORMAL;

  ret = ST25DVxxKC_GetI2CCFG_RFSWITCHOFF(&(pObj->Ctx), &reg_value);
  if (ret == NFCTAG_OK) {
      if (reg_value != 0x00) {
          ret = ST25DVxxKC_GetRF_MNGT_DYN_RFOFF(&(pObj->Ctx), &reg_value);
          if (ret == NFCTAG_OK) {
              *slaveMode = (reg_value != 0x00)  ? ST25DVXXKC_SLAVE_MODE_RFOFF : ST25DVXXKC_SLAVE_MODE_NORMAL;
          }
      }
      else {
          *slaveMode = ST25DVXXKC_SLAVE_MODE_NORMAL;
      }
  }  
  return ret;
}

/**
  * @brief  Sets the I2C slave address.
  * @param[in] pObj pointer to the device structure object.
  * @param[in] deviceCode Device Code to be used with subsequent I2C communications.
  * @param[in] E0 E0 bit to be used with subsequent I2C communications.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_WriteI2CSlaveAddress(const ST25DVxxKC_Object_t *const pObj, const uint8_t deviceCode, const uint8_t E0)
{
  int32_t ret;

  ret = ST25DVxxKC_SetI2CCFG_DEVICECODE(&(pObj->Ctx), &deviceCode);
  if (ret == NFCTAG_OK) {
      ret = ST25DVxxKC_SetI2CCFG_E0(&(pObj->Ctx), &E0);
  }
  
  return ret;
}

/**
  * @brief  Gets the I2C slave address.
  * @param[in] pObj pointer to the device structure object.
  * @param[out] deviceCode pointer on Device Code being used with I2C communications.
  * @param[out] E0 pointer on E0 value being used with I2C communications.
  * @return int32_t enum status.
  */
int32_t ST25DVxxKC_ReadI2CSlaveAddress(const ST25DVxxKC_Object_t *const pObj, uint8_t *const deviceCode, uint8_t *const E0)
{
  int32_t ret;

  *deviceCode = 0x00;
  *E0 = 0x00;

  ret = ST25DVxxKC_GetI2CCFG_DEVICECODE(&(pObj->Ctx), deviceCode);
  if (ret == NFCTAG_OK) {
    ret = ST25DVxxKC_GetI2CCFG_E0(&(pObj->Ctx), E0);
  }

  return ret;
}


/**
 * @}
 */

/**
 * @}
 */

