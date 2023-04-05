
#include "xil_io.h"
#include "platform.h"
#include "xil_printf.h"

int main()
{
    init_platform();
    u64 ADRS ;
    u32 din = 0x00002020;
    u32 i = 0;
    ADRS = XPAR_BRAM_0_BASEADDR ;

    xil_printf("\n\r Hello From Zynq \n\r");
	usleep(7000000);
	xil_printf("\n\r Start filling the BRAM \n\r");
	while(ADRS < XPAR_BRAM_0_HIGHADDR)
			{
				Xil_Out32(ADRS, 0xFFFFFFFF);
				ADRS += 4 ;
				//xil_printf("i=%i  0x%08lx    ",i, din);
			}
	        xil_printf("\n\r End of filling the BRAM \n\r");

	        ADRS = XPAR_BRAM_0_BASEADDR ;
	    	while(ADRS < XPAR_BRAM_0_HIGHADDR)
	    			{
	    				din = Xil_In32(ADRS);
	    				if (din != 0xFFFFFFFF)
	    					xil_printf("Error at address  0x%08lx  ", ADRS);

	    				ADRS += 4 ;
	    				i++;
	    				//xil_printf("i=%i  0x%08lx    ",i, din);
	    			}
	    	xil_printf("End i is   %i  ", i);



	    	usleep(7000000);
	    	ADRS = XPAR_BRAM_0_BASEADDR ;
	    	xil_printf("\n\r Start filling the BRAM \n\r");
	    	while(ADRS < XPAR_BRAM_0_HIGHADDR)
	    			{
	    				Xil_Out32(ADRS, 0x88888888);
	    				ADRS += 4 ;
	    				//xil_printf("i=%i  0x%08lx    ",i, din);
	    			}
	    	        xil_printf("\n\r End of filling the BRAM \n\r");

	    	        ADRS = XPAR_BRAM_0_BASEADDR ;
	    	    	while(ADRS < XPAR_BRAM_0_HIGHADDR)
	    	    			{
	    	    				din = Xil_In32(ADRS);
	    	    				if (din != 0x88888888)
	    	    					xil_printf("Error at address  0x%08lx   reading:  0x%08lx ", ADRS,din);

	    	    				ADRS += 4 ;
	    	    				i++;
	    	    				//xil_printf("i=%i  0x%08lx    ",i, din);
	    	    			}
	    	    	xil_printf("End i is   %i  ", i);

}
