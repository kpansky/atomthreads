#ifndef __MB_INTERFACE_H__
#define __MB_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*XInterruptHandler) (void *InstancePtr);

void microblaze_enable_interrupts(void);
void microblaze_disable_interrupts(void);
void microblaze_register_handler(XInterruptHandler, void *);

#define mfmsr()     ({ unsigned int _msr; __asm__ __volatile__ ("mfs %0,rmsr\n\t" : "=d"(_msr)); _msr; })
#define mtmsr(_msr) ({ __asm__ __volatile__ ("mts rmsr,%0\n\t nop\n\t" : : "d"(_msr)); })

#ifdef __cplusplus
}
#endif
#endif /* __MB_INTERFACE_H__ */
