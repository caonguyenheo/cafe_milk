/*
 * system.h
 *
 *  Created on: Aug 30, 2018
 *      Author: Admin
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_


#include <ti/drivers/net/wifi/simplelink.h>

#define PORT_NUM            5001
#define BUFF_SIZE            1400
#define TCP_PACKET_COUNT    1000
//#define IP_ADDR             0xc0a80064
#define TCP_PROTOCOL_FLAGS      0
#define BUF_LEN                (MAX_BUF_SIZE)
#define BUFF_LEN_FILE          (1024)
#define MAX_FILE_SIZE          (1024)
#define MAX_FILE_SIZE_NEW      (MAX_FILE_SIZE  * 3)
#define MAX_FILE_SIZE1          (100)

#define MAX_AP_SSID_LEN         32
#define SSID_LEN_MAX        	32
#define MAX_AP_PASS_LEN         64
#define MAX_AP_SEC_LEN          8


#define DEV_NOT_REGISTERED          0xC3
#define DEV_REGISTERED              0xAB

#define REGISTRATION_ERROR_SETMODESTA (-50)
#define REGISTRATION_ERROR_SETMODEAP (-51)
#define REGISTRATION_ERROR_STOPHTTP (-52)
#define REGISTRATION_ERROR_INITDRVSTA (-53)
#define REGISTRATION_ERROR_CONNECTAP (-54)
#define REGISTRATION_ERROR_REGPOSTFAIL (-55)
#define REGISTRATION_ERROR_REGTIMEOUT (-56)

#define SECOND_PERDAY (24*3600)
#define PERIOD_REBOOT_TIME_MS (7*SECOND_PERDAY*1000)

#define OPENFILE			0


typedef enum
{
    // Choosing -0x7D0 to avoid overlap w/ host-driver's error codes
    SOCKET_CREATE_ERROR = -0x7D0,
    BIND_ERROR = SOCKET_CREATE_ERROR - 1,
    LISTEN_ERROR = BIND_ERROR -1,
    SOCKET_OPT_ERROR = LISTEN_ERROR -1,
    CONNECT_ERROR = SOCKET_OPT_ERROR -1,
    ACCEPT_ERROR = CONNECT_ERROR - 1,
    SEND_ERROR = ACCEPT_ERROR -1,
    RECV_ERROR = SEND_ERROR -1,
    SOCKET_CLOSE_ERROR = RECV_ERROR -1,
    DEVICE_NOT_IN_STATION_MODE_S = SOCKET_CLOSE_ERROR - 1,
    STATUS_CODE_MAX_S = -0xBB8
}e_AppStatusCodes;

typedef union
{
    uint32_t ipv4;          /* Ipv4 Address */
    uint8_t ipv6[16];       /* Ipv6 Address */
}ip_t;

typedef union
{
    SlSockAddrIn6_t in6;       /* Socket info for Ipv6 */
    SlSockAddrIn_t in4;        /* Socket info for Ipv4 */
}sockAddr_t;


int system_init(void);
int system_registration();
int system_IsRegistered();
void system_reboot();
void system_reboot_1();
int system_Deregister(void);



void DisplayUsage();
int32_t DisplayAppBanner(char* appName, char* appVersion);
void InitializeAppVariables(void);
void printBorder(char ch, int n);

int TcpClient(unsigned int uip, unsigned short  usPort, char *buff, int nb, int tx);
int TcpServer_cp(unsigned short  usPort, int nb, int tx);
int TcpClient_cp(unsigned int uip, unsigned short  usPort, char *buff, int nb, int tx);
int TcpServer(unsigned short  usPort, int nb, int tx);
int SendFile_Tcp(unsigned int uip, unsigned short  usPort);
int ReceFile_Tcp(unsigned short  usPort);
int TcpServer_multithread(unsigned short  usPort);
int TcpClient_multithread(unsigned int uip, unsigned short usPort);
int Udp_server(unsigned short  usPort);
int Udp_client(unsigned int uip, unsigned short  usPort);
int client_group(unsigned int uip, unsigned short  usPort);

void *receive(void * param);
void *client_group2(void * param);

void PrintIPAddress(unsigned char ipv6,void *ip);
int32_t ipv4AddressParse(char *str, uint32_t *ipv4ip);
int sec_string_to_num();

int32_t set_wireless_config(char *ssid, char *pass, char *auth_type);
int32_t get_wireless_config();
void fw_version_format(char* l_output, char* l_input);


#endif /* SYSTEM_H_ */
