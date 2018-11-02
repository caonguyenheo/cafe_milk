/*
 * cc3220_system.h
 *
 *  Created on: Sep 10, 2018
 *      Author: Admin
 */

#ifndef CC3220_SYSTEM_H_
#define CC3220_SYSTEM_H_


#define MAX_UID_SIZE			25
#define MAX_MAC_ADDR_SIZE		13


#define SYSTEM_VERSION     "000002"
#define VENDOR				0
#define TYPE_NUM			0
#define STR_MODEL			"001"



int get_mac_address(char *str_mac_addr);
int set_mac_address(char *str_mac_addr);
int get_uid(char *uid);
int get_version(char *l_version);

#endif /* CC3220_SYSTEM_H_ */
