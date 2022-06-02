
#include "xparameters.h"
#include "xadcps.h"
#include "xscugic.h"
#include "xil_exception.h"
#include <stdio.h>
#include "platform.h"
#include "xsysmon.h"

#define SYSMON_DEVICE_ID XPAR_SYSMON_0_DEVICE_ID //ID of xadc_wiz_0
#define XSysMon_RawToExtVoltage(AdcData) \
((((float)(AdcData))*(1.0f))/65536.0f) //(ADC 16bit result)/16/4096 = (ADC 16bit result)/65536
// voltage value = (ADC 16bit result)/65536 * 1Volt

static XSysMon SysMonInst; //a sysmon instance
static int SysMonFractionToInt(float FloatNum);

int main()
{

	u8 SeqMode;
	u32 TempRawData,VccIntRawData,ExtVolRawData,i;
	float TempData,VccIntData,ExtVolData;
	int xStatus;

	// configuration information for the System Monitor/ADC device
	XSysMon_Config *SysMonConfigPtr;
	XSysMon *SysMonInstPtr = &SysMonInst;
	init_platform();


	/********** SysMon Initialize ***********/

	SysMonConfigPtr = XSysMon_LookupConfig(SYSMON_DEVICE_ID);

	if(SysMonConfigPtr == NULL)
	{
		printf("LookupConfig FAILURE\n\r");
		return 0 ;
	}


	xStatus = XSysMon_CfgInitialize(SysMonInstPtr, SysMonConfigPtr,SysMonConfigPtr->BaseAddress);

	if(XST_SUCCESS != xStatus)
	{
		printf("CfgInitialize FAILED\r\n");
		return 0 ;
	}

	// Clear the old status
	XSysMon_GetStatus(SysMonInstPtr);

	int cnt = 0 ;
	while(1)
	{
		cnt++;

		//Read the on-chip Temperature Data
		TempRawData = XSysMon_GetAdcData(SysMonInstPtr, XSM_CH_TEMP);
		TempData = XSysMon_RawToTemperature(TempRawData);
		printf("\r\nThe Current Temperature is %0d.%03d Centigrades.\r\n", (int)(TempData), SysMonFractionToInt(TempData));

		//Read the on-chip Vccint Data
		VccIntRawData = XSysMon_GetAdcData(SysMonInstPtr, XSM_CH_VCCINT);
		VccIntData = XSysMon_RawToVoltage(VccIntRawData);
		printf("The Current VCCINT is %0d.%03d Volts. \r\n", (int)(VccIntData), SysMonFractionToInt(VccIntData));

		//Read the external VpVn Data (In my board they both are grounded)
		ExtVolRawData = XSysMon_GetAdcData(SysMonInstPtr,XSM_CH_VPVN);
		ExtVolData = XSysMon_RawToExtVoltage(ExtVolRawData);
		printf("The Current VpVn is %0d.%03d Volts. \r\n",(int)(ExtVolData), SysMonFractionToInt(ExtVolData));

		//Read the external Vaux7 Data
		ExtVolRawData = XSysMon_GetAdcData(SysMonInstPtr,XSM_CH_AUX_MIN+7);
		ExtVolData = XSysMon_RawToExtVoltage(ExtVolRawData);
		printf("The Current Vaux7 is %0d.%03d Volts. \r\n", (int)(ExtVolData), SysMonFractionToInt(ExtVolData));

		//Read the external Vaux9 Data (In my Hardware, they are not activated and the pins are not assigned
		ExtVolRawData = XSysMon_GetAdcData(SysMonInstPtr,XSM_CH_AUX_MIN+9);
		ExtVolData = XSysMon_RawToExtVoltage(ExtVolRawData);
		printf("The Current Vaux9 is %0d.%03d Volts. \r\n", (int)(ExtVolData), SysMonFractionToInt(ExtVolData));

		//wait for 3 seconds
		usleep(3000000);
	}

	cleanup_platform();
	return 1;

}



int SysMonFractionToInt(float FloatNum)
{
float Temp;
Temp = FloatNum;
if (FloatNum < 0) {
Temp = -(FloatNum);
}
return( ((int)((Temp -(float)((int)Temp)) * (1000.0f))));
}
