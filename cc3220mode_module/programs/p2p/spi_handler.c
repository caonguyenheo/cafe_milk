/*
 * spi_handler.c
 *
 *  Created on: Sep 18, 2018
 *      Author: CNN
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>


#include "uart_terminal.h"
#include "main_thread.h"
#include "spi_handler.h"

#include <ti/devices/cc32xx/inc/hw_memmap.h>
#include <ti/devices/cc32xx/driverlib/udma.h>
#include <ti/devices/cc32xx/driverlib/spi.h>
#include <ti/display/Display.h>
#include <ti/devices/cc32xx/driverlib/pin.h>
#include <ti/devices/cc32xx/driverlib/interrupt.h>
#include <ti/devices/cc32xx/inc/hw_ints.h>
#include <ti/devices/cc32xx/inc/hw_mcspi.h>


#define SPI_20_MHZ              200000000
#define ENABLE_MODE_SPI			(1)
#define PIN_READY_SPI			(0)

#define SPICMD_OFFSET_ID		(0)
#define SPICMD_OFFSET_TPYE		(SPICMD_OFFSET_ID+1)
#define SPICMD_OFFSET_ADDR		(SPICMD_OFFSET_ID+2)
#define SPICMD_OFFSET_DATA		(SPICMD_OFFSET_ID+2)
#define SPIRESP_OFFSET_ID		(0)
#define SPIRESP_OFFSET_DATA		(1)
#define MYSPI_CONFIG			(1)




#define SPI_MSG_LENGTH  (1024)
//#define SPI_MSG_LENGTH  (50)
#define SLAVE_MSG       ("Hello from slave, msg#: ")



#define MAX_LOOP_SPI        (100)
#define BUFF_SIZE			(16)
#define RING_BUFF_SIZE		(1024 * 16)
#define SPI_SEND_BUFF_SIZE	((BUFF_SIZE/2)* 1024)
#define SPI_RECV_BUFF_SIZE	(BUFF_SIZE * 1024)
#define DMA_SIZE                1024



//static unsigned char slaveRxBuffer[SPI_MSG_LENGTH] = {0};
//static unsigned char slaveTxBuffer[SPI_MSG_LENGTH] = {0};
static unsigned char data_buff[SPI_MSG_LENGTH] = {0};
static void spi_transfer1(uint8_t* tx, uint8_t* rx);
static unsigned int interrupt_count;
static unsigned int transfer_count;
static unsigned int tx_count;
static unsigned int rx_count = 0;
static unsigned char tx_buffer[1024] = {0};
static unsigned char rx_buffer[1024] = {0};
//static Display_Handle display;

#if (SPI_MASTER_MODE)
#define SPI_MSG_LENGTH_MT  (50)
#define MASTER_MSG      ("Hello slaves from master , msg#: ")
unsigned char masterRxBuffer[SPI_MSG_LENGTH_MT];
unsigned char masterTxBuffer[SPI_MSG_LENGTH_MT];
#endif
/* Semaphore to block master until slave is ready for transfer */
//extern sem_t slaveSem;
extern sem_t slave;
#if (SPI_MASTER_MODE)
extern sem_t masterSem;
#endif
//sem_t slaveSem;
//sem_t masterSem;
int gspi_recv_pos_write = 0;
int gspi_recv_pos_read = 0;
int gspi_init_buff_status = 0;
int gspi_init = 0;

char *boot_mem = (char *) 0x20000000;
//#pragma DATA_SECTION(boot_mem,".mem1")
//char *boot_mem = NULL;
char *spi_send_ptr = NULL;
int gspi_send_pos_write = 0;
int gspi_send_pos_read = 0;
int flag1 = 0;

/*SPI_Handle      slaveSpi;
bool            transferOK;
SPI_Transaction transaction;
SPI_Params      spiParams;*/
extern sem_t slaveSem;
SPI_Transaction transaction;
uint32_t        i;
bool            transferOK;

SPI_Params      spiParams;
SPI_Handle      slaveSpi;

static void print_buffer(uint8_t * buffer, _u32 bufLen, int strat_pos)
{
    int i = 0;

    for (i = strat_pos; i < bufLen; i++)
    {
        UART_PRINT("%02X", buffer[i]);
    }
    UART_PRINT("\n\r");
}
unsigned short swap2byte(unsigned char highByte, unsigned char lowByte)
{
	unsigned short x = 0;
	x = (((highByte << 8) & 0xff00) | (lowByte & 0x00ff));
	return(x);
}
void UDMASetupTransfer(unsigned long ulChannel, unsigned long ulMode,
                            unsigned long ulItemCount, unsigned long ulItemSize,
                            unsigned long ulArbSize, void *pvSrcBuf,
                            unsigned long ulSrcInc, void *pvDstBuf,
                            unsigned long ulDstInc)
{
    MAP_uDMAChannelControlSet(ulChannel, ulItemSize | ulSrcInc | ulDstInc | ulArbSize);
    MAP_uDMAChannelAttributeEnable(ulChannel,UDMA_ATTR_USEBURST);
    MAP_uDMAChannelTransferSet(ulChannel, ulMode, pvSrcBuf, pvDstBuf, ulItemCount);
    MAP_uDMAChannelEnable(ulChannel);
}
static void spi_transfer1(uint8_t* tx, uint8_t* rx)
{
    MAP_SPIDisable(GSPI_BASE);
    UDMASetupTransfer(UDMA_CH6_GSPI_RX, UDMA_MODE_BASIC, 20,
                      UDMA_SIZE_8, UDMA_ARB_1,
                      (void *)(GSPI_BASE + MCSPI_O_RX0), UDMA_SRC_INC_NONE,
                      rx, UDMA_DST_INC_8);

    UDMASetupTransfer(UDMA_CH7_GSPI_TX, UDMA_MODE_BASIC, 20,
                      UDMA_SIZE_8, UDMA_ARB_1,
                      tx, UDMA_SRC_INC_8,
                      (void *)(GSPI_BASE + MCSPI_O_TX0), UDMA_DST_INC_NONE);
    MAP_SPIEnable(GSPI_BASE);
}
void init_buff()
{
	if(gspi_init_buff_status == 1)
	{
		UART_PRINT("gspi_init_buff_status:%d\n\r",gspi_init_buff_status);
		return ;
	}

//	boot_mem = (char *)malloc(SPI_RECV_BUFF_SIZE);
//	memset(boot_mem, 0x00, 20);
/*    memset(boot_mem, 0x00, RING_BUFF_SIZE);*/
	spi_send_ptr = (char *)malloc(SPI_SEND_BUFF_SIZE);
	memset(spi_send_ptr, 0x0, SPI_SEND_BUFF_SIZE);
	gspi_init_buff_status = 1;
	UART_PRINT("init_buff done:%d\n\r", gspi_init_buff_status);

}
void transferCompleteFxn(SPI_Handle handle, SPI_Transaction *transaction)
{
    sem_post(&slaveSem);
}

#if (SPI_MASTER_MODE)
void slaveReadyFxn(uint_least8_t index)
{
    sem_post(&masterSem);
}
#endif

#if 0
void spi_slave_init1()
{
    SPI_Handle      slaveSpi;
    SPI_Params      spiParams;
    SPI_Transaction transaction;
    uint32_t        i;
    bool            transferOK;

    int status;
    unsigned short usbyte=0;
#if (PIN_READY_SPI)
    GPIO_setConfig(Board_SPI_SLAVE_READY, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_SPI_MASTER_READY, GPIO_CFG_INPUT);

    GPIO_write(Board_SPI_SLAVE_READY, 1);
    while (GPIO_read(Board_SPI_MASTER_READY) == 0);

/*    status  = sem_init(&slaveSem, 0, 0);

    if (status  != 0)
    {
    	UART_PRINT("Error creating slaveSem line [%d] function [%d]\n\r",__LINE__, __FUNCTION__);

        while(1);
    }*/
    while (GPIO_read(Board_SPI_MASTER_READY));
#endif

    SPI_Params_init(&spiParams);
    spiParams.frameFormat = SPI_POL0_PHA1;
    spiParams.mode = SPI_SLAVE;
    spiParams.bitRate = SPI_20_MHZ;
    spiParams.dataSize = 8;
#if (PIN_READY_SPI)
    spiParams.transferCallbackFxn = transferCompleteFxn;
    spiParams.transferMode = SPI_MODE_CALLBACK;
#endif
    slaveSpi = SPI_open(Board_SPI_SLAVE, &spiParams);
    if (slaveSpi == NULL)
    {
//    	Display_printf(display, 0, 0, "Error initializing slave SPI\n\r");
    	UART_PRINT("Error initializing slave SPI\n\r");
        while (1);
    }
    else
    {
    	UART_PRINT("Slave SPI initialized\n\r");
    }

    strncpy((char *) slaveTxBuffer, SLAVE_MSG, SPI_MSG_LENGTH);
//
//    for (i = 0; i < MAX_LOOP_SPI; i++)
	while (1)
	{
         /* Initialize slave SPI transaction structure */
//		slaveTxBuffer[sizeof(SLAVE_MSG) - 1] = (i % 10) + '0';
		memset((void *) slaveRxBuffer, 0, SPI_MSG_LENGTH);
		transaction.count = 20;
		transaction.txBuf = (void *) slaveTxBuffer;
		transaction.rxBuf = (void *) slaveRxBuffer;

         GPIO_toggle(Board_GPIO_LED1);

		transferOK = SPI_transfer(slaveSpi, &transaction);
		if (transferOK)
		{
//			char *pPacket = &slaveRxBuffer[0];
/*			GPIO_write(Board_SPI_SLAVE_READY, 0);
			sem_wait(&slaveSem);
			GPIO_write(Board_SPI_SLAVE_READY, 1);*/
//			sem_wait(&slaveSem);
/*			UART_PRINT("Slave received: %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n\r", slaveRxBuffer[0], \
					slaveRxBuffer[1], slaveRxBuffer[2], slaveRxBuffer[3], slaveRxBuffer[4], \
					slaveRxBuffer[5], slaveRxBuffer[6], slaveRxBuffer[7], slaveRxBuffer[8], \
					slaveRxBuffer[9], slaveRxBuffer[10], slaveRxBuffer[11], slaveRxBuffer[12],
					slaveRxBuffer[13], slaveRxBuffer[14], slaveRxBuffer[15], slaveRxBuffer[16], \
					slaveRxBuffer[17], \
					slaveRxBuffer[18], slaveRxBuffer[19], slaveRxBuffer[20], slaveRxBuffer[21], \
					slaveRxBuffer[22], slaveRxBuffer[23], slaveRxBuffer[24], slaveRxBuffer[25], \
					slaveRxBuffer[26]);*/

			UART_PRINT("Slave received: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n\r", slaveRxBuffer[0], \
								slaveRxBuffer[1], slaveRxBuffer[2], slaveRxBuffer[3], slaveRxBuffer[4], \
								slaveRxBuffer[5], slaveRxBuffer[6], slaveRxBuffer[7], slaveRxBuffer[8], \
								slaveRxBuffer[9], slaveRxBuffer[10], slaveRxBuffer[11], slaveRxBuffer[12],
								slaveRxBuffer[13], slaveRxBuffer[14], slaveRxBuffer[15], slaveRxBuffer[16], \
								slaveRxBuffer[17], \
								slaveRxBuffer[18], slaveRxBuffer[19], slaveRxBuffer[20]);

//		    	usbyte = *(slaveRxBuffer + i);
//		    	UART_PRINT("%02x\n\r",usbyte);


		}
		else
		{
			UART_PRINT("Unsuccessful slave SPI transfer\n\r");
		}
	}

	SPI_close(slaveSpi);
#if (PIN_READY_SPI)
	GPIO_setConfig(Board_SPI_MASTER_READY, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);
	GPIO_write(Board_SPI_SLAVE_READY, 0);
#endif
	UART_PRINT("Done\n\r");

}
#endif
int flag_inter = 0;

#if 1

void spi_slave_handler()
{
	uint32_t recv_data;
	uint32_t status = MAP_SPIIntStatus(GSPI_BASE, true);
//	UART_PRINT("status :%02X\n\r",status);
	int i = 0;
	 MAP_SPIIntClear(GSPI_BASE,SPI_INT_RX_FULL|SPI_INT_TX_EMPTY);

/*	if(status & SPI_INT_TX_EMPTY)
	{

		UART_PRINT("TX_EMPTY:%02X\n\r", status & SPI_INT_TX_EMPTY);
		MAP_SPIDataPut(GSPI_BASE,tx_buffer[tx_count % 1024]);
		tx_count++;

	}*/

	if(status & SPI_INT_RX_FULL)
	{
		MAP_SPIDataGetNonBlocking(GSPI_BASE,&rx_buffer[rx_count++]);
		if(rx_count == 18)
		{
			for(i = 0; i< rx_count; i++)
			{
				UART_PRINT("%02X",rx_buffer[i]);
			}
			UART_PRINT("\n\r");
			rx_count = 0;
			memset(rx_buffer, 0x0, sizeof(rx_buffer));
		}

/*
		UART_PRINT("RX_FULL:%02X\n\r",status & SPI_INT_RX_FULL);
//		MAP_SPIDataGetNonBlocking(GSPI_BASE,&recv_data);
		MAP_SPIDataGet(GSPI_BASE,&recv_data);
		UART_PRINT("RX_data1:%02X\n\r",recv_data);
		rx_buffer[rx_count % 1024] = recv_data;
		UART_PRINT("rx_buffer:%02X\n\r",rx_buffer[rx_count]);
		rx_count++;
*/
	}
}
#endif
int recv_buff_spi(char *l_data_buff)
{
		memcpy(l_data_buff, boot_mem + gspi_recv_pos_read, 19);
		UART_PRINT("recv_buff_spi\n\r");
		print_buffer(l_data_buff, 19, 0);

		return 1;

}
int send_buff_spi(char *l_data_buff)
{
	if(spi_send_ptr[gspi_send_pos_write] != 0)
	{
		memcpy(l_data_buff, spi_send_ptr + gspi_send_pos_write, 1024);
		gspi_send_pos_write = (gspi_send_pos_write + 1024) % SPI_SEND_BUFF_SIZE;
		return 1;
	}
	else
	{
		UART_PRINT("spi_send_ptr fail\n\r");
		return -1;
	}
}
int spi_data_processing(void *spi_data, uint32_t spi_data_len, void * spi_respone, uint32_t respone_spi_sz)
{
	unsigned char packet_type = MAX_PACKET_TYPE;
	int packet_addr = 0;
	int packet_addr1 = 0;
	int pdata = 0;
	int ret_val = 0;
	int i = 0;
	char *ptr_data = (char *)spi_data;

	if(ptr_data == NULL)
	{
		ret_val = -1;
		UART_PRINT("Error ptr_data \n\r");
		goto exit_error;
	}
	packet_type = *(ptr_data + SPICMD_OFFSET_TPYE);
	if(packet_type > MAX_PACKET_TYPE)
	{
		UART_PRINT("Error MAX_PACKET_TYPE \r\n");
		ret_val = -1;
		goto exit_error;
	}

	switch(packet_type)
	{
		case CMD_GET:
		{

		}
		break;
		case CMD_SET:
		{
			UART_PRINT("write data from MCU\n\r");
			UART_PRINT("Type:%02X\n\r", packet_type);
			packet_addr = *(ptr_data + 1);
			UART_PRINT("packet_addr:%02X\n\r", packet_addr);
			UART_PRINT("\n\r");
		}
		break;
		default:
			UART_PRINT("Unknown type of cmd!\n\r");
			break;
	}
	free(ptr_data);
exit_error:
	free(ptr_data);
	return ret_val;
}
void spi_slave_init()
{
    int status;
    int  i = 0;
	unsigned char packet_type = MAX_PACKET_TYPE;
	unsigned short packet_addr = 0;
	unsigned char highByte = 0;
	unsigned char lowByte = 0;
    if(gspi_init == 1)
    {
    	UART_PRINT("SPI slave has been initialized\n\r");
    	goto recv;
    }
    else
    {
		UART_PRINT("SPI slave init\n\r");
		SPI_Params_init(&spiParams);
		spiParams.frameFormat = SPI_POL0_PHA1;
		spiParams.mode = SPI_SLAVE;
		spiParams.bitRate = SPI_20_MHZ;
		spiParams.dataSize = 8;

		slaveSpi = SPI_open(Board_SPI_SLAVE, &spiParams);
		if (slaveSpi == NULL)
		{
			UART_PRINT("Error initializing slave SPI\n\r");
			while (1);
		}
		else
		{
			gspi_init = 1;
			UART_PRINT("Slave SPI initialized\n\r");
		}
    }
    recv:

    UART_PRINT("SPI TRANFER\n\r");
    while(1)
    {
    	memcpy(tx_buffer, rx_buffer+3, 16);
/*    	UART_PRINT("TX\n\r");
    	print_buffer(tx_buffer, 16, 0);*/

		memset((void *) rx_buffer, 0, SPI_MSG_LENGTH);
		transaction.count = 19;
		transaction.txBuf = (void *)tx_buffer;
		transaction.rxBuf = (void *)rx_buffer;
	    GPIO_toggle(Board_GPIO_LED1);

		transferOK = SPI_transfer(slaveSpi, &transaction);
		if(transferOK)
		{
			UART_PRINT("RX\n\r");
			print_buffer(rx_buffer, 19, 0);
			packet_type = *(rx_buffer + 0);
			switch(packet_type)
			{
				case CMD_GET:
				{

				}
				break;
				case CMD_SET:
				{
					UART_PRINT("write data from MCU\n\r");
					UART_PRINT("Type:%02X\n\r", packet_type);
					highByte = (*(rx_buffer + 1));
					lowByte = (*(rx_buffer + 2));
					packet_addr = swap2byte(highByte, lowByte);
					UART_PRINT("packet_addr:%02X\n\r", packet_addr);

				}
				break;
				default:
					UART_PRINT("Unknown type of cmd!\n\r");
					break;
			}
		}
		else
		{
			UART_PRINT("Unsuccessful slave SPI transfer\n\r");
		}

		UART_PRINT("Done\n\r");

    }
//	SPI_close(slaveSpi);
//	if(flag1 == 0)
//	{
//		UART_PRINT("spi deinit\n\r");
//		SPI_close(slaveSpi);
//	}

}
#if MYSPI_CONFIG

#define MAX_NUM_CH              64  //32*2 entries
#define CTL_TBL_SIZE            64  //32*2 entries
#define UDMA_CH5_BITID          (1<<5)
typedef void (*tAppCallbackHndl)(void);
#pragma DATA_ALIGN(gpCtlTbl, 1024)
tDMAControlTable gpCtlTbl[CTL_TBL_SIZE];

volatile unsigned char iDone;
tAppCallbackHndl gfpAppCallbackHndl[MAX_NUM_CH];
void DmaSwIntHandler(void)
{
    unsigned long uiIntStatus;
    iDone = 1;
    uiIntStatus = MAP_uDMAIntStatus();
    MAP_uDMAIntClear(uiIntStatus);
}
void DmaErrorIntHandler(void)
{
    MAP_uDMAIntClear(MAP_uDMAIntStatus());
}
void PinMuxConfig(void)
{

    MAP_PinTypeSPI(PIN_05, PIN_MODE_7);
    MAP_PinTypeSPI(PIN_06, PIN_MODE_7);
    MAP_PinTypeSPI(PIN_07, PIN_MODE_7);
    MAP_PinTypeSPI(PIN_08, PIN_MODE_7);
    MAP_PRCMPeripheralClkEnable(PRCM_GSPI, PRCM_RUN_MODE_CLK);
    MAP_PRCMPeripheralReset(PRCM_GSPI);
}
void UDMAInit()
{
    unsigned int uiLoopCnt;

    //
    // Enable McASP at the PRCM module
    //
    MAP_PRCMPeripheralClkEnable(PRCM_UDMA,PRCM_RUN_MODE_CLK);
    MAP_PRCMPeripheralReset(PRCM_UDMA);

	//
    // Register interrupt handlers
    //
#if defined(USE_TIRTOS) || defined(USE_FREERTOS) || defined(SL_PLATFORM_MULTI_THREADED)
	// USE_TIRTOS: if app uses TI-RTOS (either networking/non-networking)
	// USE_FREERTOS: if app uses Free-RTOS (either networking/non-networking)
	// SL_PLATFORM_MULTI_THREADED: if app uses any OS + networking(simplelink)
    osi_InterruptRegister(INT_UDMA, DmaSwIntHandler, INT_PRIORITY_LVL_1);
    osi_InterruptRegister(INT_UDMAERR, DmaErrorIntHandler, INT_PRIORITY_LVL_1);
#else
	MAP_IntPrioritySet(INT_UDMA, INT_PRIORITY_LVL_1);
    MAP_uDMAIntRegister(UDMA_INT_SW, DmaSwIntHandler);

	MAP_IntPrioritySet(INT_UDMAERR, INT_PRIORITY_LVL_1);
	MAP_uDMAIntRegister(UDMA_INT_ERR, DmaErrorIntHandler);
#endif

    //
    // Enable uDMA using master enable
    //
    MAP_uDMAEnable();

    //
    // Set Control Table
    //
    memset(gpCtlTbl,0,sizeof(tDMAControlTable)*CTL_TBL_SIZE);
    MAP_uDMAControlBaseSet(gpCtlTbl);

	//
    // Reset App Callbacks
    //
    for(uiLoopCnt = 0; uiLoopCnt < MAX_NUM_CH; uiLoopCnt++)
    {
        gfpAppCallbackHndl[uiLoopCnt] = NULL;
    }
}
void spi_init()
{

	MAP_PRCMPeripheralReset(PRCM_GSPI);
	MAP_SPIReset(GSPI_BASE);
	MAP_SPIConfigSetExpClk(GSPI_BASE, MAP_PRCMPeripheralClockGet(PRCM_GSPI),
	    						SPI_20_MHZ, SPI_MODE_SLAVE, SPI_SUB_MODE_0,
	                           (SPI_HW_CTRL_CS |
	                            SPI_4PIN_MODE |
	                            SPI_TURBO_ON |
	                            SPI_CS_ACTIVELOW |
	                            SPI_WL_8));


	    MAP_SPIIntRegister(GSPI_BASE, spi_slave_handler);
	    //
	    // Enable Interrupts
	    //
	    MAP_SPIIntEnable(GSPI_BASE,SPI_INT_RX_FULL|SPI_INT_TX_EMPTY);


	    MAP_SPIEnable(GSPI_BASE);
}
void spi_deinit(void)
{
//    MAP_SPIDisable(GSPI_BASE);
    memset(boot_mem, 0x00, 20);
    if (spi_send_ptr!=NULL)
    {
        memset(spi_send_ptr, 0x00, SPI_SEND_BUFF_SIZE);
    }
}


#endif
void * spiSlaveHandleTask(void * param)
{
	int ret_val = 0;

#if ENABLE_MODE_SPI
//	spi_slave_init1();
//	spi_slave_init();
	struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 14;
    sem_timedwait(&slave, &ts);
    int  i =0;
	interrupt_count = 0;
	transfer_count = 0;
	tx_count = 0;
	rx_count = 0;

	uint32_t output_count = 0;


	spi_slave_init();
//	spi_data_processing(data_buff, 19, NULL, 0);


/*	init_buff();

	ret_val = recv_buff_spi(data_buff);
	if (flag_inter == 1)
	{
		UART_PRINT("Disable SPI\n\r");
		spi_deinit();
		flag_inter  = 0;

	}*/





/*		sem_wait(&slave);
		UART_PRINT("gspi_init_buff_status_loop:%d\n\r",gspi_init_buff_status);
		init_buff();
		ret_val = recv_buff_spi(data_buff);*/
//	    MAP_PRCMPeripheralReset(PRCM_GSPI);



#else
	spi_master();
#endif

	return(0);
}
