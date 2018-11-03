/*
 * spi_handler_temp.c
 *
 *  Created on: Oct 22, 2018
 *      Author: Admin
 */


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>


#include "uart_terminal.h"
#include "main_thread.h"
#include "spi_handler_temp.h"
#include "system.h"
#include "tcp.h"

#include <ti/devices/cc32xx/inc/hw_memmap.h>
#include <ti/devices/cc32xx/driverlib/udma.h>
#include <ti/devices/cc32xx/driverlib/spi.h>
#include <ti/display/Display.h>
#include <ti/devices/cc32xx/driverlib/pin.h>
#include <ti/devices/cc32xx/driverlib/interrupt.h>
#include <ti/devices/cc32xx/inc/hw_ints.h>
#include <ti/devices/cc32xx/inc/hw_mcspi.h>



#define SPI_20_MHZ              200000000

#define PIN_READY_SPI			(0)


#define BUFF_SIZE1			(16)
#define RING_BUFF_SIZE		(1024 * 16)
#define SPI_SEND_BUFF_SIZE	((BUFF_SIZE1/2)* 1024)
#define SPI_RECV_BUFF_SIZE	(BUFF_SIZE1 * 1024)
#define DMA_SIZE			(1024)


unsigned char tx_buffer1[SPI_SIZE] = {0};

unsigned char rx_buffer1[SPI_SIZE] = {0};

unsigned short grecv_pos_write = 0;
unsigned short grecv_pos_read = 0;
int ginit_buff_status = 0;
SPI_Handle      slaveSpi1;

/*char *bootmem = (char *) 0x20000000;

char *send_ptr = NULL;*/
int gsend_pos_write = 0;
int gsend_pos_read = 0;

extern sem_t slave;
extern unsigned char gBuff_rx[];
extern unsigned char gBuff_rx1[];

extern int iSockIDServer;
extern int iSockID;
//extern int iSockIDServer;
//extern int connectserver;
int spi_slave_init = 0;

void print_buffer_data(uint8_t * buffer, _u32 bufLen, int strat_pos)
{
    int i = 0;

    for (i = strat_pos; i < bufLen; i++)
    {
        UART_PRINT("%02X", buffer[i]);
    }
    UART_PRINT("\n\r");
}

unsigned short swapbyte2(unsigned char highByte, unsigned char lowByte)
{
	unsigned short x = 0;
	x = (((highByte << 8) & 0xff00) | (lowByte & 0x00ff));
	return(x);
}

void transferFxn1(SPI_Handle handle, SPI_Transaction *transaction)
{
	sem_post(&slave);
//	UART_PRINT("CALLBACK \n\r");
}


#define ENABLE_MODE_SPI			(0)


extern int iStatus_init_tcp_done;
extern unsigned char gBuff_tx[];

//void spi_recv_handler(unsigned char *buffer)
void spi_recv_handler()
{

	bool            transferOK1;
	SPI_Transaction transaction1;
	int i = 0;
    	memset((void *)rx_buffer1, 0x0, SPI_SIZE);
    	memset((void *)tx_buffer1, 0x0, SPI_SIZE);

#if(ENABLE_MODE_SPI == 1)
//    	memcpy(tx_buffer1, buffer, SPI_SIZE);
    	memcpy(tx_buffer1, gBuff_rx1, SPI_SIZE);
#endif


		transaction1.count = SPI_SIZE;
		transaction1.txBuf = (void *)(tx_buffer1);
		transaction1.rxBuf = (void *)(NULL);
//		transaction1.rxBuf = (void *)(rx_buffer1);

		transferOK1 = SPI_transfer(slaveSpi1, &transaction1);

}
//void spi_tranfer_handler()
void spi_tranfer_handler()
{

	bool            transferOK1;
	SPI_Transaction transaction1;
	int i = 0;
    for(;;)
    {
    	memset((void *)rx_buffer1, 0x0, SPI_SIZE);
//    	memset((void *)tx_buffer1, 0x0, SPI_SIZE);
#if(ENABLE_MODE_SPI == 2)
    	memcpy(tx_buffer1, gBuff_rx1, SPI_SIZE);
#endif

/*
#if(ENABLE_MODE_SPI)

    	memcpy(tx_buffer1, gBuff_rx1, SPI_SIZE);

#else

    	memcpy(tx_buffer1, gBuff_rx, SPI_SIZE);

#endif
*/

		transaction1.count = SPI_SIZE;
		transaction1.txBuf = (void *)(NULL);
//		transaction1.txBuf = (void *)(tx_buffer1);
		transaction1.rxBuf = (void *)(rx_buffer1);

		transferOK1 = SPI_transfer(slaveSpi1, &transaction1);

		if(transferOK1)
		{
//			sem_wait(&slave);
#if(ENABLE_MODE_SPI == 0)

				if(iStatus_init_tcp_done == 2)
				{
					tcp_write(iSockID, rx_buffer1, 2000);
				}
				usleep(5000);
//				memset((void *)tx_buffer1, 0x0, SPI_SIZE);

#endif
/*
#if(ENABLE_MODE_SPI)

			if(iStatus_init_tcp_done == 1)
			{
				tcp_write(iSockIDServer, rx_buffer1, TCP_SIZE);
			}

#else

			if(iStatus_init_tcp_done == 2)
			{
				tcp_write(iSockID, rx_buffer1, TCP_SIZE);
			}

#endif
*/
		}
		else
		{
			UART_PRINT("Unsuccessful slave SPI transfer\n\r");
		}
	}
}
void spi_slave_init_handler()
{

	SPI_Params      spiParams1;


	if(spi_slave_init == 0)
	{
		UART_PRINT(ANSI_COLOR_YELLOW"[Slave SPI Start Init]\n\r"ANSI_COLOR_RESET);
		SPI_Params_init(&spiParams1);
		spiParams1.frameFormat = SPI_POL0_PHA0;
		spiParams1.mode = SPI_SLAVE;
/*		spiParams1.transferMode = SPI_MODE_CALLBACK;
		spiParams1.transferCallbackFxn = transferFxn1;*/
		spiParams1.bitRate = SPI_20_MHZ;
		spiParams1.dataSize = 8;


		slaveSpi1 = SPI_open(Board_SPI_SLAVE, &spiParams1);
		if (slaveSpi1 == NULL)
		{
			UART_PRINT(ANSI_COLOR_RED "Error initializing slave SPI\n\r"ANSI_COLOR_RESET);
			while (1);
		}
		else
		{

			UART_PRINT(ANSI_COLOR_GREEN"Slave SPI Init Done [%u]\n\r"ANSI_COLOR_RESET,slaveSpi1);
		}
	}

	if(spi_slave_init == 0)
	{
#if(ENABLE_MODE_SPI == 1)

	spi_recv_handler(NULL);
#else

	spi_tranfer_handler();

#endif
	}
	spi_slave_init = 1;


//	SPI_close(slaveSpi);

}

void *SlaveHandleTask(void *param)
{
/*	struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 2;
    sem_timedwait(&slave, &ts);*/

	spi_slave_init_handler();
//	for(;;)
//	{
//
//    	spi_tranfer_handler();
//    	sleep(2);
//	}

/*	for(;;)
	{
		spi_tranfer_handler();
	}*/
//    transferFxn1(slaveSpi, &transaction);

	return(0);
}
