/*
 * my_lwip_lib.h
 *
 *  Created on: Jun. 9, 2022err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err);
 *      Author: zaabxa
 */

 #include "lwip/err.h"
 #include "lwip/tcp.h"
 #include "xil_printf.h"


#ifndef SRC_MY_LWIP_LIB_H_
#define SRC_MY_LWIP_LIB_H_

#define PLATFORM_EMAC_BASEADDR XPAR_XEMACPS_0_BASEADDR
#define PLATFORM_ZYNQ


void print_ip(char *msg, ip_addr_t *ip);
 err_t recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
 err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err);
 int start_application();



#endif /* SRC_MY_LWIP_LIB_H_ */




 ip_addr_t *dummy_ipaddr;

 /*
  * my_lwip_lib.cpp
  *
  *  Created on: Jun. 9, 2022
  *      Author: zaabxa
  */






 /* This function print the ip on the console */

 void print_ip(char *msg, ip_addr_t *ip)
 {
 	print(msg);
 	xil_printf("%d.%d.%d.%d\n\r", ip4_addr1(ip), ip4_addr2(ip),
 			ip4_addr3(ip), ip4_addr4(ip));
 }




 err_t recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
 {
 	/* do not read the packet if we are not in ESTABLISHED state */
 	if (!p) {
 		tcp_close(tpcb);
 		tcp_recv(tpcb, NULL);
 		return ERR_OK;
 	}

 	*dummy_ipaddr	= tpcb->remote_ip ;
 	print_ip("we have received the following data from: ", dummy_ipaddr);


 	/* indicate that the packet has been received */
 	/* This function must be called when the application has received the data. len indicates the length of the received data.
 	 */
 	tcp_recved(tpcb, p->len);

 	/* echo back the payload */
 	/* in this case, we assume that the payload is < TCP_SND_BUF */

 	xil_printf(" the size of the packet is 		: %d \n\r", p->len);
 	xil_printf(" the content of the packet is 	: %s \n\r", p->payload);

 	if (tcp_sndbuf(tpcb) > p->len) {
 		err = tcp_write(tpcb, p->payload, p->len, 1);
 	} else
 		xil_printf("no space in tcp_sndbuf\n\r");

 	/* free the received pbuf */
 	pbuf_free(p);

 	return ERR_OK;
 }

 err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
 {
 	static int connection = 1;


 	*dummy_ipaddr	= newpcb->remote_ip;
 	print_ip("A connection established from: ", dummy_ipaddr);



 	/* set the receive callback for this connection */
 	/* Sets the callback function that will be called when new data arrives
 	   on the connection associated with pcb. The callback function will be
 	   passed a NULL pbuf to indicate that the remote host has closed the connection.*/
 	tcp_recv(newpcb, recv_callback);

 	/* just use an integer number indicating the connection id as the
 	   callback argument */
 	tcp_arg(newpcb, (void*)(UINTPTR)connection);
 	/* increment for subsequent accepted connections */
 	connection++;
 	return ERR_OK;
 }


 int start_application()
 {
 	struct tcp_pcb *pcb;
 	err_t err;
 	unsigned port = 7;
 		/* create new TCP PCB structure */
 	pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
 	if (!pcb) {
 		xil_printf("Error creating PCB. Out of Memory\n\r");
 		return -1;
 	}

 		/* bind to the specified @port */
 	err = tcp_bind(pcb, IP_ANY_TYPE, port);
 	if (err != ERR_OK) {
 		xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
 		return -2;
 	}

 		/* we do not need any arguments to callback functions */
 	tcp_arg(pcb, NULL);

 		/* listen for connections */
 	pcb = tcp_listen(pcb);
 	if (!pcb) {
 		xil_printf("Out of memory while tcp_listen\n\r");
 		return -3;
 	}
 	xil_printf("The Zynq start listening requests on @ port %d\n\r", port);

 		/* Specify the callback function that should be called when a new connection arrives for a listening TCP PCB. */
 	tcp_accept(pcb, accept_callback);

 	xil_printf("The Zynq start accepting the request \n\r");


 	return 0;

 }

