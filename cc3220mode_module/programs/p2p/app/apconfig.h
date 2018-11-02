/*
 * apconfig.h
 *
 *  Created on: Sep 10, 2018
 *      Author: Admin
 */


#ifndef APCONFIG_H_
#define APCONFIG_H_

#include <ti/devices/cc32xx/driverlib/utils.h>
#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/drivers/net/wifi/fs.h>

#define MAX_SSID_LEN            64
#define DEF_AP_IP_ADDR          0xC0A8DE01
#define DEF_SUBNET_MASK         0xFFFFFF00
#define DHCP_IP_START           0xC0A8DE02
#define DHCP_IP_END             0xC0A8DEFE
#define DHCP_LEASE_TIME         (1*3600)
#define PRODUCT_NAME            "Wifi-T"


int32_t ap_setname(char *dev_name, char *mac_addr);

int32_t ap_setip(int ip);

int32_t ap_dhcp_range(int ip_start, int ip_end);

int32_t ap_config(void);

#endif /* APCONFIG_H_ */
