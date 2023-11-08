/** COPYRIGHT� 2020 STMicroelectronics, all rights reserved
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  */
#include <zephyr/drivers/nfc/st25dvxxkc/lib_wrapper.h>
#include <zephyr/drivers/nfc/st25dvxxkc/lib_NDEF_config.h>

typedef struct __attribute__((packed)) {
  uint8_t Ver;
  uint8_t Nbr;
  uint8_t Nbw;
  uint16_t  NmaxB;
  uint8_t rfu[4];
  uint8_t WriteFlag;
  uint8_t RWFlag;
  uint8_t Ln[3];
  uint16_t Checksum;
} TT3_Attr_Info_t;

static TT3_Attr_Info_t NDEF_Attr_Info;
uint16_t NfcType3_GetLength(const struct device *dev, uint16_t* Length)
{
  uint32_t t3NdefLen;
  uint8_t err = NDEF_Wrapper_ReadData(dev, (uint8_t*)&NDEF_Attr_Info,0,16);
  if(err != NDEF_OK)
  {
    return NDEF_ERROR;
  }
  t3NdefLen = NDEF_Attr_Info.Ln[0] << 16 | 
              NDEF_Attr_Info.Ln[1] <<  8 |
              NDEF_Attr_Info.Ln[2];
  if(t3NdefLen < 0x10000)
    *Length = t3NdefLen;
  else
    return NDEF_ERROR;
  
  return NDEF_OK;
}

uint16_t NfcType3_ReadNDEF(const struct device *dev, uint8_t* pData )
{
  uint16_t length;
  uint8_t err;
  uint16_t status = NfcType3_GetLength(dev, &length);
  if(status != NDEF_OK)
  {
    return status;
  }
  err = NDEF_Wrapper_ReadData(dev, pData,16,length);
  if(err != NDEF_OK)
  {
    return NDEF_ERROR;
  }
  return NDEF_OK;

}

uint16_t NfcType3_WriteNDEF(const struct device *dev, uint16_t Length, uint8_t* pData )
{
  uint8_t err = NDEF_Wrapper_WriteData(dev, pData,16,Length);
  if(err != NDEF_OK)
  {
    return NDEF_ERROR;
  }

  err = NDEF_Wrapper_ReadData(dev, (uint8_t *)&NDEF_Attr_Info,0,16);
  if(err != NDEF_OK)
  {
    return NDEF_ERROR;
  }
  // update the length field
  NDEF_Attr_Info.Ln[0] = (Length >> 16) & 0xFF;
  NDEF_Attr_Info.Ln[1] = (Length >> 8) & 0xFF;
  NDEF_Attr_Info.Ln[2] = Length & 0xFF;
  err = NDEF_Wrapper_WriteData(dev, (uint8_t *)&NDEF_Attr_Info,0,16);
  if(err != NDEF_OK)
  {
    return NDEF_ERROR;
  }

  return NDEF_OK;
}

uint16_t NfcType3_WriteProprietary(uint16_t Length, uint8_t* pData )
{
  return NDEF_ERROR;
}
