/*
 * system.c
 *
 *  Created on: Aug 30, 2018
 *      Author: Admin
 */

#include "system.h"
#include "tcp.h"
#include "shelltask.h"
#include "network_handler.h"
#include "sflash.h"
#include "userconfig.h"
#include "main_thread.h"
#include "cc3220_system.h"



/* BSD support */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ti/net/slnetutils.h>
#include <sys/select.h>
#include <sys/types.h>


#define TEST_FILENAME_MY        "/sys/test2.txt"
#define FILENAME_MY        		"newfile1.text"




char ap_ssid[MAX_AP_SSID_LEN] = {0};
char ap_pass[MAX_AP_PASS_LEN] = {0};
char ap_sec[MAX_AP_SEC_LEN] = {0};
extern int32_t gMode;
char lineBreak[]                = "\n\r";
unsigned long  g_ulDestinationIp;
unsigned int   g_uiPortNum = PORT_NUM;
volatile unsigned long  g_ulPacketCount = TCP_PACKET_COUNT;
unsigned char  g_ucConnectionStatus = 0;
unsigned char  g_ucSimplelinkstarted = 0;
unsigned long  g_ulIpAddr = 0;

//char gBuff_tx[1019]= {0};
/*unsigned char gBuff_tx[TCP_SIZE]= {0};
unsigned char gBuff_rx[TCP_SIZE] = {0};
unsigned char gBuff_Temp[TCP_SIZE] = {0};
extern unsigned char tx_buffer1[];*/

#if 0
char *gBuff_tx2[] =
{
		{"helpStr, cmdHelpCallback, printHelpUsage"},
		{"scanStr, cmdScanCallback, printScanUsage"},
		{"setPolicyStr, cmdSetPolicyCallback, printSetPolicyUsage"},
		{"wlanConnectStr, cmdWlanConnectCallback, printWlanConnectUsage"},
		{"ap_start_str, cmdWlanStartApCallback, printWlanStartApUsage"},
		{"ap_stop_str, cmdWlanStopApCallback, printWlanStopApUsage"},
		{"wlanDisconnectStr, cmdWlanDisconnectCallback, printWlanDisconnectUsage"},
		{"ConnectedStationsStr, cmdConnectedStationsCallback, printConnectedStationsUsage"},
		{"pingStr, cmdPingCallback, printPingUsage"},
		{"sendStr, cmdSendCallback, printSendUsage"},
	    {"recvStr, cmdRecvCallback, printRecvUsage"},
	    {"createFilterStr, cmdCreateFilterCallback, printCreateFilterUsage"},
	    {"enableFilterStr, cmdEnableFilterCallback, printEnableFilterUsage"},
	    {"disableFilterStr, cmdDisableFilterCallback, printDisableFilterUsage"},
	    {"deleteFilterStr, cmdDeleteFilterCallback, printDeleteFilterUsage"},
	    {"enableWoWLANStr, cmdEnableWoWLANCallback, printEnableWoWLANUsage"},
		{"mDNSAdvertiseStr, mDNSAdvertiseCallback, printmDNSAdvertiseUsage"},
		{"mDNSQueryStr, mDNSQueryCallback, printmDNSQueryUsage"},
		{"radiotool_Str, cmdTranceiverModecallback, printTranceiverModeUsage"},
		{"p2pStartcmdStr, cmdP2PModecallback, printP2PStartUsage"},
		{"p2pStopcmdStr, cmdP2PStopcallback, printP2PStopUsage"},
		{"clearStr, cmdClearcallback, printClearUsage "}
};
#endif

#if OPENFILE
char gcBsdBuf_File[BUFF_LEN_FILE]= {0};
char gcBsdBuf_File_Rx[BUFF_LEN_FILE]= {0};
#endif

int audio_mode = -1;
//extern char buff_In[];
extern sem_t guserconfig_init;
void printBorder(char ch, int n);
void DisplayUsage();
static char *ParseString (char *instring);
void PrintIPAddress(unsigned char ipv6, void *ip);
int32_t ipv4AddressParse(char *str, uint32_t *ipv4ip);



void printBorder(char ch, int n)
{
    int        i = 0;

    for(i=0; i<n; i++)
    {
    	putch(ch);
    }
}
void DisplayUsage()
{
	printBorder('=', 100);
	UART_PRINT(lineBreak);
	UART_PRINT("\t\t\t\tAT COMMAND SUPPORT:\r\n");
    UART_PRINT("\tconnect [SSID] [PASSWD]\t");
    UART_PRINT("\tdisconnect\t");
    UART_PRINT("\treboot\n\r");
    UART_PRINT("\tsend -i [ip] -p [port number] [data] [nb 0/1] [tx 0/1]\t");
    UART_PRINT("recv -p [port number] [nb 0/1] [tx 0/1]\n\r");
    UART_PRINT("\tsendfile -i [ip] -p [port number]\t");
    UART_PRINT("\t\trecvfile -p [port number]\n\r");
    UART_PRINT("\tip\t");
    UART_PRINT("\t\t\t\t\t\tclear\n\r"); //clear screen
    UART_PRINT("\tresetfactory\n\r");
    printBorder('=', 100);
    UART_PRINT(lineBreak);

}

#if 0
int32_t DisplayAppBanner(char* appName, char* appVersion)
{
     int32_t            ret = 0;
     uint8_t            macAddress[SL_MAC_ADDR_LEN];
     uint16_t           macAddressLen = SL_MAC_ADDR_LEN;
     uint16_t           ConfigSize = 0;
     uint8_t            ConfigOpt = SL_DEVICE_GENERAL_VERSION;
     SlDeviceVersion_t ver = {0};

     ConfigSize = sizeof(SlDeviceVersion_t);

    /* Print device version info. */
    ret =
        sl_DeviceGet(SL_DEVICE_GENERAL, &ConfigOpt, &ConfigSize,
                     (uint8_t*)(&ver));
    ASSERT_ON_ERROR(ret, DEVICE_ERROR);

    /* Print device Mac address */
    ret = sl_NetCfgGet(SL_NETCFG_MAC_ADDRESS_GET, 0, &macAddressLen,
                       &macAddress[0]);
    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    UART_PRINT(lineBreak);
    UART_PRINT("\t");
    printBorder('=', 46);
    UART_PRINT(lineBreak);
    UART_PRINT("\t   %s Ver: %s",appName, appVersion);
    UART_PRINT(lineBreak);
    UART_PRINT("\t");
    printBorder('=', 46);
    UART_PRINT(lineBreak);
    UART_PRINT(lineBreak);
    UART_PRINT("\t CHIP: 0x%x",ver.ChipId);
    UART_PRINT(lineBreak);
    UART_PRINT("\t MAC:  %d.%d.%d.%d",ver.FwVersion[0],ver.FwVersion[1],
               ver.FwVersion[2],
               ver.FwVersion[3]);
    UART_PRINT(lineBreak);
    UART_PRINT("\t PHY:  %d.%d.%d.%d",ver.PhyVersion[0],ver.PhyVersion[1],
               ver.PhyVersion[2],
               ver.PhyVersion[3]);
    UART_PRINT(lineBreak);
    UART_PRINT("\t NWP:  %d.%d.%d.%d",ver.NwpVersion[0],ver.NwpVersion[1],
               ver.NwpVersion[2],
               ver.NwpVersion[3]);
    UART_PRINT(lineBreak);
    UART_PRINT("\t ROM:  %d",ver.RomVersion);
    UART_PRINT(lineBreak);
    UART_PRINT("\t HOST: %s", SL_DRIVER_VERSION);
    UART_PRINT(lineBreak);
    UART_PRINT("\t MAC address: %02x:%02x:%02x:%02x:%02x:%02x",
               macAddress[0],
               macAddress[1],
               macAddress[2],
               macAddress[3],
               macAddress[4],
               macAddress[5]);
    UART_PRINT(lineBreak);
    UART_PRINT(lineBreak);
    UART_PRINT("\t");
    printBorder('=', 44);
    UART_PRINT(lineBreak);
    UART_PRINT(lineBreak);

    return(ret);
}
#endif

static char *ParseString (char *instring)
{
    char *ptr1 = instring, *ptr2 = instring;

    if (instring == NULL)
        return instring;

    while (isspace (*ptr1))
        ptr1++;

    while (*ptr1)
    {
        if (*ptr1 == '+')
        {
            ptr1++; *ptr2++ = ' ';
        }
        else if (*ptr1 == '%' && isxdigit (*(ptr1 + 1)) && isxdigit (*(ptr1 + 2)))
        {
            ptr1++;
            *ptr2    = ((*ptr1 >= '0' && *ptr1 <= '9') ? (*ptr1 - '0') : ((char)toupper(*ptr1) - 'A' + 10)) << 4;
            ptr1++;
            *ptr2++ |= ((*ptr1 >= '0' && *ptr1 <= '9') ? (*ptr1 - '0') : ((char)toupper(*ptr1) - 'A' + 10));
            ptr1++;
        }
        else
            *ptr2++ = *ptr1++;
    }

    /*while (ptr2 > instring && isspace(*(ptr2 - 1)))
        ptr2--;*/

    *ptr2 = '\0';

    return instring;
}
void PrintIPAddress(unsigned char ipv6,
                    void *ip)
{
    uint32_t        *pIPv4;
    uint8_t         *pIPv6;
    int32_t          i=0;

    if(!ip)
    {
        return;
    }

    if(ipv6)
    {
        pIPv6 = (uint8_t*) ip;

        for(i = 0; i < 14; i+=2)
        {
            UART_PRINT("%02x%02x:", pIPv6[i], pIPv6[i+1]);
        }

        UART_PRINT("%02x%02x", pIPv6[i], pIPv6[i+1]);
    }
    else
    {
        pIPv4 = (uint32_t*)ip;
        UART_PRINT("%d.%d.%d.%d",
                    SL_IPV4_BYTE(*pIPv4,3),
                    SL_IPV4_BYTE(*pIPv4,2),
                    SL_IPV4_BYTE(*pIPv4,1),
                    SL_IPV4_BYTE(*pIPv4,0));
    }
    return;
}
int32_t ipv4AddressParse(char *str, uint32_t *ipv4ip)
{
    volatile int32_t i = 0;
    uint32_t n;
    uint32_t ipv4Address = 0;
    char             *token;

    token = strtok(str, ".");
    if(token)
    {
        n = (int)strtoul(token, 0, 10);
    }
    else
    {
        return(-1);
    }

    while(i < 4)
    {
       if((token != NULL) && (n < 256))
       {
           ipv4Address |= n;
           if(i < 3)
           {
               ipv4Address = ipv4Address << 8;
           }
           token=strtok(NULL,".");
           if (token)
           {
               n = (int)strtoul(token, 0, 10);
           }
           i++;
       }
       else
       {
           return -1;
       }
    }
    *ipv4ip = ipv4Address;
/*    ipv4ip = &ipv4Address;
    UART_PRINT("ipv4ip parser:%x\n\r", *ipv4ip);*/
    return(0);
}
int sec_string_to_num()
{
	int l_len=strlen(ap_sec);
	int i;
//	int iRetval = -1;
	for(i=0;i<l_len;i++)
	{
		ap_sec[i]=tolower(ap_sec[i]);
	}
	UART_PRINT("Wifi Sec %s\r\n",ap_sec);
	if (strcmp (ap_sec, "open" )==0)
	{
//		iRetval = 0;
		return 0;
	}
	if (strcmp (ap_sec, "wep" )==0)
	{
//		iRetval = 1;
		return 1;
	}
	if (strcmp (ap_sec, "wpa" )==0)
	{
//		iRetval = 2;
		return 2;
	}

	return 2;
}
int32_t set_wireless_config(char *ssid, char *pass, char *auth_type)
{
    // auth_type not used
    if (ssid == NULL || pass == NULL)
    {
        return -1;
    }
    // URL decoder
    ParseString(ssid);
    ParseString(pass);
    UART_PRINT("SSID:%s, PASS:%s, SEC:%s\r\n", ssid, pass, auth_type);

    memset(ap_ssid, 0, MAX_AP_SSID_LEN);
    memset(ap_pass, 0, MAX_AP_PASS_LEN);
    memset(ap_sec, 0, MAX_AP_SEC_LEN);

    memcpy(ap_ssid, ssid, strlen((char const *)ssid));
    memcpy(ap_pass, pass, strlen((char const *)pass));
    memcpy(ap_sec, auth_type, strlen((char const *)auth_type));

    return 0;
}

int32_t get_wireless_config()
{
	int ret_val = -1;

    ret_val = userconfig_get(ap_ssid, MAX_AP_SSID_LEN, WIFI_SSID_ID);
    if (ret_val < 0)
    {
        UART_PRINT("Can not read WIFI SSID, error=%d\r\n", ret_val);
    }

    ret_val = userconfig_get(ap_pass, MAX_AP_PASS_LEN, WIFI_KEY_ID);
    if (ret_val < 0)
    {
        UART_PRINT("Can not read WIFI KEY, error=%d\r\n", ret_val);
    }

    UART_PRINT("Get SSID and PASS\n\r");
    UART_PRINT("SSID:%s, PASS:%s\n\r", ap_ssid, ap_pass);
    return ret_val;
}

void fw_version_format(char* l_output, char* l_input)
{
	l_output[0]=l_input[0];
	l_output[1]=l_input[1];
	l_output[2]='.';
	l_output[3]=l_input[2];
	l_output[4]=l_input[3];
	l_output[5]='.';
	l_output[6]=l_input[4];
	l_output[7]=l_input[5];
	l_output[8]=0;
}

int ParseNProcessCmd1(char *pcCmdBuffer)
{
	char *pcInpString;
	int iRetVal = -1;
	pcInpString = strtok(pcCmdBuffer, " ");
	if ((!strcmp(pcInpString, "s")) || (!strcmp(pcInpString, "S")))
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
		set_wireless_config(ssid, passwd, authtype);
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

		return lRetVal;
	}
	else if(!strcmp(pcInpString, "scan"))
	{
	    sl_WlanProfileDel(0xFF);
	    ConfigureSimpleLinkToDefaultState(gMode);
	    InitializeAppVariables();
		Network_IF_provisionAP();
	    sl_Stop(0xFF);
	    iRetVal = Network_IF_WifiSetMode(ROLE_AP);

	    if (iRetVal < 0)
	    {
	        return REGISTRATION_ERROR_SETMODEAP;
	    }
	    iRetVal = 1;
	}
	else if(!strcmp(pcInpString, "clear"))
	{
		ClearTerm();
	    iRetVal = 1;
	}
	return iRetVal;
}
int system_init(void)
{
//	int RetVal = -1;
//	InitFile_Test();
	int ret_val = -1;

//	sl_Start(0, 0, 0);


	userconfig_init();
	userconfig_flash_read();
	sem_post(&guserconfig_init);

    ret_val = userconfig_get(ap_ssid, MAX_AP_SSID_LEN, WIFI_SSID_ID);
    if (ret_val < 0)
    {
        UART_PRINT("Can not read WIFI SSID, error=%d\r\n", ret_val);
    }

    ret_val = userconfig_get(ap_pass, MAX_AP_PASS_LEN, WIFI_KEY_ID);
    if (ret_val < 0)
    {
        UART_PRINT("Can not read WIFI KEY, error=%d\r\n", ret_val);
    }

    UART_PRINT("SSID:%s, PASS:%s\r\n", ap_ssid, ap_pass);
/*    UART_PRINT("\r\nNetwork init start\r\n");

    InitializeAppVariables();
    Network_IF_provisionAP();*/

	return 0;
}
void system_reboot()
{
	/*
    PRCMMCUReset(TRUE);
    // PRCMSOCReset();
	*/

	// Another reboot method
	sl_WlanPolicySet(SL_POLICY_PM , SL_LOW_LATENCY_POLICY, NULL, 0);
//	sl_Stop(0);
	MAP_PRCMHibernateWakeupSourceEnable(PRCM_HIB_SLOW_CLK_CTR);
	MAP_UtilsDelay(8000000);
	MAP_PRCMHibernateIntervalSet(330);
	MAP_PRCMHibernateEnter();
    while(1);

}
void system_reboot_1()
{

    PRCMMCUReset(TRUE);
}

int system_registration()
{
	int RetVal = 0;
//	char acCmdStore[512];

    UART_PRINT("Network init start\n\r");

    sl_WlanProfileDel(0xFF);
    ConfigureSimpleLinkToDefaultState(gMode);
    InitializeAppVariables();
    RetVal = Network_IF_WifiSetMode(ROLE_STA);
    if (RetVal < 0)
    {
        return REGISTRATION_ERROR_SETMODESTA;
    }

    UART_PRINT("Network init done\n\r");
    Network_IF_provisionAP();
    UART_PRINT("Scan wifi done\n\r");
    RetVal = sl_Stop(0xFF);
    RetVal = Network_IF_WifiSetMode(ROLE_AP);

    if (RetVal < 0)
    {
        return REGISTRATION_ERROR_SETMODEAP;
    }

#if 0

    char acCmdStore[512];
    do
    {

    	UART_PRINT("Select SSID:");
    	UART_PRINT("\n\r");
		RetVal = GetCmd(acCmdStore, sizeof(acCmdStore));

		if (RetVal < 0)
		{

			UART_PRINT("Command length exceeded 512 bytes in system \n\r");
		}
		else
		{
			RetVal = ParseNProcessCmd1(acCmdStore);
			if (RetVal < 0)
			{
				UART_PRINT("Error in processing command in system\n\r");
			}
		}
		UART_PRINT("\n\r");
    }while(RetVal != 0);



#endif
#if 1
    char SSID[32]="AlincoVN2";
    char SSID_KEY[64]="aevn2016";
    char AUTHTYPE[8]="WPA";
	set_wireless_config(SSID, SSID_KEY, AUTHTYPE);
	Network_IF_DeInitDriver();
	RetVal = Network_IF_InitDriver(ROLE_STA);
	if (RetVal < 0)
	{
		ERR_PRINT(RetVal);
		return REGISTRATION_ERROR_SETMODESTA;
	}
    RetVal = Network_IF_Connect2AP_static_ip(ap_ssid, ap_pass, 0);
    if (RetVal<0)
    {

        UART_PRINT("Connect to AP fail\n");

        return REGISTRATION_ERROR_CONNECTAP;
    }
#endif

    RetVal = userconfig_set(ap_ssid, strlen(ap_ssid), WIFI_SSID_ID);
    if (RetVal < 0)
    {
		UART_PRINT("Cannot write ap_ssid, error=%d\n\r", RetVal);
    }

    RetVal= userconfig_set(ap_pass, strlen(ap_pass), WIFI_KEY_ID);
    if (RetVal < 0)
    {
		UART_PRINT("Cannot write ap_pass, error=%d\n\r", RetVal);
    }

    RetVal = userconfig_set(NULL, DEV_REGISTERED, MODE_ID);
    if (RetVal < 0)
    {
        UART_PRINT("Cannot write boot flag, error=%d\r\n", RetVal);
    }
    userconfig_flash_write();

    return RetVal;
}
int system_IsRegistered()
{
    int modeID;

    if ((modeID = userconfig_get(NULL, 0, MODE_ID)) < 0)
    {
    	 UART_PRINT("MODE_GET_CONFIG_FAIL, error=%d\r\n", modeID);
        return -1;
    }
    if (modeID != DEV_REGISTERED)
    {
    	audio_mode = DEV_NOT_REGISTERED;
    	UART_PRINT("MODE_GET_CONFIG DEV_NOT_REGISTERED %d\r\n", audio_mode);
        return -1;
    }
    else
    {
    	 audio_mode = DEV_REGISTERED;
    	 UART_PRINT("MODE_GET_CONFIG DEV_REGISTERED %d\r\n", audio_mode);
        return 0;
    }
}

int system_Deregister(void)
{
//    int32_t lRetVal;
    userconfig_set(NULL, DEV_NOT_REGISTERED, MODE_ID);
    userconfig_flash_write();

    sl_WlanProfileDel(0xFF);
    ConfigureSimpleLinkToDefaultState(gMode);
    /*InitializeAppVariables();*/
    return 0;
}

#if OPENFILE

int SendFile_Tcp(unsigned int uip, unsigned short  usPort)
{
	int             iCounter;
	int             iAddrSize;
	int             iSockID;
	int             iStatus;
	int             itx = 1;

	SlSockAddr_t	*sa;
	sockAddr_t 		sAddr;
	int32_t         nonBlocking;
	long 			buflen;
	unsigned long   Offset = 0;
	_i32 fd = -1;
	int nb			= 0;


	Offset = 0;
//	sl_Start(0, 0, 0);
	fd = sl_FsOpen(TEST_FILENAME_MY, SL_FS_READ, 0);
	if(fd < 0)
	{
		UART_PRINT("sl_FsOpen error: %d\n\r", fd);
	}
	else
	{
		UART_PRINT("Reading file\n\r");
		buflen =  sl_FsRead(fd, Offset, (_u8*)gcBsdBuf_File, MAX_FILE_SIZE);
		UART_PRINT("Read \"%s\" (%d bytes)...\n\r", gcBsdBuf_File, buflen);
	}

	//filling the TCP server socket address
	sAddr.in4.sin_family = SL_AF_INET;
	sAddr.in4.sin_port = sl_Htons((unsigned short)usPort);
	sAddr.in4.sin_addr.s_addr = sl_Htonl((unsigned int)uip);

	sa = (SlSockAddr_t*)&sAddr.in4;
	iAddrSize = sizeof(SlSockAddrIn_t);

	// creating a TCP socket
	iSockID = sl_Socket(sa->sa_family, SL_SOCK_STREAM, 0);
	ASSERT_ON_ERROR(iSockID, SL_SOCKET_ERROR);

	if(nb == TRUE)
	{
		nonBlocking = TRUE;
		iStatus = sl_SetSockOpt(iSockID, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &nonBlocking, sizeof(nonBlocking));

		if(iStatus< 0)
		{
			UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, iStatus,
					   SL_SOCKET_ERROR);
			sl_Close(iSockID);
			return(-1);
		}
	}
	iStatus = -1;

	// connecting to TCP server
	while(iStatus < 0)
	{

		iStatus = sl_Connect(iSockID, sa, iAddrSize);
		UART_PRINT("iStatus %d\n\r", iStatus);
		if((iStatus == SL_ERROR_BSD_EALREADY)&& (nb == TRUE))
		{
			sleep(1);
			continue;
		}
		else if(iStatus < 0)
		{
			UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, iStatus, SL_SOCKET_ERROR);
			sl_Close(iSockID);
			return(-1);
		}
		break;
	}

	iCounter = 0;
	if(itx == 1)
	{
		while(iCounter < 1)
		{
			/* Send packets to the server */
			iStatus = sl_Send(iSockID, &gcBsdBuf_File, buflen, 0);
			if((iStatus == SL_ERROR_BSD_EAGAIN) && (nb == TRUE))
			{
				sleep(1);
				continue;
			}
			else if(iStatus < 0)
			{
				UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, iStatus, SL_SOCKET_ERROR);
				sl_Close(iSockID);
				return(-1);
			}
			iCounter++;
		}
		UART_PRINT("Sent %u packets (%u bytes) (data:%s) successfully\n\r", iCounter, buflen, gcBsdBuf_File);

	}

	iStatus = sl_Close(iSockID);
	ASSERT_ON_ERROR(iStatus, SL_SOCKET_ERROR);

	fd = sl_FsClose(fd, 0, 0, 0);
	if(fd < 0)
	{
		UART_PRINT("sl_FsClose error: %d\n\r", fd);
	}
	return 0;
}

int ReceFile_Tcp(unsigned short  usPort)
{

//	int32_t             iCounter;
	int32_t             iAddrSize;
	int32_t				iSockID;
	int32_t            	iStatus;
	int32_t             iNewSockID = -1;
	int32_t         	lNonBlocking = FALSE;
	_i32            RetVal = -1;
	SlSockAddr_t    *sa;
	SlSockAddr_t    *csa;
	sockAddr_t 		sAddr;
	int nb			= 0;
	int rx			= 1;
	_i32 fd 		= -1;
	unsigned long   Offset = 0;
	long 			buflen = 0;
//	sl_Start(0, 0, 0);

	SlFsFileInfo_t FsFileInfo;

	if(sl_FsGetInfo(FILENAME_MY, 0, &FsFileInfo) < 0)
	{
		UART_PRINT("Prepare Create File\n\r");
		fd = sl_FsOpen((unsigned char *)FILENAME_MY, (SL_FS_CREATE | SL_FS_WRITE  | SL_FS_CREATE_MAX_SIZE(MAX_FILE_SIZE_NEW)), 0);
		if(fd < 0)
		{
			UART_PRINT("sl_FsOpen error ReceFile_Tcp: %d\n\r", fd);
			return fd;
		}
		else
		{
			UART_PRINT("Create File Success\n\r");
	        RetVal = sl_FsClose(fd, 0, 0, 0);
	        if (SL_RET_CODE_OK != RetVal)
	        {
	            _ASSERT_ON_ERROR(FILE_CLOSE_ERROR);
	        }
		}

	}
	else
	{
		UART_PRINT("File Exists\n\r");
	}
	st_listFiles(40,0);

	    // filling the buffer
	memset(gcBsdBuf_File_Rx, 0x0, sizeof(gcBsdBuf_File_Rx));
//	for (iCounter=0 ; iCounter<1024 ; iCounter++)
//	{
//		gcBsdBuf_File_Rx[iCounter] = (char)(iCounter % 10);
//	}

	sAddr.in4.sin_family = SL_AF_INET;

	/* Set the server's port:
	   We'll receive connection requests on this port */
	sAddr.in4.sin_port = sl_Htons((unsigned short)usPort);
	sAddr.in4.sin_addr.s_addr = SL_INADDR_ANY;

	sa = (SlSockAddr_t*)&sAddr.in4;
	csa = (SlSockAddr_t*)&sAddr.in4;
	iAddrSize = sizeof(SlSockAddrIn_t);
	// creating a TCP socket
	iSockID = sl_Socket(sa->sa_family, SL_SOCK_STREAM, 0);
	ASSERT_ON_ERROR(iSockID, SL_SOCKET_ERROR);



	// binding the TCP socket to the TCP server address
	iStatus = sl_Bind(iSockID, sa, iAddrSize);

	if( iStatus < 0 )
	{
		// error
		UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, iStatus,
				   SL_SOCKET_ERROR);
		sl_Close(iSockID);
		return(-1);
	}

	// putting the socket for listening to the incoming TCP connection
	iStatus = sl_Listen(iSockID, 0);
	if( iStatus < 0 )
	{
		UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, iStatus,
				   SL_SOCKET_ERROR);
		sl_Close(iSockID);
		return(-1);
	}
	if(nb == TRUE)
	{
		lNonBlocking = TRUE;

		iStatus = sl_SetSockOpt(iSockID, SL_SOL_SOCKET, SL_SO_NONBLOCKING,
								&lNonBlocking, sizeof(lNonBlocking));
		if(iStatus < 0)
		{
			UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, iStatus,
					   SL_SOCKET_ERROR);
			return(-1);
		}
		iNewSockID = SL_ERROR_BSD_EAGAIN;
	}

	// waiting for an incoming TCP connection
	while( iNewSockID < 0 )
	{
		// accepts a connection form a TCP client, if there is any
		// otherwise returns SL_EAGAIN
		iNewSockID = sl_Accept(iSockID, csa, (SlSocklen_t*)&iAddrSize);
		UART_PRINT("iNewSockID:%d\n\r",iNewSockID);
		if( (iNewSockID == SL_ERROR_BSD_EAGAIN) && nb == TRUE)
		{
			UART_PRINT("Sleep\n\r");
			sleep(1);
		}
		else if( iNewSockID < 0 )
		{
			// error
			UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, iStatus, SL_SOCKET_ERROR);
			sl_Close(iNewSockID);
			sl_Close(iSockID);
			return(-1);
		}
	}

	UART_PRINT("Connected to client: ");
	sAddr.in4.sin_addr.s_addr = sl_Htonl(sAddr.in4.sin_addr.s_addr);
	PrintIPAddress(0,(void*)&sAddr.in4.sin_addr);
	UART_PRINT("\n\r");

//	int i = 0;
	if(rx == 1)
	{
		uint32_t rcvd_bytes = 0;
		while(rcvd_bytes < (MAX_FILE_SIZE))
		{
			iStatus = sl_Recv(iNewSockID, &gcBsdBuf_File_Rx, (MAX_FILE_SIZE),0);
			if((iStatus == SL_ERROR_BSD_EAGAIN) && (nb == TRUE))
			{
				sleep(1);
				continue;
			}
			else if(iStatus < 0)
			{
				UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, iStatus, BSD_SOCKET_ERROR);
				sl_Close(iSockID);
				return(-1);
			}
			else if(iStatus == 0)
			{
				UART_PRINT("TCP Client closed the connection \n\r");
				break;
			}

			rcvd_bytes += iStatus;
		}

		UART_PRINT("Received %u packets (%u bytes) (data:%s) successfully \n\r",
				   (1), rcvd_bytes, gcBsdBuf_File_Rx);

		fd = sl_FsOpen(FILENAME_MY, SL_FS_WRITE , 0);
		if(fd < 0)
		{
			UART_PRINT("Not Open File \"%s\" %d To Write\n\r", FILENAME_MY, fd);
		}
		else
		{
			buflen = sl_FsWrite(fd, Offset,(_u8 *)gcBsdBuf_File_Rx, MAX_FILE_SIZE);
			UART_PRINT("Wrote %d bytes...\n\r", buflen);
		}

	}

	iStatus = sl_Close(iNewSockID);
	ASSERT_ON_ERROR(iStatus, SL_SOCKET_ERROR);

	iStatus = sl_Close(iSockID);
	ASSERT_ON_ERROR(iStatus, SL_SOCKET_ERROR);

	sl_FsClose(fd, NULL, NULL , 0);

	Offset = 0;
	fd  = -1;
	buflen = 0;
	fd = sl_FsOpen(FILENAME_MY, SL_FS_READ, 0);
    if(fd < 0)
    {
        UART_PRINT("sl_FsOpen error: %d\n\r", fd);
    }
    else
    {
    	memset(gcBsdBuf_File, 0x0, sizeof(gcBsdBuf_File));
    	buflen = sl_FsRead(fd, Offset, (_u8*)gcBsdBuf_File, MAX_FILE_SIZE);
    	if(buflen > 0)
    	{
			UART_PRINT("Read back %d bytes...\n\r", buflen);
			UART_PRINT("Read from file %s \n\r", gcBsdBuf_File);

    	}

        fd  = sl_FsClose(fd, 0, 0, 0);
        if(fd < 0)
        {
            UART_PRINT("sl_FsClose error: %d\n\r", fd );
        }
    }
	return(0);
}
#endif


# if 0
int TcpClient(unsigned int uip, unsigned short  usPort, char *buff, int nb, int tx)
{
    int             iCounter;
    int             iAddrSize;
    int             iSockID;
    int             iStatus;

    SlSockAddr_t	*sa;
    sockAddr_t 		sAddr;
//    int32_t         nonBlocking;
//    _u32 TimeOut = 0;
//    start:
    memset(gBuff_tx, 0x0, sizeof(gBuff_tx));
    for (iCounter=0 ; iCounter<BUFF_SIZE ; iCounter++)
    {
//        gBuff_tx[iCounter] = (char)(iCounter % 10);
    	gBuff_tx[iCounter] =  buff[iCounter];
    }
//    memset(&app_CB.gDataBuffer.nwData, 0x0, sizeof(app_CB.gDataBuffer));
//
//    /* filling the buffer with data */
//    for(iCounter = 0; iCounter < MAX_BUF_SIZE; iCounter++)
//    {
////        app_CB.gDataBuffer.nwData[i] = (char)(i % 10);
//        app_CB.gDataBuffer.nwData[iCounter] = buff[iCounter];
//    }

    //filling the TCP server socket address
    sAddr.in4.sin_family = SL_AF_INET;
    sAddr.in4.sin_port = sl_Htons((unsigned short)usPort);
    sAddr.in4.sin_addr.s_addr = sl_Htonl((unsigned int)uip);

    sa = (SlSockAddr_t*)&sAddr.in4;
    iAddrSize = sizeof(SlSockAddrIn_t);

    // creating a TCP socket
    iSockID = sl_Socket(sa->sa_family, SL_SOCK_STREAM, 0);
    ASSERT_ON_ERROR(iSockID, SL_SOCKET_ERROR);

    if(nb == TRUE)
    {
//        nonBlocking = TRUE;
//        nonBlocking = FALSE;
    	SlSockNonblocking_t BlockingOption;
    	BlockingOption.NonBlockingEnabled = 0;
//        TimeOut = 120;
        iStatus = sl_SetSockOpt(iSockID, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &BlockingOption, sizeof(BlockingOption));
//        iStatus = sl_SetSockOpt(iSockID, SL_SOL_SOCKET, SL_SO_KEEPALIVETIME,( _u8*) &TimeOut, sizeof(TimeOut));
        if(iStatus< 0)
        {
            UART_PRINT("[line:%d], [error:%d], [function: %s], [%s]\n\r", __LINE__, iStatus, __FUNCTION__,
                       SL_SOCKET_ERROR);
            sl_Close(iSockID);
            return(-1);
        }
    }
    iStatus = -1;

    // connecting to TCP server
    while(iStatus < 0)
    {

    	iStatus = sl_Connect(iSockID, sa, iAddrSize);
    	UART_PRINT("iStatus %d\n\r", iStatus);
        if((iStatus == SL_ERROR_BSD_EALREADY)&& (nb == TRUE))
        {
            sleep(1);
            continue;
        }
        else if(iStatus < 0)
        {
            UART_PRINT("[line:%d], [error:%d], [function:%s], [%s]\n\r", __LINE__, iStatus, __FUNCTION__, SL_SOCKET_ERROR);
            sl_Close(iSockID);
            return(-1);
        }
        break;
    }

    iCounter = 0;
    if(tx == 1)
    {
		int32_t buflen;
		uint32_t sent_bytes = 0;
		uint32_t bytes_to_send = (1000 * BUFF_SIZE);
		while(sent_bytes < bytes_to_send)
		{

			if(bytes_to_send - sent_bytes >= BUFF_SIZE)
			{
				buflen = BUFF_SIZE;
			}
			else
			{
				buflen = bytes_to_send - sent_bytes;
			}

			/* Send packets to the server */
//			iStatus = sl_Send(iSockID, &gBuff_tx, buflen, 0);
			iStatus = sl_Send(iSockID, &gBuff_tx, sizeof(gBuff_tx), 0);
			//SL_ERROR_BSD_ECONNREFUSED  SL_ERROR_BSD_EAGAIN //
			if((iStatus == SL_ERROR_BSD_EAGAIN) && (nb == TRUE))
			{
				sleep(1);
				continue;
			}
			else if(iStatus < 0)
			{
				UART_PRINT("[line:%d], [error:%d], [function:%s], [%s]\n\r", __LINE__, iStatus, __FUNCTION__, SL_SOCKET_ERROR);
				sl_Close(iSockID);
				return(-1);
			}
			iCounter++;
			sent_bytes += iStatus;

		}
		UART_PRINT("Sent %u packets (%u bytes) (data:%s) successfully\n\r", iCounter, sent_bytes, gBuff_tx);
		memset(gBuff_tx, 0x0, sizeof(gBuff_tx));
		iStatus = sl_Recv(iSockID, &gBuff_tx, sizeof(gBuff_tx), 0);
		if( 0 > iStatus )
		{
			UART_PRINT("[line:%d], [error:%d], [function:%s], [%s]\n\r", __LINE__, iStatus, __FUNCTION__, SL_SOCKET_ERROR);
		}
		UART_PRINT("Recv [data:%s] [function:%s] successfully\n\r", gBuff_tx, __FUNCTION__);
    }
    else
    {
    	uint32_t rcvd_bytes = 0;

		while(rcvd_bytes < (1000 * BUFF_SIZE))
		{
			iStatus = sl_Recv(iSockID, &gBuff_tx, MAX_BUF_SIZE, 0);
			if((iStatus == SL_ERROR_BSD_EAGAIN) && (TRUE == 1))
			{
				UART_PRINT("Sleep rece\n\r");
				sleep(1);
				continue;
			}
			else if(iStatus < 0)
			{
				UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, iStatus, BSD_SOCKET_ERROR);
				sl_Close(iSockID);
				return(-1);
			}
			else if(iStatus == 0)
			{
				UART_PRINT("TCP Server closed the connection\n\r");
				break;
			}
			rcvd_bytes += iStatus;
		}

		UART_PRINT("Received %u packets (%u bytes) (data:%s) successfully\n\r",
				   (rcvd_bytes / BUFF_SIZE), rcvd_bytes, &gBuff_tx);
    }
    iStatus = sl_Close(iSockID);
    ASSERT_ON_ERROR(iStatus, SL_SOCKET_ERROR);
    return 0;
}

int TcpServer(unsigned short  usPort, int nb, int tx)
{

	int32_t             iCounter;
	int32_t             iAddrSize;
    int32_t				iSockID;
    int32_t            	iStatus;

    int32_t             iNewSockID = -1;
//    int32_t         	lNonBlocking = FALSE;

    SlSockAddr_t    *sa;
    SlSockAddr_t    *csa;
    sockAddr_t sAddr;
//    _u32 TimeOut = 0;


	SlTimeval_t timeout;
	timeout.tv_sec = 30;
	SlFdSet_t readsds;
	SlFdSet_t writesds;
	int32_t            Status = -1;
    // filling the buffer
    memset(gBuff_tx, 0x0, sizeof(gBuff_tx));
    for (iCounter=0 ; iCounter<BUF_LEN ; iCounter++)
    {
    	gBuff_tx[iCounter] = (char)(iCounter % 10);
    }
//    memset(&app_CB.gDataBuffer.nwData, 0x0, sizeof(app_CB.gDataBuffer));
//
//    /* clear the global data buffer */
//    for(iCounter = 0; iCounter < MAX_BUF_SIZE; iCounter++)
//    {
////        app_CB.gDataBuffer.nwData[i] = (char)(i % 10);
//        app_CB.gDataBuffer.nwData[iCounter] = (char)(iCounter % 10);
//    }
    //filling the TCP server socket address
    sAddr.in4.sin_family = SL_AF_INET;

    /* Set the server's port:
	   We'll receive connection requests on this port */
    sAddr.in4.sin_port = sl_Htons((unsigned short)usPort);
    sAddr.in4.sin_addr.s_addr = SL_INADDR_ANY;

    sa = (SlSockAddr_t*)&sAddr.in4;
    csa = (SlSockAddr_t*)&sAddr.in4;
    iAddrSize = sizeof(SlSockAddrIn_t);
    // creating a TCP socket
    iSockID = sl_Socket(sa->sa_family, SL_SOCK_STREAM, 0);
    ASSERT_ON_ERROR(iSockID, SL_SOCKET_ERROR);



    // binding the TCP socket to the TCP server address
    iStatus = sl_Bind(iSockID, sa, iAddrSize);

    if( iStatus < 0 )
    {
        // error
    	UART_PRINT("[line:%d], [error:%d], [function:%s], [%s]\n\r", __LINE__, iStatus, __FUNCTION__, SL_SOCKET_ERROR);
        sl_Close(iSockID);
        return(-1);
    }

    // putting the socket for listening to the incoming TCP connection
    iStatus = sl_Listen(iSockID, 2);
    if( iStatus < 0 )
    {
    	UART_PRINT("[line:%d], [error:%d], [function:%s], [%s]\n\r", __LINE__, iStatus, __FUNCTION__, SL_SOCKET_ERROR);
        sl_Close(iSockID);
        return(-1);
    }
    else
    {
    	Report("Prepare Connection\n\r");

		SL_SOCKET_FD_ZERO(&readsds);
		SL_SOCKET_FD_SET(iSockID + 1, &readsds);
		Status = sl_Select(iSockID + 1, &readsds, &writesds, NULL, &timeout);
		if(Status > 0)
		{
			Report("Connection readsds [%d] writesds[%d] Status[%d] \n\r", readsds, writesds, Status);
		}
		else if(Status == 0)
		{
			Report("Connection Timeout [%d]s\n\r",timeout.tv_sec);
			sl_Close(iSockID);
			_ASSERT_ON_ERROR(LISTEN_ERROR);
			system_reboot();
		}
        else
        {
			Report("connection error (code : %d)\r\n", Status);
			sl_Close(iSockID);
			_ASSERT_ON_ERROR(LISTEN_ERROR);
			system_reboot();
        }
    }
    if(nb == TRUE)
    {
//    	lNonBlocking = TRUE;
//    	lNonBlocking = FALSE;
    	SlSockNonblocking_t BlockingOption;
    	BlockingOption.NonBlockingEnabled = 0;
    	Report("KEEP ALIVE TIME\n\r");
        iStatus = sl_SetSockOpt(iSockID, SL_SOL_SOCKET, SL_SO_NONBLOCKING,
                                &BlockingOption, sizeof(BlockingOption));
//        TimeOut = 120;
//        iStatus = sl_SetSockOpt(iSockID, SL_SOL_SOCKET, SL_SO_KEEPALIVETIME,( _u8*) &TimeOut, sizeof(TimeOut));
//        iStatus = sl_SetSockOpt(iSockID, SL_SOL_SOCKET, SL_SO_KEEPALIVE,( _u8*) &enableOption, sizeof(enableOption));
        if(iStatus < 0)
        {
        	UART_PRINT("[line:%d], [error:%d], [function:%s], [%s]\n\r", __LINE__, iStatus, __FUNCTION__, SL_SOCKET_ERROR);
            return(-1);
        }
        iNewSockID = SL_ERROR_BSD_EAGAIN;
    }

    // waiting for an incoming TCP connection
    while( iNewSockID < 0 )
    {
        // accepts a connection form a TCP client, if there is any
        // otherwise returns SL_EAGAIN
		iNewSockID = sl_Accept(iSockID, csa, (SlSocklen_t*)&iAddrSize);
		UART_PRINT("iNewSockID:%d\n\r",iNewSockID);

        if( (iNewSockID == SL_ERROR_BSD_EAGAIN) && nb == TRUE)
        {
        	UART_PRINT("Sleep\n\r");
        	sleep(1);
        }
        else if( iNewSockID < 0 )
        {
            // error
        	UART_PRINT("[line:%d], [error:%d], [function:%s], [%s]\n\r", __LINE__, iNewSockID, __FUNCTION__, SL_SOCKET_ERROR);
            sl_Close(iNewSockID);
            sl_Close(iSockID);
            return(-1);
        }

    }
    UART_PRINT("Connected to client: ");
    sAddr.in4.sin_addr.s_addr = sl_Htonl(sAddr.in4.sin_addr.s_addr);
    PrintIPAddress(0,(void*)&sAddr.in4.sin_addr);
    UART_PRINT("\n\r");
    // waits for 1000 packets from the connected TCP client
    int i = 0;
	if(tx == 1)
	{
		int32_t buflen;
		uint32_t sent_bytes = 0;
		uint32_t bytes_to_send = (1000 * BUF_LEN);

		while(sent_bytes < bytes_to_send)
		{
		if(bytes_to_send - sent_bytes >= BUF_LEN)
		{
			buflen = BUF_LEN;
		}
		else
		{
			buflen = bytes_to_send - sent_bytes;
		}

		/* Send packets to server */
		iStatus = sl_Send(iSockID, &gBuff_tx, buflen, 0);
		//            status = sl_Send(newsock, &app_CB.CmdBuffer, buflen, 0);
		if((iStatus == SL_ERROR_BSD_EAGAIN) && (TRUE == 1))
		{
			sleep(1);
			continue;
		}
		else if(iStatus < 0)
		{
			UART_PRINT("[line:%d], [error:%d], [function:%s], [%s]\n\r", __LINE__, iStatus, __FUNCTION__, SL_SOCKET_ERROR);
			sl_Close(iNewSockID);
			sl_Close(iSockID);
			return(-1);
		}
		i++;
		sent_bytes += iStatus;
		}

		UART_PRINT("Sent %u packets (%u bytes) (data:%s) successfully\n\r",i,
			   sent_bytes, &app_CB.CmdBuffer[30]);
	}
	else
	{
		uint32_t rcvd_bytes = 0;

		while(rcvd_bytes < (1000 * BUF_LEN))
		{
			iStatus = sl_Recv(iNewSockID, &gBuff_tx, sizeof(gBuff_tx),0);
			if((iStatus == SL_ERROR_BSD_EAGAIN) && (nb == TRUE))
			{
				sleep(1);
				continue;
			}
			else if(iStatus < 0)
			{
				UART_PRINT("[line:%d], [error:%d], [function:%s], [%s]\n\r", __LINE__, iStatus, __FUNCTION__, BSD_SOCKET_ERROR);
				sl_Close(iSockID);
				return(-1);
			}
			else if(iStatus == 0)
			{
				UART_PRINT("TCP Client closed the connection \n\r");
				break;
			}

			rcvd_bytes += iStatus;

		}
		iStatus = sl_Send(iNewSockID, "ACK", strlen("ACK"), 0 );
		if( strlen("ACK") != iStatus )
		{
			UART_PRINT("[line:%d], [error:%d], [function:%s], [%s]\n\r", __LINE__, iStatus, __FUNCTION__, BSD_SOCKET_ERROR);
		}
		UART_PRINT("Received %u packets (%u bytes) (data:%s) successfully \n\r",
				   (rcvd_bytes / BUF_LEN), rcvd_bytes, &gBuff_tx);
		memset(gBuff_tx, 0x0, sizeof(gBuff_tx));

	}

	iStatus = sl_Recv(iNewSockID, &gBuff_tx, sizeof(gBuff_tx),0);

	if(0 == iStatus)
	{
		UART_PRINT("Status [%d] function[%s]\n\r", iStatus, __FUNCTION__);
		iStatus = sl_Close(iNewSockID);
		ASSERT_ON_ERROR(iStatus, SL_SOCKET_ERROR);

		iStatus = sl_Close(iSockID);
		ASSERT_ON_ERROR(iStatus, SL_SOCKET_ERROR);
	}

	return(0);
}


int TcpServer_cp(unsigned short  usPort, int nb, int tx)
{

	int32_t             iCounter;
	int32_t             iAddrSize;
    int32_t				iSockID;
    int32_t            	iStatus;

//    int32_t             iNewSockID = -1;
//    int32_t         	lNonBlocking = FALSE;

    SlSockAddr_t    *sa;
    SlSockAddr_t    *csa;
    sockAddr_t sAddr;
//    sockAddr_t sAddrclient;
//    _u32 TimeOut = 0;

//
//	SlTimeval_t timeout;
//	timeout.tv_sec = 120;
	SlFdSet_t fds,readfds;

//	SlFdSet_t writesds;
//	int Status = -1;
	int client_socket[10] = {0};
	int max_client = 10;
	int numbersock = 0;
	uint32_t rcvd_bytes = 0;
	unsigned int numpackets = 0;

    // filling the buffer
    memset(gBuff_tx, 0x0, sizeof(gBuff_tx));

    memset(client_socket, 0x0, sizeof(client_socket));



    sAddr.in4.sin_family = SL_AF_INET;
    sAddr.in4.sin_port = sl_Htons((unsigned short)usPort);
    sAddr.in4.sin_addr.s_addr = SL_INADDR_ANY;

    sa = (SlSockAddr_t*)&sAddr.in4;
    csa = (SlSockAddr_t*)&sAddr.in4;
    iAddrSize = sizeof(SlSockAddrIn_t);
    // creating a TCP socket
//    iSockID = sl_Socket(sa->sa_family, SL_SOCK_STREAM, 0);
    iSockID = sl_Socket(sa->sa_family, SL_SOCK_STREAM, SL_IPPROTO_TCP);
    ASSERT_ON_ERROR(iSockID, SL_SOCKET_ERROR);

    // binding the TCP socket to the TCP server address
    iStatus = sl_Bind(iSockID, sa, iAddrSize);

    if( iStatus < 0 )
    {
        // error
    	ASSERT_ON_ERROR(iStatus, SL_SOCKET_ERROR);
        sl_Close(iSockID);
        return(-1);
    }

    // putting the socket for listening to the incoming TCP connection
    iStatus = sl_Listen(iSockID, 10);
    if( iStatus < 0 )
    {
    	ASSERT_ON_ERROR(iStatus, SL_SOCKET_ERROR);
        sl_Close(iSockID);
        return(-1);
    }

	SL_SOCKET_FD_ZERO(&fds);
	SL_SOCKET_FD_SET(iSockID, &fds);
    while(1)
    {
    	 readfds = fds;
    	 iStatus = sl_Select(SL_FD_SETSIZE, &readfds, NULL, NULL, NULL);
    	 _ASSERT_ON_ERROR(iStatus);


		for ( iCounter = 0 ; iCounter < SL_FD_SETSIZE ; iCounter++)
		{
			if(SL_SOCKET_FD_ISSET(iCounter, &readfds))
			{
				if(iSockID == iCounter)
				{
					if(numbersock < max_client)
					{
						client_socket[numbersock] = sl_Accept(iSockID, csa, (SlSocklen_t*)&iAddrSize);
						if(client_socket[numbersock] == -1)
						{
							_ASSERT_ON_ERROR(client_socket[numbersock]);
						}
						SL_SOCKET_FD_SET(client_socket[numbersock], &fds);
						numbersock++;
					}
					else
					{
						UART_PRINT("Ran out of socket space\n\r");
					}
				}
				else
				{

//					 bzero(gBuff_tx1, sizeof(gBuff_tx1));
					 while(rcvd_bytes < 1019)
					 {
						 iStatus = sl_Recv(iCounter, gBuff_tx, 1019,0);
						 if(iStatus  < 0)
						 {
							UART_PRINT("Reading Stream Message\n\r");
						 }
						 else if(iStatus == 0)
						 {
							UART_PRINT("Ending Connection\n\r");
						 }
						 else
						 {
							UART_PRINT("bytes at read [%d]\n\r", iStatus);
							UART_PRINT("-->data [%s]\n\r",gBuff_tx);
							rcvd_bytes += iStatus;
						 }

					 }
					 numpackets++;
					 UART_PRINT("-->one packet enough [%u] bytes [%u] numpackets[%d] \n\r",(rcvd_bytes/1024), rcvd_bytes, numpackets);
					 rcvd_bytes = 0;
					 memset(gBuff_tx, 0x0, sizeof(gBuff_tx));
				}

			}
		}

    }

	return(0);
}

/*int TcpServer_multithread(unsigned short  usPort)
{
	fd_set fds;
	fd_set readfds;
	int i = 0;
	int clientaddrlen = 0;
	int clientsock[NUMBERSOCKET] = {0};
	int rc = 0;
	int numsocks = 0;
	int maxsocks = 10;
	int ssocket = 0;
	int iStatus = 0;

	socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	return 0;
}*/



int TcpClient_cp(unsigned int uip, unsigned short  usPort, char *buff, int nb, int tx)
{
    int             iCounter = 0;
    int             iAddrSize;
    int             iSockID;
    int             iStatus;
//    char acCmdStore[512];


//    int RetVal = 0;
    SlSockAddr_t	*sa;
    sockAddr_t 		sAddr;



    // filling the buffer
//    for (iCounter=0 ; iCounter<BUFF_SIZE ; iCounter++)
//    {
////        gBuff_tx[iCounter] = (char)(iCounter % 10);
//        gBuff_tx[iCounter] =  buff[iCounter];
//    }

    //filling the TCP server socket address
    sAddr.in4.sin_family = SL_AF_INET;
    sAddr.in4.sin_port = sl_Htons((unsigned short)usPort);
    sAddr.in4.sin_addr.s_addr = sl_Htonl((unsigned int)uip);

    sa = (SlSockAddr_t*)&sAddr.in4;
    iAddrSize = sizeof(SlSockAddrIn_t);

    // creating a TCP socket
    iSockID = sl_Socket(sa->sa_family, SL_SOCK_STREAM, 0);
    ASSERT_ON_ERROR(iSockID, SL_SOCKET_ERROR);


	iStatus = sl_Connect(iSockID, sa, iAddrSize);
	UART_PRINT("iStatus %d\n\r", iStatus);
	if(iStatus < 0)
    {
		ASSERT_ON_ERROR(iStatus, SL_SOCKET_ERROR);
        sl_Close(iSockID);
        return(-1);
    }

	UART_PRINT("SEND\n\r");
#if 0
    while(1)
    {


		iStatus = sl_Send(iSockID, *(gBuff_tx2+iCounter), 1024, 0);
		if(iStatus < 0)
		{
			_ASSERT_ON_ERROR(iStatus);
			goto Exit;
		}
		UART_PRINT("Send [data:%s] successfully\n\r", *(gBuff_tx2+iCounter));
		sleep(2);
		iCounter++;
		if(iCounter == 22)
		{
			iCounter = 0;
		}
    }
#endif

/*    while(1)
    {
    	UART_PRINT("SEND\n\r");

		RetVal = GetCmd(acCmdStore, sizeof(acCmdStore));

		if (RetVal < 0)
		{

			UART_PRINT("Command length exceeded 512 bytes in system \n\r");
		}

		iStatus = sl_Send(iSockID, &acCmdStore, sizeof(acCmdStore), 0);
		_ASSERT_ON_ERROR(iStatus);
		UART_PRINT("Send [data:%s] successfully\n\r", acCmdStore);

    }*/
Exit:
    iStatus = sl_Close(iSockID);
    ASSERT_ON_ERROR(iStatus, SL_SOCKET_ERROR);

    return 0;
}
int iSockID ;
int flag = 0;
//int client_group(unsigned int uip, unsigned short  usPort)
pthread_t reader = (pthread_t)NULL;
extern pthread_mutex_t clientMutex;
extern sem_t clientsem;
void *receive(void * param);

#endif
