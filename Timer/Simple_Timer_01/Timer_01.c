/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xscutimer.h"

 #include <stdio.h>
 #include "platform.h"
 #include "xttcps.h"
 #include "xscutimer.h"
 #include "xscugic.h"
 #include "xil_exception.h"
 #include "xparameters.h"

#define TIMER_LOAD_VALUE      0x27BC86AA
#define TTC_DEVICE_ID       XPAR_XTTCPS_0_DEVICE_ID
#define TIMER_DEVICE_ID		XPAR_XSCUTIMER_0_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define TIMER_IRPT_INTR		XPAR_SCUTIMER_INTR

XScuGic IntcInstance;		/* Interrupt Controller Instance */
XScuTimer TimerInstance;	/* Cortex Scu Private Timer Instance */
static void TimerIntrHandler(void *CallBackRef);
void SetupScuTimerIntr(XScuGic *IntcInstancePtr, XScuTimer *TimerInstancePtr, u16 TimerDeviceId, u16 TimerIntrId);
int CNTR;



int main()
{
    init_platform();
    CNTR = 0 ;


    xil_printf("Successfully start the application application\n\r");
    xil_printf("Start setting up the Timer_0 / TTC0 (no pin)\n\r");

    //timer initialization
    SetupScuTimerIntr(&IntcInstance, &TimerInstance, TIMER_DEVICE_ID, TIMER_IRPT_INTR);

    while(1);


    cleanup_platform();
    return 0;
}

void SetupScuTimerIntr(XScuGic *IntcInstancePtr, XScuTimer *TimerInstancePtr, u16 TimerDeviceId, u16 TimerIntrId)
{
	int stat;
	XScuTimer_Config *ConfigPtr;
	XScuGic_Config *IntcConfig;

		// Initialize the Scu Private Timer driver.
	ConfigPtr = XScuTimer_LookupConfig(TimerDeviceId);
		// This is where the virtual address would be used, this example uses physical address.
	XScuTimer_CfgInitialize(TimerInstancePtr, ConfigPtr, ConfigPtr->BaseAddr);
		// You can check if the timer is fine or not here:
	stat = XScuTimer_SelfTest(TimerInstancePtr);
	if (stat != XST_SUCCESS)
	{
		xil_printf("The timer is failed in the self test \n\r");
	}
		// Initialize the interrupt controller driver so that it is ready to use.
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig, IntcConfig->CpuBaseAddress);
	Xil_ExceptionInit();
		// Connect the interrupt controller interrupt handler to the hardware
		// interrupt handling logic in the processor.
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, IntcInstancePtr);

		// Connect the device driver handler that will be called when an
		// interrupt for the device occurs, the handler defined above performs
		// the specific interrupt processing for the device.
	XScuGic_Connect(IntcInstancePtr, TimerIntrId, (Xil_ExceptionHandler)TimerIntrHandler, (void *)TimerInstancePtr);
		// Enable the interrupt for the device.
	XScuGic_Enable(IntcInstancePtr, TimerIntrId);
		// Enable the timer interrupts for timer mode.
	XScuTimer_EnableInterrupt(TimerInstancePtr);
		// Enable interrupts in the Processor.
	Xil_ExceptionEnable();

		// Connect the device to interrupt subsystem so that interrupts can occur.
	//TimerSetupIntrSystem(IntcInstancePtr, TimerInstancePtr, TimerIntrId);
		// Enable Auto reload mode.
	XScuTimer_EnableAutoReload(TimerInstancePtr);
		// Load the timer counter register.
	XScuTimer_LoadTimer(TimerInstancePtr, TIMER_LOAD_VALUE);
		// Start the timer counter and then wait for it to timeout a number of times.
	XScuTimer_Start(TimerInstancePtr);
	/*
		// Disable and disconnect the interrupt system.
	TimerDisableIntrSystem(IntcInstancePtr, TimerIntrId);
	*/
}

static void TimerIntrHandler(void *CallBackRef)
{
//	XScuTimer *TimerInstancePtr = (XScuTimer *) CallBackRef;
	CNTR++;
	xil_printf("Timer event happened %i \n\r", CNTR);
}


