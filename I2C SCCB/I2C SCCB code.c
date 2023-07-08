

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xiic.h"
#include "xgpiops.h"
#include "xscugic.h"



#define IIC_BASE_ADDRESS	XPAR_IIC_0_BASEADDR
#define IIC_DEVICE_ID	    XPAR_IIC_0_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_INTC_0_DEVICE_ID
#define SCCB_SLAVE_ADDRESS 0x30
#define GPIO_DEVICE_ID		XPAR_XGPIOPS_0_DEVICE_ID
#define cam_PWDN	54
#define cam_RST	55
XIic Iic ;
static void initI2C();
static int writeI2C(u8 *wdata, int ByteCount);
static int readI2C(u8 *rdata, int ByteCount);

static void initSCCB();
static int writeSCCB(u8 subAddr, u8 wdata);
static int readSCCB(u8 subAddr, u8 *rdata);

int main()
{

    init_platform();
    u8 *wdat ;
    u8 *rdat ;
    int Status;
    xil_printf("Hello from Zynq\n\r");
    XGpioPs Gpio;
    XGpioPs_Config *ConfigPtr;
    ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
    Status = XGpioPs_CfgInitialize(&Gpio, ConfigPtr, ConfigPtr->BaseAddr);
    if (Status != XST_SUCCESS) { xil_printf("Failed to init GPIO \n\r"); }
    XGpioPs_SetDirectionPin(&Gpio, cam_PWDN, 1);
    XGpioPs_SetOutputEnablePin(&Gpio, cam_PWDN, 1);
    XGpioPs_SetDirectionPin(&Gpio, cam_RST, 1);
    XGpioPs_SetOutputEnablePin(&Gpio, cam_RST, 1);
    XGpioPs_WritePin(&Gpio, cam_PWDN, 0);
    usleep(50);
    XGpioPs_WritePin(&Gpio, cam_RST, 0);
    usleep(50);
    XGpioPs_WritePin(&Gpio, cam_RST, 1);
    usleep(50);
    xil_printf("End of the GPIO code\n\r");


    initI2C();


    /////////////////////////////////////////////


    wdat[0] = 0xFF ;  wdat[1] = 0x00 ;   /* Reserved                              */
    Status = writeI2C(wdat, 2);
    if (Status != XST_SUCCESS) { xil_printf("Failed to write to the Camera \n\r"); }

    Status = readSCCB(0X51, rdat);
    if (Status != XST_SUCCESS) { xil_printf("Failed to write to the Camera \n\r"); } else
    	xil_printf("The read value is:  %02x\n\r",rdat[0]);
	

    /////////////////////////////////////////////



    cleanup_platform();
    return 0;
}


static void initI2C(){
	XIic_Config *ConfigPtr;
	ConfigPtr = XIic_LookupConfig(XPAR_IIC_0_DEVICE_ID);
	XIic_CfgInitialize(&Iic, ConfigPtr, ConfigPtr->BaseAddress);
	u32 CntlReg = XIic_ReadReg(Iic.BaseAddress, XIIC_CR_REG_OFFSET);
	CntlReg &= ~XIIC_CR_NO_ACK_MASK;
	XIic_WriteReg(Iic.BaseAddress, XIIC_CR_REG_OFFSET, CntlReg);
	XIic_Start(&Iic);
	XIic_SetAddress(&Iic, XII_ADDR_TO_SEND_TYPE, 0x30);
}



static int writeI2C(u8 *wdata, int ByteCount){
	int Status;//
	//to send/receive more than one byte
	Iic.Options = XII_REPEATED_START_OPTION ;
	//Status = XIic_MasterSend(&Iic, wdata, ByteCount);
	Status = XIic_Send(Iic.BaseAddress, SCCB_SLAVE_ADDRESS, wdata, ByteCount, XIIC_STOP);
	if (Status != ByteCount) { return XST_FAILURE; }
	while ((XIic_IsIicBusy(&Iic) == TRUE)){}
	Iic.Options = 0x0;
	Status = XIic_Stop(&Iic);
	if (Status != XST_SUCCESS) { return XST_FAILURE; }
	usleep(200);
	return XST_SUCCESS;
}

static int readI2C(u8 *rdata, int ByteCount){
	int Status;//
	//to send/receive more than one byte
	Iic.Options = XII_REPEATED_START_OPTION ;
	//Status = XIic_MasterRecv(&Iic, rdata, ByteCount);
	Status = XIic_Recv(Iic.BaseAddress, SCCB_SLAVE_ADDRESS, rdata, ByteCount, XIIC_STOP);
	if (Status != ByteCount) { return XST_FAILURE; }
	while ((XIic_IsIicBusy(&Iic) == TRUE)){}
	Iic.Options = 0x0;
	Status = XIic_Stop(&Iic);
	if (Status != XST_SUCCESS) { return XST_FAILURE; }
	usleep(200);
	//xil_printf("(1)The read value is:  %02x\n\r",rdata[0]);
	return XST_SUCCESS;
}


static void initSCCB(){
	initI2C();
}
static int writeSCCB(u8 subAddr, u8 wdata){
	u8 *wdat ;
	wdat[0] = subAddr ;
	wdat[1] = wdata ;
	int Status;
	Status = writeI2C(wdat, 2);
	return Status;
}
static int readSCCB(u8 subAddr, u8 *rdata){
	u8 *rdat ;
	rdat[0] = subAddr;
	int Status;
	Status = writeI2C(rdat, 1);
	if (Status != XST_SUCCESS) { return XST_FAILURE; }
	Status = readI2C(rdat, 1);
	if (Status != XST_SUCCESS) { return XST_FAILURE; }
	//xil_printf("(2)The read value is:  %02x\n\r",rdat[0]);
	rdata[0] = rdat[0];
	return XST_SUCCESS;
}
