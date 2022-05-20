
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include <xparameters_ps.h>
#include "xil_io.h"
#include "xparameters.h"

//#define DDR_BASEARDDR      XPAR_DDR_MEM_BASEADDR + 0x10000000

// These values are defined in xparameters_ps.h
//#define XPAR_DDR_MEM_BASEADDR 0x00000000U
//#define XPAR_DDR_MEM_HIGHADDR 0x3FFFFFFFU

int main()
{
    init_platform();
    UINTPTR ADRS1, ADRS2;
    u32 i = 0 ;
    u16 j = 0 ;

    u8 c ;
    ADRS1 = XPAR_DDR_MEM_BASEADDR + 0x00000010;
    ADRS2 = XPAR_DDR_MEM_BASEADDR + 0x00020010;

    j = 0x1234 ;
    c = (u8)('A');

    print("Hello \n\r\n\r");
	// In this loop we write into the DDR ram (Xil_Out8, Xil_Out16).
    for(i=0;i<5;i++)
    {
    	Xil_Out8(ADRS1+i, c);
    	Xil_Out16(ADRS2+2*i, j);    // Since we write 16 bits or two bytes, we should increment the address twice
    	xil_printf("%x %x %c\n\r",i,c,c);
    	c++;
    	j++;
    }
    print("\n\rReading from DDR ram (ADRS1): \n\r\n\r");
    ADRS1 = XPAR_DDR_MEM_BASEADDR + 0x00000010;
    j = 0x0000 ;
	// In this loop we read from DDR ram (Xil_in8). We read from address ADRS1
    for(i=0;i<5;i++)
    {
    	ADRS1++;
    	c = Xil_In8(ADRS1);
    	xil_printf("at ADRS1 %x %x %c\n\r",i,c,c);
    }
    xil_printf("\n\rReading from DDR ram (ADRS2): \n\r\n\r\n\r");
    ADRS2 = XPAR_DDR_MEM_BASEADDR + 0x00020010;
    for(i=0;i<15;i++)
    {
    	c = Xil_In8(ADRS2);
    	ADRS2++;
    	xil_printf("at ADRS2 %x %x\n\r",i,c);
    }

    print("Successfully ran DDR Memory read and write application");
    cleanup_platform();
    return 0;
}

