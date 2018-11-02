/*
 * userconfig.c
 *
 *  Created on: Sep 5, 2018
 *      Author: Admin
 */
#include "userconfig.h"
#include "sflash.h"
#include "main_thread.h"

UserConfig USERCONFIG;

#if (PLATFORM == CC3220)

static _i32 lFileHandle;
static unsigned long ulToken;

#endif

unsigned char *get_userconfig_pointer()
{
    return (unsigned char *)&USERCONFIG;
}
int get_userconfig_size()
{
    return sizeof(USERCONFIG);
}
static int simple_cksum(unsigned char* arg, int len_of_arg)
{
    int cksum=0;
    int i;
    if(!arg)
    {
        return -1;
    }
    for (i=0;i<len_of_arg;i++)
    {
        cksum += arg[i];
    }
    return cksum;

}

int userconfig_init()
{
    memset(&USERCONFIG, 0, sizeof(USERCONFIG));
#if(PLATFORM == CC3220)
    InitFile(&ulToken, lFileHandle);
#endif
   return 0;
}

static void feed_data(char *destination, char *source, int len)
{
    if(!destination || len<=0)
        return;
    if(source)
        memcpy(destination, source, len);
    else
        memset(destination, 0, len);
}
static void feed_len(int *destination, int len)
{
    if(!destination || len<=0)
    {
        return;
    }
    *destination = len;
}

#if 0
static void feed_word(int *destination, int len)
{
    if(!destination)
        return;
    *destination = len;
}
#endif

static int need_checksum(int id)
{
    switch(id)
    {
        case MASTERKEY_ID:
        case TOKEN_ID:
            return 1;
        default:
            break;
    }
    return 0;
}

static int assign_information(int id, int *expected_len, int **len_desbuffer, char **data_desbuffer, int **checksum)
{
    switch(id)
    {
		case MODE_ID:
			*len_desbuffer = (int *)&(USERCONFIG.mode);
		break;
		case WIFI_SSID_ID:
			*expected_len = WIFI_SSID_LEN;
			*len_desbuffer = (int *)&(USERCONFIG.wifi_ssid_len);
			*data_desbuffer = (char *)USERCONFIG.wifi_ssid;
		break;
		case WIFI_KEY_ID:
			*expected_len = WIFI_KEY_LEN;
			*len_desbuffer = (int *)&(USERCONFIG.wifi_key_len);
			*data_desbuffer = (char *)USERCONFIG.wifi_key;
		break;
		default:
			return -1;
		break;
    }
    return 0;
}

int userconfig_set(char *data_buffer, int data_len, int id)
{
    int expected_len;
    char *data_desbuffer = NULL;
    int *len_desbuffer = NULL;
//    int *len_sourcebuffer = NULL;
    int *checksum = NULL;
    if(id>=MAX_ELEMENT || id<0)
    {
        return -1;
    }
    if(assign_information(id, &expected_len, &len_desbuffer, &data_desbuffer, &checksum)!=0)
    {
        return -1;
    }
    if(id == MODE_ID)//special case for mode field
    {
        if(len_desbuffer!=NULL)
        {
            *len_desbuffer = data_len;
            return 0;
        }
        else
        {
            return -1;
        }
    }
    if(!data_buffer)
    {
        return -1;
    }

    if(data_len > expected_len)
    {
        return -1;
    }

    if(len_desbuffer!=NULL)
    {
        feed_len(len_desbuffer, data_len);
    }

    if(data_desbuffer!=NULL)
    {
        feed_data((char *)data_desbuffer, data_buffer, data_len);
    }

    if((need_checksum(id)) && (checksum!=NULL))
    {
        *checksum = simple_cksum((unsigned char *)data_buffer, WORD_LEN);
    }
    return 0;
}
int userconfig_flash_read()
{

#if(PLATFORM == CC3220)
    return ReadFileFromDevice(ulToken, lFileHandle, (unsigned char*) &USERCONFIG, sizeof(USERCONFIG));
#endif

    return 0;
}
int userconfig_flash_write()
{
#if(PLATFORM == CC3220)

    return WriteFileToDevice(&ulToken, lFileHandle, (unsigned char*) &USERCONFIG, sizeof(USERCONFIG));
#endif

    return 0;
}
int userconfig_get(char *data_buffer, int buf_sz, int id)
{
    int expected_len;
    char *data_desbuffer = NULL;
    int *len_desbuffer = NULL;
//    int *len_sourcebuffer = NULL;
    int *checksum = NULL;
    if(id >= MAX_ELEMENT || id < 0)
    {
        return -1;
    }
    if(assign_information(id, &expected_len, &len_desbuffer, &data_desbuffer, &checksum)!=0)
    {
        return -1;
    }
    if(id == MODE_ID)//special case for mode field
    {
        if(len_desbuffer!=NULL)
        {
            return *len_desbuffer;
        }
        else
        {
            return -1;
        }
    }
    /*Get length of element*/
    /*Get the real data of element*/
    if (*len_desbuffer > buf_sz)
    {
        UART_PRINT("len_desbuffer=%d, buf_sz=%d\n\r", len_desbuffer, buf_sz);
        return -3;
    }
    if(data_desbuffer && len_desbuffer)
    {
        feed_data(data_buffer, (char *)data_desbuffer, *len_desbuffer);
    }
    else
    {
        return -1;
    }
    /*Get checksum if needed*/
    if(need_checksum(id) && checksum)
    {
        int ck = simple_cksum((unsigned char *)data_buffer, WORD_LEN);
        if(ck != *checksum)
        {
            return -2; //check sum fail
        }
    }
    return *len_desbuffer;

}
