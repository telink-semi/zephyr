/**
  ******************************************************************************
  * @file    lib_NDEF.h
  * @author  MMY Application Team
  * @version 1.3.2
  * @date    28-Feb-2022
  * @brief   This file help to manage NDEF file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIB_NDEF_H
#define __LIB_NDEF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* include file which match the HW configuration */
#include "lib_wrapper.h"
#include <string.h>
   
#define NDEF_ACTION_COMPLETED       0x9000

#ifndef errorchk
#define errorchk(fCall) if (status = (fCall), status != NDEF_ACTION_COMPLETED) \
  {goto Error;} else
#endif

/* Error codes for Higher level */
#define NDEF_OK                     RESULTOK
#define NDEF_ERROR                  ERRORCODE_GENERIC
#define NDEF_ERROR_MEMORY_TAG       2
#define NDEF_ERROR_MEMORY_INTERNAL  3
#define NDEF_ERROR_LOCKED           4
#define NDEF_ERROR_NOT_FORMATED     5

#define NDEF_MAX_SIZE               NFC_DEVICE_MAX_NDEFMEMORY

#ifdef __cplusplus
}
#endif
#endif /* __LIB_NDEF_H */