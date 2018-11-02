/*
 * cc3220_system.c
 *
 *  Created on: Sep 10, 2018
 *      Author: Admin
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cc3220_system.h"
#include "apconfig.h"
#include "nxcGenUDID.h"
#include <ti/drivers/net/wifi/simplelink.h>
#include "system.h"

static char g_device_mac[MAX_MAC_ADDR_SIZE] = {0};
static char g_udid[MAX_UID_SIZE] = {0};


int get_mac_address(char * str_mac_addr)
{
	int ret_val;
	static uint8_t mac_addr_init = 0;
	_u8 mac_addr[SL_MAC_ADDR_LEN];
	_u16 macAddressLen = SL_MAC_ADDR_LEN;

	if (str_mac_addr == NULL)
	{
		return -1;
	}

	if (mac_addr_init == 0)
	{
		ret_val = sl_NetCfgGet(SL_NETCFG_MAC_ADDRESS_GET, NULL, &macAddressLen,(_u8 *) mac_addr);
		if (ret_val != 0)
		{
			return ret_val;
		}

		ret_val = snprintf(g_device_mac, MAX_MAC_ADDR_SIZE, "%02X%02X%02X%02X%02X%02X",
		         mac_addr[0], mac_addr[1], mac_addr[2],
		         mac_addr[3], mac_addr[4], mac_addr[5]);

		if (ret_val <= 0)
		{
			return -1;
		}

		mac_addr_init = 1;
	}

	strcpy(str_mac_addr, g_device_mac);
	return 0;
}

int get_uid(char *uid)
{
	static uint8_t uid_init = 0;

	if (uid == NULL)
	{
		return -1;
	}

	if (uid_init == 0)
	{
		char mac_addr[MAX_MAC_ADDR_SIZE] = {0};
		get_mac_address(mac_addr);
		memset(g_udid, 0, MAX_UID_SIZE);
		inxcGenUDID(VENDOR, TYPE_NUM, STR_MODEL, mac_addr, g_udid);
		uid_init = 1;
	}

	strncpy(uid, g_udid, strlen(g_udid));
	return 0;
}

int get_version(char *l_version)
{
	fw_version_format(l_version, SYSTEM_VERSION);
	return 0;
}
