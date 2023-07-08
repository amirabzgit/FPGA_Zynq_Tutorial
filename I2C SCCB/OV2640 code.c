

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


    wdat[0] = 0xFF ;
    wdat[1] = 0x01 ;

    initI2C();


    /////////////////////////////////////////////


  wdat[0] = 0xFF ;  wdat[1] = 0x00 ;   /* Reserved                              */
    Status = writeI2C(wdat, 2);
  wdat[0] = 0x2c ;  wdat[1] = 0xff ;   /* Reserved                              */
    Status = writeI2C(wdat, 2);
  wdat[0] = 0x2e ;  wdat[1] = 0xdf ;   /* Reserved                              */
    Status = writeI2C(wdat, 2);
  wdat[0] = 0xff ;  wdat[1] = 0x01 ;   /* Device control register list Table 13 */    Status = writeI2C(wdat, 2);
    Status = writeI2C(wdat, 2);
  wdat[0] = 0x3c ;  wdat[1] = 0x32 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x11 ;  wdat[1] = 0x00 ;   /* Clock Rate Control                    */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x09 ;  wdat[1] = 0x02 ;   /* Common control 2                      */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x04 ;  wdat[1] = 0xA8 ;   /* Mirror                                */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x13 ;  wdat[1] = 0xe5 ;   /* Common control 8                      */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x14 ;  wdat[1] = 0x48 ;   /* Common control 9                      */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x2c ;  wdat[1] = 0x0c ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x33 ;  wdat[1] = 0x78 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x3a ;  wdat[1] = 0x33 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x3b ;  wdat[1] = 0xfB ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x3e ;  wdat[1] = 0x00 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x43 ;  wdat[1] = 0x11 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x16 ;  wdat[1] = 0x10 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x4a ;  wdat[1] = 0x81 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x21 ;  wdat[1] = 0x99 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x24 ;  wdat[1] = 0x40 ;   /* Luminance signal High range           */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x25 ;  wdat[1] = 0x38 ;   /* Luminance signal low range            */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x26 ;  wdat[1] = 0x82 ;   /*                                       */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x5c ;  wdat[1] = 0x00 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x63 ;  wdat[1] = 0x00 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x46 ;  wdat[1] = 0x3f ;   /* Frame length adjustment               */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x0c ;  wdat[1] = 0x3c ;   /* Common control 3                      */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x61 ;  wdat[1] = 0x70 ;   /* Histogram algo low level              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x62 ;  wdat[1] = 0x80 ;   /* Histogram algo high level             */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x7c ;  wdat[1] = 0x05 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x20 ;  wdat[1] = 0x80 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x28 ;  wdat[1] = 0x30 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x6c ;  wdat[1] = 0x00 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x6d ;  wdat[1] = 0x80 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x6e ;  wdat[1] = 0x00 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x70 ;  wdat[1] = 0x02 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x71 ;  wdat[1] = 0x94 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x73 ;  wdat[1] = 0xc1 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x3d ;  wdat[1] = 0x34 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x5a ;  wdat[1] = 0x57 ;   /* Reserved                              */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x12 ;  wdat[1] = 0x00 ;   /* Common control 7                      */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x11 ;  wdat[1] = 0x00 ;   /* Clock Rate Control                   2*/
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x17 ;  wdat[1] = 0x11 ;   /* Horiz window start MSB 8bits          */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x18 ;  wdat[1] = 0x75 ;   /* Horiz window end MSB 8bits            */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x19 ;  wdat[1] = 0x01 ;   /* Vert window line start MSB 8bits      */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x1a ;  wdat[1] = 0x97 ;   /* Vert window line end MSB 8bits        */
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x32 ;  wdat[1] = 0x36 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x03 ;  wdat[1] = 0x0f ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x37 ;  wdat[1] = 0x40 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x4f ;  wdat[1] = 0xbb ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x50 ;  wdat[1] = 0x9c ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x5a ;  wdat[1] = 0x57 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x6d ;  wdat[1] = 0x80 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x6d ;  wdat[1] = 0x38 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x39 ;  wdat[1] = 0x02 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x35 ;  wdat[1] = 0x88 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x22 ;  wdat[1] = 0x0a ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x37 ;  wdat[1] = 0x40 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x23 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x34 ;  wdat[1] = 0xa0 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x36 ;  wdat[1] = 0x1a ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x06 ;  wdat[1] = 0x02 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x07 ;  wdat[1] = 0xc0 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x0d ;  wdat[1] = 0xb7 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x0e ;  wdat[1] = 0x01 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x4c ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xff ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xe5 ;  wdat[1] = 0x7f ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xf9 ;  wdat[1] = 0xc0 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x41 ;  wdat[1] = 0x24 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xe0 ;  wdat[1] = 0x14 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x76 ;  wdat[1] = 0xff ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x33 ;  wdat[1] = 0xa0 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x42 ;  wdat[1] = 0x20 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x43 ;  wdat[1] = 0x18 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x4c ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x87 ;  wdat[1] = 0xd0 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x88 ;  wdat[1] = 0x3f ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xd7 ;  wdat[1] = 0x03 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xd9 ;  wdat[1] = 0x10 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xd3 ;  wdat[1] = 0x82 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xc8 ;  wdat[1] = 0x08 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xc9 ;  wdat[1] = 0x80 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x7d ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x7c ;  wdat[1] = 0x03 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x7d ;  wdat[1] = 0x48 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x7c ;  wdat[1] = 0x08 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x7d ;  wdat[1] = 0x20 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x7d ;  wdat[1] = 0x10 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x7d ;  wdat[1] = 0x0e ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x90 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x91 ;  wdat[1] = 0x0e ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x91 ;  wdat[1] = 0x1a ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x91 ;  wdat[1] = 0x31 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x91 ;  wdat[1] = 0x5a ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x91 ;  wdat[1] = 0x69 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x91 ;  wdat[1] = 0x75 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x91 ;  wdat[1] = 0x7e ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x91 ;  wdat[1] = 0x88 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x91 ;  wdat[1] = 0x8f ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x91 ;  wdat[1] = 0x96 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x91 ;  wdat[1] = 0xa3 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x91 ;  wdat[1] = 0xaf ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x91 ;  wdat[1] = 0xc4 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x91 ;  wdat[1] = 0xd7 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x91 ;  wdat[1] = 0xe8 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x91 ;  wdat[1] = 0x20 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x92 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x93 ;  wdat[1] = 0x06 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x93 ;  wdat[1] = 0xe3 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x93 ;  wdat[1] = 0x02 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x93 ;  wdat[1] = 0x02 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x93 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x93 ;  wdat[1] = 0x04 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x93 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x93 ;  wdat[1] = 0x03 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x93 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x93 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x93 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x93 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x93 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x93 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x93 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x96 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x97 ;  wdat[1] = 0x08 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x97 ;  wdat[1] = 0x19 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x97 ;  wdat[1] = 0x02 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x97 ;  wdat[1] = 0x0c ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x97 ;  wdat[1] = 0x24 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x97 ;  wdat[1] = 0x30 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x97 ;  wdat[1] = 0x28 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x97 ;  wdat[1] = 0x26 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x97 ;  wdat[1] = 0x02 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x97 ;  wdat[1] = 0x98 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x97 ;  wdat[1] = 0x80 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x97 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x97 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xc3 ;  wdat[1] = 0xef ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xff ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xba ;  wdat[1] = 0xdc ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xbb ;  wdat[1] = 0x08 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xb6 ;  wdat[1] = 0x24 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xb8 ;  wdat[1] = 0x33 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xb7 ;  wdat[1] = 0x20 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xb9 ;  wdat[1] = 0x30 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xb3 ;  wdat[1] = 0xb4 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xb4 ;  wdat[1] = 0xca ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xb5 ;  wdat[1] = 0x43 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xb0 ;  wdat[1] = 0x5c ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xb1 ;  wdat[1] = 0x4f ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xb2 ;  wdat[1] = 0x06 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xc7 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xc6 ;  wdat[1] = 0x51 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xc5 ;  wdat[1] = 0x11 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xc4 ;  wdat[1] = 0x9c ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xbf ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xbc ;  wdat[1] = 0x64 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa6 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x1e ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x6b ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x47 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x33 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x23 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x2e ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x85 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x42 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x33 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x23 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x1b ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x74 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x42 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x33 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xa7 ;  wdat[1] = 0x23 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xc0 ;  wdat[1] = 0xc8 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xc1 ;  wdat[1] = 0x96 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x8c ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x86 ;  wdat[1] = 0x3d ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x50 ;  wdat[1] = 0x92 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x51 ;  wdat[1] = 0x90 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x52 ;  wdat[1] = 0x2c ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x53 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x54 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x55 ;  wdat[1] = 0x88 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x5a ;  wdat[1] = 0x50 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x5b ;  wdat[1] = 0x3c ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x5c ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xd3 ;  wdat[1] = 0x04 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x7f ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xda ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xe5 ;  wdat[1] = 0x1f ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xe1 ;  wdat[1] = 0x67 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xe0 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xdd ;  wdat[1] = 0x7f ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x05 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xff ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xe0 ;  wdat[1] = 0x04 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xc0 ;  wdat[1] = 0xc8 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xc1 ;  wdat[1] = 0x96 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x86 ;  wdat[1] = 0x3d ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x50 ;  wdat[1] = 0x92 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x51 ;  wdat[1] = 0x90 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x52 ;  wdat[1] = 0x2c ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x53 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x54 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x55 ;  wdat[1] = 0x88 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x57 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x5a ;  wdat[1] = 0x50 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x5b ;  wdat[1] = 0x3c ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x5c ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xd3 ;  wdat[1] = 0x04 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xe0 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xFF ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x05 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xDA ;  wdat[1] = 0x08 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xda ;  wdat[1] = 0x09 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x98 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x99 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x00 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xff ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xe0 ;  wdat[1] = 0x04 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xc0 ;  wdat[1] = 0xc8 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xc1 ;  wdat[1] = 0x96 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x86 ;  wdat[1] = 0x3d ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x50 ;  wdat[1] = 0x89 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x51 ;  wdat[1] = 0x90 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x52 ;  wdat[1] = 0x2c ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x53 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x54 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x55 ;  wdat[1] = 0x88 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x57 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x5a ;  wdat[1] = 0xA0 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x5b ;  wdat[1] = 0x78 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0x5c ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xd3 ;  wdat[1] = 0x02 ;
	Status = writeI2C(wdat, 2);
  wdat[0] = 0xe0 ;  wdat[1] = 0x00 ;
	Status = writeI2C(wdat, 2);






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
