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

#ifndef _TAGNFCA_H_
#define _TAGNFCA_H_

#include "lib_NDEF.h"

uint16_t NfcType4_GetLength(uint16_t* Length);
uint16_t NfcType4_ReadNDEF( uint8_t* pData );
uint16_t NfcType4_WriteNDEF(uint16_t Length, uint8_t* pData );

#endif // _TAGNFCA_H_
