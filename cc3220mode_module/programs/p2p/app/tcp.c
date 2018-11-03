/*
 * tcp.c
 *
 *  Created on: Aug 30, 2018
 *      Author: Admin
 */

#include "tcp.h"
#include "shelltask.h"
#include "system.h"
#include "network_handler.h"
#include "sflash.h"
#include "main_thread.h"
#include "userconfig.h"
#include "cc3220_system.h"
#include "uart_terminal.h"
#include <pthread.h>
#include "spi_handler_temp.h"

#include <ti/devices/cc32xx/inc/hw_memmap.h>
#include <ti/devices/cc32xx/driverlib/udma.h>
#include <ti/devices/cc32xx/driverlib/spi.h>
#include <ti/devices/cc32xx/driverlib/pin.h>

#define SOCKET_TIMEOUT_S  			(0)
#define SOCKET_TIMEOUT_US			(0)
//#define SOCKET_TIMEOUT_US			(5000)





#define NUMBERSOCKET 			10

#define PORT                    2000

#define IP_ADDR                 0xc0a80168
unsigned int uip1 = IP_ADDR;


int32_t  gMode = -1;
extern sem_t slave;


unsigned char gBuff_tx[TCP_SIZE]= {0};
unsigned char gBuff_rx[TCP_SIZE] = {0};
unsigned char gBuff_rx1[TCP_SIZE] = {0};


int iStatus_init_tcp_done = 0;
int connectcl = 0;
int iSockID;
int iSockIDServer;

//extern pthread_mutex_t clientMutex;

//int tcp_read(int socket_id, char *buffer, int length)
int tcp_read(int socket_id, unsigned char *buffer, int length)
{
	int recv_len = 0;
	int ret_val = 0;
	SlTimeval_t tv;
	SlFdSet_t fdset;


//	UART_PRINT("tcp_read: read socket_id=%d length=%d\r\n", socket_id, length);
	if (buffer == NULL)
	{
		UART_PRINT(ANSI_COLOR_RED"tcp_read: buffer is NULL\n\r"ANSI_COLOR_RESET );
		return -1;
	}

	SL_SOCKET_FD_ZERO(&fdset);
	SL_SOCKET_FD_SET(socket_id, &fdset);

	// setup time out
	memset((char *)&tv, 0x00, sizeof(tv));
	tv.tv_sec = SOCKET_TIMEOUT_S;
	tv.tv_usec = SOCKET_TIMEOUT_US;

//	if (sl_Select(socket_id +1, &fdset, NULL, NULL, &tv) == 1)
//	{
		do
		{
			ret_val = sl_Recv(socket_id, buffer + recv_len, length - recv_len, 0);
//			ret_val = sl_Recv(socket_id, buffer + recv_len, length - recv_len, SL_MSG_DONTWAIT);
			if (ret_val > 0)
			{
//				UART_PRINT("tcp_read: read %d bytes\r\n", ret_val);
				recv_len += ret_val;
			}
			else if (ret_val == 0)
			{
				UART_PRINT("tcp_read: end\r\n", ret_val);
				break;
			}
			else
			{
				if (ret_val == SL_ERROR_BSD_EWOULDBLOCK || ret_val == SL_ERROR_BSD_EAGAIN)
				{
					UART_PRINT("tcp_read: read time out, read %d bytes\r\n", recv_len);
					break;
				}

				UART_PRINT(ANSI_COLOR_RED"tcp_read: read %d bytes\r\n"ANSI_COLOR_RESET, ret_val);
				recv_len = ret_val;
				break;
			}
		} while (recv_len < length);
//	}

	return recv_len;

}

int tcp_write(int socket_id, unsigned char *buffer, int length)
{

	int ret_val = -1;
	int remain_len = length;

	if (buffer == NULL)
	{
		UART_PRINT("tcp_write: buffer is NULL\n\r");
		return -1;
	}
	while (remain_len > 0)
	{
		ret_val = sl_Send(socket_id, buffer + (length - remain_len), length, 0);
		// FIXME: error handle
		if (ret_val == 0)
		{
			UART_PRINT("tcp_write: end\r\n");
			length -= remain_len;
			break;
		}
		else if (ret_val > 0)
		{
//			UART_PRINT("tcp_write: send %d bytes\r\n", ret_val);
			remain_len -= ret_val;
		}
		else
		{
			UART_PRINT("tcp_write: send error %d\r\n", ret_val);
			remain_len = ret_val;
			break;
		}
	}
	return length;
}

int tcp_open_server()
{
//	int32_t             iCounter;
	int32_t             iAddrSize;
    int32_t				iSockID;
    int32_t            	iStatus;

    SlSockAddr_t    *sa;
    SlSockAddr_t    *csa;
    sockAddr_t sAddr;


	int client_socket[10] = {0};

	int ret_val = 0;


	int32_t iNewSockID = SL_ERROR_BSD_EAGAIN;

    memset(client_socket, 0x0, sizeof(client_socket));

	sAddr.in4.sin_family = SL_AF_INET;
	sAddr.in4.sin_port = sl_Htons(PORT);
	sAddr.in4.sin_addr.s_addr = SL_INADDR_ANY;

	sa = (SlSockAddr_t*)&sAddr.in4;
	csa = (SlSockAddr_t*)&sAddr.in4;
	iAddrSize = sizeof(SlSockAddrIn_t);
	// creating a TCP socket
    iSockID = sl_Socket(sa->sa_family, SL_SOCK_STREAM, 0);
	ASSERT_ON_ERROR(iSockID, SL_SOCKET_ERROR);

	// binding the TCP socket to the TCP server address
	iStatus = sl_Bind(iSockID, sa, iAddrSize);

	if( iStatus < 0 )
	{
		// error
		ASSERT_ON_ERROR(iStatus, SL_SOCKET_ERROR);
		sl_Close(iSockID);
		return(-1);
	}


	iStatus = sl_Listen(iSockID, 10);
	if( iStatus < 0 )
	{
		ASSERT_ON_ERROR(iStatus, SL_SOCKET_ERROR);
		sl_Close(iSockID);
		return(-1);
	}

/*	SlSockNonblocking_t BlockingOption;
	BlockingOption.NonBlockingEnabled = 0;
	iStatus = sl_SetSockOpt(iSockID, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &BlockingOption, sizeof(BlockingOption));
	if(iStatus< 0)
	{
		UART_PRINT("[line:%d], [error:%d], [function: %s], [%s]\n\r", __LINE__, iStatus, __FUNCTION__,
				   SL_SOCKET_ERROR);
		sl_Close(iSockID);
		return(-1);
	}*/

	while( iNewSockID < 0 )
	{
		// accepts a connection form a TCP client, if there is any
		// otherwise returns SL_EAGAIN
		iNewSockID = sl_Accept(iSockID, csa, (SlSocklen_t*)&iAddrSize);
		UART_PRINT("iNewSockID:%d\n\r",iNewSockID);
		iSockIDServer = iNewSockID ;
		if( (iNewSockID == SL_ERROR_BSD_EAGAIN))
		{
			UART_PRINT("Sleep\n\r");
			sleep(1);
		}
		else if( iNewSockID < 0 )
		{
			// error
			UART_PRINT("[line:%d], [error:%d], [function:%s], [%s]\n\r", __LINE__, iNewSockID, __FUNCTION__, SL_SOCKET_ERROR);
			sl_Close(iNewSockID);
			sl_Close(iSockID);
			return(-1);
		}
	}

	UART_PRINT("Connected to client: ");
	sAddr.in4.sin_addr.s_addr = sl_Htonl(sAddr.in4.sin_addr.s_addr);
	PrintIPAddress(0,(void*)&sAddr.in4.sin_addr);
	UART_PRINT("\n\r");
	iStatus_init_tcp_done = 1;


    UART_PRINT(ANSI_COLOR_GREEN"Received From Tcp iNewSockID [%d] \n\r"ANSI_COLOR_RESET, iNewSockID);

    for(;;)
    {
//    	sem_post(&slave);
		ret_val = tcp_read(iNewSockID, gBuff_rx1, 2000);
//		spi_recv_handler(gBuff_rx1);
		spi_recv_handler();
//		memset(gBuff_rx1, 0x0, TCP_SIZE);
//		memset(gBuff_rx1, 0x0, TCP_SIZE);

//    	sl_Recv(iNewSockID, gBuff_rx1, TCP_SIZE, 0);
/*		if( ret_val > 0)
		{

			//do something
		}*/

    }



	return 0;
}

int tcp_client()
{
//    int iCounter = 0;
    int iAddrSize;
    int iStatus;
//    int rev_len = 0;
    int ret_val = 0;
//    int  i = 0;

    SlSockAddr_t	*sa;
    sockAddr_t 		sAddr;

	if(connectcl == 0)
	{
		memset(gBuff_tx, 0x0, TCP_SIZE);

		UART_PRINT(ANSI_COLOR_YELLOW"sizeof _u8:%d\n\r"ANSI_COLOR_RESET, sizeof(_u8));
		UART_PRINT(ANSI_COLOR_YELLOW"sizeof unsigned char:%d\n\r"ANSI_COLOR_RESET, sizeof(unsigned char));

		UART_PRINT(ANSI_COLOR_YELLOW"Init socket client\n\r"ANSI_COLOR_RESET);

		sAddr.in4.sin_family = SL_AF_INET;
		sAddr.in4.sin_port = sl_Htons(PORT);
		sAddr.in4.sin_addr.s_addr = sl_Htonl((unsigned int)uip1);

		sa = (SlSockAddr_t*)&sAddr.in4;
		iAddrSize = sizeof(SlSockAddrIn_t);

		// creating a TCP socket
		iSockID = sl_Socket(sa->sa_family, SL_SOCK_STREAM, 0);
		ASSERT_ON_ERROR(iSockID, SL_SOCKET_ERROR);

/*		SlSockNonblocking_t BlockingOption;
		BlockingOption.NonBlockingEnabled = 0;
		iStatus = sl_SetSockOpt(iSockID, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &BlockingOption, sizeof(BlockingOption));
		if(iStatus< 0)
		{
			UART_PRINT("[line:%d], [error:%d], [function: %s], [%s]\n\r", __LINE__, iStatus, __FUNCTION__,
					   SL_SOCKET_ERROR);
			sl_Close(iSockID);
			return(-1);
		}*/
		iStatus = -1;
		while(iStatus < 0)
		{
			UART_PRINT(ANSI_COLOR_GREEN"Try Connect To Server\n\r"ANSI_COLOR_RESET);
			iStatus = sl_Connect(iSockID, sa, iAddrSize);
			UART_PRINT(ANSI_COLOR_YELLOW"Client iStatus [%d] iSockID [%d]\n\r"ANSI_COLOR_RESET, iStatus, iSockID);
			if(iStatus < 0)
			{
				UART_PRINT("[line:%d], [error:%d], [function:%s], [%s]\n\r", __LINE__, iStatus, __FUNCTION__, SL_SOCKET_ERROR);
				sleep(1);
				continue;
//			sl_Close(iSockID);
//				return(-1);
			}
			else
			{
				UART_PRINT(ANSI_COLOR_GREEN"Client Connect Successful\n\r"ANSI_COLOR_RESET);
			}
		}
	}
	connectcl = 1;
	iStatus_init_tcp_done = 2;
/*	for(;;)
	{
//    	sem_post(&slave);
		ret_val = tcp_write(iSockID, gBuff_tx, TCP_SIZE);
		if(ret_val  > 0)
		{
			//do something
		}
	}*/
/*	for(;;)
	{
//    	sem_post(&slave);
		ret_val = tcp_read(iSockID, gBuff_rx, TCP_SIZE);
		if(ret_val  > 0)
		{
			//do something
		}
	}*/

	return 0;
}


#define SERVERENABLE	 			(0)


void * TcpThread(void *param)
{

	int RetVal = 0;
//	int iStatus = 0;
	char gversion_8char[9] = {0};
	get_version(gversion_8char);
	printBorder('*', 52);
	UART_PRINT("\n\r");
	UART_PRINT("\t\tCC3220 version= %s\r\n", gversion_8char);
	printBorder('*', 52);
	UART_PRINT("\n\r");

	gMode = sl_Start(0, 0, 0);
	UART_PRINT("Mode sl_Start: %d\n\r",gMode);
	if(gMode > 0)
	{
		system_init();
	}
	else
	{
		userconfig_flash_read();
	}

	RetVal = system_IsRegistered();
	UART_PRINT("-system_IsRegistered : %d\n\r",RetVal);
	if((RetVal < 0))
	{

		system_registration();
		osi_Sleep(1000);
		ClearTerm();

#if (SERVERENABLE == 1)
	    SlNetCfgIpV4Args_t ipV4;
	    ipV4.Ip          = (_u32)SL_IPV4_VAL(192,168,1,104);
	    ipV4.IpMask      = (_u32)SL_IPV4_VAL(255,255,255,0);
	    ipV4.IpGateway   = (_u32)SL_IPV4_VAL(192,168,1,1);
	    ipV4.IpDnsServer = (_u32)SL_IPV4_VAL(192,168,1,1);
	    sl_NetCfgSet(SL_NETCFG_IPV4_STA_ADDR_MODE,SL_NETCFG_ADDR_STATIC,sizeof(SlNetCfgIpV4Args_t),(_u8 *)&ipV4);
#endif
		system_reboot();

	}
	else
	{
		UART_PRINT("-REGISTER_DONE\n\r");
		GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);
		get_wireless_config();

#if (SERVERENABLE == 1)

		tcp_open_server();
#else
		tcp_client();
#endif



#if 0
		iStatus = client_group1();

		if(!iStatus)
		{
			UART_PRINT("Connect sockets\n\r");
			flag = 1;
			sem_post(&clientsem);
			sleep(2);
			while(1)
			{

				 GetCmd(gcBsdBuf_temp, sizeof(gcBsdBuf_temp));
				 UART_PRINT("%s\n\r",gcBsdBuf_temp);
				 UART_PRINT("\n\r");
				 pthread_mutex_lock(&clientMutex);
				 sprintf(gcBsdBuf_tx,"%s%s", gcBsdBuf_temp, "\n\r");
				 iStatus = sl_Send(iSockID, &gcBsdBuf_tx, sizeof(gcBsdBuf_tx), 0);
//				 flag = 1;
				 memset(gcBsdBuf_tx, 0x0, sizeof(gcBsdBuf_tx));
				 memset(gcBsdBuf_temp, 0x0, sizeof(gcBsdBuf_temp));
				 GPIO_toggle(Board_GPIO_LED1);
				 pthread_mutex_unlock(&clientMutex);
			}

		}
#endif
	}
	return 0;
}
