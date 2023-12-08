#ifndef __CSR_UTILS_H
#define __CSR_UTILS_H

/***************************************************************************************
 * NDS CSR registers
 ***************************************************************************************/
#define NDS_MXSTATUS                 0x7c4
#define NDS_MCACHE_CTL               0x7ca
#define NDS_MMISC_CTL                0x7d0
#define NDS_MHARTID                  0xf14

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
