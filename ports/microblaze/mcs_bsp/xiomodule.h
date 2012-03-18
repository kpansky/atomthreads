#ifndef __IOMODULE_H__
#define __IOMODULE_H__

#include "xparameters.h"
#include "mb_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
* Basic
*****************************************************************************/

/* Basic types */
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL  (void *)0
#endif
#ifndef u8
#define u8    unsigned char
#endif
#ifndef u16
#define u16   unsigned short
#endif
#ifndef u32
#define u32   unsigned int
#endif
#ifndef u64
#define u64   unsigned long long
#endif
#ifndef s8
#define s8    signed char
#endif
#ifndef s16
#define s16   signed short
#endif
#ifndef s32
#define s32   signed int
#endif
#ifndef s64
#define s64   signed long long
#endif

#define XIL_COMPONENT_IS_READY   0x11111111
#define XIL_COMPONENT_IS_STARTED 0x22222222
#define XST_SUCCESS              0x00
#define XST_FAILURE              0x01


/* Basic access macros */
#define XIo_In8(Addr)        (*(volatile  u8 *)(Addr))
#define XIo_In16(Addr)       (*(volatile u16 *)(Addr))
#define XIo_In32(Addr)       (*(volatile u32 *)(Addr))
#define XIo_Out8(Addr, Val)  (*(volatile  u8 *)((Addr)) = (Val))
#define XIo_Out16(Addr, Val) (*(volatile u16 *)((Addr)) = (Val))
#define XIo_Out32(Addr, Val) (*(volatile u32 *)((Addr)) = (Val))

char inbyte(void);
void outbyte(char);

/*****************************************************************************
* IOModule Interrupt Controller - Defines and Data Structures
*****************************************************************************/

/* Register field bitmasks */
#define XIN_SVC_SGL_ISR_OPTION                 0x1
#define XIN_SVC_ALL_ISRS_OPTION                0x2

/* Register address offsets */
#define XIN_ISR_OFFSET                         0x00000030
#define XIN_IPR_OFFSET                         0x00000034
#define XIN_IER_OFFSET                         0x00000038
#define XIN_IAR_OFFSET                         0x0000003C

/* Interrupt enumerations */
#define XIN_IOMODULE_FIT_4_INTERRUPT_INTR      10
#define XIN_IOMODULE_FIT_3_INTERRUPT_INTR      9
#define XIN_IOMODULE_FIT_2_INTERRUPT_INTR      8
#define XIN_IOMODULE_FIT_1_INTERRUPT_INTR      7
#define XIN_IOMODULE_PIT_4_INTERRUPT_INTR      6
#define XIN_IOMODULE_PIT_3_INTERRUPT_INTR      5
#define XIN_IOMODULE_PIT_2_INTERRUPT_INTR      4
#define XIN_IOMODULE_PIT_1_INTERRUPT_INTR      3
#define XIN_IOMODULE_UART_RX_INTERRUPT_INTR    2
#define XIN_IOMODULE_UART_TX_INTERRUPT_INTR    1
#define XIN_IOMODULE_UART_ERROR_INTERRUPT_INTR 0
#define XIN_IOMODULE_EXTERNAL_INTERRUPT_INTR   16

/* Data types */
typedef struct {
  XInterruptHandler Handler;
  void *CallBackRef;
} XIOModule_Intc_VectorTableEntry;


/*****************************************************************************
* IOModule GPIO - Defines and Data Structures
*****************************************************************************/

/* General */
#define XGPO_DEVICE_COUNT       4
#define XGPI_DEVICE_COUNT       4

/* Register address offsets */
#define XGO_OUT_OFFSET          0x00000010
#define XGI_IN_OFFSET           0x00000020
#define XGPO_DATA_OFFSET        XGO_OUT_OFFSET
#define XGPO_CHAN_OFFSET        4
#define XGPI_DATA_OFFSET        XGI_IN_OFFSET
#define XGPI_CHAN_OFFSET        4


/*****************************************************************************
* IOModule UART - Defines and Data Structures
*****************************************************************************/

/* Register field bitmasks */
#define XUL_SR_PARITY_ERROR       0x80
#define XUL_SR_FRAMING_ERROR      0x40
#define XUL_SR_OVERRUN_ERROR      0x20
#define XUL_SR_INTR_ENABLED       0x10
#define XUL_SR_TX_FIFO_FULL       0x08
#define XUL_SR_RX_FIFO_VALID_DATA 0x01
#define XUL_STOP_BITS             1
#define XUL_PARITY_NONE           0
#define XUL_PARITY_ODD            1
#define XUL_PARITY_EVEN           2

/* Register address offsets */
#define XUL_RX_OFFSET             0x00000000
#define XUL_TX_OFFSET             0x00000004
#define XUL_STATUS_REG_OFFSET     0x00000008

/* Interrupt Callback profile */
typedef void (*XIOModule_Uart_Handler)(void *CallBackRef, u32 ByteCount);


/*****************************************************************************
* IOModule PIT - Defines and Data Structures
*****************************************************************************/

/* Register field bitmasks */
#define XTC_INT_MODE_OPTION       0x00
#define XTC_AUTO_RELOAD_OPTION    0x02
#define XTC_CSR_ENABLE_TMR_MASK   0x01
#define XTC_CSR_AUTO_RELOAD_MASK  0x02

/* Register address offsets */
#define XTC_TLR_OFFSET            0x00000040
#define XTC_TCR_OFFSET            0x00000044
#define XTC_TCSR_OFFSET           0x00000048

/* General */
#define XTC_DEVICE_TIMER_COUNT    4

/*****************************************************************************
* IOModule General - Defines and Data Structures
*****************************************************************************/

/* Data types */
typedef struct {
  u16 DeviceId;
  u32 BaseAddress;
  u32 IoBaseAddress;
  u32 IntcAckBeforeService;
  u32 IntcOptions;
  u8  PitUsed[XTC_DEVICE_TIMER_COUNT];
  u8  PitSize[XTC_DEVICE_TIMER_COUNT];
  u8  PitPrescaler[XTC_DEVICE_TIMER_COUNT];
  u8  PitReadable[XTC_DEVICE_TIMER_COUNT];
  u32 GpoInit[XGPO_DEVICE_COUNT];
  XIOModule_Intc_VectorTableEntry HandlerTable[XPAR_IOMODULE_INTC_MAX_INTR_SIZE];
} XIOModule_Config;

typedef struct {
  u32 IsReady;
  u32 IsStarted;
  XIOModule_Config *CfgPtr;
  u32 GpoValue[XGPO_DEVICE_COUNT];
  u32 IntEnabledMask;
  u32 TimerTCSR[XTC_DEVICE_TIMER_COUNT];
  u32 TimerTLR[XTC_DEVICE_TIMER_COUNT];
} XIOModule;


/*****************************************************************************
* IOModule General - Functions
*****************************************************************************/

/* Functions and Function-like Macros */
s32     XIOModule_Initialize(XIOModule * InstancePtr, u16 DeviceId);
s32     XIOModule_Start(XIOModule * InstancePtr);
void    XIOModule_Stop(XIOModule * InstancePtr);
u32     XIOModule_IoReadWord(XIOModule *InstancePtr, u32 ByteOffset);
u16     XIOModule_IoReadHalfword(XIOModule *InstancePtr, u32 ByteOffset);
u8      XIOModule_IoReadByte(XIOModule *InstancePtr, u32 ByteOffset);
void    XIOModule_IoWriteWord(XIOModule *InstancePtr, u32 ByteOffset, u32 Data);
void    XIOModule_IoWriteHalfword(XIOModule *InstancePtr, u32 ByteOffset, u16 Data);
void    XIOModule_IoWriteByte(XIOModule *InstancePtr, u32 ByteOffset, u8 Data);

/*****************************************************************************
* IOModule Interrupt Controller - Functions
*****************************************************************************/

/* Provide compatability for the haphazard IOModule namespace */
#define XIOModule_VectorTableEntry             XIOModule_Intc_VectorTableEntry
#define XIOModule_LowLevelInterruptHandler     XIOModule_Intc_LowLevelInterruptHandler
#define XIOModule_DeviceInterruptHandler       XIOModule_Intc_DeviceInterruptHandler
#define XIOModule_RegisterHandler              XIOModule_Intc_RegisterHandler
#define XIOModule_Connect                      XIOModule_Intc_Connect
#define XIOModule_Disconnect                   XIOModule_Intc_Disconnect
#define XIOModule_Enable                       XIOModule_Intc_Enable
#define XIOModule_Disable                      XIOModule_Intc_Disable
#define XIOModule_Acknowledge                  XIOModule_Intc_Acknowledge
#define XIOModule_InterruptHandler             XIOModule_Intc_InterruptHandler
#define XIOModule_SetOptions                   XIOModule_Intc_SetOptions
#define XIOModule_GetOptions                   XIOModule_Intc_GetOptions
/* Private -
#define XIOModule_EnableIntr                   XIOModule_Intc_EnableIntr
#define XIOModule_DisableIntr                  XIOModule_Intc_DisableIntr
#define XIOModule_AckIntr                      XIOModule_Intc_AckIntr
#define XIOModule_GetIntrStatus                XIOModule_Intc_GetIntrStatus
#define XIOModule_VoidInterruptHandler         XIOModule_Intc_VoidInterruptHandler
#define XIOModule_SetIntrSvcOption             XIOModule_Intc_SetIntrSvcOption
*/


/* Functions and Function-like Macros */
void    XIOModule_Intc_LowLevelInterruptHandler(void);
void    XIOModule_Intc_DeviceInterruptHandler(void *DeviceId);
s32     XIOModule_Intc_Connect(XIOModule * InstancePtr, u8 Id, XInterruptHandler Handler, void *CallBackRef);
void    XIOModule_Intc_Disconnect(XIOModule * InstancePtr, u8 Id);
void    XIOModule_Intc_Enable(XIOModule * InstancePtr, u8 Id);
void    XIOModule_Intc_Disable(XIOModule * InstancePtr, u8 Id);
void    XIOModule_Intc_Acknowledge(XIOModule * InstancePtr, u8 Id);
void    XIOModule_Intc_InterruptHandler(XIOModule * InstancePtr);
s32     XIOModule_Intc_SetOptions(XIOModule * InstancePtr, u32 Options);
u32     XIOModule_Intc_GetOptions(XIOModule * InstancePtr);

/* Private -
void    XIOModule_Intc_SetIntrSvcOption(u32 BaseAddress, u32 Option);
void    XIOModule_Intc_RegisterHandler(u32 BaseAddress, u32 InterruptId, XInterruptHandler Handler, void *CallBackRef);
#define XIOModule_Intc_EnableIntr(BaseAddress, EnableMask)   XIo_Out32((BaseAddress) + XIN_IER_OFFSET, (EnableMask))
#define XIOModule_Intc_DisableIntr(BaseAddress, DisableMask) XIo_Out32((BaseAddress) + XIN_IER_OFFSET, ~(DisableMask))
#define XIOModule_Intc_AckIntr(BaseAddress, AckMask)         XIo_Out32((BaseAddress) + XIN_IAR_OFFSET, (AckMask))
#define XIOModule_Intc_GetIntrStatus(BaseAddress)            (XIo_In32((BaseAddress) + XIN_IPR_OFFSET))
*/


/*****************************************************************************
* IOModule GPIO - Functions
*****************************************************************************/

/* Provide compatability for the haphazard IOModule namespace */
#define XIOModule_DiscreteRead  XIOModule_GPI_DiscreteRead
#define XIOModule_DiscreteWrite XIOModule_GPO_DiscreteWrite
#define XIOModule_DiscreteSet   XIOModule_GPO_DiscreteSet
#define XIOModule_DiscreteClear XIOModule_GPO_DiscreteClear

/* Functions and Function-like Macros */
u32     XIOModule_GPI_DiscreteRead(XIOModule *InstancePtr, u8 Channel);
void    XIOModule_GPO_DiscreteWrite(XIOModule *InstancePtr, u8 Channel, u32 Mask);
void    XIOModule_GPO_DiscreteSet(XIOModule *InstancePtr, u8 Channel, u32 Mask);
void    XIOModule_GPO_DiscreteClear(XIOModule *InstancePtr, u8 Channel, u32 Mask);




/*****************************************************************************
* IOModule UART - Functions
*****************************************************************************/

/* Provide compatability for the haphazard IOModule namespace */
#define XIOModule_ResetFifos(a)   ({;})
#define XIOModule_Send            XIOModule_Uart_Send
#define XIOModule_Recv            XIOModule_Uart_Recv
#define XIOModule_IsSending       XIOModule_Uart_IsSending
#define XIOModule_Handler         XIOModule_Uart_Handler
/* Private -- do not want to call these
#define XIOModule_GetStatusReg    XIOModule_Uart_GetStatusReg
#define XIOModule_IsReceiveEmpty  XIOModule_Uart_IsReceiveEmpty
#define XIOModule_IsTransmitFull  XIOModule_Uart_IsTransmitFull
#define XIOModule_SendByte        XIOModule_Uart_SendByte
#define XIOModule_RecvByte        XIOModule_Uart_RecvByte
*/

/* Functions and Function-like Macros */
u32     XIOModule_Uart_Send(XIOModule *InstancePtr, u8 *DataBufferPtr, u32 NumBytes);
u32     XIOModule_Uart_Recv(XIOModule *InstancePtr, u8 *DataBufferPtr, u32 NumBytes);
s32     XIOModule_Uart_IsSending(XIOModule *InstancePtr);

/*****************************************************************************
* IOModule PIT - Functions
*****************************************************************************/

/* Provide compatability for the haphazard IOModule namespace */
#define XIOModule_GetValue        XIOModule_Timer_GetValue
#define XIOModule_SetResetValue   XIOModule_Timer_SetResetValue
#define XIOModule_GetCaptureValue XIOModule_Timer_GetValue
#define XIOModule_IsExpired       XIOModule_Timer_IsExpired
#define XIOModule_Reset           XIOModule_Timer_Reset
#define XIOModule_SetHandler      XIOModule_Timer_SetHandler

/* Functions and Function-like Macros */
s32     XIOModule_Timer_Initialize(XIOModule * InstancePtr, u16 DeviceId);
void    XIOModule_Timer_Start(XIOModule * InstancePtr, u8 TimerNumber);
void    XIOModule_Timer_Stop(XIOModule * InstancePtr, u8 TimerNumber);
u32     XIOModule_Timer_GetValue(XIOModule * InstancePtr, u8 TimerNumber);
void    XIOModule_Timer_SetResetValue(XIOModule * InstancePtr, u8 TimerNumber, u32 ResetValue);
u32     XIOModule_Timer_GetCaptureValue(XIOModule * InstancePtr, u8 TimerNumber);
s32     XIOModule_Timer_IsExpired(XIOModule * InstancePtr, u8 TimerNumber);
void    XIOModule_Timer_Reset(XIOModule * InstancePtr, u8 TimerNumber);
void    XIOModule_Timer_SetOptions(XIOModule * InstancePtr, u8 TimerNumber, u32 Options);
u32     XIOModule_Timer_GetOptions(XIOModule * InstancePtr, u8 TimerNumber);

#ifdef __cplusplus
}
#endif
#endif /* __IOMODULE_H__ */
