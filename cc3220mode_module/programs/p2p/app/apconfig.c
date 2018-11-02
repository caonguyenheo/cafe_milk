/*
 * apconfig.c
 *
 *  Created on: Sep 10, 2018
 *      Author: Admin
 */

#include <string.h>
#include <stdio.h>

#include "apconfig.h"
#include "cc3220_system.h"
#include "main_thread.h"
#include "main_thread.h"
char wifi_name[32];
int32_t ap_setname(char *dev_name, char *mac_addr)
{
    int32_t ret_val = -1;
    char ssid[MAX_SSID_LEN+1];

    if ((NULL == dev_name) || (NULL == mac_addr))
    {
        return -1;
    }


    if (strlen(dev_name) + strlen(mac_addr) + 1 > MAX_SSID_LEN)
    {
        return -1;
    }

    int32_t mac_pos = strlen(mac_addr) - 6;
    if (mac_pos < 0)
    {
        return -1;
    }

    snprintf(ssid, sizeof(ssid), "%s%s%s", dev_name, STR_MODEL, &mac_addr[mac_pos]);
    sprintf( wifi_name, "%s%s%s", dev_name, STR_MODEL, &mac_addr[mac_pos]);
    UART_PRINT("SSID_BoardCast: %s\n\r", ssid);

    ret_val = sl_WlanSet(SL_WLAN_CFG_AP_ID, SL_WLAN_AP_OPT_SSID, strlen(ssid), (unsigned char*)ssid);
    if (ret_val < 0)
    {
        return ret_val;
    }

    ret_val = sl_Stop(SL_STOP_TIMEOUT);

    return sl_Start(NULL, NULL, NULL);
}

int32_t ap_setip(int ip)
{
    int32_t ret_val = -1;
    SlNetCfgIpV4Args_t ipv4;
    ipv4.Ip = ip;
    ipv4.IpMask = DEF_SUBNET_MASK;
    ipv4.IpGateway = ip;
    ipv4.IpDnsServer = ip;

    ret_val = sl_NetCfgSet(SL_NETCFG_IPV4_STA_ADDR_MODE, 1, sizeof(SlNetCfgIpV4Args_t), (uint8_t *)&ipv4);
    if (ret_val < 0)
    {
        return ret_val;
    }

    ret_val = sl_Stop(SL_STOP_TIMEOUT);

    return sl_Start(NULL, NULL, NULL);
}

int32_t ap_dhcp_range(int ip_start, int ip_end)
{
    int32_t ret_val = -1;
    SlNetAppDhcpServerBasicOpt_t dhcp_params;
    dhcp_params.lease_time = DHCP_LEASE_TIME;
    dhcp_params.ipv4_addr_start = ip_start;
    dhcp_params.ipv4_addr_last = ip_end;
    sl_NetAppStop(SL_NETAPP_DHCP_SERVER_ID);
    ret_val = sl_NetAppSet(SL_NETAPP_DHCP_SERVER_ID, SL_NETAPP_DHCP_SRV_BASIC_OPT, sizeof(SlNetAppDhcpServerBasicOpt_t), \
                           (uint8_t *)&dhcp_params);
    if (ret_val < 0)
    {
        return ret_val;
    }
    return sl_NetAppStart(SL_NETAPP_DHCP_SERVER_ID);
}

int32_t ap_config()
{
    int32_t ret_val = -1;
    char    mac_addr[MAX_MAC_ADDR_SIZE] = {0};

    get_mac_address(mac_addr);

    ret_val = ap_setname(PRODUCT_NAME, mac_addr);
    if (ret_val < 0)
    {
        UART_PRINT("Set AP name failed \n");
        return ret_val;
    }

/*    ret_val = ap_setip(DEF_AP_IP_ADDR);
    if (ret_val < 0)
    {
        UART_PRINT("Set AP IP address failed \n");
        return ret_val;
    }

    ret_val = ap_dhcp_range(DHCP_IP_START, DHCP_IP_END);
    if (ret_val < 0)
    {
        UART_PRINT("Set AP DHCP failed \n");
        return ret_val;
    }*/
    return 0;
}
