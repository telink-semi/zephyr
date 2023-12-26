/**
  ******************************************************************************
  * @file    common_h.ftl
  * @author  SRA Application Team
  * @version 1.0.1
  * @date    27-Apr-2023
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_H
#define __COMMON_H

/* Includes ------------------------------------------------------------------*/

#include "stm32l4s5i_iot01_nfctag.h"

#include <string.h>

/* Exported types ------------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

/* Exported constants --------------------------------------------------------*/
#define MAX_NDEF_MEM                 0x200
#define ST25DVXXKC_MAX_SIZE              NFCTAG_4K_SIZE
#define ST25DVXXKC_NDEF_MAX_SIZE         MIN(ST25DVXXKC_MAX_SIZE,MAX_NDEF_MEM)
#define NFC_DEVICE_MAX_NDEFMEMORY    ST25DVXXKC_NDEF_MAX_SIZE

#define RESULTOK                     0x00
#define ERRORCODE_GENERIC            1

#define TOSTRING(s) #s
#define STRINGIZE(s) TOSTRING(s)

#define MCU_FW_VERSION_MAJOR              1
#define MCU_FW_VERSION_MINOR              0
#define MCU_FW_VERSION_REVISION           0
#define MCU_FW_VERSION                    STRINGIZE(MCU_FW_VERSION_MAJOR) "." \
                                          STRINGIZE(MCU_FW_VERSION_MINOR) "." \
                                          STRINGIZE(MCU_FW_VERSION_REVISION)

/* Exported functions ------------------------------------------------------- */

#endif /* __COMMON_H */
