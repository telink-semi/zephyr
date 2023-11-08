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
#ifndef _LIB_NDEF_CONFIG_H_
#define _LIB_NDEF_CONFIG_H_

#include <stdint.h>
#include "lib_NDEF.h"

int32_t NDEF_Wrapper_ReadData(const struct device *dev, uint8_t* pData, uint32_t offset, uint32_t length );
int32_t NDEF_Wrapper_WriteData(const struct device *dev, const uint8_t* pData, uint32_t offset, uint32_t length );
uint32_t NDEF_Wrapper_GetMemorySize(const struct device *dev);

#endif // _LIB_NDEF_CONFIG_H_
