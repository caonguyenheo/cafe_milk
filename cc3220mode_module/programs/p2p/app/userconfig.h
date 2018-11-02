/*
 * userconfig.h
 *
 *  Created on: Sep 5, 2018
 *      Author: Admin
 */

#ifndef USERCONFIG_H_
#define USERCONFIG_H_


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <ti/drivers/net/wifi/fs.h>

#define WORD_LEN						4
#define WIFI_SSID_LEN					(WORD_LEN*8)
#define WIFI_KEY_LEN					(WORD_LEN*16)

#define CC3220     		3
#define PLATFORM		CC3220


enum
{
	MODE_ID = 0,
    MASTERKEY_ID,
    TOKEN_ID,
    WIFI_SSID_ID,
    WIFI_KEY_ID,
    WIFI_SEC_ID,
    MAX_ELEMENT
};

typedef struct _UserConfig
{
	int mode;
    int wifi_ssid_len;
    unsigned char wifi_ssid[WIFI_SSID_LEN];

    int wifi_key_len;
    unsigned char wifi_key[WIFI_KEY_LEN];

} UserConfig;


int userconfig_init(void);
int userconfig_set(char *data_buffer, int data_len, int id);
int userconfig_get(char *data_buffer, int buf_sz, int id);
int userconfig_flash_read();
int userconfig_flash_write();
unsigned char *get_userconfig_pointer();
int get_userconfig_size();

#endif /* USERCONFIG_H_ */
