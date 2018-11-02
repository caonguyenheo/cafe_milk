/*
 * spi_handler_temp.h
 *
 *  Created on: Oct 22, 2018
 *      Author: Admin
 */

#ifndef SPI_HANDLER_TEMP_H_
#define SPI_HANDLER_TEMP_H_


enum PACKET_TYPE
{
	CMD_GET = 0x00,
	CMD_SET = 0x01,
	MAX_PACKET
};

void *SlaveHandleTask(void *param);
void spi_slave_init_handler();
void spi_tranfer_handler();
void print_buffer_data(uint8_t * buffer, _u32 bufLen, int strat_pos);

#endif /* SPI_HANDLER_TEMP_H_ */
