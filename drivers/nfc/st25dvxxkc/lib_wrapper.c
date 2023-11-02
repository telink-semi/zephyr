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
#include <zephyr/drivers/nfc/st25dvxxkc/tagtype5_wrapper.h>
#include <zephyr/drivers/nfc/st25dvxxkc/tagtype4_wrapper.h>
#include <zephyr/drivers/nfc/st25dvxxkc/tagtype3_wrapper.h>

static NFCTAG_Protocol_Id_t gCurrentProtocol = NFCTAG_NOTSET;

uint16_t NfcTag_SelectProtocol(NFCTAG_Protocol_Id_t protocol)
{
  if(protocol >= NFCTAG_NOTSET) {
    gCurrentProtocol = NFCTAG_NOTSET;
    return NDEF_ERROR;
  }
  
  if(protocol == NFCTAG_TYPE1)
    ccFileOffset = 10;
  else if (protocol == NFCTAG_TYPE2)
    ccFileOffset = 12;
  else
    ccFileOffset = 0;

  gCurrentProtocol = protocol;
  return NDEF_OK;
}

uint16_t NfcTag_ReadNDEF( uint8_t* pData )
{
  uint16_t status = NDEF_ERROR;
  switch (gCurrentProtocol)
  {
    case NFCTAG_TYPE4:
      status = NfcType4_ReadNDEF(pData);
    break;
    case NFCTAG_TYPE3:
      status = NfcType3_ReadNDEF(pData);
    break;
    case NFCTAG_TYPE5:
    case NFCTAG_TYPE2:
    case NFCTAG_TYPE1:
      status = NfcType5_ReadNDEF(pData);
    break;
    default:
      status = NDEF_ERROR;
    break;
  }
  return status;
}


uint16_t NfcTag_WriteNDEF(uint16_t Length , uint8_t *pData )
{
  uint16_t status = NDEF_ERROR;
  switch (gCurrentProtocol)
  {
    case NFCTAG_TYPE4:
      status = NfcType4_WriteNDEF(Length, pData);
    break;
    case NFCTAG_TYPE3:
      status = NfcType3_WriteNDEF(Length,pData);
    break;
    case NFCTAG_TYPE5:
    case NFCTAG_TYPE2:
    case NFCTAG_TYPE1:
      status = NfcType5_WriteNDEF(Length, pData);
    break;
    default:
      status = NDEF_ERROR;
    break;
  }
  return status;
}

uint16_t NfcTag_GetLength(uint16_t* Length)
{
  uint16_t status = NDEF_ERROR;
  switch (gCurrentProtocol)
  {
    case NFCTAG_TYPE4:
      status = NfcType4_GetLength(Length);
    break;
    case NFCTAG_TYPE3:
      status = NfcType3_GetLength(Length);
    break;
    case NFCTAG_TYPE5:
    case NFCTAG_TYPE2:
    case NFCTAG_TYPE1:
      status = NfcType5_GetLength(Length);
    break;
    default:
      status = NDEF_ERROR;
    break;
  }
  return status;
}

