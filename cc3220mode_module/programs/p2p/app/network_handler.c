/*
 * network_handler.c
 *
 *  Created on: Aug 30, 2018
 *      Author: Admin
 */
#include "network_handler.h"
#include "tcp.h"
#include "shelltask.h"
#include "system.h"
#include "sflash.h"
#include "main_thread.h"
#include "apconfig.h"


char router_list[2048] = {0};

Sl_WlanNetworkEntry_t  g_NetEntries[SCAN_TABLE_SIZE];


/* Station IP address                                                        */
unsigned long g_ulStaIp = 0;
/* Network Gateway IP address                                                */
unsigned long g_ulGatewayIP = 0;
/* Connection SSID                                                           */
unsigned char g_ucConnectionSSID[SL_WLAN_SSID_MAX_LENGTH + 1];
/* Connection BSSID                                                          */
unsigned char g_ucConnectionBSSID[SL_WLAN_BSSID_LENGTH ];
/* SimpleLink Status                                                         */
volatile unsigned long g_ulStatus = 0;
/* Connection time delay index                                               */
volatile unsigned short g_usConnectIndex;

void SimpleLinkHttpServerEventHandler(
    SlNetAppHttpServerEvent_t *pSlHttpServerEvent,
    SlNetAppHttpServerResponse_t *
    pSlHttpServerResponse)
{
}

void SimpleLinkNetAppRequestEventHandler(SlNetAppRequest_t *pNetAppRequest,
                                         SlNetAppResponse_t *pNetAppResponse)
{
}

void SimpleLinkNetAppRequestMemFreeEventHandler(uint8_t *buffer)
{
}

//*****************************************************************************
//!
//! On Successful completion of Wlan Connect, This function triggers connection
//! status to be set.
//!
//! \param[in]  pSlWlanEvent    - pointer indicating Event type
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pSlWlanEvent)
{
    SlWlanEventDisconnect_t* pEventData = NULL;

    switch(pSlWlanEvent->Id)
    {
    case SL_WLAN_EVENT_CONNECT:
        SET_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);

        memcpy(g_ucConnectionSSID, pSlWlanEvent->Data.Connect.SsidName,
               pSlWlanEvent->Data.Connect.SsidLen);
        memcpy(g_ucConnectionBSSID, pSlWlanEvent->Data.Connect.Bssid,
               SL_WLAN_BSSID_LENGTH);

        UART_PRINT(
            "\n\r[WLAN EVENT] STA Connected to the AP: %s , BSSID: "
            "%x:%x:%x:%x:%x:%x\n\r", g_ucConnectionSSID,
            g_ucConnectionBSSID[0], g_ucConnectionBSSID[1],
            g_ucConnectionBSSID[2],
            g_ucConnectionBSSID[3], g_ucConnectionBSSID[4],
            g_ucConnectionBSSID[5]);
        break;

    case SL_WLAN_EVENT_DISCONNECT:
        CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
        CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_ACQUIRED);

        pEventData = &pSlWlanEvent->Data.Disconnect;

        /* If the user has initiated 'Disconnect' request, 'reason_code' */
        /* is SL_WLAN_DISCONNECT_USER_INITIATED                          */
        if(SL_WLAN_DISCONNECT_USER_INITIATED == pEventData->ReasonCode)
        {
            UART_PRINT("Device disconnected from the AP on application's "
                       "request \n\r");
        }
        else
        {
            UART_PRINT("Device disconnected from the AP on an ERROR..!! \n\r");
        }
        break;

    case SL_WLAN_EVENT_STA_ADDED:
        /* when device is in AP mode and any client connects to it.      */
        SET_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
        break;

    case SL_WLAN_EVENT_STA_REMOVED:
        /* when device is in AP mode and any client disconnects from it. */
        CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
        CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);
        break;

    default:
        UART_PRINT("[WLAN EVENT] Unexpected event %d\n\r", pSlWlanEvent->Id);
        break;
    }
//    UART_PRINT("\n\r");
}

//*****************************************************************************
//
//! The Function Handles the Fatal errors
//!
//! \param[in]  slFatalErrorEvent - Pointer to Fatal Error Event info
//!
//! \return     None
//!
//*****************************************************************************
void SimpleLinkFatalErrorEventHandler(SlDeviceFatal_t *slFatalErrorEvent)
{
    switch(slFatalErrorEvent->Id)
    {
    case SL_DEVICE_EVENT_FATAL_DEVICE_ABORT:
        UART_PRINT(
            "[ERROR] - FATAL ERROR: Abort NWP event detected: "
            "AbortType=%d, AbortData=0x%x\n\r",
            slFatalErrorEvent->Data.DeviceAssert.Code,
            slFatalErrorEvent->Data.DeviceAssert.Value);
        break;

    case SL_DEVICE_EVENT_FATAL_DRIVER_ABORT:
        UART_PRINT("[ERROR] - FATAL ERROR: Driver Abort detected. \n\r");
        break;

    case SL_DEVICE_EVENT_FATAL_NO_CMD_ACK:
        UART_PRINT(
            "[ERROR] - FATAL ERROR: No Cmd Ack detected "
            "[cmd opcode = 0x%x] \n\r",
            slFatalErrorEvent->Data.NoCmdAck.Code);
        break;

    case SL_DEVICE_EVENT_FATAL_SYNC_LOSS:
        UART_PRINT("[ERROR] - FATAL ERROR: Sync loss detected n\r");
        break;

    case SL_DEVICE_EVENT_FATAL_CMD_TIMEOUT:
        UART_PRINT(
            "[ERROR] - FATAL ERROR: Async event timeout detected "
            "[event opcode =0x%x]  \n\r",
            slFatalErrorEvent->Data.CmdTimeout.Code);
        break;

    default:
        UART_PRINT("[ERROR] - FATAL ERROR: Unspecified error detected \n\r");
        break;
    }
}

//*****************************************************************************
//
//! This function handles network events such as IP acquisition, IP leased, IP
//! released etc.
//!
//! \param[in]  pNetAppEvent - Pointer to NetApp Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent)
{
    switch(pNetAppEvent->Id)
    {
    case SL_NETAPP_EVENT_IPV4_ACQUIRED:
    case SL_NETAPP_EVENT_IPV6_ACQUIRED:
        SET_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_ACQUIRED);
//        UART_PRINT("[NETAPP EVENT] IP acquired by the device\n\r");
        break;

    case SL_NETAPP_EVENT_DHCPV4_LEASED:
        SET_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);
        g_ulStaIp = (pNetAppEvent)->Data.IpLeased.IpAddress;

        UART_PRINT("[NETAPP EVENT] IP Leased to Client: IP=%d.%d.%d.%d , ",
                   SL_IPV4_BYTE(g_ulStaIp,
                                3),
                   SL_IPV4_BYTE(g_ulStaIp,
                                2),
                   SL_IPV4_BYTE(g_ulStaIp, 1), SL_IPV4_BYTE(g_ulStaIp, 0));
        break;

    case SL_NETAPP_EVENT_DHCPV4_RELEASED:
        CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);

        UART_PRINT("[NETAPP EVENT] IP Released for Client: "
                   "IP=%d.%d.%d.%d , ", SL_IPV4_BYTE(g_ulStaIp,
                                                     3),
                   SL_IPV4_BYTE(g_ulStaIp,
                                2),
                   SL_IPV4_BYTE(g_ulStaIp, 1), SL_IPV4_BYTE(g_ulStaIp, 0));
        break;

    default:
        UART_PRINT("[NETAPP EVENT] Unexpected event [0x%x] \n\r",
                   pNetAppEvent->Id);
        break;
    }
}

//*****************************************************************************
//
//! This function handles resource request
//!
//! \param[in]  pNetAppRequest  - Contains the resource requests
//! \param[in]  pNetAppResponse - Should be filled by the user with the
//!                               relevant response information
//!
//! \return     None
//!
//*****************************************************************************
void SimpleLinkNetAppRequestHandler(SlNetAppRequest_t *pNetAppRequest,
                                    SlNetAppResponse_t *pNetAppResponse)
{
    /* Unused in this application                                            */
}

//*****************************************************************************
//
//! This function handles HTTP server events
//!
//! \param[in]  pServerEvent     - Contains the relevant event information
//! \param[in]  pServerResponse  - Should be filled by the user with the
//!                                relevant response information
//!
//! \return None
//!
//****************************************************************************
void SimpleLinkHttpServerCallback(SlNetAppHttpServerEvent_t *pHttpEvent,
                                  SlNetAppHttpServerResponse_t *pHttpResponse)
{
    /* Unused in this application                                            */
}

//*****************************************************************************
//
//! This function handles General Events
//!
//! \param[in]  pDevEvent - Pointer to General Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent)
{
    /* Most of the general errors are not FATAL. are to be handled           */
    /* appropriately by the application.                                     */
    UART_PRINT("[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n",
               pDevEvent->Data.Error.Code,
               pDevEvent->Data.Error.Source);
}

//*****************************************************************************
//
//! This function handles socket events indication
//!
//! \param[in]  pSock - Pointer to Socket Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
    /* This application doesn't work w/ socket - Events are not expected     */
    switch(pSock->Event)
    {
    case SL_SOCKET_TX_FAILED_EVENT:
        switch(pSock->SocketAsyncEvent.SockTxFailData.Status)
        {
        case SL_ERROR_BSD_ECLOSE:
            UART_PRINT(
                "[SOCK ERROR] - close socket (%d) operation "
                "failed to transmit all queued packets\n\r",
                pSock->SocketAsyncEvent.SockTxFailData.Sd);
            break;
        default:
            UART_PRINT(
                "[SOCK ERROR] - TX FAILED  :  socket %d , "
                "reason (%d) \n\n",
                pSock->SocketAsyncEvent.SockTxFailData.Sd,
                pSock->SocketAsyncEvent.SockTxFailData.Status);
            break;
        }
        break;
    case SL_SOCKET_ASYNC_EVENT:
    {
        UART_PRINT("[SOCK ERROR] an event received on socket %d\r\n",
                   pSock->SocketAsyncEvent.SockAsyncData.Sd);
        switch(pSock->SocketAsyncEvent.SockAsyncData.Type)
        {
        case SL_SSL_NOTIFICATION_CONNECTED_SECURED:
            UART_PRINT("[SOCK ERROR] SSL handshake done");
            break;
        case SL_SSL_NOTIFICATION_HANDSHAKE_FAILED:
            UART_PRINT("[SOCK ERROR] SSL handshake failed with error %d\r\n",
                       pSock->SocketAsyncEvent.SockAsyncData.Val);
            break;
        case SL_SSL_ACCEPT:
            UART_PRINT(
                "[SOCK ERROR] Recoverable error occurred "
                "during the handshake %d\r\n",
                pSock->SocketAsyncEvent.SockAsyncData.Val);
            break;
        case SL_OTHER_SIDE_CLOSE_SSL_DATA_NOT_ENCRYPTED:
            UART_PRINT("[SOCK ERROR] Other peer terminated the SSL layer.\r\n");
            break;
        case SL_SSL_NOTIFICATION_WRONG_ROOT_CA:
            UART_PRINT("[SOCK ERROR] Used wrong CA to verify the peer.\r\n");
            break;
        default:
            break;
        }
        break;
    }
    default:
        UART_PRINT("[SOCK EVENT] - Unexpected Event [%x0x]\n\n", pSock->Event);
        break;
    }
}
void InitializeAppVariables(void)
{
    g_ulStatus = 0;
    g_ulStaIp = 0;
    g_ulGatewayIP = 0;

    memset(g_ucConnectionSSID, 0, sizeof(g_ucConnectionSSID));
    memset(g_ucConnectionBSSID, 0, sizeof(g_ucConnectionBSSID));
}

//*****************************************************************************
//
//! The function initializes a CC32xx device and triggers it to start operation
//!
//! \param[in]  uiMode  - device mode in which device will be configured
//!
//! \return 0 : success, -ve : failure
//
//*****************************************************************************
long Network_IF_InitDriver(uint32_t uiMode)
{
    long lRetVal = -1;

    /* Reset CC32xx Network State Machine                                    */
    InitializeAppVariables();

    /* Following function configure the device to default state by cleaning  */
    /* the persistent settings stored in NVMEM (viz. connection profiles     */
    /* & policies, power policy etc) Applications may choose to skip this    */
    /* step if the developer is sure that the device is in its default state */
    /* at start of application. Note that all profiles and persistent        */
    /* settings that were done on the device will be lost.                   */
    lRetVal = sl_Start(NULL, NULL, NULL);

    if(lRetVal < 0)
    {
        UART_PRINT("Failed to start the device \n\r");
        LOOP_FOREVER();
    }

    switch(lRetVal)
    {
    case ROLE_STA:
        UART_PRINT("Device came up in Station mode\n\r");
        break;
    case ROLE_AP:
        UART_PRINT("Device came up in Access-Point mode\n\r");
        break;
    case ROLE_P2P:
        UART_PRINT("Device came up in P2P mode\n\r");
        break;
    default:
        UART_PRINT("Error:unknown mode\n\r");
        break;
    }

    if(uiMode != lRetVal)
    {
        UART_PRINT("Switching Networking mode on application request\n\r");

        /* Switch to AP role and restart                                     */
        lRetVal = sl_WlanSetMode(uiMode);
        ASSERT_ON_ERROR(lRetVal,WLAN_ERROR);

        lRetVal = sl_Stop(SL_STOP_TIMEOUT);
        lRetVal = sl_Start(0, 0, 0);
        ASSERT_ON_ERROR(lRetVal,WLAN_ERROR);

        if(lRetVal == uiMode)
        {
            switch(lRetVal)
            {
            case ROLE_STA:
                UART_PRINT("Device came up in Station mode\n\r");
                break;
            case ROLE_AP:
                UART_PRINT("Device came up in Access-Point mode\n\r");
                /* If the device is in AP mode, we need to wait for this */
                /* event before doing anything.                          */
                while(!IS_IP_ACQUIRED(g_ulStatus))
                {
                    usleep(1000);
                }
                break;
            case ROLE_P2P:
                UART_PRINT("Device came up in P2P mode\n\r");
                break;
            default:
                UART_PRINT("Error:unknown mode\n\r");
                break;
            }
        }
        else
        {
            UART_PRINT("could not configure correct networking mode\n\r");
            LOOP_FOREVER();
        }
    }
    else
    {
        if(lRetVal == ROLE_AP)
        {
            while(!IS_IP_ACQUIRED(g_ulStatus))
            {
                usleep(1000);
            }
        }
    }
    return(0);
}

//*****************************************************************************
//
//! The function de-initializes a CC32xx device
//!
//! \param  None
//!
//! \return On success, zero is returned. On error, other
//
//*****************************************************************************
long Network_IF_DeInitDriver(void)
{
    long lRetVal = -1;

    UART_PRINT("SL Disconnect...\n\r");

    /* Disconnect from the AP                                                */
    lRetVal = Network_IF_DisconnectFromAP();

    /* Stop the simplelink host                                              */
    sl_Stop(SL_STOP_TIMEOUT);

    /* Reset the state to uninitialized                                      */
    Network_IF_ResetMCUStateMachine();
    return(lRetVal);
}

//*****************************************************************************
//
//! Connect to an Access Point using the specified SSID
//!
//! \param[in]  pcSsid          - is a string of the AP's SSID
//! \param[in]  SecurityParams  - is Security parameter for AP
//!
//! \return On success, zero is returned. On error, -ve value is returned
//
//*****************************************************************************
long Network_IF_ConnectAP(char *pcSsid,
                          SlWlanSecParams_t SecurityParams)
{
    char acCmdStore[128];
    unsigned short usConnTimeout;
    unsigned char ucRecvdAPDetails;
    long lRetVal;
    unsigned long ulIP = 0;
    unsigned long ulSubMask = 0;
    unsigned long ulDefGateway = 0;
    unsigned long ulDns = 0;

    g_usConnectIndex = 0;

    /* Disconnect from the AP                                                */
    Network_IF_DisconnectFromAP();

    CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
    CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_ACQUIRED);

    /* Continue only if SSID is not empty                                    */
    if(pcSsid != NULL)
    {
        /* This triggers the CC32xx to connect to a specific AP.             */
        lRetVal =
            sl_WlanConnect((signed char *) pcSsid, strlen((const char *) pcSsid),
                           NULL, &SecurityParams, NULL);
        ASSERT_ON_ERROR(lRetVal,WLAN_ERROR);

        /* Wait for ~10 sec to check if connection to desire AP succeeds     */
        while(g_usConnectIndex < 10)
        {
            sleep(1);

            if(IS_CONNECTED(g_ulStatus) && IS_IP_ACQUIRED(g_ulStatus))
            {
                break;
            }
            g_usConnectIndex++;
        }
    }
    else
    {
        UART_PRINT("Empty SSID, Could not connect\n\r");
        return(-1);
    }

    /* Check and loop until AP connection successful, else ask new AP SSID   */
    while(!(IS_CONNECTED(g_ulStatus)) || !(IS_IP_ACQUIRED(g_ulStatus)))
    {
        /* Disconnect the previous attempt                                   */
        Network_IF_DisconnectFromAP();

        CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
        CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_ACQUIRED);
        UART_PRINT("Device could not connect to %s\n\r", pcSsid);

        do
        {
            ucRecvdAPDetails = 0;

            UART_PRINT("\n\r\n\rPlease enter the AP(open) SSID name # ");
            system_reboot();
            /* Get the AP name to connect over the UART                      */
            lRetVal = GetCmd(acCmdStore, sizeof(acCmdStore));
            if(lRetVal > 0)
            {
                /* remove start/end spaces if any                            */
                lRetVal = TrimSpace(acCmdStore);

                if(lRetVal)
                {
                    /* Parse the AP name                                     */
                    strncpy(pcSsid, acCmdStore, lRetVal);
                    if(pcSsid != NULL)
                    {
                        ucRecvdAPDetails = 1;
                        pcSsid[lRetVal] = '\0';
                    }
                }
            }
        }
        while(ucRecvdAPDetails == 0);

        /* Set Security Parameters to OPEN security type.                    */
        SecurityParams.Key = (signed char *) "";
        SecurityParams.KeyLen = 0;
        SecurityParams.Type = SL_WLAN_SEC_TYPE_OPEN;

        UART_PRINT("\n\rTrying to connect to AP: %s ...\n\r", pcSsid);

        /* Get the current timer tick and setup the timeout accordingly.     */
        usConnTimeout = g_usConnectIndex + 15;

        /* This triggers the CC32xx to connect to specific AP.               */
        lRetVal =
            sl_WlanConnect((signed char *) pcSsid, strlen((const char *) pcSsid),
                           NULL, &SecurityParams,NULL);
        ASSERT_ON_ERROR(lRetVal,WLAN_ERROR);

        /* Wait ~10 sec to check if connection to specified AP succeeds      */
        while(!(IS_CONNECTED(g_ulStatus)) || !(IS_IP_ACQUIRED(g_ulStatus)))
        {
            sleep(1);

            if(g_usConnectIndex >= usConnTimeout)
            {
                break;
            }
            g_usConnectIndex++;
        }
    }

    /* Put message on UART                                                   */
    UART_PRINT("\n\rDevice has connected to %s\n\r", pcSsid);

    /* Get IP address                                                        */
    lRetVal = Network_IF_IpConfigGet(&ulIP, &ulSubMask, &ulDefGateway, &ulDns);
    ASSERT_ON_ERROR(lRetVal,WLAN_ERROR);

    /* Send the information                                                  */
    UART_PRINT("Device IP Address is %d.%d.%d.%d\t",
               SL_IPV4_BYTE(ulIP, 3), SL_IPV4_BYTE(ulIP, 2), SL_IPV4_BYTE(ulIP,
                                                                          1),
               SL_IPV4_BYTE(ulIP, 0));

    UART_PRINT("Gateway is %d.%d.%d.%d \n\r\n\r",
               SL_IPV4_BYTE(ulDefGateway, 3), SL_IPV4_BYTE(ulDefGateway, 2), SL_IPV4_BYTE(ulDefGateway, 1),
               SL_IPV4_BYTE(ulDefGateway, 0));
    return(0);
}

//*****************************************************************************
//
//! Disconnects from an Access Point
//!
//! \param  none
//!
//! \return 0 disconnected done, other already disconnected
//
//*****************************************************************************
long Network_IF_DisconnectFromAP(void)
{
    long lRetVal = 0;

    if(IS_CONNECTED(g_ulStatus))
    {
        lRetVal = sl_WlanDisconnect();
        if(0 == lRetVal)
        {
            while(IS_CONNECTED(g_ulStatus))
            {
                usleep(1000);
            }
            return(lRetVal);
        }
        else
        {
            return(lRetVal);
        }
    }
    else
    {
        return(lRetVal);
    }
}

//*****************************************************************************
//
//! Get the IP Address of the device.
//!
//! \param[in]  pulIP               - IP Address of Device
//! \param[in]  pulSubnetMask       - Subnetmask of Device
//! \param[in]  pulDefaultGateway   - Default Gateway value
//! \param[in]  pulDNSServer        - DNS Server
//!
//! \return On success, zero is returned. On error, -1 is returned
//
//*****************************************************************************
long Network_IF_IpConfigGet(unsigned long *pulIP,
                            unsigned long *pulSubnetMask,
                            unsigned long *pulDefaultGateway,
                            unsigned long *pulDNSServer)
{
    unsigned short usDHCP = 0;
    long lRetVal = -1;
    unsigned short len = sizeof(SlNetCfgIpV4Args_t);
    SlNetCfgIpV4Args_t ipV4 = { 0 };

    /* get network configuration                                             */
    lRetVal =
        sl_NetCfgGet(SL_NETCFG_IPV4_STA_ADDR_MODE, &usDHCP, &len,
                     (unsigned char *) &ipV4);
    ASSERT_ON_ERROR(lRetVal,WLAN_ERROR);

    *pulIP = ipV4.Ip;
    *pulSubnetMask = ipV4.IpMask;
    *pulDefaultGateway = ipV4.IpGateway;
//    *pulDefaultGateway = ipV4.IpDnsServer;

    return(lRetVal);
}

//*****************************************************************************
//
//!  This function obtains the server IP address using a DNS lookup
//!
//! \param[in]  pcHostName        The server hostname
//! \param[out] pDestinationIP    This parameter is filled with host IP address
//!
//! \return On success, +ve value is returned. On error, -ve value is returned
//
//*****************************************************************************
long Network_IF_GetHostIP(char* pcHostName,
                          unsigned long * pDestinationIP)
{
    long lStatus = 0;

    lStatus =
        sl_NetAppDnsGetHostByName((signed char *) pcHostName, strlen(
                                      pcHostName), pDestinationIP, SL_AF_INET);
    ASSERT_ON_ERROR(lStatus,WLAN_ERROR);

    UART_PRINT("Get Host IP succeeded.\n\rHost: %s IP: %d.%d.%d.%d \n\r\n\r",
               pcHostName, SL_IPV4_BYTE(*pDestinationIP,
                                        3), SL_IPV4_BYTE(*pDestinationIP,
                                                         2),
               SL_IPV4_BYTE(*pDestinationIP, 1),
               SL_IPV4_BYTE(*pDestinationIP, 0));

    return(lStatus);
}

//*****************************************************************************
//
//! Reset state from the state machine
//!
//! \param  None
//!
//! \return none
//!
//*****************************************************************************
void Network_IF_ResetMCUStateMachine()
{
    g_ulStatus = 0;
}

//*****************************************************************************
//
//! Return the current state bits
//!
//! \param  None
//!
//! \return none
//!
//
//*****************************************************************************
unsigned long Network_IF_CurrentMCUState()
{
    return(g_ulStatus);
}

//*****************************************************************************
//
//! Sets a state from the state machine
//!
//! \param[in]  cStat   - Status of State Machine defined in e_StatusBits
//!
//! \return none
//!
//*****************************************************************************
void Network_IF_SetMCUMachineState(char cStat)
{
    SET_STATUS_BIT(g_ulStatus, cStat);
}

//*****************************************************************************
//
//! Unsets a state from the state machine
//!
//! \param[in]  cStat   - Status of State Machine defined in e_StatusBits
//!
//! \return none
//!
//*****************************************************************************
void Network_IF_UnsetMCUMachineState(char cStat)
{
    CLR_STATUS_BIT(g_ulStatus, cStat);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

int Network_IF_Connect2AP(char* ssid, char* pass, int retry)
{
	SlSecParams_t secParams = {0};
	long lRetVal = -1;
	int i = 0;

	//
	// Connect to the Access Point
	//
	while((i<2)&&(lRetVal<0))
	{
		secParams.Key = (signed char*)pass;
		secParams.KeyLen = strlen(pass);
		secParams.Type = SL_SEC_TYPE_WPA_WPA2;
//		secParams.Type = sec_string_to_num();
		lRetVal = Network_IF_ConnectAP(ssid,secParams);
//		UART_PRINT("Connection to AP:%s, pass:%s, sec:%d, ret %d\r\n", ssid, secParams.Key, secParams.Type, lRetVal);
		UART_PRINT("Connection to AP:%s, pass:%s, sec:%s, ret %d\r\n", ssid, secParams.Key,
				((secParams.Type == 0)? "Open":
				(secParams.Type == 1)? "WEP":
				(secParams.Type == 2)? "WPA":
				"WPA2"), lRetVal);
		i++;
	}
	return lRetVal;
}
int Network_IF_WifiSetMode(int iMode)
{
  int lRetVal = -1;
  unsigned char str[10] = "EU";
  lRetVal = sl_Start(NULL, NULL, NULL);
  // desire mode duplicate with previous mode
  if(lRetVal == iMode)
  {
    UART_PRINT("Current mode available\n\r");
    return 0;
  }

  sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID, WLAN_GENERAL_PARAM_OPT_COUNTRY_CODE, 2, str);
  /**< set device in AP mode >*/
  if(ROLE_AP == iMode)
  {
    UART_PRINT("Switching to AP mode on application request\n\r");
    // Switch to AP role and restart
    lRetVal = sl_WlanSetMode(iMode);
    _ASSERT_ON_ERROR(lRetVal);
    ap_config();

    /**< end config ssid name > */
    // restart
    lRetVal = sl_Stop(0xFF);
    // reset status bits
    CLR_STATUS_BIT_ALL(g_ulStatus);
    lRetVal = sl_Start(0, 0, 0);
    _ASSERT_ON_ERROR(lRetVal);

    // Check if the device is up in AP Mode
    if (ROLE_AP == lRetVal)
    {
      // If the device is in AP mode, we need to wait for this event
      // before doing anything
      while(!IS_IP_ACQUIRED(g_ulStatus))
      {
    	  usleep(1000);

      }
    }
    else
    {
      // We don't want to proceed if the device is not coming up in AP-mode
      _ASSERT_ON_ERROR(DEVICE_NOT_IN_AP_MODE);
    }
    UART_PRINT("Re-started SimpleLink Device: AP Mode\n\r");
  }

  /**< set device in STA mode >*/
  else if(ROLE_STA == iMode)
  {
    UART_PRINT("Switching to STA mode on application request\n\r");
    // Switch to AP role and restart
    lRetVal = sl_WlanSetMode(iMode);
    _ASSERT_ON_ERROR(lRetVal);
    // restart
    lRetVal = sl_Stop(0xFF);
    // reset status bits
    CLR_STATUS_BIT_ALL(g_ulStatus);
    lRetVal = sl_Start(0, 0, 0);
    _ASSERT_ON_ERROR(lRetVal);
    // Check if the device is in station again
    if (ROLE_STA != lRetVal)
    {
      // We don't want to proceed if the device is not coming up in STA-mode
      _ASSERT_ON_ERROR(DEVICE_NOT_IN_STATION_MODE);
    }
    UART_PRINT("Re-started SimpleLink Device: STA Mode\n\r");
  }

  /**< set device in P2P mode >*/
  else if(ROLE_P2P == iMode)
  {
    UART_PRINT("Switching to P2P mode on application request\n\r");
    // Switch to AP role and restart
    lRetVal = sl_WlanSetMode(iMode);
    _ASSERT_ON_ERROR(lRetVal);
    // restart
    lRetVal = sl_Stop(0xFF);
    // reset status bits
    CLR_STATUS_BIT_ALL(g_ulStatus);
    lRetVal = sl_Start(0, 0, 0);
    _ASSERT_ON_ERROR(lRetVal);
    // Check if the device is in station again
    if (ROLE_P2P != lRetVal)
    {
      // We don't want to proceed if the device is not coming up in P2P-mode
      _ASSERT_ON_ERROR(DEVICE_NOT_IN_P2P_MODE);
    }
    UART_PRINT("Re-started SimpleLink Device: P2P Mode\n\r");
  }

  else
  {
    UART_PRINT("Invalid Wifi mode\n\r");
    return -1;
  }
  return 0;
}

int Network_IF_Connect2AP_static_ip(char* ssid, char* pass, int retry)
{
    long lRetVal = 0;
    if (ssid == NULL || pass == NULL)
    {
//        g_input_state=LED_STATE_NONE;
        return -1;
    }

/*    sl_WlanProfileDel(0xFF);
    ConfigureSimpleLinkToDefaultState();
    InitializeAppVariables();*/
    lRetVal = Network_IF_WifiSetMode(ROLE_STA);
    if (lRetVal < 0) {
        return lRetVal;
    }

    lRetVal = Network_IF_Connect2AP(ssid, pass, 0);
    if (lRetVal < 0) {
        return lRetVal;
    }
    // Set connect policy
    // Auto + SmartConfig + fast connect
    lRetVal = sl_WlanPolicySet(SL_POLICY_CONNECTION,
                                SL_CONNECTION_POLICY(1, 1, 0, 0, 1), NULL, 0);
    if (lRetVal < 0) {
        return lRetVal;
    }
    /*
    * Save connection profile
    */
    lRetVal = sl_WlanProfileDel(0xFF);
    if (lRetVal < 0) {
        return lRetVal;
    }

    // Set nearly max priority
    // Initialize AP security params
    SlSecParams_t SecurityParams = {0}; // AP Security Parameters
    SecurityParams.Key = (signed char*)pass;
    SecurityParams.KeyLen = strlen(pass);
    // SecurityParams.Type = SECURITY_TYPE;
    //SecurityParams.Type = SL_SEC_TYPE_WPA_WPA2;
    SecurityParams.Type = sec_string_to_num();
    lRetVal = sl_WlanProfileAdd((signed char*)ssid,
                      strlen(ssid), 0, &SecurityParams, 0, 6, 0);
    if (lRetVal < 0) {
        return lRetVal;
    }

    #if (USING_STATIC_IP)
    /*
    * Set static ip
    */
    // Get current ip
    uint8_t len = sizeof(SlNetCfgIpV4Args_t);
    uint8_t dhcpIsOn = 0;
    SlNetCfgIpV4Args_t ipV4 = {0};
    sl_NetCfgGet(SL_IPV4_STA_P2P_CL_GET_INFO,&dhcpIsOn,&len,(_u8 *)&ipV4);

    UART_PRINT("DHCP is %s IP %d.%d.%d.%d MASK %d.%d.%d.%d GW %d.%d.%d.%d DNS %d.%d.%d.%d\n",
            (dhcpIsOn > 0) ? "ON" : "OFF",
            SL_IPV4_BYTE(ipV4.ipV4,3),SL_IPV4_BYTE(ipV4.ipV4,2),SL_IPV4_BYTE(ipV4.ipV4,1),SL_IPV4_BYTE(ipV4.ipV4,0),
            SL_IPV4_BYTE(ipV4.ipV4Mask,3),SL_IPV4_BYTE(ipV4.ipV4Mask,2),SL_IPV4_BYTE(ipV4.ipV4Mask,1),SL_IPV4_BYTE(ipV4.ipV4Mask,0),
            SL_IPV4_BYTE(ipV4.ipV4Gateway,3),SL_IPV4_BYTE(ipV4.ipV4Gateway,2),SL_IPV4_BYTE(ipV4.ipV4Gateway,1),SL_IPV4_BYTE(ipV4.ipV4Gateway,0),
            SL_IPV4_BYTE(ipV4.ipV4DnsServer,3),SL_IPV4_BYTE(ipV4.ipV4DnsServer,2),SL_IPV4_BYTE(ipV4.ipV4DnsServer,1),SL_IPV4_BYTE(ipV4.ipV4DnsServer,0));

    // Change to STATIC
    sl_NetCfgSet(SL_IPV4_STA_P2P_CL_STATIC_ENABLE,IPCONFIG_MODE_ENABLE_IPV4,sizeof(SlNetCfgIpV4Args_t),(_u8 *)&ipV4);
    ASSERT_ON_ERROR(lRetVal);
  #endif
    return lRetVal;
}
ConfigureSimpleLinkToDefaultState(int32_t Mode)
{
     uint8_t                              ucConfigOpt;
     uint8_t                              ucPower;
     int32_t                              RetVal = -1;
//     int32_t                              Mode = -1;
     uint32_t                             IfBitmap = 0;
     SlWlanScanParamCommand_t             ScanDefault = {0};
     SlWlanRxFilterOperationCommandBuff_t RxFilterIdMask = {{0}};

     /* Turn NWP on */

/*    Mode = sl_Start(0, 0, 0);
  	if(Mode < 0)
  	{
  		 UART_PRINT("sl_Start error:  %d\n\r", Mode);
  		 return NULL;
  	}
  	InitFile();
  	CheckFile_System();
	ASSERT_ON_ERROR(Mode, DEVICE_ERROR);*/

    if(Mode != ROLE_STA)
    {
        /* Set NWP role as STA */
        Mode = sl_WlanSetMode(ROLE_STA);
        ASSERT_ON_ERROR(Mode, WLAN_ERROR);

         /* For changes to take affect, we restart the NWP */
         RetVal = sl_Stop(SL_STOP_TIMEOUT);
         ASSERT_ON_ERROR(RetVal, DEVICE_ERROR);

         Mode = sl_Start(0, 0, 0);
         ASSERT_ON_ERROR(Mode, DEVICE_ERROR);
     }

    if(Mode != ROLE_STA)
    {
        UART_PRINT("Failed to configure device to it's default state");
        return(-1);
    }

    /* Set policy to auto only */
    RetVal =
        sl_WlanPolicySet(SL_WLAN_POLICY_CONNECTION,
                         SL_WLAN_CONNECTION_POLICY(1,0,0,
                                                   0),
                         NULL,0);
    ASSERT_ON_ERROR(RetVal, WLAN_ERROR);

    /* Disable Auto Provisioning */
    RetVal = sl_WlanProvisioning(SL_WLAN_PROVISIONING_CMD_STOP,
                                 0xFF,
                                 0,
                                 NULL,
                                 0x0);
    ASSERT_ON_ERROR(RetVal, WLAN_ERROR);

     /* Delete existing profiles */

     /* RetVal = sl_WlanProfileDel(0xFF);
        ASSERT_ON_ERROR(RetVal, WLAN_ERROR); */


     /* enable DHCP client */
     RetVal = sl_NetCfgSet(SL_NETCFG_IPV4_STA_ADDR_MODE,
     SL_NETCFG_ADDR_DHCP, 0, 0);
     ASSERT_ON_ERROR(RetVal, NETAPP_ERROR);

     /* Disable ipv6 */
     IfBitmap = !(SL_NETCFG_IF_IPV6_STA_LOCAL | SL_NETCFG_IF_IPV6_STA_GLOBAL);
     RetVal = sl_NetCfgSet(SL_NETCFG_IF, SL_NETCFG_IF_STATE, sizeof(IfBitmap),
                          (const unsigned char *)&IfBitmap);
     ASSERT_ON_ERROR(RetVal, NETAPP_ERROR);

     /* Configure scan parameters to default */
     ScanDefault.ChannelsMask = CHANNEL_MASK_ALL;
     ScanDefault.RssiThreshold = RSSI_TH_MAX;

    RetVal =
        sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID,
                   SL_WLAN_GENERAL_PARAM_OPT_SCAN_PARAMS,
                   sizeof(ScanDefault),
                   (uint8_t *)&ScanDefault);
    ASSERT_ON_ERROR(RetVal, WLAN_ERROR);

    /* Disable scans */
    ucConfigOpt = SL_WLAN_SCAN_POLICY(0, 0);
    RetVal = sl_WlanPolicySet(SL_WLAN_POLICY_SCAN, ucConfigOpt, NULL, 0);
    ASSERT_ON_ERROR(RetVal, WLAN_ERROR);

    /* Set TX power lvl to max */
    ucPower = 0;
    RetVal =
        sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID,
                   SL_WLAN_GENERAL_PARAM_OPT_STA_TX_POWER,
                   1,
                   (uint8_t *)&ucPower);
    ASSERT_ON_ERROR(RetVal, WLAN_ERROR);

    /* Set NWP Power policy to 'normal' */
    RetVal =
         sl_WlanPolicySet(SL_WLAN_POLICY_PM, SL_WLAN_NORMAL_POLICY, NULL,0);
    ASSERT_ON_ERROR(RetVal, WLAN_ERROR);

    /* Unregister mDNS services */
    RetVal = sl_NetAppMDNSUnRegisterService(0, 0, 0);
    ASSERT_ON_ERROR(RetVal, NETAPP_ERROR);

    /* Remove all 64 RX filters (8*8) */
    memset(RxFilterIdMask.FilterBitmap, 0xFF, 8);

    RetVal =
        sl_WlanSet(SL_WLAN_RX_FILTERS_ID, SL_WLAN_RX_FILTER_REMOVE,
                   sizeof(SlWlanRxFilterOperationCommandBuff_t),
                   (uint8_t *)&RxFilterIdMask);
    ASSERT_ON_ERROR(RetVal, WLAN_ERROR);

    /* Set NWP role as STA */
    RetVal = sl_WlanSetMode(ROLE_STA);
    ASSERT_ON_ERROR(RetVal, WLAN_ERROR);

    /* For changes to take affect, we restart the NWP */
    RetVal = sl_Stop(SL_STOP_TIMEOUT);
    ASSERT_ON_ERROR(RetVal, DEVICE_ERROR);

    Mode = sl_Start(0, 0, 0);
    ASSERT_ON_ERROR(Mode, DEVICE_ERROR);

    if(ROLE_STA != Mode)
    {
        UART_PRINT("Failed to configure device to it's default state");
        return(-1);
    }
    else
    {
        app_CB.Role = ROLE_STA;
        SET_STATUS_BIT(app_CB.Status, STATUS_BIT_NWP_INIT);
    }

    return(0);
}
int Network_IF_GetScanRTResult(Sl_WlanNetworkEntry_t* netEntries)
{
	unsigned char   policyOpt;
	unsigned long IntervalVal = 10;
	int lRetVal;

	policyOpt = SL_CONNECTION_POLICY(0, 0, 0, 0, 0);

	lRetVal = sl_WlanPolicySet(SL_POLICY_CONNECTION , policyOpt, NULL, 0);

	if(lRetVal < 0)
	{
	UART_PRINT("set connect policy failed\n");
	return lRetVal;
	}

	// enable scan
	policyOpt = SL_SCAN_POLICY(1);

	// set scan policy - this starts the scan
	lRetVal = sl_WlanPolicySet(SL_POLICY_SCAN , policyOpt,
							 (unsigned char *)(IntervalVal), sizeof(IntervalVal));

	if(lRetVal < 0)
	{
	UART_PRINT("set scan policy failed\n");
	return lRetVal;
	}

	// extern long ConfigureSimpleLinkToDefaultState();
	// extern void InitializeAppVariables();

	// delay 1 second to verify scan is started
	//  usleep(1000);
	osi_Sleep(1000);

	// lRetVal indicates the valid number of entries
	// The scan results are occupied in netEntries[]

	lRetVal = sl_WlanGetNetworkList(0, SCAN_TABLE_SIZE, netEntries);
	if(lRetVal < 0)
	{
	UART_PRINT("get ap list failed\n");
	return lRetVal;
	}

	// Disable scan
	policyOpt = SL_SCAN_POLICY(0);

	// set scan policy - this stops the scan
	sl_WlanPolicySet(SL_POLICY_SCAN , policyOpt,
				   (unsigned char *)(IntervalVal), sizeof(IntervalVal));
	if(lRetVal < 0)
	{
	UART_PRINT("disable scan policy failed\n");
	return lRetVal;
	}

	return lRetVal;

}

int Network_IF_provisionAP()
{
	long lCountSSID = 0;
	long ssid_cnt = 0;
	long i = 0;
	long j = 0;
	unsigned char temp_var = 0;
	char mac_address[32] = "\0";
	//  static char router_list_resp[1024] = {0};
	char router_list_data[2048] = {0};
	int l_scantotalnum=0;

	char router_info_fm[200] = "<w>\n" \
	"<s>\"%s\"</s>\n" \
	  "<b>%s</b>\n" \
		"<a>%s</a>\n" \
		  "<q>%d</q>\n" \
			"<si>%d</si>\n" \
			  "<nl>%d</nl>\n" \
				"<ch>%d</ch>\n" \
				  "</w>\n";

	char xml_header[100] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"\
	"<wl v=\"2.0\">\n"\
	  "<n>%d</n>\n";
	//  while(!g_ucProvisioningDone)
	for(j=0;j<6;j++)//j loop for number of rescan
	{

		#if 1
		//Get Scan Result
		lCountSSID = Network_IF_GetScanRTResult(&g_NetEntries[0]);

		if(lCountSSID < 0)
		{
			return lCountSSID;
		}
		#else
		lCountSSID = 0;
		#endif

		#if 1
		for(ssid_cnt = 0; ssid_cnt < lCountSSID; ssid_cnt++)
		{
				for(i = 0; i < 6; i++)
				{
					temp_var = g_NetEntries[ssid_cnt].Bssid[i];
				//            UART_PRINT("%x ", temp_var);
					if(temp_var<0x0f)
					{
						sprintf(mac_address + strlen(mac_address), "0%x", temp_var);
					}
					else
					{
						sprintf(mac_address + strlen(mac_address), "%x", temp_var);
					}
					if(i<5)
					{
						sprintf(mac_address + strlen(mac_address), ":");
					}
				}

				char raw_ssid[SSID_LEN_MAX];
				char encode_ssid[SSID_LEN_MAX];
				memset(raw_ssid, 0x00, sizeof(raw_ssid));
				memset(encode_ssid, 0x00, sizeof(encode_ssid));
				sprintf(raw_ssid, "%.*s", g_NetEntries[ssid_cnt].SsidLen, g_NetEntries[ssid_cnt].Ssid);
				write_xml_string(raw_ssid, encode_ssid);
				if ((l_scantotalnum>=SCAN_TABLE_SIZE) || (strstr(router_list_data,encode_ssid)!=NULL))
				{
				  memset(mac_address, 0, sizeof(mac_address));
				  continue;
				}
		//      UART_PRINT("ssid %d,%d: %s\r\n", ssid_cnt, l_scantotalnum, encode_ssid);
				UART_PRINT("ssid : %s\r\n", encode_ssid);
		//      UART_PRINT("Ssid						Security\r\n");
		//      UART_PRINT("%s		 			    %-4s\r\n", encode_ssid,(g_NetEntries[ssid_cnt].SecurityInfo== 0)? "Open": \
		//              (g_NetEntries[ssid_cnt].SecurityInfo == 1)? "WEP": \
		//                (g_NetEntries[ssid_cnt].SecurityInfo == 2)? "WPA": \
		//                  "WPA2");
			  // assign format for rounter info
				sprintf(router_list_data + strlen(router_list_data), router_info_fm,
					  encode_ssid,
					  mac_address,
					  (g_NetEntries[ssid_cnt].SecurityInfo== 0)? "Open":
						(g_NetEntries[ssid_cnt].SecurityInfo == 1)? "WEP":
						  (g_NetEntries[ssid_cnt].SecurityInfo == 2)? "WPA":
							"WPA2",
							((g_NetEntries[ssid_cnt].Rssi+255)*100/255),
							((g_NetEntries[ssid_cnt].Rssi+255)*100/255),
							0,
							0
							  );

				memset(mac_address, 0, sizeof(mac_address));
				l_scantotalnum++;
		}
		#endif

	}//end for j
	memset(router_list, 0, sizeof(router_list));
	sprintf(router_list, xml_header, l_scantotalnum);
	memcpy(router_list + strlen(router_list), router_list_data, strlen(router_list_data));
	memcpy(router_list + strlen(router_list), "</wl>", strlen("</wl>"));
	UART_PRINT("SSID Total: %d\r\n",l_scantotalnum);
	return 0;
}
