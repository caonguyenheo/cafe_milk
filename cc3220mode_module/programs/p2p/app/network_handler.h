/*
 * network_handler.h
 *
 *  Created on: Aug 30, 2018
 *      Author: Admin
 */

#ifndef NETWORK_HANDLER_H_
#define NETWORK_HANDLER_H_

#include <ti/drivers/net/wifi/simplelink.h>


long Network_IF_InitDriver(uint32_t uiMode);
long Network_IF_DeInitDriver(void);
long Network_IF_ConnectAP(char * pcSsid,
                          SlWlanSecParams_t SecurityParams);
long Network_IF_DisconnectFromAP();
long Network_IF_IpConfigGet(unsigned long *aucIP,
                            unsigned long *aucSubnetMask,
                            unsigned long *aucDefaultGateway,
                            unsigned long *aucDNSServer);
long Network_IF_GetHostIP(char* pcHostName,
                          unsigned long * pDestinationIP);
unsigned long Network_IF_CurrentMCUState(void);
void Network_IF_UnsetMCUMachineState(char stat);
void Network_IF_SetMCUMachineState(char stat);
void Network_IF_ResetMCUStateMachine(void);
int Network_IF_Connect2AP_static_ip(char* ssid, char* pass, int retry);

int Network_IF_provisionAP();
int Network_IF_GetScanRTResult(SlWlanNetworkEntry_t* netEntries);
int Network_IF_WifiSetMode(int iMode);
void InitializeAppVariables(void);
int Network_IF_Connect2AP_static_ip(char* ssid, char* pass, int retry);

int32_t ConfigureSimpleLinkToDefaultState(int32_t Mode);
#endif /* NETWORK_HANDLER_H_ */
