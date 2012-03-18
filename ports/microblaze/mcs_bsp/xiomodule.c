#include "xparameters.h"
#include "xiomodule.h"
#include "mb_interface.h"

#define ASSERT(x) if(!(x)) while(1)

/*****************************************************************************
* Basic STDIO support for newlib
*****************************************************************************/
char inbyte(void)
{
  u8 XIOModule_Uart_RecvByte(u32 BaseAddress);
  return XIOModule_Uart_RecvByte(XPAR_IOMODULE_0_BASEADDR);
}

void outbyte(char c)
{
  void XIOModule_Uart_SendByte(u32 BaseAddress, u8 Data);
  XIOModule_Uart_SendByte(XPAR_IOMODULE_0_BASEADDR, c);
}



/*****************************************************************************
* Minimal Microblaze support library
*****************************************************************************/
void XNullHandler(void *NullParameter);
void XNullHandler(void *NullParameter)
{
}

static XInterruptHandler Microblaze_InterruptHandler = XNullHandler;
static void *Microblaze_InterruptCallBackRef;

void __interrupt_handler(void) __attribute__ ((interrupt_handler));
void __interrupt_handler(void)
{
	Microblaze_InterruptHandler(Microblaze_InterruptCallBackRef);
}

void microblaze_enable_interrupts(void)
{
    unsigned int msr = mfmsr();
    msr |= 0x2;
    mtmsr(msr);
}

void microblaze_disable_interrupts(void)
{
    unsigned int msr = mfmsr();
    msr &= ~0x2;
    mtmsr(msr);
}

void microblaze_register_handler(XInterruptHandler Handler, void *DataPtr)
{
   Microblaze_InterruptHandler = Handler;
   Microblaze_InterruptCallBackRef = DataPtr;
}

/*****************************************************************************
* IOModule support library
*****************************************************************************/

static XIOModule_Config theXIOModule_Config =
{
  .DeviceId = XPAR_IOMODULE_0_DEVICE_ID,
  .BaseAddress = XPAR_IOMODULE_0_BASEADDR,
  .IoBaseAddress = XPAR_IOMODULE_0_IO_BASEADDR,
  .IntcAckBeforeService = XPAR_IOMODULE_0_INTC_LEVEL_EDGE,
  .IntcOptions = XIN_SVC_SGL_ISR_OPTION,
  .PitUsed =
  {
    XPAR_IOMODULE_0_USE_PIT1,
    XPAR_IOMODULE_0_USE_PIT2, 
    XPAR_IOMODULE_0_USE_PIT3, 
    XPAR_IOMODULE_0_USE_PIT4
  },
  .PitSize =
  {
    XPAR_IOMODULE_0_PIT1_SIZE, 
    XPAR_IOMODULE_0_PIT2_SIZE, 
    XPAR_IOMODULE_0_PIT3_SIZE, 
    XPAR_IOMODULE_0_PIT4_SIZE
  },
  .PitPrescaler =
  {
    XPAR_IOMODULE_0_PIT1_PRESCALER, 
    XPAR_IOMODULE_0_PIT2_PRESCALER, 
    XPAR_IOMODULE_0_PIT3_PRESCALER, 
    XPAR_IOMODULE_0_PIT4_PRESCALER
  },
  .PitReadable =
  {
    XPAR_IOMODULE_0_PIT1_READABLE, 
    XPAR_IOMODULE_0_PIT2_READABLE, 
    XPAR_IOMODULE_0_PIT3_READABLE, 
    XPAR_IOMODULE_0_PIT4_READABLE
  },
  .GpoInit =
  {
    XPAR_IOMODULE_0_GPO1_INIT, 
    XPAR_IOMODULE_0_GPO2_INIT, 
    XPAR_IOMODULE_0_GPO3_INIT, 
    XPAR_IOMODULE_0_GPO4_INIT
  },
  .HandlerTable =
  {
    {XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},
    {XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},
    {XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},
    {XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},
    {XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},
    {XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},
    {XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},
    {XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL},{XNullHandler, (void *)NULL}
  }
};


/*****************************************************************************
* IOModule General
*****************************************************************************/

s32 XIOModule_Initialize(XIOModule * InstancePtr, u16 DeviceId)
{
  unsigned int ii = 0;

  if (DeviceId != XPAR_IOMODULE_0_DEVICE_ID)
      return XST_FAILURE;

  ASSERT(InstancePtr != NULL);
  
  if (InstancePtr->IsReady == XIL_COMPONENT_IS_READY)
      return XST_SUCCESS;

  ASSERT(InstancePtr->IsStarted != XIL_COMPONENT_IS_STARTED);

  InstancePtr->CfgPtr = &theXIOModule_Config;

  InstancePtr->IsReady = 0;
  InstancePtr->IsStarted = 0;

  /* Initialize GPO */
  for (ii=0; ii<XGPO_DEVICE_COUNT; ii++)
  {
    InstancePtr->GpoValue[ii] = InstancePtr->CfgPtr->GpoInit[ii];
  }

  /* Initialize GPI -- nothing */

  /* Initialize UART -- nothing */

  /* Initialize PIT */
  for (ii=0; ii<XTC_DEVICE_TIMER_COUNT; ii++)
  {
    InstancePtr->TimerTCSR[ii] = 0;
    InstancePtr->TimerTLR[ii] = 0;
  }

  /* Initialize Interrupt Controller */
  InstancePtr->IntEnabledMask = 0;

  InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

  return XST_SUCCESS;
}

s32 XIOModule_Start(XIOModule * InstancePtr)
{
  ASSERT(InstancePtr != NULL);
  ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

  InstancePtr->IsStarted = XIL_COMPONENT_IS_STARTED;

  return XST_SUCCESS;
}

void XIOModule_Stop(XIOModule * InstancePtr)
{
  ASSERT(InstancePtr != NULL);
  ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

  InstancePtr->IsStarted = 0;
}

u32 XIOModule_IoReadWord(XIOModule *InstancePtr, u32 ByteOffset)
{
  ASSERT(InstancePtr != NULL);
  ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
	
  return XIo_In32(InstancePtr->CfgPtr->IoBaseAddress + ByteOffset);
}

u16 XIOModule_IoReadHalfword(XIOModule *InstancePtr, u32 ByteOffset)
{
  ASSERT(InstancePtr != NULL);
  ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
	
  return XIo_In16(InstancePtr->CfgPtr->IoBaseAddress + ByteOffset);
}

u8 XIOModule_IoReadByte(XIOModule *InstancePtr, u32 ByteOffset)
{
  ASSERT(InstancePtr != NULL);
  ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
	
  return XIo_In8(InstancePtr->CfgPtr->IoBaseAddress + ByteOffset);
}

void XIOModule_IoWriteWord(XIOModule *InstancePtr, u32 ByteOffset, u32 Data)
{
  ASSERT(InstancePtr != NULL);
  ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
	
  XIo_Out32(InstancePtr->CfgPtr->IoBaseAddress + ByteOffset, Data);
}

void XIOModule_IoWriteHalfword(XIOModule *InstancePtr, u32 ByteOffset, u16 Data)
{
  ASSERT(InstancePtr != NULL);
  ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
	
  XIo_Out16(InstancePtr->CfgPtr->IoBaseAddress + ByteOffset, Data);
}

void XIOModule_IoWriteByte(XIOModule *InstancePtr, u32 ByteOffset, u8 Data)
{
  ASSERT(InstancePtr != NULL);
  ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
	
  XIo_Out8(InstancePtr->CfgPtr->IoBaseAddress + ByteOffset, Data);
}


/*****************************************************************************
* IOModule UART
*****************************************************************************/

/* Private Functions and Function-like Macros */
#define XIOModule_Uart_IsReceiveEmpty(BaseAddress) ((XIo_In32(BaseAddress + XUL_STATUS_REG_OFFSET) & XUL_SR_RX_FIFO_VALID_DATA) != XUL_SR_RX_FIFO_VALID_DATA)
#define XIOModule_Uart_IsTransmitFull(BaseAddress) ((XIo_In32(BaseAddress + XUL_STATUS_REG_OFFSET) & XUL_SR_TX_FIFO_FULL) == XUL_SR_TX_FIFO_FULL)

void XIOModule_Uart_SendByte(u32 BaseAddress, u8 Data)
{
  ASSERT(BaseAddress != 0);
  while (XIOModule_Uart_IsTransmitFull(BaseAddress));
  XIo_Out32((BaseAddress) + XUL_TX_OFFSET, Data);
}

u8 XIOModule_Uart_RecvByte(u32 BaseAddress)
{
  ASSERT(BaseAddress != 0);
  while (XIOModule_Uart_IsReceiveEmpty(BaseAddress));
  return (u8)XIo_In32((BaseAddress) + XUL_RX_OFFSET);
}

/* Functions and Function-like Macros */
u32 XIOModule_Uart_Send(XIOModule *InstancePtr, u8 *DataBufferPtr, u32 NumBytes)
{
  u32 ii = 0;

  ASSERT(InstancePtr != NULL);
  ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
	
  for(ii=0; ii<NumBytes; ii++)
  {
    XIOModule_Uart_SendByte(InstancePtr->CfgPtr->BaseAddress, DataBufferPtr[ii]);
  }
  
  return ii;
}

u32 XIOModule_Uart_Recv(XIOModule *InstancePtr, u8 *DataBufferPtr, u32 NumBytes)
{
  u32 ii = 0;

  ASSERT(InstancePtr != NULL);
  ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
	
  for(ii=0; ii<NumBytes; ii++)
  {
    DataBufferPtr[ii] = XIOModule_Uart_RecvByte(InstancePtr->CfgPtr->BaseAddress);
  }
  
  return ii;
}

s32 XIOModule_Uart_IsSending(XIOModule *InstancePtr)
{
  ASSERT(InstancePtr != NULL);
  ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

  return XIOModule_Uart_IsTransmitFull(InstancePtr->CfgPtr->BaseAddress);
}


/*****************************************************************************
* IOModule Interrupt Controller
*****************************************************************************/

/* Functions and Function-like Macros */
void XIOModule_Intc_DeviceInterruptHandler(void *DeviceId)
{
    XIOModule_Config* CfgPtr = NULL;
    u32 Pending = 0;
    u32 IntMask = 0;
    u32 IntNum = 0;

    ASSERT((u32)DeviceId == theXIOModule_Config.DeviceId);
    CfgPtr = &theXIOModule_Config;
    ASSERT(CfgPtr != NULL);

    Pending = XIo_In32(CfgPtr->BaseAddress + XIN_IPR_OFFSET);

    for (IntMask = 0x1, IntNum = 0; Pending != 0; IntMask <<= 1, IntNum++)
    {
        if (IntMask & Pending)
        {
            if (CfgPtr->IntcAckBeforeService & IntMask)
                XIo_Out32(CfgPtr->BaseAddress + XIN_IAR_OFFSET, IntMask);

            if (CfgPtr->HandlerTable[IntNum].Handler != NULL)
                CfgPtr->HandlerTable[IntNum].Handler(CfgPtr->HandlerTable[IntNum].CallBackRef);

            if (~CfgPtr->IntcAckBeforeService & IntMask)
                XIo_Out32(CfgPtr->BaseAddress + XIN_IAR_OFFSET, IntMask);

            Pending &= ~IntMask;

            if (CfgPtr->IntcOptions & XIN_SVC_SGL_ISR_OPTION)
                Pending = 0;
        }
    }
    return;
}

s32 XIOModule_Intc_Connect(XIOModule * InstancePtr, u8 Id, XInterruptHandler Handler, void *CallBackRef)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    ASSERT(Id < XPAR_IOMODULE_INTC_MAX_INTR_SIZE);
    
    if(InstancePtr->CfgPtr->HandlerTable[Id].Handler != XNullHandler)
        return XST_FAILURE;
		

    InstancePtr->CfgPtr->HandlerTable[Id].Handler = Handler;
    InstancePtr->CfgPtr->HandlerTable[Id].CallBackRef = CallBackRef;

    return XST_SUCCESS;
}

void XIOModule_Intc_Disconnect(XIOModule * InstancePtr, u8 Id)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    ASSERT(Id < XPAR_IOMODULE_INTC_MAX_INTR_SIZE);
    
    InstancePtr->CfgPtr->HandlerTable[Id].Handler = XNullHandler;
    InstancePtr->CfgPtr->HandlerTable[Id].CallBackRef = NULL;
}

void XIOModule_Intc_Enable(XIOModule * InstancePtr, u8 Id)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    ASSERT(Id < XPAR_IOMODULE_INTC_MAX_INTR_SIZE);

    InstancePtr->IntEnabledMask |= (1 << Id);

    XIo_Out32(InstancePtr->CfgPtr->BaseAddress + XIN_IER_OFFSET, InstancePtr->IntEnabledMask);
}

void XIOModule_Intc_Disable(XIOModule * InstancePtr, u8 Id)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    ASSERT(Id < XPAR_IOMODULE_INTC_MAX_INTR_SIZE);

    InstancePtr->IntEnabledMask &= ~(1 << Id);

    XIo_Out32(InstancePtr->CfgPtr->BaseAddress + XIN_IER_OFFSET, InstancePtr->IntEnabledMask);
}

void XIOModule_Intc_Acknowledge(XIOModule * InstancePtr, u8 Id)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    ASSERT(Id < XPAR_IOMODULE_INTC_MAX_INTR_SIZE);

    XIo_Out32(InstancePtr->CfgPtr->BaseAddress + XIN_IAR_OFFSET, (1 << Id));
}

void XIOModule_Intc_InterruptHandler(XIOModule * InstancePtr)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XIOModule_Intc_DeviceInterruptHandler((void *)(u32)InstancePtr->CfgPtr->DeviceId);
}

s32 XIOModule_Intc_SetOptions(XIOModule * InstancePtr, u32 Options)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    
    InstancePtr->CfgPtr->IntcOptions = Options;

    return XST_SUCCESS;
}

u32 XIOModule_Intc_GetOptions(XIOModule * InstancePtr)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    
    return InstancePtr->CfgPtr->IntcOptions;
}




/*****************************************************************************
* IOModule PIT
*****************************************************************************/

/* Functions and Function-like Macros */
s32 XIOModule_Timer_Initialize(XIOModule * InstancePtr, u16 DeviceId)
{
	u32 ii = 0;
	
	ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
	
	for (ii=0; ii < XTC_DEVICE_TIMER_COUNT; ii++)
	{
		InstancePtr->TimerTCSR[ii] = 0;
		InstancePtr->TimerTLR[ii] = 0;
		
		XIo_Out32(InstancePtr->CfgPtr->BaseAddress + (ii*0x10) + XTC_TCSR_OFFSET, InstancePtr->TimerTCSR[ii]);
		XIo_Out32(InstancePtr->CfgPtr->BaseAddress + (ii*0x10) + XTC_TLR_OFFSET, InstancePtr->TimerTLR[ii]);
	}
	
    return XST_SUCCESS;
}

void XIOModule_Timer_Start(XIOModule * InstancePtr, u8 TimerNumber)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    ASSERT(TimerNumber < XTC_DEVICE_TIMER_COUNT);
    ASSERT(InstancePtr->CfgPtr->PitUsed[TimerNumber]);

    InstancePtr->TimerTCSR[TimerNumber] |= XTC_CSR_ENABLE_TMR_MASK;

    XIo_Out32(InstancePtr->CfgPtr->BaseAddress + (TimerNumber*0x10) + XTC_TCSR_OFFSET, InstancePtr->TimerTCSR[TimerNumber]);
}

void XIOModule_Timer_Stop(XIOModule * InstancePtr, u8 TimerNumber)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    ASSERT(TimerNumber < XTC_DEVICE_TIMER_COUNT);
    ASSERT(InstancePtr->CfgPtr->PitUsed[TimerNumber]);

    InstancePtr->TimerTCSR[TimerNumber] &= ~XTC_CSR_ENABLE_TMR_MASK;

    XIo_Out32(InstancePtr->CfgPtr->BaseAddress + (TimerNumber*0x10) + XTC_TCSR_OFFSET, InstancePtr->TimerTCSR[TimerNumber]);
}

u32 XIOModule_Timer_GetValue(XIOModule * InstancePtr, u8 TimerNumber)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    ASSERT(TimerNumber < XTC_DEVICE_TIMER_COUNT);
    ASSERT(InstancePtr->CfgPtr->PitUsed[TimerNumber]);
    ASSERT(InstancePtr->CfgPtr->PitReadable[TimerNumber]);

    return XIo_In32(InstancePtr->CfgPtr->BaseAddress + (TimerNumber*0x10) + XTC_TCR_OFFSET);
}

void XIOModule_Timer_SetResetValue(XIOModule * InstancePtr, u8 TimerNumber, u32 ResetValue)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    ASSERT(TimerNumber < XTC_DEVICE_TIMER_COUNT);
    ASSERT(InstancePtr->CfgPtr->PitUsed[TimerNumber]);
	ASSERT(ResetValue < (1 << (InstancePtr->CfgPtr->PitSize[TimerNumber]-1)));
	
    InstancePtr->TimerTLR[TimerNumber] = ResetValue;

    XIo_Out32(InstancePtr->CfgPtr->BaseAddress + (TimerNumber*0x10) + XTC_TLR_OFFSET, InstancePtr->TimerTLR[TimerNumber]);
}

s32 XIOModule_Timer_IsExpired(XIOModule * InstancePtr, u8 TimerNumber)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    ASSERT(TimerNumber < XTC_DEVICE_TIMER_COUNT);
    ASSERT(InstancePtr->CfgPtr->PitUsed[TimerNumber]);
    ASSERT(InstancePtr->CfgPtr->PitReadable[TimerNumber]);

    return (XIo_In32(InstancePtr->CfgPtr->BaseAddress + (TimerNumber*0x10) + XTC_TCR_OFFSET) == 0);
}

void XIOModule_Timer_Reset(XIOModule * InstancePtr, u8 TimerNumber)
{
	ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    ASSERT(TimerNumber < XTC_DEVICE_TIMER_COUNT);
    ASSERT(InstancePtr->CfgPtr->PitUsed[TimerNumber]);
	
	if (InstancePtr->TimerTCSR[TimerNumber] & XTC_CSR_ENABLE_TMR_MASK)
	{
		XIo_Out32(InstancePtr->CfgPtr->BaseAddress + (TimerNumber*0x10) + XTC_TCSR_OFFSET, InstancePtr->TimerTCSR[TimerNumber] & ~XTC_CSR_ENABLE_TMR_MASK);
		XIo_Out32(InstancePtr->CfgPtr->BaseAddress + (TimerNumber*0x10) + XTC_TCSR_OFFSET, InstancePtr->TimerTCSR[TimerNumber]);
	}
}

void XIOModule_Timer_SetOptions(XIOModule * InstancePtr, u8 TimerNumber, u32 Options)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    ASSERT(TimerNumber < XTC_DEVICE_TIMER_COUNT);
    ASSERT(InstancePtr->CfgPtr->PitUsed[TimerNumber]);

    InstancePtr->TimerTCSR[TimerNumber] = Options;

    XIo_Out32(InstancePtr->CfgPtr->BaseAddress + (TimerNumber*0x10) + XTC_TCSR_OFFSET, InstancePtr->TimerTCSR[TimerNumber]);
}

u32 XIOModule_Timer_GetOptions(XIOModule * InstancePtr, u8 TimerNumber)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    ASSERT(TimerNumber < XTC_DEVICE_TIMER_COUNT);
    ASSERT(InstancePtr->CfgPtr->PitUsed[TimerNumber]);

    return InstancePtr->TimerTCSR[TimerNumber];
}

/*****************************************************************************
* IOModule GPIO
*****************************************************************************/

/* Functions and Function-like Macros */
u32 XIOModule_GPI_DiscreteRead(XIOModule *InstancePtr, u8 Channel)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    ASSERT(Channel < XGPI_DEVICE_COUNT);

    if (Channel == 0) ASSERT(XPAR_IOMODULE_0_USE_GPI1);
    if (Channel == 1) ASSERT(XPAR_IOMODULE_0_USE_GPI2);
    if (Channel == 2) ASSERT(XPAR_IOMODULE_0_USE_GPI3);
    if (Channel == 3) ASSERT(XPAR_IOMODULE_0_USE_GPI4);

    return XIo_In32(InstancePtr->CfgPtr->BaseAddress + XGI_IN_OFFSET + (Channel*XGPI_CHAN_OFFSET));
}

void XIOModule_GPO_DiscreteWrite(XIOModule *InstancePtr, u8 Channel, u32 Mask)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    ASSERT(Channel < XGPO_DEVICE_COUNT);

    if (Channel == 0) ASSERT(XPAR_IOMODULE_0_USE_GPO1);
    if (Channel == 1) ASSERT(XPAR_IOMODULE_0_USE_GPO2);
    if (Channel == 2) ASSERT(XPAR_IOMODULE_0_USE_GPO3);
    if (Channel == 3) ASSERT(XPAR_IOMODULE_0_USE_GPO4);

    InstancePtr->GpoValue[Channel] = Mask;

    XIo_Out32(InstancePtr->CfgPtr->BaseAddress + XGO_OUT_OFFSET + (Channel*XGPO_CHAN_OFFSET), InstancePtr->GpoValue[Channel]);
}

void XIOModule_GPO_DiscreteSet(XIOModule *InstancePtr, u8 Channel, u32 Mask)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    ASSERT(Channel < XGPO_DEVICE_COUNT);

    if (Channel == 0) ASSERT(XPAR_IOMODULE_0_USE_GPO1);
    if (Channel == 1) ASSERT(XPAR_IOMODULE_0_USE_GPO2);
    if (Channel == 2) ASSERT(XPAR_IOMODULE_0_USE_GPO3);
    if (Channel == 3) ASSERT(XPAR_IOMODULE_0_USE_GPO4);

    InstancePtr->GpoValue[Channel] |= Mask;

    XIo_Out32(InstancePtr->CfgPtr->BaseAddress + XGO_OUT_OFFSET + (Channel*XGPO_CHAN_OFFSET), InstancePtr->GpoValue[Channel]);
}

void XIOModule_GPO_DiscreteClear(XIOModule *InstancePtr, u8 Channel, u32 Mask)
{
    ASSERT(InstancePtr != NULL);
    ASSERT(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    ASSERT(Channel < XGPO_DEVICE_COUNT);

    if (Channel == 0) ASSERT(XPAR_IOMODULE_0_USE_GPO1);
    if (Channel == 1) ASSERT(XPAR_IOMODULE_0_USE_GPO2);
    if (Channel == 2) ASSERT(XPAR_IOMODULE_0_USE_GPO3);
    if (Channel == 3) ASSERT(XPAR_IOMODULE_0_USE_GPO4);

    InstancePtr->GpoValue[Channel] &= ~Mask;

    XIo_Out32(InstancePtr->CfgPtr->BaseAddress + XGO_OUT_OFFSET + (Channel*XGPO_CHAN_OFFSET), InstancePtr->GpoValue[Channel]);
}
