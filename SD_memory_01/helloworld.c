/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
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
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
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
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "ff.h"
#include "xstatus.h"
#include "xil_cache.h"


static FATFS fatfs;


static int SD_Init()
{
	FRESULT rc;
	TCHAR *Path = "0:/";
	rc = f_mount(&fatfs,Path,0);
	if (rc) {
		xil_printf(" ERROR : f_mount returned %d\r\n", rc);
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}
static int SD_Eject()
{
	FRESULT rc;
	TCHAR *Path = "0:/";
	rc = f_mount(&fatfs,Path,1);
	if (rc) {
		xil_printf(" ERROR : f_mount returned %d\r\n", rc);
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}

static int ReadFile(char *FileName,u32 DestinationAddress,u32 ByteLength)
{
    FIL fil;
    FRESULT rc;
    UINT br;

    rc = f_open(&fil,FileName,FA_READ);
    if(rc)
    {
        xil_printf("ERROR : f_open returned %d\r\n",rc);
        return XST_FAILURE;
    }
    rc = f_lseek(&fil, 0);
    if(rc)
    {
        xil_printf("ERROR : f_lseek returned %d\r\n",rc);
        return XST_FAILURE;
    }
    rc = f_read(&fil, (void*)DestinationAddress,ByteLength,&br);
    if(rc)
    {
        xil_printf("ERROR : f_read returned %d\r\n",rc);
        return XST_FAILURE;
    }
    rc = f_close(&fil);
    if(rc)
    {
        xil_printf(" ERROR : f_close returned %d\r\n", rc);
        return XST_FAILURE;
    }
    return XST_SUCCESS;
}

static int WriteFile(char *FileName,u32 SourceAddress,u32 ByteLength)
{
    FIL fil;
    FRESULT rc;
    UINT bw;

    rc = f_open(&fil,FileName,FA_CREATE_ALWAYS | FA_WRITE);
    if(rc)
    {
        xil_printf("ERROR : f_open returned %d\r\n",rc);
        return XST_FAILURE;
    }
    rc = f_lseek(&fil, 0);
    if(rc)
    {
        xil_printf("ERROR : f_lseek returned %d\r\n",rc);
        return XST_FAILURE;
    }
    rc = f_write(&fil,(void*) SourceAddress,ByteLength,&bw);
    if(rc)
    {
        xil_printf("ERROR : f_write returned %d\r\n", rc);
        return XST_FAILURE;
    }
    rc = f_close(&fil);
    if(rc){
        xil_printf("ERROR : f_close returned %d\r\n",rc);
        return XST_FAILURE;
    }
    return XST_SUCCESS;
}



int main()
{
    init_platform();


    print("\r\n\r\n\r\n\r\nInitializing the SD card The second round\r\n");
    usleep(5000000);
    int Status;
    int i=0;
        Status = SD_Init(&fatfs);
        if (Status != XST_SUCCESS)
        {
      	 print("file system init failed\n");
        	 return XST_FAILURE;
        }
        print("SD Card is initialized successfully \r\n");
        usleep(1000000);


        char SDtmp_str[200] ; //= "It is the first line writing in the SD card\r\nIt is the second line\r\nIt is the third Line\r\n";
        //for (i=0;i<200;i++) SDtmp_str[i]=NULL;
        //xil_printf("The content of SDtmp: \r\n%s\r\n",SDtmp_str);

/*
        print("Start writing to the SD card, SDfile.txt \r\n");
        Status = WriteFile("SDfile.txt",(u32)SDtmp_str,strlen(SDtmp_str)); // When the len is directly specified, it is not written, you need to specify a larger length to write out, the reason is unknown
        if (Status != XST_SUCCESS)
        {
           	print("file write failed\n");
           	return XST_FAILURE;
        }
        print("successfully wrote in the SDfile.txt. \r\n\r\n");

        for (i=0;i<200;i++) SDtmp_str[i]=NULL;
        usleep(1000000);
*/

        print("Start reading SDfile.txt from SD card\r\n");
        Status = ReadFile("SDfile.txt",(u32)SDtmp_str,200);
            if (Status != XST_SUCCESS)
            {
            	print("file read failed\r\n");
            	return XST_FAILURE;
            }
            print("The content of SDfile.txt is :\r\n");
            xil_printf("%s\r\n",SDtmp_str);
            for (i=0;i<200;i++) SDtmp_str[i]=NULL;

        usleep(1000000);
        print("Ejecting the SD card \r\n");
        Status=SD_Eject(&fatfs);
            if (Status != XST_SUCCESS)
            {
            	print("SD card unmount failed\r\n");
                return XST_FAILURE;
            }
           	xil_printf("\r\n\r\nThe project is done successfully...\r\n");

    cleanup_platform();
    return 0;
}
