/*
 * tcp.h
 *
 *  Created on: Aug 30, 2018
 *      Author: Admin
 */

#ifndef TCP_H_
#define TCP_H_



#define SPI_SIZE  		(816)
#define TCP_SIZE  		(816)



#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


void * TcpThread(void *param);
//extern void *TcpTask(void *param);


int tcp_read(int socket_id, unsigned char *buffer, int length);

int tcp_write(int socket_id, unsigned char *buffer, int length);

int tcp_open_server();

//int tcp_client(unsigned char *tcp_rx_buf);
int tcp_client();

#endif /* TCP_H_ */
