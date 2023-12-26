/**
  ******************************************************************************
  * @file    lib_NDEF_config.h 
  * @author  MMY Application Team
  * @version 1.3.2
  * @date    28-Feb-2022
  * @brief   
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef _LIB_NDEF_CONFIG_H_
#define _LIB_NDEF_CONFIG_H_

#include <stdint.h>
#include "lib_NDEF.h"

int32_t NDEF_Wrapper_ReadData(const struct device *dev, uint8_t* pData, uint32_t offset, uint32_t length );
int32_t NDEF_Wrapper_WriteData(const struct device *dev, const uint8_t* pData, uint32_t offset, uint32_t length );
uint32_t NDEF_Wrapper_GetMemorySize(const struct device *dev);

#endif // _LIB_NDEF_CONFIG_H_