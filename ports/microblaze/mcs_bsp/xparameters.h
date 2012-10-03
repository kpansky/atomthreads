#ifndef __XPARAMETERS_H__
#define __XPARAMETERS_H__

/*****************************************************************************
* Global configuration (modify for your generated Microblaze MCS)
*****************************************************************************/
#define XPAR_CPU_ID                          0
#define XPAR_CPU_CORE_CLOCK_FREQ_HZ          100000000

/*****************************************************************************
* XIOModule configuration (modify for your generated Microblaze MCS)
*****************************************************************************/
#define XPAR_IOMODULE_0_USE_UART_RX          1
#define XPAR_IOMODULE_0_USE_UART_TX          1
#define XPAR_IOMODULE_0_UART_BAUDRATE        115200
#define XPAR_IOMODULE_0_UART_DATA_BITS       8
#define XPAR_IOMODULE_0_UART_USE_PARITY      0
#define XPAR_IOMODULE_0_UART_ODD_PARITY      0
#define XPAR_IOMODULE_0_UART_RX_INTERRUPT    1
#define XPAR_IOMODULE_0_UART_TX_INTERRUPT    1
#define XPAR_IOMODULE_0_UART_ERROR_INTERRUPT 1
#define XPAR_IOMODULE_0_USE_FIT1             0
#define XPAR_IOMODULE_0_FIT1_NO_CLOCKS       0
#define XPAR_IOMODULE_0_FIT1_INTERRUPT       0
#define XPAR_IOMODULE_0_USE_FIT2             0
#define XPAR_IOMODULE_0_FIT2_NO_CLOCKS       0
#define XPAR_IOMODULE_0_FIT2_INTERRUPT       0
#define XPAR_IOMODULE_0_USE_FIT3             0
#define XPAR_IOMODULE_0_FIT3_NO_CLOCKS       0
#define XPAR_IOMODULE_0_FIT3_INTERRUPT       0
#define XPAR_IOMODULE_0_USE_FIT4             0
#define XPAR_IOMODULE_0_FIT4_NO_CLOCKS       0
#define XPAR_IOMODULE_0_FIT4_INTERRUPT       0
#define XPAR_IOMODULE_0_USE_PIT1             1
#define XPAR_IOMODULE_0_PIT1_SIZE            32
#define XPAR_IOMODULE_0_PIT1_READABLE        1
#define XPAR_IOMODULE_0_PIT1_PRESCALER       0
#define XPAR_IOMODULE_0_PIT1_INTERRUPT       1
#define XPAR_IOMODULE_0_USE_PIT2             1
#define XPAR_IOMODULE_0_PIT2_SIZE            32
#define XPAR_IOMODULE_0_PIT2_READABLE        1
#define XPAR_IOMODULE_0_PIT2_PRESCALER       0
#define XPAR_IOMODULE_0_PIT2_INTERRUPT       1
#define XPAR_IOMODULE_0_USE_PIT3             1
#define XPAR_IOMODULE_0_PIT3_SIZE            32
#define XPAR_IOMODULE_0_PIT3_READABLE        1
#define XPAR_IOMODULE_0_PIT3_PRESCALER       0
#define XPAR_IOMODULE_0_PIT3_INTERRUPT       1
#define XPAR_IOMODULE_0_USE_PIT4             1
#define XPAR_IOMODULE_0_PIT4_SIZE            32
#define XPAR_IOMODULE_0_PIT4_READABLE        1
#define XPAR_IOMODULE_0_PIT4_PRESCALER       0
#define XPAR_IOMODULE_0_PIT4_INTERRUPT       1
#define XPAR_IOMODULE_0_USE_GPO1             1
#define XPAR_IOMODULE_0_GPO1_SIZE            32
#define XPAR_IOMODULE_0_GPO1_INIT            0x00000000
#define XPAR_IOMODULE_0_USE_GPO2             1
#define XPAR_IOMODULE_0_GPO2_SIZE            32
#define XPAR_IOMODULE_0_GPO2_INIT            0x00000000
#define XPAR_IOMODULE_0_USE_GPO3             1
#define XPAR_IOMODULE_0_GPO3_SIZE            32
#define XPAR_IOMODULE_0_GPO3_INIT            0x00000000
#define XPAR_IOMODULE_0_USE_GPO4             1
#define XPAR_IOMODULE_0_GPO4_SIZE            32
#define XPAR_IOMODULE_0_GPO4_INIT            0x00000000
#define XPAR_IOMODULE_0_USE_GPI1             1
#define XPAR_IOMODULE_0_GPI1_SIZE            32
#define XPAR_IOMODULE_0_USE_GPI2             1
#define XPAR_IOMODULE_0_GPI2_SIZE            32
#define XPAR_IOMODULE_0_USE_GPI3             1
#define XPAR_IOMODULE_0_GPI3_SIZE            32
#define XPAR_IOMODULE_0_USE_GPI4             1
#define XPAR_IOMODULE_0_GPI4_SIZE            32
#define XPAR_IOMODULE_0_INTC_USE_EXT_INTR    1
#define XPAR_IOMODULE_0_INTC_INTR_SIZE       16
#define XPAR_IOMODULE_0_INTC_LEVEL_EDGE      0x0000
#define XPAR_IOMODULE_0_INTC_POSITIVE        0xFFFF


/*****************************************************************************
* Global constant configuration 
*      (constants for the Microblaze MCS - should not need to be modified)
*****************************************************************************/
#define XPAR_XIOMODULE_NUM_INSTANCES                  1
#define XPAR_IOMODULE_0_DEVICE_ID                     0
#define XPAR_IOMODULE_0_BASEADDR                      0x80000000
#define XPAR_IOMODULE_0_HIGHADDR                      0x8000007F
#define XPAR_IOMODULE_0_IO_BASEADDR                   0xC0000000
#define XPAR_IOMODULE_0_IO_HIGHADDR                   0xFFFFFFFF
#define XPAR_IOMODULE_INTC_MAX_INTR_SIZE              32 
#define XPAR_IOMODULE_SINGLE_BASEADDR                 XPAR_IOMODULE_0_BASEADDR
#define XPAR_IOMODULE_SINGLE_HIGHADDR                 XPAR_IOMODULE_0_HIGHADDR
#define XPAR_IOMODULE_INTC_SINGLE_DEVICE_ID           XPAR_IOMODULE_0_DEVICE_ID
#define XPAR_MICROBLAZE_CORE_CLOCK_FREQ_HZ            XPAR_CPU_CORE_CLOCK_FREQ_HZ
#define XPAR_MICROBLAZE_ID                            XPAR_CPU_ID
#define XPAR_MICROBLAZE_FREQ                          XPAR_CPU_CORE_CLOCK_FREQ_HZ
#define XPAR_MICROBLAZE_ENDIANNESS                    1
#define XPAR_MICROBLAZE_USE_MSR_INSTR                 0
#define XPAR_MICROBLAZE_USE_PCMP_INSTR                0
#define XPAR_MICROBLAZE_USE_BARREL                    0
#define XPAR_MICROBLAZE_USE_DIV                       0
#define XPAR_MICROBLAZE_USE_HW_MUL                    0
#define XPAR_MICROBLAZE_USE_FPU                       0
#define XPAR_MICROBLAZE_USE_STACK_PROTECTION          0
#define XPAR_MICROBLAZE_USE_MMU                       0
#define XPAR_MICROBLAZE_USE_INTERRUPT                 1
#define XPAR_MICROBLAZE_USE_EXT_BRK                   1
#define XPAR_MICROBLAZE_USE_EXT_NM_BRK                1
#define XPAR_MICROBLAZE_USE_BRANCH_TARGET_CACHE       0
#define XPAR_MICROBLAZE_HW_VER                        "8.20.b"

#endif /* __XPARAMETERS_H__ */
