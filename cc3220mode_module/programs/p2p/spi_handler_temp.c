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


#define ENABLE_MODE_SPI_WRITE		(0)


#define SPI_SIZE  					(1632)


unsigned char tx_buffer_spi[SPI_SIZE] = {0};

unsigned char rx_buffer_spi[SPI_SIZE] = {0};

unsigned char tx_buffer_tcp[2][SPI_SIZE] = {0};



int grecv_pos_write = 0;
int grecv_pos_read = 0;

SPI_Handle      slaveSpi1;

/*char *bootmem = (char *) 0x20000000;

char *send_ptr = NULL;*/
int gsend_pos_write = 0;
int gsend_pos_read = 0;

extern sem_t slave;
extern unsigned char gBuff_rx[];

extern unsigned char gBuff_tx[];

extern int iSockIDServer;
extern int iSockID;
extern int iStatus_tcp_done;

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
	gsend_pos_write = ((gsend_pos_write + 1) % 2);
	sem_post(&slave);
//	UART_PRINT("CALLBACK:%d \n\r", gsend_pos_write);
}

extern sem_t clientsem;

void spi_tranfer_handler()
{

	bool            transferOK1;
	SPI_Transaction transaction1;
	int ret_val = 0;
//	struct timespec ts;
//	clock_gettime(CLOCK_REALTIME, &ts);
//	ts.tv_sec += 3;

	for(;;)
	{
		memset((void *)rx_buffer_spi, 0x0, SPI_SIZE);

		memcpy(tx_buffer_spi, gBuff_rx, SPI_SIZE/2);


		transaction1.count = 818;
		transaction1.txBuf = (void *)(tx_buffer_spi);
		transaction1.rxBuf = (void *)(rx_buffer_spi);

	//	sem_timedwait(&clientsem, &ts);
		transferOK1 = SPI_transfer(slaveSpi1, &transaction1);

		if(transferOK1)
		{
			sem_wait(&slave);
	//		memcpy(&tx_buffer_tcp[((gsend_pos_write + 1) % 2)][0], rx_buffer_spi, SPI_SIZE);


			if(iStatus_tcp_done == 1)
			{
	//			ret_val = tcp_write(iSockIDServer, &tx_buffer_tcp[((gsend_pos_write + 1)%2)][0], SPI_SIZE);
				ret_val = tcp_write(iSockIDServer, rx_buffer_spi, SPI_SIZE);
			}

			if(iStatus_tcp_done == 2)
			{
	//				tcp_write(iSockID, rx_buffer_spi, SPI_SIZE);
	//			ret_val = tcp_write(iSockID, &tx_buffer_tcp[((gsend_pos_write + 1)%2)][0], SPI_SIZE);
				ret_val = tcp_write(iSockID, rx_buffer_spi, SPI_SIZE);
			}

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
		spiParams1.transferMode = SPI_MODE_CALLBACK;
		spiParams1.transferCallbackFxn = transferFxn1;
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
	spi_slave_init = 1;

	spi_tranfer_handler();

//	SPI_close(slaveSpi);

}

void *SlaveHandleTask(void *param)
{
/*	struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 2;
    sem_timedwait(&slave, &ts);*/

	spi_slave_init_handler();
/*
	for(;;)
	{
		spi_tranfer_handler();
	}
*/

	return(0);
}
