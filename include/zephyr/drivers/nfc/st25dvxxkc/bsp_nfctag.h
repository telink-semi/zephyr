/**
  ******************************************************************************
  * @file    bsp_nfctag.h
  * @author  MCD Application Team
  * @brief   Linux support package
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
#ifndef BSP_NFCTAG_H
#define BSP_NFCTAG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
//#include "stm32l4s5i_iot01.h"
#include "st25dvxxkc.h"
#include "st25dv-i2c_zephyr.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L4S5I_IOT01
  * @{
  */
   
/** @addtogroup STM32L4S5I_IOT01_NFCTAG
  * @{
  */
/* Exported types ------------------------------------------------------------*/
/**
 * @brief  NFCTAG status enumerator definition.
 */


/* Exported constants --------------------------------------------------------*/
#define NFCTAG_4K_SIZE            ((uint32_t) 0x200)
#define NFCTAG_16K_SIZE           ((uint32_t) 0x800)
#define NFCTAG_64K_SIZE           ((uint32_t) 0x2000)
   
/**
 * @brief  NFCTAG status enumerator definition.
 */
#define NFCTAG_OK      (0)
#define NFCTAG_ERROR   (-1)
#define NFCTAG_BUSY    (-2)
#define NFCTAG_TIMEOUT (-3)
#define NFCTAG_NACK    (-102)

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

/* External variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported function	--------------------------------------------------------*/
/** @defgroup STM32L4S5I_IOT01_NFCTAG_Exported_Functions
  * @{
  */
int32_t BSP_NFCTAG_Init( uint32_t Instance );
void BSP_NFCTAG_DeInit( uint32_t Instance );
uint8_t BSP_NFCTAG_isInitialized( uint32_t Instance );
int32_t BSP_NFCTAG_ReadID( uint32_t Instance, uint8_t * const wai_id );
int32_t BSP_NFCTAG_ReadData(const struct device *dev, uint32_t Instance, uint8_t * const pData, const uint16_t TarAddr, const uint16_t Size );
int32_t BSP_NFCTAG_WriteData(const struct device *dev, uint32_t Instance, const uint8_t * const pData, const uint16_t TarAddr, const uint16_t Size );
int32_t BSP_NFCTAG_ReadRegister(const struct device *dev, uint32_t Instance, uint8_t * const pData, const uint16_t TarAddr, const uint16_t Size );
int32_t BSP_NFCTAG_WriteRegister(const struct device *dev, uint32_t Instance, const uint8_t * const pData, const uint16_t TarAddr, const uint16_t Size );
int32_t BSP_NFCTAG_IsDeviceReady(const struct device *dev, uint32_t Instance,const uint32_t Trials );

uint32_t BSP_NFCTAG_GetByteSize(const struct device *dev, uint32_t Instance );
int32_t BSP_NFCTAG_ReadMemSize(uint32_t Instance, ST25DVxxKC_MEM_SIZE_t * const pSizeInfo );
int32_t BSP_NFCTAG_ReadRFMngt_Dyn(uint32_t Instance, ST25DVxxKC_RF_MNGT_t * const pRF_Mngt );
int32_t BSP_NFCTAG_SetRFDisable_Dyn(uint32_t Instance);
int32_t BSP_NFCTAG_ResetRFDisable_Dyn(uint32_t Instance);
int32_t BSP_NFCTAG_GetRFSleep_Dyn(uint32_t Instance, ST25DVxxKC_EN_STATUS_E * const pRFSleep );
int32_t BSP_NFCTAG_SetRFSleep_Dyn(uint32_t Instance);
int32_t BSP_NFCTAG_ResetRFSleep_Dyn(uint32_t Instance);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */ 

#endif /* BSP_NFCTAG_H */

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/
