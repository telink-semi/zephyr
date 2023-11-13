/**
  ******************************************************************************
  * @file    lib_NDEF.h
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file help to manage NDEF file.
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT� 2020 STMicroelectronics, all rights reserved
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
#ifndef __LIB_NDEF_H
#define __LIB_NDEF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* include file which match the HW configuration */
#include <zephyr/drivers/nfc/st25dvxxkc/lib_wrapper.h>
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


/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
