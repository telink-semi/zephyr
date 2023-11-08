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

uint16_t NfcType4_GetLength(const struct device *dev, uint16_t* Length)
{
  uint8_t err = NDEF_Wrapper_ReadData(dev, (uint8_t*)Length,0,2);
  *Length = (*Length & 0xFF) << 8 | (*Length & 0xFF00) >> 8;

  if(err != NDEF_OK)
  {
    return NDEF_ERROR;
  }
  return NDEF_OK;
}

uint16_t NfcType4_ReadNDEF(const struct device *dev, uint8_t* pData )
{
  uint16_t length;
  uint8_t err;
  uint16_t status = NfcType4_GetLength(dev, &length);
  if(status != NDEF_OK)
  {
    return status;
  }
  err = NDEF_Wrapper_ReadData(dev, pData,2,length);
  if(err != NDEF_OK)
  {
    return NDEF_ERROR;
  }
  return NDEF_OK;

}

uint16_t NfcType4_WriteNDEF(const struct device *dev, uint16_t Length, uint8_t* pData )
{
  uint8_t txLen[2];
  txLen[0] = Length >> 8;
  txLen[1] = Length & 0xFF;
  uint16_t status = NDEF_Wrapper_WriteData(dev, txLen, 0, 2);
  if(status != NDEF_OK)
  {
    return status;
  }
  return NDEF_Wrapper_WriteData(dev, pData, 2, Length);
}

