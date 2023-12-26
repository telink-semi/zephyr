/**
  ******************************************************************************
  * @file    tagtype4_wrapper.h
  * @author  MMY Application Team
  * @version 1.0.1
  * @date    28-Feb-2022
  * @brief   This file provides an abstraction layer to the libNDEF for the NFC Forum Type4 Tag.
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

#ifndef _TAGNFCA_H_
#define _TAGNFCA_H_

#include "lib_NDEF.h"

uint16_t NfcType4_GetLength(uint16_t* Length);
uint16_t NfcType4_ReadNDEF( uint8_t* pData );
uint16_t NfcType4_WriteNDEF(uint16_t Length, uint8_t* pData );

#endif // _TAGNFCA_H_