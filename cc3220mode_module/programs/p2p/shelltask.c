/*
 * shelltask.c
 *
 *  Created on: Aug 27, 2018
 *      Author: Admin
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* TI-DRIVERS Header files */
#include <ti/drivers/net/wifi/simplelink.h>

/* Example/Board Header files */
#include "main_thread.h"
#include "tcp.h"
#include "system.h"
#include "network_handler.h"
#include "sflash.h"
#include "cc3220_system.h"
#include "userconfig.h"

#define FOREVER			(1)

int ParseNProcessCmd(char *pcCmdBuffer);
int32_t cmd_set_wifi(char *param);
int32_t cmd_set_tcp(char *param);
int32_t cmd_get_tcp(char *param);
int32_t cmd_get_tcp_file (char *param);
int32_t cmd_set_tcp_file (char *param);
int32_t get_ip();
int cmd_send_server(char *param);


void * ShellThread(void *param)
{
    int lRetVal;
    char acCmdStore[512];
    while(FOREVER)
    {
    	UART_PRINT("user:\n\r");
    	UART_PRINT("\n\r");
		lRetVal = GetCmd(acCmdStore, sizeof(acCmdStore));

		if (lRetVal < 0)
		{

			UART_PRINT("Command length exceeded 512 bytes \r\n");
		}
		else
		{
			lRetVal = ParseNProcessCmd(acCmdStore);
			if (lRetVal < 0)
			{
				UART_PRINT("Error in processing command\n\r");
			}
		}
    }

//    return ;
}

int ParseNProcessCmd(char *pcCmdBuffer)
{
	char *pcInpString;
	int iRetVal = -1;
	pcInpString = strtok(pcCmdBuffer, " ");

	if (!strcmp(pcInpString, "help"))
	{
		UART_PRINT("\n\r");
		DisplayUsage();
		iRetVal = 0;
	}
	else if (!strcmp(pcInpString, "connect"))
	{
		iRetVal = cmd_set_wifi(pcInpString);
		if (iRetVal<0)
		{
			UART_PRINT("FAIL\n\r");
		}
		else
		{
			UART_PRINT("OK\n\r");
			GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);
		}
		iRetVal = 0;
	}

	else if (!strcmp(pcInpString, "reboot"))
	{
		UART_PRINT("\n\r");
		system_reboot();
		iRetVal = 0;
	}
	else if (!strcmp(pcInpString, "disconnect"))
	{
		UART_PRINT("\n\r");
		sl_WlanProfileDel(0xFF);
		sl_WlanPolicySet(SL_WLAN_POLICY_SCAN, SL_SCAN_POLICY(0), 0, 0);
		iRetVal = Network_IF_DeInitDriver();
		if (iRetVal<0)
		{
			UART_PRINT("FAIL\n\r");
		}
		else
		{
			UART_PRINT("OK\n\r");
			GPIO_write(Board_GPIO_LED1, Board_GPIO_LED_ON);
		}
		system_reboot();
		iRetVal = 0;
	}
	else if (!strcmp(pcInpString, "recv"))
	{

		iRetVal = cmd_get_tcp(pcInpString);
		if(iRetVal < 0)
		{
			UART_PRINT("FAIL recv\n\r");
		}
		else
		{
			UART_PRINT("OK recv\n\r");
		}
		iRetVal = 0;
	}

	else if (!strcmp(pcInpString, "send"))
	{
		iRetVal = cmd_set_tcp(pcInpString);
		if(iRetVal < 0)
		{
			UART_PRINT("FAIL send\n\r");
		}
		else
		{
			UART_PRINT("OK send\n\r");
		}
		iRetVal = 0;
	}
	else if (!strcmp(pcInpString, "sendfile"))
	{
		iRetVal = cmd_set_tcp_file(pcInpString);
		if(iRetVal < 0)
		{
			UART_PRINT("FAIL sendfile\n\r");
		}
		else
		{
			UART_PRINT("OK sendfile\n\r");
		}
		iRetVal = 0;
	}
	else if (!strcmp(pcInpString, "recvfile"))
	{
		iRetVal = cmd_get_tcp_file(pcInpString);
		if(iRetVal < 0)
		{
			UART_PRINT("FAIL recvfile\n\r");
		}
		else
		{
			UART_PRINT("OK recvfile\n\r");
		}
		iRetVal = 0;
	}
	else if (!strcmp(pcInpString, "ip"))
	{
		iRetVal = get_ip();
		if(iRetVal < 0)
		{
			UART_PRINT("FAIL Get Ip\n\r");
		}
		else
		{
			UART_PRINT("OK Ip\n\r");
		}
		iRetVal = 0;
	}
	else if (!strcmp(pcInpString, "ver"))
	{
		char gversion_8char[9] = {0};
		iRetVal = get_version(gversion_8char);
		if(iRetVal < 0)
		{
			UART_PRINT("FAIL Get version\n\r");
		}
		else
		{
			UART_PRINT("version currently: [%s]\n\r",gversion_8char);
			UART_PRINT("OK version\n\r");
		}
		iRetVal = 0;
	}
	else if (!strcmp(pcInpString, "clear"))
	{
		ClearTerm();
		iRetVal = 0;
	}
	else if (!strcmp(pcInpString, "resetfactory"))
	{
        osi_Sleep(1000);
        userconfig_set(NULL, DEV_NOT_REGISTERED, MODE_ID);
        userconfig_flash_write();
        UART_PRINT("\n\r");
        UART_PRINT("OK\n\r");
        system_reboot();
        iRetVal = 0;
	}
	else if(!strcmp(pcInpString, "client"))
	{
		iRetVal = cmd_send_server(pcInpString);
		if(iRetVal < 0)
		{
			UART_PRINT("FAIL\n\r");
		}
		else
		{
			UART_PRINT("OK\n\r");
		}
		iRetVal = 0;
	}
	else
	{
		UART_PRINT("Unsupported command\n\r");
		return iRetVal;
	}
	return iRetVal;
}


int32_t cmd_set_wifi(char *param)
{
    char *ssid = NULL;
    char *passwd = NULL;
    char *authtype = NULL;

    int lRetVal;

    ssid = strtok(NULL, " ");
    if (ssid == NULL)
    {
        return -1;
    }
    passwd = strtok(NULL, " ");
    if (passwd == NULL)
    {
        return -1;
    }

    authtype = strtok(NULL, " ");
    if (authtype == NULL)
    {
        return -1;
    }

    UART_PRINT("\r\n\t- TRY TO CONNECT: %s %s %s\r\n", ssid, passwd, authtype);
    Network_IF_DeInitDriver();
    lRetVal = Network_IF_InitDriver(ROLE_STA);
    if (lRetVal < 0)
    {
        ERR_PRINT(lRetVal);
        return REGISTRATION_ERROR_SETMODESTA;
    }
    lRetVal = Network_IF_Connect2AP_static_ip(ssid, passwd, 5);

    if (lRetVal<0)
    {
        UART_PRINT("Connect to AP fail\n");
        return REGISTRATION_ERROR_CONNECTAP;
    }
    else
    {
    	lRetVal = set_wireless_config(ssid, passwd, authtype);
    }
    return lRetVal;

}
int32_t cmd_set_tcp(char *param)
{
    char *ip_str = NULL;
    char *port = NULL;
    char *p_optionStr = NULL;
    char *ip_optionStr = NULL;
    char *data = NULL;
    char *nb = NULL;
    char *tx = NULL;
    unsigned short usPort = 0;
    int inb = 0;
    int itx = 0;
    int iRetVal;
    uint32_t ip_temp = 0;

    ip_optionStr  = strtok(NULL, " ");
    if(!strcmp(ip_optionStr, "-i"))
    {
        UART_PRINT("\n\r");
    	UART_PRINT("Prepare Send Data To Ip\t");
    }
    else
    {
    	return -1;
    }

    ip_str = strtok(NULL, " ");
	if (ip_str == NULL)
	{
		return -1;
	}
	else
	{
		UART_PRINT("%s ", ip_str);
	}

    p_optionStr  = strtok(NULL, " ");
    if(!strcmp(p_optionStr, "-p"))
    {
    	UART_PRINT("And Port Number ");
    }
    else
    {
    	return -1;
    }

	port = strtok(NULL, " ");
	if (port == NULL)
	{
		return -1;
	}
	else
	{
	    usPort = atoi(port);
	    UART_PRINT("%d\n\r", usPort);
	}

	data = strtok(NULL, " ");
	if (data == NULL)
	{
		return -1;
	}
	else
	{
	    UART_PRINT("%s\n\r", data);
	}

	nb = strtok(NULL, " ");
	if (nb == NULL)
	{
		return -1;
	}
	else
	{
		inb = atoi(nb);
	    UART_PRINT("inb:%d\n\r", inb);
	}

	tx = strtok(NULL, " ");
	if (tx == NULL)
	{
		return -1;
	}
	else
	{
		itx = atoi(tx);
	    UART_PRINT("tx:%d\n\r", itx);
	}


	ipv4AddressParse(ip_str, &ip_temp);
	UART_PRINT("ip_temp:%x\n\r", ip_temp);
//    iRetVal = TcpClient(ip_temp, usPort, data, inb, itx);
//    iRetVal = TcpClient_cp(ip_temp, usPort, data, inb, itx);

    return iRetVal;
}
int32_t cmd_get_tcp(char *param)
{
    char *port = NULL;
    char *nb = NULL;
    unsigned short usPort = 0;
    int inb = -1;
    char *p_optionStr = NULL;

    char *tx = NULL;
    int itx = 0;

    int iRetVal;

    p_optionStr  = strtok(NULL, " ");
    if(!strcmp(p_optionStr, "-p"))
    {
        UART_PRINT("\n\r");
    	UART_PRINT("Prepare Received Data and Packet On Port Number: ");
    }
    else
    {
    	return -1;
    }

    port = strtok(NULL, " ");
    if (port == NULL)
    {
        return -1;
    }
    else
    {
        usPort = atoi(port);
        UART_PRINT("%d\n\r", usPort);
    }
    nb = strtok(NULL, " ");
    if (nb == NULL)
    {
        return -1;
    }
	else
	{
		inb = atoi(nb);
	    UART_PRINT("inb:%d\n\r", inb);
	}

	tx = strtok(NULL, " ");
	if (tx == NULL)
	{
		return -1;
	}
	else
	{
		itx = atoi(tx);
	    UART_PRINT("tx:%d\n\r", itx);
	}
//    iRetVal = TcpServer(usPort, inb, itx);
//    iRetVal = TcpServer_cp(usPort, inb, itx);

	return iRetVal;
}
int32_t cmd_set_tcp_file (char *param)
{
	char *ip_str = NULL;
	char *port = NULL;
	char *p_optionStr = NULL;
	char *ip_optionStr = NULL;
//	int iRetVal;

    unsigned short usPort = 0;
    uint32_t ip_temp = 0;

    ip_optionStr  = strtok(NULL, " ");
    if(!strcmp(ip_optionStr, "-i"))
    {
        UART_PRINT("\n\r");
    	UART_PRINT("Prepare Send Data To Ip\t");
    }
    else
    {
    	return -1;
    }

    ip_str = strtok(NULL, " ");
	if (ip_str == NULL)
	{
		return -1;
	}
	else
	{
		UART_PRINT("%s ", ip_str);
	}

    p_optionStr  = strtok(NULL, " ");
    if(!strcmp(p_optionStr, "-p"))
    {
    	UART_PRINT("And Port Number ");
    }
    else
    {
    	return -1;
    }

	port = strtok(NULL, " ");
	if (port == NULL)
	{
		return -1;
	}
	else
	{
	    usPort = atoi(port);
	    UART_PRINT("%d\n\r", usPort);
	}
	ipv4AddressParse(ip_str, &ip_temp);
	UART_PRINT("ip_temp:%x\n\r", ip_temp);
#if OPENFILE
	SendFile_Tcp(ip_temp, usPort);
#endif
    return 0;
}
int32_t cmd_get_tcp_file (char *param)
{

    char *port = NULL;
    unsigned short usPort = 0;
    char *p_optionStr = NULL;

//    int iRetVal;

    p_optionStr  = strtok(NULL, " ");
    if(!strcmp(p_optionStr, "-p"))
    {
        UART_PRINT("\n\r");
    	UART_PRINT("Prepare Received Data and Packet On Port Number: ");
    }
    else
    {
    	return -1;
    }

    port = strtok(NULL, " ");
    if (port == NULL)
    {
        return -1;
    }
    else
    {
        usPort = atoi(port);
        UART_PRINT("%d\n\r", usPort);
    }
#if OPENFILE
    iRetVal = ReceFile_Tcp(usPort);
#endif
	return 0;
//	return iRetVal;
}
int get_ip()
{
    long RetVal = -1;
    unsigned long ulIP = 0;
    unsigned long ulSubMask = 0;
    unsigned long ulDefGateway = 0;
    unsigned long ulDns = 0;
//    char *ipv4_optionStr = NULL;

/*    ipv4_optionStr = strtok(NULL, " ");
    if(!strcmp(ipv4_optionStr, "v4"))
    {
        RetVal = Network_IF_IpConfigGet(&ulIP, &ulSubMask, &ulDefGateway, &ulDns);
        ASSERT_ON_ERROR(RetVal,WLAN_ERROR);

        UART_PRINT("Device IP Address is %d.%d.%d.%d \n\r\n\r",
                   SL_IPV4_BYTE(ulIP, 3), SL_IPV4_BYTE(ulIP, 2), SL_IPV4_BYTE(ulIP,1),
                   SL_IPV4_BYTE(ulIP, 0));
    }*/

	RetVal = Network_IF_IpConfigGet(&ulIP, &ulSubMask, &ulDefGateway, &ulDns);
	ASSERT_ON_ERROR(RetVal,WLAN_ERROR);

	UART_PRINT("Device IP Address is %d.%d.%d.%d \n\r\n\r",
			   SL_IPV4_BYTE(ulIP, 3), SL_IPV4_BYTE(ulIP, 2), SL_IPV4_BYTE(ulIP,1),
			   SL_IPV4_BYTE(ulIP, 0));


    return RetVal;
}

int cmd_send_server(char *param)
{
	char *ip_str = NULL;
	char *port = NULL;
	char *p_optionStr = NULL;
	char *ip_optionStr = NULL;
//	int iRetVal;

    unsigned short usPort = 0;
    uint32_t ip_temp = 0;

    ip_optionStr  = strtok(NULL, " ");
    if(!strcmp(ip_optionStr, "-i"))
    {
        UART_PRINT("\n\r");
    	UART_PRINT("Prepare Send Data To Ip\t");
    }
    else
    {
    	return -1;
    }

    ip_str = strtok(NULL, " ");
	if (ip_str == NULL)
	{
		return -1;
	}
	else
	{
		UART_PRINT("%s ", ip_str);
	}

    p_optionStr  = strtok(NULL, " ");
    if(!strcmp(p_optionStr, "-p"))
    {
    	UART_PRINT("And Port Number ");
    }
    else
    {
    	return -1;
    }

	port = strtok(NULL, " ");
	if (port == NULL)
	{
		return -1;
	}
	else
	{
	    usPort = atoi(port);
	    UART_PRINT("%d\n\r", usPort);
	}
	ipv4AddressParse(ip_str, &ip_temp);
	UART_PRINT("ip_temp:%x\n\r", ip_temp);
//	client_group(ip_temp, usPort);
	return 0;
}

