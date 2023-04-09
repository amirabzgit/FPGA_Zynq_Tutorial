#include "xil_io.h"
#include "platform.h"
#include "xil_printf.h"

int main()
{
    init_platform();
    u64 ADRS ;
    u32 din = 0x00002020;
    ADRS = XPAR_BRAM_0_BASEADDR ;

    xil_printf("\n\r Hello From Zynq \n\r");
	Xil_Out32(ADRS, 0xF0F0F0F0);
    xil_printf("\n\r End of filling the BRAM \n\r");
    ADRS = XPAR_BRAM_0_BASEADDR ;
	din = Xil_In32(ADRS);
	xil_printf("Error at address  0x%08lx  ", din);
}
