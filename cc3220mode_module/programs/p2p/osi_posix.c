/*
 * osi_posix.c
 *
 *  Created on: Aug 29, 2018
 *      Author: Admin
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
#include <mqueue.h>
#include "osi.h"
#include "uart_terminal.h"
#include <ti/drivers/net/wifi/simplelink.h>  /* sl_Task */
#include <ti/devices/cc32xx/inc/hw_ints.h>
#include <ti/drivers/dpl/HwiP.h>
#include "ti/devices/cc32xx/driverlib/rom_map.h"
#include <ti/devices/cc32xx/driverlib/interrupt.h>
/* RTOS header files */

#ifdef USE_FREERTOS
#else
#define SPAWN_TASK_PRIORITY     9
#endif

pthread_t slSpawnThread;


static void st_GetAbsoluteTime(OsiTime_t msecToAdd, struct timespec *pOutTM)
{
    clock_gettime(0, pOutTM);
    pOutTM->tv_sec += msecToAdd / 1000 ;
    pOutTM->tv_nsec += (msecToAdd % 1000)*1000000;

    pOutTM->tv_sec += (pOutTM->tv_nsec / 1000000000);
    pOutTM->tv_nsec = pOutTM->tv_nsec % 1000000000;
}

unsigned long GetAbsoluteTimeMS()
{
    struct timespec tm;
    clock_gettime(0, &tm);
    return (tm.tv_sec * 1000) + (tm.tv_nsec / 1000000);
}

/*!
    \brief  This function registers an interrupt in NVIC table

    The sync object is used for synchronization between different thread or ISR and
    a thread.

    \param  iIntrNum    -   Interrupt number to register
    \param  pEntry      -   Pointer to the interrupt handler e.g. INT_PRIORITY_LVL_1
    \param ucPriority   -   Priority of the interrupt

    \return upon successful creation the function should return 0
            Otherwise, a negative value indicating the error code shall be returned
    \note
    \warning
*/
typedef struct _osiIntContext_t
{
    int                     intNum;
    HwiP_Handle             intHandle;
    struct _osiIntContext_t *pNext;
} osiIntContext_t;
osiIntContext_t *g_osiInterrupts = NULL;

OsiReturnVal_e osi_InterruptRegister(int iIntrNum, P_OSI_INTR_ENTRY fInterruptHdl, unsigned char ucPriority)
{
#ifdef SL_PLATFORM_MULTI_THREADED

    HwiP_Params intParams;
    HwiP_Handle intHandle;
    OsiReturnVal_e ret = OSI_FAILURE;

    //OSI_HEADER_NL();
    HwiP_clearInterrupt(iIntrNum);

    if(!fInterruptHdl)
    {
        ret = OSI_INVALID_PARAMS;
    }
    else
    {
        intParams.priority = ucPriority; //

        intHandle = HwiP_create(iIntrNum , (HwiP_Fxn)(fInterruptHdl) , &intParams);

        if(intHandle)
        {
            osiIntContext_t *pIntContext = (osiIntContext_t*)malloc(sizeof(osiIntContext_t));
            if(pIntContext)
            {
                pIntContext->intNum = iIntrNum;
                pIntContext->intHandle = intHandle;
                pIntContext->pNext = g_osiInterrupts;
                g_osiInterrupts = pIntContext;

                ret = _OSI_OK;
            }
        }
    }
    return ret;

#else

    if(!fInterruptHdl)
    {
        MAP_IntDisable(INT_NWPIC);
        MAP_IntUnregister(INT_NWPIC);
        MAP_IntPendClear(INT_NWPIC);
    }
    else
    {
        MAP_IntRegister(INT_NWPIC, fInterruptHdl);
        MAP_IntPrioritySet(INT_NWPIC, INT_PRIORITY_LVL_1);
        MAP_IntPendClear(INT_NWPIC);
        MAP_IntEnable(INT_NWPIC);
    }

    return 0 ;
#endif //SL_PLATFORM_MULTI_THREADED
}

/*!
    \brief  This function De registers an interrupt in NVIC table

    \param  iIntrNum    -   Interrupt number to register

    \return none
    \note
    \warning
*/
void osi_InterruptDeRegister(int iIntrNum)
{
    osiIntContext_t *pIntContext, *pPrev;

    //OSI_HEADER_NL();
    for(pIntContext=g_osiInterrupts;
            pIntContext && pIntContext->intNum!=iIntrNum;
            pIntContext=pIntContext->pNext)
        pPrev = pIntContext;

    if(pIntContext)
    {
        HwiP_delete(pIntContext->intHandle);
        if(pPrev)
            pPrev->pNext = pIntContext->pNext;
        else if(pIntContext == g_osiInterrupts)
            g_osiInterrupts = NULL;
        free(pIntContext);
    }
}


int osi_ThreadCreate(void *(*pEntry)(void *),unsigned short stackDepth,void *pvParameters,unsigned long uxPriority,pthread_t *pThread)
{
    int ret;
    pthread_attr_t attr;
    struct sched_param priParam;
    int detachState;
    ret = pthread_attr_init(&attr);
    priParam.sched_priority = uxPriority;
    ret |= pthread_attr_setschedparam(&attr, &priParam);
    detachState = PTHREAD_CREATE_DETACHED;
    ret |= pthread_attr_setdetachstate(&attr, detachState);
    ret |= pthread_attr_setstacksize(&attr, stackDepth);
    if (ret == 0)
        ret = pthread_create(pThread, &attr, pEntry, pvParameters);
    return ret;
}


typedef struct
{
    mqd_t           mq;
    unsigned long   MsgSize;
} osiMSGQContext_t;

/*!
    \brief  This function used to create a message queue
    \param  pMsgQ       - pointer to the message queue
    \param  pMsgQName   - Name of the Message queue
    \param  MsgSize     - Size of msg on the message queue
    \param  MaxMsgs     - Max number of msgs on the queue
    \return - OsiReturnVal_e
    \note
    \warning
*/
OsiReturnVal_e osi_MsgQCreate(OsiMsgQ_t     *pMsgQ,
                              char          *pMsgQName,
                              unsigned long  MsgSize,
                              unsigned long  MaxMsgs)
{
    int ret = OSI_FAILURE;
    mq_attr attr;
    osiMSGQContext_t *pMqContext;

    attr.mq_maxmsg = MaxMsgs;         // queue size
    attr.mq_msgsize = MsgSize;      // Size of message

    pMqContext = (osiMSGQContext_t*) malloc (sizeof (osiMSGQContext_t));
    if(pMqContext)
    {
        pMqContext->mq = mq_open(pMsgQName, O_CREAT, 0, &attr);
        //OSI_PRINT(("%p->%p %d %d\r\n",  pMqContext, pMqContext->mq, MsgSize, MaxMsgs));
        if(pMqContext->mq == 0)
        {
            free (pMqContext);
        }
        else
        {
            pMqContext->MsgSize = MsgSize;
            *pMsgQ = (void *)pMqContext;
            ret = _OSI_OK;
        }
    }
    return (OsiReturnVal_e)ret;
}
/*!
    \brief  This function used to delete a message queue
    \param  pMsgQ   -   pointer to the msg queue
    \return - OsiReturnVal_e
    \note
    \warning
*/
OsiReturnVal_e osi_MsgQDelete(OsiMsgQ_t *pMsgQ)
{
    osiMSGQContext_t *pMqContext = (osiMSGQContext_t*)*pMsgQ;
    //OSI_HEADER_NL();
    mq_close(pMqContext->mq);
    free(pMqContext);

    return _OSI_OK;
}

/*!
    \brief  This function is used to write data to the MsgQ

    \param  pMsgQ   -   pointer to the message queue
    \param  pMsg    -   pointer to the Msg strut to read into
    \param  Timeout -   timeout to wait for the Msg to be available

    \return - OsiReturnVal_e
    \note
    \warning
*/
OsiReturnVal_e osi_MsgQWrite(OsiMsgQ_t *pMsgQ, void *pMsg, OsiTime_t Timeout)
{
    osiMSGQContext_t *pMqContext = (osiMSGQContext_t*)*pMsgQ;
    int ret;

    //OSI_PRINT(("%p\r\n",  pMqContext));
    //
    //signal provisioning task about SL Event
    //
    if(Timeout == OSI_WAIT_FOREVER)
    {
        ret = mq_send(pMqContext->mq, (char *)pMsg, pMqContext->MsgSize, 0);

    }
    else
    {
        struct  timespec tm;
        st_GetAbsoluteTime(Timeout, &tm);
        ret = mq_timedsend(pMqContext->mq, (char *)pMsg, pMqContext->MsgSize, 0, &tm);
    }
    if(ret < 0)
        ret = OSI_FAILURE;
    else
        ret = _OSI_OK;

    return (OsiReturnVal_e)ret;
}

/*!
    \brief  This function is used to read data from the MsgQ

    \param  pMsgQ   -   pointer to the message queue
    \param  pMsg    -   pointer to the Msg strut to read into
    \param  Timeout -   timeout to wait for the Msg to be available

    \return - OsiReturnVal_e
    \note
    \warning
*/
OsiReturnVal_e osi_MsgQRead(OsiMsgQ_t* pMsgQ, void* pMsg , OsiTime_t Timeout)
{
    osiMSGQContext_t *pMqContext = (osiMSGQContext_t*)*pMsgQ;
    int ret;
    unsigned int prio;


    //OSI_PRINT(("%p\r\n",  pMqContext));
    //
    //signal provisioning task about SL Event
    //
    if(Timeout == OSI_WAIT_FOREVER)
    {
        ret = mq_receive(pMqContext->mq, (char *)pMsg, pMqContext->MsgSize, &prio);
    }
    else
    {
        struct  timespec tm;
        st_GetAbsoluteTime(Timeout, &tm);
        ret = mq_timedreceive(pMqContext->mq, (char *)pMsg, pMqContext->MsgSize, &prio, &tm);
    }
    if(ret == ETIMEDOUT)
        ret = OSI_TIMEOUT;
    else if (ret < 0)
        ret = OSI_FAILURE;
    else
        ret = _OSI_OK;

    return (OsiReturnVal_e)ret;
}

