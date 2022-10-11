/*
 * Copyright (C) 2018 - 2019 Xilinx, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 */

/* Connection handle for a TCP Client session */

#include "tcp_perf_client.h"
#include "my_variables.h"

#define Server_IP	"72.138.97.82"//"192.168.1.12"//
#define TCP_PORT	80

static struct tcp_pcb *c_pcb;


static struct perf_stats client;


static void print_tcp_conn_stats()
{
#if LWIP_IPv6==1
	xil_printf("[%3d] local %s port %d connected with ",
			client.client_id, inet6_ntoa(c_pcb->local_ip),
			c_pcb->local_port);
	xil_printf("%s port %d\r\n",inet6_ntoa(c_pcb->remote_ip),
			c_pcb->remote_port);
#else
	xil_printf("[%3d] local %s port %d connected with ",
			client.client_id, inet_ntoa(c_pcb->local_ip),
			c_pcb->local_port);
	xil_printf("%s port %d\r\n",inet_ntoa(c_pcb->remote_ip),
			c_pcb->remote_port);
#endif /* LWIP_IPV6 */

}

/** Close a tcp session */
static void tcp_client_close(struct tcp_pcb *pcb)
{
	err_t err;

	if (pcb != NULL) {
		tcp_sent(pcb, NULL);
		tcp_err(pcb, NULL);
		err = tcp_close(pcb);
		if (err != ERR_OK) {
			/* Free memory with abort */
			tcp_abort(pcb);
		}
	}
}

/** Error callback, tcp session aborted */

err_t TCP_Send(char SendData[])
{
	err_t err;
	u8_t apiflags = TCP_WRITE_FLAG_COPY | TCP_WRITE_FLAG_MORE;
	if (c_pcb == NULL) {
		xil_printf("TCP client: Error on c_tcp..\r\n");
		return ERR_CONN;
	}

//	while (tcp_sndbuf(c_pcb) > sizeof(SendData)) {
		err = tcp_write(c_pcb, SendData, strlen(SendData), apiflags);
		xil_printf("size of SendData is: %d\r\n",strlen(SendData));
		if (err != ERR_OK) {
					xil_printf("TCP client: Error on tcp_write: %d\r\n",err);
					return err;
				}
		//Forces all enqueued data to be sent now.
		err = tcp_output(c_pcb);
		if (err != ERR_OK) {
					xil_printf("TCP client: Error on tcp_output: %d\r\n", err);
					return err;
				}
//	}
}

static err_t tcp_Rec(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
	c_pcb = tpcb;
	/* if we receive an empty tcp frame from client => close connection */
	  if (p == NULL) {
	    /* remote host closed connection */
		  xil_printf("We have received an empty packet and we close the connection \n\r");
		  tcp_client_close(c_pcb);
		  c_pcb = NULL;
		  return ERR_OK;
	    } else {
	    	xil_printf("We have received an packet \n\r");
	    	//tcp_recved(tpcb, p->tot_len);
	    	xil_printf("The received packet is: %d bytes and it includes:\n\r\n\r %s \n\r",p->len,p->payload);
	    	p = NULL;
	    	pbuf_free(p);
	    	return ERR_OK;
	    }
}

/** TCP connected callback (active connection), send data now */
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
	if (err != ERR_OK) {
		tcp_client_close(tpcb);
		xil_printf("Connection error\n\r");
		return err;
	}
	/* store state */
	c_pcb = tpcb;

	client.start_time = get_time_ms();
	client.end_time = TCP_TIME_INTERVAL * 1000; /* ms */
	client.client_id++;
	clientID++;
	client.total_bytes = 0;

	/* report interval time in ms */
	client.i_report.report_interval_time = INTERIM_REPORT_INTERVAL * 1000;
	client.i_report.last_report_time = 0;
	client.i_report.start_time = 0;
	client.i_report.total_bytes = 0;


	xil_printf("The connection request has been sent \r\n");
	print_tcp_conn_stats();


	char sentData[200];
	strcpy(sentData,"GET /\r\n");
	if (TCP_Send(sentData) != ERR_OK)
		xil_printf("Failed to send the message \r\n");
	else xil_printf("We have sent the message quote\r\n");

	/* initiate data transfer */
	return ERR_OK;
}





void start_application(void)
{
	err_t err;
	struct tcp_pcb* pcb;
	ip_addr_t remote_addr;


#if LWIP_IPV6==1
	remote_addr.type= IPADDR_TYPE_V6;
	err = inet6_aton(TCP_SERVER_IPV6_ADDRESS, &remote_addr);
#else
	err = inet_aton(Server_IP, &remote_addr);
#endif /* LWIP_IPV6 */

	if (!err) {
		xil_printf("Invalid Server IP address: %d\r\n", err);
		return;
	}

	/* Create Client PCB */
	pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
	if (!pcb) {
		xil_printf("Error in PCB creation. out of memory\r\n");
		return;
	}

	xil_printf("Sending connection request to the server \r\n");
	err = tcp_connect(pcb, &remote_addr, TCP_PORT,	tcp_client_connected);
	if (err) {
		xil_printf("Error on tcp_connect: %d\r\n", err);
		tcp_client_close(pcb);
		return;
	}

	tcp_recv(pcb,	tcp_Rec);
		if (err) {
			xil_printf("Error on tcp_recv: %d\r\n", err);
			return;
		}

	client.client_id = 0;
	clientID = 0;

	return;
}
