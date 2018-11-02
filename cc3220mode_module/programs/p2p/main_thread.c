/*
 * main_thread.c
 *
 *  Created on: Aug 29, 2018
 *      Author: Admin
 */
#include "main_thread.h"
#include "tcp.h"
#include "shelltask.h"
#include "system.h"
#include "network_handler.h"


//pthread_t           gSpawn_thread = (pthread_t)NULL;
//pthread_t           gShell_thread = (pthread_t)NULL;
//pthread_t           gTcp_thread = (pthread_t)NULL;
appControlBlock     app_CB;

#if 0
int32_t    initAppVariables(void)
{
    int32_t ret = 0;

    app_CB.Status = 0 ;
    app_CB.Role = ROLE_RESERVED;
    app_CB.Exit = FALSE;

    memset(&app_CB.CmdBuffer, 0x0, CMD_BUFFER_LEN);
    memset(&app_CB.gDataBuffer, 0x0, sizeof(app_CB.gDataBuffer));
    memset(&app_CB.CON_CB, 0x0, sizeof(app_CB.CON_CB));

    ret = sem_init(&app_CB.CON_CB.connectEventSyncObj,    0, 0);
    if(ret != 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return(-1);
    }

    ret = sem_init(&app_CB.CON_CB.eventCompletedSyncObj,  0, 0);
    if(ret != 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return(-1);
    }

    ret = sem_init(&app_CB.CON_CB.ip4acquireEventSyncObj, 0, 0);
    if(ret != 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return(-1);
    }

    ret = sem_init(&app_CB.CON_CB.ip6acquireEventSyncObj, 0, 0);
    if(ret != 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return(-1);
    }

    memset(&app_CB.P2P_CB, 0x0, sizeof(app_CB.P2P_CB));

    ret = sem_init(&app_CB.P2P_CB.DeviceFound, 0, 0);
    if(ret != 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return(-1);
    }

    ret = sem_init(&app_CB.P2P_CB.RcvConReq, 0, 0);
    if(ret != 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return(-1);
    }

    ret = sem_init(&app_CB.P2P_CB.RcvNegReq, 0, 0);
    if(ret != 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return(-1);
    }

    ret = sem_init(&app_CB.WowlanSleepSem, 0, 0);
    if(ret != 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return(-1);
    }

    return(ret);
}
#endif

void * mainThread1(void *arg)
{
//    int32_t             RetVal ;
//    struct sched_param  priParam;
    struct timespec     ts = {0};
    struct timespec     ts_begin = {0};
    struct timespec     ts_curent = {0};
    int l_timeout;
//    char buff[1024]={0};
//    int ibuff[1024]={0};
    /* Initializes the SPI interface to the Network
       Processor and peripheral SPI (if defined in the board file) */
    InitTerm();
    GPIO_init();
    SPI_init();

    /* Init Application variables */
//    RetVal = initAppVariables();

    /* Init Terminal UART */


    /* initialize the realtime clock */
    clock_settime(CLOCK_REALTIME, &ts);

    /* Switch off all LEDs on boards */
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_OFF);
    GPIO_write(Board_GPIO_LED1, Board_GPIO_LED_OFF);
    GPIO_write(Board_GPIO_LED2, Board_GPIO_LED_OFF);
    clock_gettime(CLOCK_REALTIME,&ts_begin);
//    UART_PRINT("sizeof buff:%d\n\r",sizeof(buff));
//    UART_PRINT("sizeof ibuff:%d\n\r",sizeof(ibuff));
#if 0
    UART_PRINT("sizeof _u8:%d\n\r",sizeof(_u8));
    UART_PRINT("sizeof _i8:%d\n\r",sizeof(_i8));
    UART_PRINT("sizeof _u16:%d\n\r",sizeof(_u16));
    UART_PRINT("sizeof _i16:%d\n\r",sizeof(_i16));
    UART_PRINT("sizeof _u32:%d\n\r",sizeof(_u32));
    UART_PRINT("sizeof _i32:%d\n\r",sizeof(_i32));

    UART_PRINT("sizeof int32_t:%d\n\r",sizeof(int32_t));
    UART_PRINT("sizeof uint32_t:%d\n\r",sizeof(uint32_t));
    UART_PRINT("sizeof int8_t:%d\n\r",sizeof(int8_t));
    UART_PRINT("sizeof uint8_t:%d\n\r",sizeof(uint8_t));
    UART_PRINT("sizeof int32_t:%d\n\r",sizeof(int32_t));
    UART_PRINT("sizeof uint32_t:%d\n\r",sizeof(uint32_t));
    UART_PRINT("sizeof int64_t:%d\n\r",sizeof(int64_t));
    UART_PRINT("sizeof int64_t:%d\n\r",sizeof(uint64_t));
#endif
    clock_gettime(CLOCK_REALTIME,&ts_curent);
    l_timeout = ts_curent.tv_sec - ts_begin.tv_sec;
    UART_PRINT("l_timeout:%d\n\r",l_timeout);
    return(0);
}
