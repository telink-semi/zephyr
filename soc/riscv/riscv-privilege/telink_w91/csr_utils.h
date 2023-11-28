#ifndef __CSR_UTILS_H
#define __CSR_UTILS_H

/***************************************************************************************
 * NDS CSR registers
 ***************************************************************************************/
#define NDS_MXSTATUS                 0x7c4
#define NDS_MCACHE_CTL               0x7ca
#define NDS_MMISC_CTL                0x7d0
#define NDS_MVENDORID                0xf11            /* vendor ID: 0000031e */
#define NDS_MARCHID                  0xf12            /* product ID: 80000025 - D25, 80000022 - N22 */
#define NDS_MIMPID                   0xf13            /* product version: [31:8] - major, [7:4] - minor, [3:0] - extension */
#define NDS_MHARTID                  0xf14            /* core ID: 0 - D25, 1 - N22 */

#ifndef __ASSEMBLER__
/***************************************************************************************
 * RISC-V CSR registers utilities
 ***************************************************************************************/
#define read_csr(var, csr)      __asm__ volatile ("csrr %0, %1" : "=r" (var) : "i" (csr))
#define write_csr(csr, val)     __asm__ volatile ("csrw %0, %1" :: "i" (csr), "r" (val))
#define set_csr(csr, bit)       __asm__ volatile ("csrs %0, %1" :: "i" (csr), "r" (bit))
#define clear_csr(csr, bit)     __asm__ volatile ("csrc %0, %1" :: "i" (csr), "r" (bit))
#endif /* __ASSEMBLER__ */

#endif /* __CSR_UTILS_H */
