/*
 * spi_handler.h
 *
 *  Created on: Sep 18, 2018
 *      Author: CNN
 */

#ifndef SPI_HANDLER_H_
#define SPI_HANDLER_H_

#define SPI_MASTER_MODE			(0)

enum SPI_PACKET_TYPE
{
	CMD_GET = 0x00,
	CMD_SET = 0x01,
	MAX_PACKET_TYPE
};


void * spiSlaveHandleTask(void * param);
void spi_init();
void spi_deinit(void);
void PinMuxConfig(void);
void UDMAInit();
void DmaErrorIntHandler(void);
void DmaSwIntHandler(void);
void spi_deinit(void);
int recv_buff_spi(char *l_data_buff);
int send_buff_spi(char *l_data_buff);
void spi_slave_handler();
void spi_slave_init();
void init_buff();
int spi_data_processing(void *spi_data, uint32_t spi_data_len, void * spi_respone, uint32_t respone_spi_sz);
void UDMASetupTransfer(unsigned long ulChannel, unsigned long ulMode,
                            unsigned long ulItemCount, unsigned long ulItemSize,
                            unsigned long ulArbSize, void *pvSrcBuf,
                            unsigned long ulSrcInc, void *pvDstBuf,
                            unsigned long ulDstInc);
#endif /* SPI_HANDLER_H_ */
