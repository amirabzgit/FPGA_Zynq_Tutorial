/******************************************************************************
* Copyright (C) 2010 - 2020 Xilinx, Inc.  All rights reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/***************************** Include Files *********************************/

#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xil_exception.h"
#include "xttcps.h"
#include "xscugic.h"
#include "xil_printf.h"

/************************** Constant Definitions *****************************


/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are only defined here such that a user can easily
 * change all the needed parameters in one place.
 * Note: To run this example on intended TTC device, following changes
 *       needs to be done
 *       - Map constants given below to the intended TTC devices
 *       - Fill SettingsTable array based on the intended device IDs.
 *         e.g. If intended device IDs are 3 and 4, then SettingsTable[3]
 *              and SettingsTable[4] should be set properly.
 */

#define TTC_PWM_DEVICE_ID	XPAR_XTTCPS_0_DEVICE_ID
#define TTC_PWM_INTR_ID		XPS_TTC0_0_INT_ID
/*
 *
 * look at page 233 of https://www.cl.cam.ac.uk/research/srg/han/ACS-P35/zynq/Zynq-7000-TRM.pdf
#define XPS_TTC0_0_INT_ID		42U
#define XPS_TTC0_1_INT_ID		43U
#define XPS_TTC0_2_INT_ID 		44U
#define XPS_TTC1_0_INT_ID		69U
#define XPS_TTC1_1_INT_ID		70U
#define XPS_TTC1_2_INT_ID		71U
 *
 *
 *
 */
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID

/*
 * Constants to set the basic operating parameters.
 * PWM_DELTA_DUTY is critical to the running time of the test. Smaller values
 * make the test run longer.
 */
#define	PWM_OUT_FREQ		350  /* PWM timer counter's output frequency */
#define PWM_DELTA_DUTY	70 /* Initial and increment to duty cycle for PWM */

/**************************** Type Definitions *******************************/
typedef struct {
	u32 OutputHz;	/* Output frequency */
	XInterval Interval;	/* Interval value */
	u8 Prescaler;	/* Prescaler value */
	u16 Options;	/* Option settings */
} TmrCntrSetup;

/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

static int PWMInterruptExample(void);  /* Main test */

/* Set up routines for timer counters */

static int SetupPWM(void);
static int SetupTimer(int DeviceID);

/* Interleaved interrupt test for both timer counters */
static int WaitForDutyCycleFull(void);
static int SetupInterruptSystem(u16 IntcDeviceID, XScuGic *IntcInstancePtr);
static void PWMHandler(void *CallBackRef, u32 StatusEvent);

/************************** Variable Definitions *****************************/

static XTtcPs TtcPsInst[1];	/* Number of available timer counters */

static TmrCntrSetup SettingsTable[1] = {
	{PWM_OUT_FREQ, 0, 0, 0}, /* PWM timer counter initial setup, only output freq */
};

XScuGic InterruptController;  /* Interrupt controller instance */

static u32 MatchValue;  /* Match value for PWM, set by PWM interrupt handler,
			updated by main test routine */

static volatile u32 PWM_UpdateFlag;	/* Flag used by Ticker to signal PWM */
static volatile u32 TickCount;		/* Ticker interrupts between PWM change */


/*****************************************************************************/
/**
*
* This function calls the Ttc interrupt example.
*
* @param	None
*
* @return
*		- XST_SUCCESS to indicate Success
*		- XST_FAILURE to indicate Failure.
*
* @note		None
*
*****************************************************************************/
int main(void)
{
	int Status;

	xil_printf("TTC Interrupt Example Test\r\n");

	Status = PWMInterruptExample();
	if (Status != XST_SUCCESS) {
		xil_printf("TTC Interrupt Example Test Failed\r\n");
		return XST_FAILURE;
	}

	xil_printf("Successfully ran TTC Interrupt Example Test\r\n");
	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function sets up the interrupt example.
*
* @param	None.
*
* @return
*		- XST_SUCCESS to indicate Success
*		- XST_FAILURE to indicate Failure.
*
* @note		None
*
****************************************************************************/
static int PWMInterruptExample(void)
{
	int Status;

	/*
	 * Make sure the interrupts are disabled, in case this is being run
	 * again after a failure.
	 */

	/*
	 * Connect the Intc to the interrupt subsystem such that interrupts can
	 * occur. This function is application specific.
	 */
	Status = SetupInterruptSystem(INTC_DEVICE_ID, &InterruptController);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set up  the PWM timer
	 */
	Status = SetupPWM();
	if (Status != XST_SUCCESS) {
		return Status;
	}

	/*
	 * Stop the counters

	XTtcPs_Stop(&(TtcPsInst[TTC_TICK_DEVICE_ID]));

	XTtcPs_Stop(&(TtcPsInst[TTC_PWM_DEVICE_ID]));
*/
	return XST_SUCCESS;
}


/****************************************************************************/
/**
*
* This function sets up the waveform output timer counter (PWM).
*
* @param	None
*
* @return	XST_SUCCESS if everything sets up well, XST_FAILURE otherwise.
*
* @note		None
*
*****************************************************************************/
int SetupPWM(void)
{
	int Status;
	TmrCntrSetup *TimerSetup;
	XTtcPs *TtcPsPWM;

	TimerSetup = &(SettingsTable[TTC_PWM_DEVICE_ID]);

	/*
	 * Set up appropriate options for PWM: interval mode  and
	 * match mode for waveform output.
	 */
	TimerSetup->Options |= (XTTCPS_OPTION_INTERVAL_MODE |
					      XTTCPS_OPTION_MATCH_MODE);

	/*
	 * Calling the timer setup routine
	 * 	initialize device
	 * 	set options
	 */
	Status = SetupTimer(TTC_PWM_DEVICE_ID);
	if(Status != XST_SUCCESS) {
		return Status;
	}

	TtcPsPWM = &(TtcPsInst[TTC_PWM_DEVICE_ID]);

	/*
	 * Connect to the interrupt controller
	 */
	Status = XScuGic_Connect(&InterruptController, TTC_PWM_INTR_ID,
		(Xil_ExceptionHandler)XTtcPs_InterruptHandler, (void *)TtcPsPWM);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XTtcPs_SetStatusHandler(&(TtcPsInst[TTC_PWM_DEVICE_ID]), &(TtcPsInst[TTC_PWM_DEVICE_ID]),
		              (XTtcPs_StatusHandler) PWMHandler);
	/*
	 * Enable the interrupt for the Timer counter
	 */
	XScuGic_Enable(&InterruptController, TTC_PWM_INTR_ID);

	/*
	 * Enable the interrupts for the tick timer/counter
	 * We only care about the interval timeout.
	 */
	XTtcPs_EnableInterrupts(TtcPsPWM, XTTCPS_IXR_INTERVAL_MASK);

	WaitForDutyCycleFull();

	/*
	 * Start the tick timer/counter
	 */
	XTtcPs_Start(TtcPsPWM);

	return Status;
}
/****************************************************************************/
/**
*
* This function uses the interrupt inter-locking between the ticker timer
* counter and the waveform output timer counter. When certain amount of
* interrupts have happened to the ticker timer counter, a flag, PWM_UpdateFlag,
* is set to true.
*
* When PWM_UpdateFlag for the waveform timer counter is true, the duty
* cycle for PWM timer counter is increased by PWM_DELTA_DUTY.
* The function exits successfully when the duty cycle for PWM timer
* counter reaches beyond 100.
*
* @param	None
*
* @return	XST_SUCCESS if duty cycle successfully reaches beyond 100,
*		otherwise, XST_FAILURE.
*
* @note		None.
*
*****************************************************************************/
int WaitForDutyCycleFull(void)
{
	TmrCntrSetup *TimerSetup;
	u8 DutyCycle;		/* The current output duty cycle */
	XTtcPs *TtcPs_PWM;	/* Pointer to the instance structure */

	TimerSetup = &(SettingsTable[TTC_PWM_DEVICE_ID]);
	TtcPs_PWM = &(TtcPsInst[TTC_PWM_DEVICE_ID]);

	/*
	 * Initialize some variables used by the interrupts and in loops.
	 */
	DutyCycle = PWM_DELTA_DUTY;
	MatchValue = (TimerSetup->Interval * DutyCycle) / 100;
	// This function enables the interrupts.
	XTtcPs_EnableInterrupts(TtcPs_PWM, XTTCPS_IXR_INTERVAL_MASK);

	return XST_SUCCESS;
}
/****************************************************************************/
/**
*
* This function sets up a timer counter device, using the information in its
* setup structure.
*  . initialize device
*  . set options
*  . set interval and prescaler value for given output frequency.
*
* @param	DeviceID is the unique ID for the device.
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None.
*
*****************************************************************************/
int SetupTimer(int DeviceID)
{
	int Status;
	XTtcPs_Config *Config;
	XTtcPs *Timer;
	TmrCntrSetup *TimerSetup;

	TimerSetup = &SettingsTable[DeviceID];

	Timer = &(TtcPsInst[DeviceID]);

	/*
	 * Look up the configuration based on the device identifier
	 */
	Config = XTtcPs_LookupConfig(DeviceID);
	if (NULL == Config) {
		return XST_FAILURE;
	}

	/*
	 * Initialize the device
	 */
	Status = XTtcPs_CfgInitialize(Timer, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the options
	 */
	XTtcPs_SetOptions(Timer, TimerSetup->Options);

	/*
	 * Timer frequency is preset in the TimerSetup structure,
	 * however, the value is not reflected in its other fields, such as
	 * IntervalValue and PrescalerValue. The following call will map the
	 * frequency to the interval and prescaler values.
	 */
	XTtcPs_CalcIntervalFromFreq(Timer, TimerSetup->OutputHz,
		&(TimerSetup->Interval), &(TimerSetup->Prescaler));

	/*
	 * Set the interval and prescale
	 */
	XTtcPs_SetInterval(Timer, TimerSetup->Interval);
	XTtcPs_SetPrescaler(Timer, TimerSetup->Prescaler);

	return XST_SUCCESS;
}

/****************************************************************************/
/**
*
* This function setups the interrupt system such that interrupts can occur.
* This function is application specific since the actual system may or may not
* have an interrupt controller.  The TTC could be directly connected to a
* processor without an interrupt controller.  The user should modify this
* function to fit the application.
*
* @param	IntcDeviceID is the unique ID of the interrupt controller
* @param	IntcInstacePtr is a pointer to the interrupt controller
*		instance.
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None.
*
*****************************************************************************/
static int SetupInterruptSystem(u16 IntcDeviceID,
				    XScuGic *IntcInstancePtr)
{
	int Status;
	XScuGic_Config *IntcConfig; /* The configuration parameters of the
					interrupt controller */

	/*
	 * Initialize the interrupt controller driver
	 */
	IntcConfig = XScuGic_LookupConfig(IntcDeviceID);

	Status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig, IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect the interrupt controller interrupt handler to the hardware
	 * interrupt handling logic in the ARM processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler) XScuGic_InterruptHandler,
			IntcInstancePtr);

	/*
	 * Enable interrupts in the ARM
	 */
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}


/***************************************************************************/
/**
*
* This function is the handler which handles the PWM interrupt.
*
* It updates the match register to reflect the change on duty cycle. It also
* disable interrupt at the end. The interrupt will be enabled by the Ticker
* timer counter.
*
* @param	CallBackRef contains a callback reference from the driver, in
*		this case it is a pointer to the MatchValue variable.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
static void PWMHandler(void *CallBackRef, u32 StatusEvent)
{

	XTtcPs *Timer;

	Timer = &(TtcPsInst[TTC_PWM_DEVICE_ID]);

	if (0 != (XTTCPS_IXR_INTERVAL_MASK & StatusEvent)) {
		// This function is used to set the match registers. There are three match registers.
		// The second input can be 0, 1 or 2.
		XTtcPs_SetMatchValue(Timer, 0, MatchValue);
	}
	else {

		// If it is not Interval event, it is an error.

	}
	// This function disables the interrupts.
	XTtcPs_DisableInterrupts(Timer, XTTCPS_IXR_ALL_MASK);

}
