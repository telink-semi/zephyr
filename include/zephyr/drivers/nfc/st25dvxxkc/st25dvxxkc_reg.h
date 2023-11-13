/**
  ******************************************************************************
  * @file    st25dvxxkc_reg.h
  * @author  MMY Ecosystem Team
  * @brief   ST25DVXXKC register header file
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2021 STMicroelectronics, all rights reserved
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
#ifndef ST25DVXXKC_REG_H
#define ST25DVXXKC_REG_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <zephyr/device.h>

/** @addtogroup BSP
  * @{
  */ 
 

/** @addtogroup ST25DVXXKC
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/**
 * @brief  ST25DVxxKC Write register function typedef definition.
 */
typedef int32_t(*ST25DVxxKC_WriteReg_Func)(const struct device *, const void *const, const uint16_t, const uint8_t *const, const uint16_t);

/**
 * @brief  ST25DVxxKC Read register function typedef definition.
 */
typedef int32_t(*ST25DVxxKC_ReadReg_Func)(const struct device *, const void *const, const uint16_t, uint8_t *const, const uint16_t);

/**
 * @brief  ST25DVxxKC context structure.
 */
typedef struct
{
  ST25DVxxKC_WriteReg_Func WriteReg;
  ST25DVxxKC_ReadReg_Func ReadReg;
  void *handle;
} ST25DVxxKC_Ctx_t;

/* Exported constants --------------------------------------------------------*/
/* Registers i2c address */
/** @brief ST25DVxxKC GPO1 register address. */
#define ST25DVXXKC_GPO1_REG                      (uint16_t)0x0000
/** @brief ST25DVxxKC GPO2 register address. */
#define ST25DVXXKC_GPO2_REG                      (uint16_t)0x0001
/** @brief ST25DVxxKC Energy Harvesting register address. */
#define ST25DVXXKC_EH_MODE_REG                   (uint16_t)0x0002
/** @brief ST25DVxxKC RF management register address. */
#define ST25DVXXKC_RF_MNGT_REG                   (uint16_t)0x0003
/** @brief ST25DVxxKC Area 1 security register address. */
#define ST25DVXXKC_RFA1SS_REG                    (uint16_t)0x0004
/** @brief ST25DVxxKC Area 1 end address register address. */
#define ST25DVXXKC_ENDA1_REG                     (uint16_t)0x0005
/** @brief ST25DVxxKC Area 2 security register address. */
#define ST25DVXXKC_RFA2SS_REG                    (uint16_t)0x0006
/** @brief ST25DVxxKC Area 2 end address register address. */
#define ST25DVXXKC_ENDA2_REG                     (uint16_t)0x0007
/** @brief ST25DVxxKC Area 3 security register address. */
#define ST25DVXXKC_RFA3SS_REG                    (uint16_t)0x0008
/** @brief ST25DVxxKC Area 3 end address register address. */
#define ST25DVXXKC_ENDA3_REG                     (uint16_t)0x0009
/** @brief ST25DVxxKC Area 4 security register address. */
#define ST25DVXXKC_RFA4SS_REG                    (uint16_t)0x000A
/** @brief ST25DVxxKC I2C security register address. */
#define ST25DVXXKC_I2CSS_REG                     (uint16_t)0x000B
/** @brief ST25DVxxKC Capability Container lock register address. */
#define ST25DVXXKC_LOCKCCFILE_REG                (uint16_t)0x000C
/** @brief ST25DVxxKC Mailbox mode register address. */
#define ST25DVXXKC_FTM_REG                       (uint16_t)0x000D
/** @brief ST25DVxxKC I2C configuration register address. */
#define ST25DVXXKC_I2CCFG_REG                    (uint16_t)0x000E
/** @brief ST25DVxxKC Configuration lock register address. */
#define ST25DVXXKC_LOCKCFG_REG                   (uint16_t)0x000F
/** @brief ST25DVxxKC DSFID lock register address. */
#define ST25DVXXKC_LOCKDSFID_REG                 (uint16_t)0x0010
/** @brief ST25DVxxKC AFI lock register address. */
#define ST25DVXXKC_LOCKAFI_REG                   (uint16_t)0x0011
/** @brief ST25DVxxKC DSFID register address. */
#define ST25DVXXKC_DSFID_REG                     (uint16_t)0x0012
/** @brief ST25DVxxKC AFI register address. */
#define ST25DVXXKC_AFI_REG                       (uint16_t)0x0013
/** @brief ST25DVxxKC Memory size register address. */
#define ST25DVXXKC_MEM_SIZE_LSB_REG              (uint16_t)0x0014
/** @brief ST25DVxxKC Memory size register address. */
#define ST25DVXXKC_MEM_SIZE_MSB_REG              (uint16_t)0x0015
/** @brief ST25DVxxKC Block size register address. */
#define ST25DVXXKC_BLK_SIZE_REG                  (uint16_t)0x0016
/** @brief ST25DVxxKC ICref register address. */
#define ST25DVXXKC_ICREF_REG                     (uint16_t)0x0017
/** @brief ST25DVxxKC UID register address. */
#define ST25DVXXKC_UID_REG                       (uint16_t)0x0018
/** @brief ST25DVxxKC IC revision register address. */
#define ST25DVXXKC_ICREV_REG                     (uint16_t)0x0020
/** @brief ST25DVxxKC I2C password register address. */
#define ST25DVXXKC_I2CPASSWD_REG                 (uint16_t)0x0900

/* Dynamic Registers i2c address */
/** @brief ST25DVxxKC GPO dynamic register address. */
#define ST25DVXXKC_GPO_DYN_REG                   (uint16_t)0x2000
/** @brief ST25DVxxKC Energy Harvesting control dynamic register address. */
#define ST25DVXXKC_EH_CTRL_DYN_REG               (uint16_t)0x2002
/** @brief ST25DVxxKC RF management dynamic register address. */
#define ST25DVXXKC_RF_MNGT_DYN_REG               (uint16_t)0x2003
/** @brief ST25DVxxKC I2C secure session opened dynamic register address. */
#define ST25DVXXKC_I2C_SSO_DYN_REG               (uint16_t)0x2004
/** @brief ST25DVxxKC Interrupt status dynamic register address. */
#define ST25DVXXKC_ITSTS_DYN_REG                 (uint16_t)0x2005
/** @brief ST25DVxxKC Mailbox control dynamic register address. */
#define ST25DVXXKC_MB_CTRL_DYN_REG               (uint16_t)0x2006
/** @brief ST25DVxxKC Mailbox message length dynamic register address. */
#define ST25DVXXKC_MBLEN_DYN_REG                 (uint16_t)0x2007
/** @brief ST25DVxxKC Mailbox buffer address. */
#define ST25DVXXKC_MAILBOX_RAM_REG               (uint16_t)0x2008

/* Registers fields definitions */
/* FTM */
#define ST25DVXXKC_FTM_MBMODE_SHIFT              (uint8_t)(0)
#define ST25DVXXKC_FTM_MBMODE_FIELD              (uint8_t)0xFE
#define ST25DVXXKC_FTM_MBMODE_MASK               (uint8_t)0x01
#define ST25DVXXKC_FTM_MBWDG_SHIFT               (uint8_t)(1)
#define ST25DVXXKC_FTM_MBWDG_FIELD               (uint8_t)0xF8
#define ST25DVXXKC_FTM_MBWDG_MASK                (uint8_t)0x0E

/* MB_LEN_Dyn */
#define ST25DVXXKC_MBLEN_DYN_MBLEN_SHIFT         (uint8_t)(0)
#define ST25DVXXKC_MBLEN_DYN_MBLEN_FIELD         (uint8_t)0x00
#define ST25DVXXKC_MBLEN_DYN_MBLEN_MASK          (uint8_t)0xFF

/* MB_CTRL_Dyn */
#define ST25DVXXKC_MB_CTRL_DYN_MBEN_SHIFT        (uint8_t)(0)
#define ST25DVXXKC_MB_CTRL_DYN_MBEN_FIELD        (uint8_t)0xFE
#define ST25DVXXKC_MB_CTRL_DYN_MBEN_MASK         (uint8_t)0x01
#define ST25DVXXKC_MB_CTRL_DYN_HOSTPUTMSG_SHIFT  (uint8_t)(1)
#define ST25DVXXKC_MB_CTRL_DYN_HOSTPUTMSG_FIELD  (uint8_t)0xFD
#define ST25DVXXKC_MB_CTRL_DYN_HOSTPUTMSG_MASK   (uint8_t)0x02
#define ST25DVXXKC_MB_CTRL_DYN_RFPUTMSG_SHIFT    (uint8_t)(2)
#define ST25DVXXKC_MB_CTRL_DYN_RFPUTMSG_FIELD    (uint8_t)0xFB
#define ST25DVXXKC_MB_CTRL_DYN_RFPUTMSG_MASK     (uint8_t)0x04
#define ST25DVXXKC_MB_CTRL_DYN_STRESERVED_SHIFT  (uint8_t)(3)
#define ST25DVXXKC_MB_CTRL_DYN_STRESERVED_FIELD  (uint8_t)0xF7
#define ST25DVXXKC_MB_CTRL_DYN_STRESERVED_MASK   (uint8_t)0x08
#define ST25DVXXKC_MB_CTRL_DYN_HOSTMISSMSG_SHIFT (uint8_t)(4)
#define ST25DVXXKC_MB_CTRL_DYN_HOSTMISSMSG_FIELD (uint8_t)0xEF
#define ST25DVXXKC_MB_CTRL_DYN_HOSTMISSMSG_MASK  (uint8_t)0x10
#define ST25DVXXKC_MB_CTRL_DYN_RFMISSMSG_SHIFT   (uint8_t)(5)
#define ST25DVXXKC_MB_CTRL_DYN_RFMISSMSG_FIELD   (uint8_t)0xDF
#define ST25DVXXKC_MB_CTRL_DYN_RFMISSMSG_MASK    (uint8_t)0x20
#define ST25DVXXKC_MB_CTRL_DYN_CURRENTMSG_SHIFT  (uint8_t)(6)
#define ST25DVXXKC_MB_CTRL_DYN_CURRENTMSG_FIELD  (uint8_t)0x3F
#define ST25DVXXKC_MB_CTRL_DYN_CURRENTMSG_MASK   (uint8_t)0xC0


/* I2C_CFG */
#define ST25DVXXKC_I2CCFG_DEVICECODE_SHIFT       (uint8_t)(0)
#define ST25DVXXKC_I2CCFG_DEVICECODE_FIELD       (uint8_t)0xF0
#define ST25DVXXKC_I2CCFG_DEVICECODE_MASK        (uint8_t)0x0F
#define ST25DVXXKC_I2CCFG_E0_SHIFT               (uint8_t)(4)
#define ST25DVXXKC_I2CCFG_E0_FIELD               (uint8_t)0xEF
#define ST25DVXXKC_I2CCFG_E0_MASK                (uint8_t)0x10
#define ST25DVXXKC_I2CCFG_RFSWITCHOFF_SHIFT      (uint8_t)(5)
#define ST25DVXXKC_I2CCFG_RFSWITCHOFF_FIELD      (uint8_t)0xDF
#define ST25DVXXKC_I2CCFG_RFSWITCHOFF_MASK       (uint8_t)0x20

/* GPO_1 */
#define ST25DVXXKC_GPO1_ENABLE_SHIFT             (uint8_t)(0)
#define ST25DVXXKC_GPO1_ENABLE_FIELD             (uint8_t)0xFE
#define ST25DVXXKC_GPO1_ENABLE_MASK              (uint8_t)0x01
#define ST25DVXXKC_GPO1_RFUSERSTATE_SHIFT        (uint8_t)(1)
#define ST25DVXXKC_GPO1_RFUSERSTATE_FIELD        (uint8_t)0xFD
#define ST25DVXXKC_GPO1_RFUSERSTATE_MASK         (uint8_t)0x02
#define ST25DVXXKC_GPO1_RFACTIVITY_SHIFT         (uint8_t)(2)
#define ST25DVXXKC_GPO1_RFACTIVITY_FIELD         (uint8_t)0xFB
#define ST25DVXXKC_GPO1_RFACTIVITY_MASK          (uint8_t)0x04
#define ST25DVXXKC_GPO1_RFINTERRUPT_SHIFT        (uint8_t)(3)
#define ST25DVXXKC_GPO1_RFINTERRUPT_FIELD        (uint8_t)0xF7
#define ST25DVXXKC_GPO1_RFINTERRUPT_MASK         (uint8_t)0x08
#define ST25DVXXKC_GPO1_FIELDCHANGE_SHIFT        (uint8_t)(4)
#define ST25DVXXKC_GPO1_FIELDCHANGE_FIELD        (uint8_t)0xEF
#define ST25DVXXKC_GPO1_FIELDCHANGE_MASK         (uint8_t)0x10
#define ST25DVXXKC_GPO1_RFPUTMSG_SHIFT           (uint8_t)(5)
#define ST25DVXXKC_GPO1_RFPUTMSG_FIELD           (uint8_t)0xDF
#define ST25DVXXKC_GPO1_RFPUTMSG_MASK            (uint8_t)0x20
#define ST25DVXXKC_GPO1_RFGETMSG_SHIFT           (uint8_t)(6)
#define ST25DVXXKC_GPO1_RFGETMSG_FIELD           (uint8_t)0xBF
#define ST25DVXXKC_GPO1_RFGETMSG_MASK            (uint8_t)0x40
#define ST25DVXXKC_GPO1_RFWRITE_SHIFT            (uint8_t)(7)
#define ST25DVXXKC_GPO1_RFWRITE_FIELD            (uint8_t)0x7F
#define ST25DVXXKC_GPO1_RFWRITE_MASK             (uint8_t)0x80
#define ST25DVXXKC_GPO1_ALL_SHIFT                (uint8_t)(0)
#define ST25DVXXKC_GPO1_ALL_MASK                 (uint8_t)0xFF

/* GPO_2 */
#define ST25DVXXKC_GPO2_I2CWRITEENABLE_SHIFT     (uint8_t)(0)
#define ST25DVXXKC_GPO2_I2CWRITEENABLE_FIELD     (uint8_t)0xFE
#define ST25DVXXKC_GPO2_I2CWRITEENABLE_MASK      (uint8_t)0x01
#define ST25DVXXKC_GPO2_RFOFF_SHIFT              (uint8_t)(1)
#define ST25DVXXKC_GPO2_RFOFF_FIELD              (uint8_t)0xFD
#define ST25DVXXKC_GPO2_RFOFF_MASK               (uint8_t)0x02
#define ST25DVXXKC_GPO2_ITTIME_SHIFT             (uint8_t)(2)
#define ST25DVXXKC_GPO2_ITTIME_FIELD             (uint8_t)0xE3
#define ST25DVXXKC_GPO2_ITTIME_MASK              (uint8_t)0x1C
#define ST25DVXXKC_GPO2_ALL_SHIFT                (uint8_t)(0)
#define ST25DVXXKC_GPO2_ALL_MASK                 (uint8_t)0x1F

/* GPO_Dyn */
#define ST25DVXXKC_GPO_DYN_ENABLE_SHIFT          (uint8_t)(0)
#define ST25DVXXKC_GPO_DYN_ENABLE_FIELD          (uint8_t)0xFE
#define ST25DVXXKC_GPO_DYN_ENABLE_MASK           (uint8_t)0x01
#define ST25DVXXKC_GPO_DYN_ALL_SHIFT             (uint8_t)(0)
#define ST25DVXXKC_GPO_DYN_ALL_MASK              (uint8_t)0x01

/* ITSTS_Dyn */
#define ST25DVXXKC_ITSTS_DYN_RFUSERSTATE_SHIFT       (uint8_t)(0)
#define ST25DVXXKC_ITSTS_DYN_RFUSERSTATE_FIELD       (uint8_t)0xFE
#define ST25DVXXKC_ITSTS_DYN_RFUSERSTATE_MASK        (uint8_t)0x01
#define ST25DVXXKC_ITSTS_DYN_RFACTIVITY_SHIFT        (uint8_t)(1)
#define ST25DVXXKC_ITSTS_DYN_RFACTIVITY_FIELD        (uint8_t)0xFD
#define ST25DVXXKC_ITSTS_DYN_RFACTIVITY_MASK         (uint8_t)0x02
#define ST25DVXXKC_ITSTS_DYN_RFINTERRUPT_SHIFT       (uint8_t)(2)
#define ST25DVXXKC_ITSTS_DYN_RFINTERRUPT_FIELD       (uint8_t)0xFB
#define ST25DVXXKC_ITSTS_DYN_RFINTERRUPT_MASK        (uint8_t)0x04
#define ST25DVXXKC_ITSTS_DYN_FIELDFALLING_SHIFT      (uint8_t)(3)
#define ST25DVXXKC_ITSTS_DYN_FIELDFALLING_FIELD      (uint8_t)0xF7
#define ST25DVXXKC_ITSTS_DYN_FIELDFALLING_MASK       (uint8_t)0x08
#define ST25DVXXKC_ITSTS_DYN_FIELDRISING_SHIFT       (uint8_t)(4)
#define ST25DVXXKC_ITSTS_DYN_FIELDRISING_FIELD       (uint8_t)0xEF
#define ST25DVXXKC_ITSTS_DYN_FIELDRISING_MASK        (uint8_t)0x10
#define ST25DVXXKC_ITSTS_DYN_RFPUTMSG_SHIFT          (uint8_t)(5)
#define ST25DVXXKC_ITSTS_DYN_RFPUTMSG_FIELD          (uint8_t)0xDF
#define ST25DVXXKC_ITSTS_DYN_RFPUTMSG_MASK           (uint8_t)0x20
#define ST25DVXXKC_ITSTS_DYN_RFGETMSG_SHIFT          (uint8_t)(6)
#define ST25DVXXKC_ITSTS_DYN_RFGETMSG_FIELD          (uint8_t)0xBF
#define ST25DVXXKC_ITSTS_DYN_RFGETMSG_MASK           (uint8_t)0x40
#define ST25DVXXKC_ITSTS_DYN_RFWRITE_SHIFT           (uint8_t)(7)
#define ST25DVXXKC_ITSTS_DYN_RFWRITE_FIELD           (uint8_t)0x7F
#define ST25DVXXKC_ITSTS_DYN_RFWRITE_MASK            (uint8_t)0x80

/* EH_MODE */
#define ST25DVXXKC_EH_MODE_SHIFT                 (uint8_t)(0)
#define ST25DVXXKC_EH_MODE_FIELD                 (uint8_t)0xFE
#define ST25DVXXKC_EH_MODE_MASK                  (uint8_t)0x01

/* EH_CTRL_Dyn */
#define ST25DVXXKC_EH_CTRL_DYN_EH_EN_SHIFT       (uint8_t)(0)
#define ST25DVXXKC_EH_CTRL_DYN_EH_EN_FIELD       (uint8_t)0xFE
#define ST25DVXXKC_EH_CTRL_DYN_EH_EN_MASK        (uint8_t)0x01
#define ST25DVXXKC_EH_CTRL_DYN_EH_ON_SHIFT       (uint8_t)(1)
#define ST25DVXXKC_EH_CTRL_DYN_EH_ON_FIELD       (uint8_t)0xFD
#define ST25DVXXKC_EH_CTRL_DYN_EH_ON_MASK        (uint8_t)0x02
#define ST25DVXXKC_EH_CTRL_DYN_FIELD_ON_SHIFT    (uint8_t)(2)
#define ST25DVXXKC_EH_CTRL_DYN_FIELD_ON_FIELD    (uint8_t)0xFB
#define ST25DVXXKC_EH_CTRL_DYN_FIELD_ON_MASK     (uint8_t)0x04
#define ST25DVXXKC_EH_CTRL_DYN_VCC_ON_SHIFT      (uint8_t)(3)
#define ST25DVXXKC_EH_CTRL_DYN_VCC_ON_FIELD      (uint8_t)0xF7
#define ST25DVXXKC_EH_CTRL_DYN_VCC_ON_MASK       (uint8_t)0x08
#define ST25DVXXKC_EH_CTRL_DYN_ALL_SHIFT         (uint8_t)(0)
#define ST25DVXXKC_EH_CTRL_DYN_ALL_MASK          (uint8_t)0x0F

/* RF_MNGT */
#define ST25DVXXKC_RF_MNGT_RFDIS_SHIFT           (uint8_t)(0)
#define ST25DVXXKC_RF_MNGT_RFDIS_FIELD           (uint8_t)0xFE
#define ST25DVXXKC_RF_MNGT_RFDIS_MASK            (uint8_t)0x01
#define ST25DVXXKC_RF_MNGT_RFSLEEP_SHIFT         (uint8_t)(1)
#define ST25DVXXKC_RF_MNGT_RFSLEEP_FIELD         (uint8_t)0xFD
#define ST25DVXXKC_RF_MNGT_RFSLEEP_MASK          (uint8_t)0x02
#define ST25DVXXKC_RF_MNGT_ALL_SHIFT             (uint8_t)(0)
#define ST25DVXXKC_RF_MNGT_ALL_MASK              (uint8_t)0x03

/* RF_MNGT_Dyn */
#define ST25DVXXKC_RF_MNGT_DYN_RFDIS_SHIFT       (uint8_t)(0)
#define ST25DVXXKC_RF_MNGT_DYN_RFDIS_FIELD       (uint8_t)0xFE
#define ST25DVXXKC_RF_MNGT_DYN_RFDIS_MASK        (uint8_t)0x01
#define ST25DVXXKC_RF_MNGT_DYN_RFSLEEP_SHIFT     (uint8_t)(1)
#define ST25DVXXKC_RF_MNGT_DYN_RFSLEEP_FIELD     (uint8_t)0xFD
#define ST25DVXXKC_RF_MNGT_DYN_RFSLEEP_MASK      (uint8_t)0x02
#define ST25DVXXKC_RF_MNGT_DYN_RFOFF_SHIFT       (uint8_t)(2)
#define ST25DVXXKC_RF_MNGT_DYN_RFOFF_FIELD       (uint8_t)0xFB
#define ST25DVXXKC_RF_MNGT_DYN_RFOFF_MASK        (uint8_t)0x04
#define ST25DVXXKC_RF_MNGT_DYN_ALL_SHIFT         (uint8_t)(0)
#define ST25DVXXKC_RF_MNGT_DYN_ALL_MASK          (uint8_t)0x07

/* RFA1SS */
#define ST25DVXXKC_RFA1SS_PWDCTRL_SHIFT           (uint8_t)(0)
#define ST25DVXXKC_RFA1SS_PWDCTRL_FIELD           (uint8_t)0xFC
#define ST25DVXXKC_RFA1SS_PWDCTRL_MASK            (uint8_t)0x03
#define ST25DVXXKC_RFA1SS_RWPROT_SHIFT            (uint8_t)(2)
#define ST25DVXXKC_RFA1SS_RWPROT_FIELD            (uint8_t)0xF3
#define ST25DVXXKC_RFA1SS_RWPROT_MASK             (uint8_t)0x0C
#define ST25DVXXKC_RFA1SS_ALL_SHIFT               (uint8_t)(0)
#define ST25DVXXKC_RFA1SS_ALL_MASK                (uint8_t)0x0F

/* RFA2SS */
#define ST25DVXXKC_RFA2SS_PWDCTRL_SHIFT           (uint8_t)(0)
#define ST25DVXXKC_RFA2SS_PWDCTRL_FIELD           (uint8_t)0xFC
#define ST25DVXXKC_RFA2SS_PWDCTRL_MASK            (uint8_t)0x03
#define ST25DVXXKC_RFA2SS_RWPROT_SHIFT            (uint8_t)(2)
#define ST25DVXXKC_RFA2SS_RWPROT_FIELD            (uint8_t)0xF3
#define ST25DVXXKC_RFA2SS_RWPROT_MASK             (uint8_t)0x0C
#define ST25DVXXKC_RFA2SS_ALL_SHIFT               (uint8_t)(0)
#define ST25DVXXKC_RFA2SS_ALL_MASK                (uint8_t)0x0F

/* RFA3SS */
#define ST25DVXXKC_RFA3SS_PWDCTRL_SHIFT           (uint8_t)(0)
#define ST25DVXXKC_RFA3SS_PWDCTRL_FIELD           (uint8_t)0xFC
#define ST25DVXXKC_RFA3SS_PWDCTRL_MASK            (uint8_t)0x03
#define ST25DVXXKC_RFA3SS_RWPROT_SHIFT            (uint8_t)(2)
#define ST25DVXXKC_RFA3SS_RWPROT_FIELD            (uint8_t)0xF3
#define ST25DVXXKC_RFA3SS_RWPROT_MASK             (uint8_t)0x0C
#define ST25DVXXKC_RFA3SS_ALL_SHIFT               (uint8_t)(0)
#define ST25DVXXKC_RFA3SS_ALL_MASK                (uint8_t)0x0F

/* RFA4SS */
#define ST25DVXXKC_RFA4SS_PWDCTRL_SHIFT           (uint8_t)(0)
#define ST25DVXXKC_RFA4SS_PWDCTRL_FIELD           (uint8_t)0xFC
#define ST25DVXXKC_RFA4SS_PWDCTRL_MASK            (uint8_t)0x03
#define ST25DVXXKC_RFA4SS_RWPROT_SHIFT            (uint8_t)(2)
#define ST25DVXXKC_RFA4SS_RWPROT_FIELD            (uint8_t)0xF3
#define ST25DVXXKC_RFA4SS_RWPROT_MASK             (uint8_t)0x0C
#define ST25DVXXKC_RFA4SS_ALL_SHIFT               (uint8_t)(0)
#define ST25DVXXKC_RFA4SS_ALL_MASK                (uint8_t)0x0F

/* I2CSS */
#define ST25DVXXKC_I2CSS_PZ1_SHIFT              (uint8_t)(0)
#define ST25DVXXKC_I2CSS_PZ1_FIELD              (uint8_t)0xFC
#define ST25DVXXKC_I2CSS_PZ1_MASK               (uint8_t)0x03
#define ST25DVXXKC_I2CSS_PZ2_SHIFT              (uint8_t)(2)
#define ST25DVXXKC_I2CSS_PZ2_FIELD              (uint8_t)0xF3
#define ST25DVXXKC_I2CSS_PZ2_MASK               (uint8_t)0x0C
#define ST25DVXXKC_I2CSS_PZ3_SHIFT              (uint8_t)(4)
#define ST25DVXXKC_I2CSS_PZ3_FIELD              (uint8_t)0xCF
#define ST25DVXXKC_I2CSS_PZ3_MASK               (uint8_t)0x30
#define ST25DVXXKC_I2CSS_PZ4_SHIFT              (uint8_t)(6)
#define ST25DVXXKC_I2CSS_PZ4_FIELD              (uint8_t)0x3F
#define ST25DVXXKC_I2CSS_PZ4_MASK               (uint8_t)0xC0

/* LOCKCCFILE */
#define ST25DVXXKC_LOCKCCFILE_BLCK0_SHIFT        (uint8_t)(0)
#define ST25DVXXKC_LOCKCCFILE_BLCK0_FIELD        (uint8_t)0xFE
#define ST25DVXXKC_LOCKCCFILE_BLCK0_MASK         (uint8_t)0x01
#define ST25DVXXKC_LOCKCCFILE_BLCK1_SHIFT        (uint8_t)(1)
#define ST25DVXXKC_LOCKCCFILE_BLCK1_FIELD        (uint8_t)0xFD
#define ST25DVXXKC_LOCKCCFILE_BLCK1_MASK         (uint8_t)0x02
#define ST25DVXXKC_LOCKCCFILE_ALL_SHIFT          (uint8_t)(0)
#define ST25DVXXKC_LOCKCCFILE_ALL_MASK           (uint8_t)0x03

/* LOCKCFG */
#define ST25DVXXKC_LOCKCFG_B0_SHIFT              (uint8_t)(0)
#define ST25DVXXKC_LOCKCFG_B0_FIELD              (uint8_t)0xFE
#define ST25DVXXKC_LOCKCFG_B0_MASK               (uint8_t)0x01

/* I2C_SSO_Dyn */
#define ST25DVXXKC_I2C_SSO_DYN_I2CSSO_SHIFT      (uint8_t)(0)
#define ST25DVXXKC_I2C_SSO_DYN_I2CSSO_FIELD      (uint8_t)0xFE
#define ST25DVXXKC_I2C_SSO_DYN_I2CSSO_MASK       (uint8_t)0x01

/**
 * @brief  NFCTAG status enumerator definition.
 */
#define NFCTAG_OK      (0)
#define NFCTAG_ERROR   (-1)
#define NFCTAG_BUSY    (-2)
#define NFCTAG_TIMEOUT (-3)
#define NFCTAG_NACK    (-102)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int32_t ST25DVxxKC_GetICREF(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value);
int32_t ST25DVxxKC_GetMEM_SIZE_MSB(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value);
int32_t ST25DVxxKC_GetBLK_SIZE(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value);
int32_t ST25DVxxKC_GetMEM_SIZE_LSB(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value);
int32_t ST25DVxxKC_GetRF_MNGT_DYN_RFSLEEP(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, uint8_t *const value);
int32_t ST25DVxxKC_SetRF_MNGT_DYN_RFSLEEP(const struct device *dev, const ST25DVxxKC_Ctx_t *const ctx, const uint8_t *const value);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
  }
#endif

#endif /* ST25DVXXKC_REG_H */

/******************* (C) COPYRIGHT 2021 STMicroelectronics *****END OF FILE****/

