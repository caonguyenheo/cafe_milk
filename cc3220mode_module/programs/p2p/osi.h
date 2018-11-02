/*
 * osi.h
 *
 *  Created on: Aug 29, 2018
 *      Author: Admin
 */

#ifndef OSI_H_
#define OSI_H_

#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <ti/sysbios/BIOS.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define OSI_WAIT_FOREVER            (0xFFFFFFFF)

#define OSI_NO_WAIT                 (0)

#ifndef SPAWN_TASK_STACK
#define STACK_LEN          (2048) /*Stack Size*/
#else
#define STACK_LEN          (SPAWN_TASK_STACK)
#endif


#define _OSI_OK                          0
#define OSI_FAILURE                     SL_ERROR_BSD_SOC_ERROR
#define OSI_OPERATION_FAILED            SL_ERROR_BSD_SOC_ERROR
#define OSI_ABORTED                     SL_ERROR_BSD_SOC_ERROR
#define OSI_INVALID_PARAMS              SL_ERROR_BSD_EINVAL
#define OSI_MEMORY_ALLOCATION_FAILURE   SL_ERROR_BSD_ENOMEM
#define OSI_TIMEOUT                     SL_ERROR_BSD_ETIMEDOUT
#define OSI_EVENTS_IN_USE               -16 //SL_ERROR_BSD_EBUSY
#define OSI_EVENT_OPEARTION_FAILURE     SL_ERROR_BSD_SOC_ERROR
#define ETIMEDOUT 					    116

typedef int OsiReturnVal_e;


//#define ENTER_CRITICAL_SECTION            osi_EnterCritical()
//#define EXIT_CRITICAL_SECTION         osi_ExitCritical()

typedef void* OsiMsgQ_t;

 /*!
    \brief type definition for a time value

    \note   On each porting or platform the type could be whatever is needed - integer, pointer to structure etc.
*/
//typedef unsigned int OsiTime_t;
typedef unsigned int OsiTime_t;
/*!
    \brief  type definition for a sync object container

    Sync object is object used to synchronize between two threads or thread and interrupt handler.
    One thread is waiting on the object and the other thread send a signal, which then
    release the waiting thread.
    The signal must be able to be sent from interrupt context.
    This object is generally implemented by binary semaphore or events.

    \note   On each porting or platform the type could be whatever is needed - integer, structure etc.
*/
//typedef unsigned int OsiSyncObj_t;
typedef sem_t OsiSyncObj_t;

/*!
    \brief  type definition for a locking object container

    Locking object are used to protect a resource from mutual accesses of two or more threads.
    The locking object should support re-entrant locks by a signal thread.
    This object is generally implemented by mutex semaphore

    \note   On each porting or platform the type could be whatever is needed - integer, structure etc.
*/
//typedef unsigned int OsiLockObj_t;
typedef pthread_mutex_t OsiLockObj_t;

/*!
    \brief  type definition for a spawn entry callback

    the spawn mechanism enable to run a function on different context.
    This mechanism allow to transfer the execution context from interrupt context to thread context
    or changing the context from an unknown user thread to general context.
    The implementation of the spawn mechanism depends on the user's system requirements and could varies
    from implementation of serialized execution using single thread to creating thread per call

    \note   The stack size of the execution thread must be at least of TBD bytes!
*/
typedef short (*P_OSI_SPAWN_ENTRY)(void* pValue);

typedef void (*P_OSI_EVENT_HANDLER)(void* pValue);

typedef void* (*P_OSI_TASK_ENTRY)(void* pValue);

typedef void (*P_OSI_INTR_ENTRY)(void);

typedef pthread_t OsiTaskHandle;

int osi_ThreadCreate(void *(*pEntry)(void *),unsigned short stackDepth,void *pvParameters,unsigned long uxPriority,pthread_t *pThread);

/*!
    \brief  This function registers an interrupt in NVIC table

    The sync object is used for synchronization between different thread or ISR and
    a thread.

    \param  iIntrNum    -   Interrupt number to register
    \param  pEntry      -   Pointer to the interrupt handler

    \return upon successful creation the function should return 0
            Otherwise, a negative value indicating the error code shall be returned
    \note
    \warning
*/
OsiReturnVal_e osi_InterruptRegister(int iIntrNum,P_OSI_INTR_ENTRY pEntry,unsigned char ucPriority);

/*!
    \brief  This function De-registers an interrupt in NVIC table

    \param  iIntrNum    -   Interrupt number to register
    \param  pEntry      -   Pointer to the interrupt handler

    \return upon successful creation the function should return Positive number
            Otherwise, a negative value indicating the error code shall be returned
    \note
    \warning
*/
void osi_InterruptDeRegister(int iIntrNum);


/*!
    \brief  This function creates a sync object

    The sync object is used for synchronization between different thread or ISR and
    a thread.

    \param  pSyncObj    -   pointer to the sync object control block

    \return upon successful creation the function should return 0
            Otherwise, a negative value indicating the error code shall be returned
    \note
    \warning
*/
//OsiReturnVal_e osi_SyncObjCreate(OsiSyncObj_t* pSyncObj);
#define osi_SyncObjCreate(pSyncObj)             sem_init(pSyncObj, 0 , 0)

/*!
    \brief  This function deletes a sync object

    \param  pSyncObj    -   pointer to the sync object control block

    \return upon successful deletion the function should return 0
            Otherwise, a negative value indicating the error code shall be returned
    \note
    \warning
*/
//OsiReturnVal_e osi_SyncObjDelete(OsiSyncObj_t* pSyncObj);
#define osi_SyncObjDelete(pSyncObj)             sem_destroy(pSyncObj)

/*!
    \brief      This function generates a sync signal for the object.

    All suspended threads waiting on this sync object are resumed

    \param      pSyncObj    -   pointer to the sync object control block

    \return     upon successful signalling the function should return 0
                Otherwise, a negative value indicating the error code shall be returned
    \note       the function could be called from ISR context
    \warning
*/
//OsiReturnVal_e osi_SyncObjSignal(OsiSyncObj_t* pSyncObj);
#define osi_SyncObjSignal(pSyncObj)             sem_post(pSyncObj)

/*!
    \brief      This function generates a sync signal for the object.
                from ISR context.

    All suspended threads waiting on this sync object are resumed

    \param      pSyncObj    -   pointer to the sync object control block

    \return     upon successful signalling the function should return 0
                Otherwise, a negative value indicating the error code shall be returned
    \note       the function is called from ISR context
    \warning
*/
//OsiReturnVal_e osi_SyncObjSignalFromISR(OsiSyncObj_t* pSyncObj);
#define osi_SyncObjSignalFromISR(pSyncObj)      sem_post(pSyncObj)

/*!
    \brief  This function waits for a sync signal of the specific sync object

    \param  pSyncObj    -   pointer to the sync object control block
    \param  Timeout     -   numeric value specifies the maximum number of mSec to
                            stay suspended while waiting for the sync signal
                            Currently, the simple link driver uses only two values:
                                - OSI_WAIT_FOREVER
                                - OSI_NO_WAIT

    \return upon successful reception of the signal within the timeout window return 0
            Otherwise, a negative value indicating the error code shall be returned
    \note
    \warning
*/
//OsiReturnVal_e osi_SyncObjWait(OsiSyncObj_t* pSyncObj , OsiTime_t Timeout);
#define osi_SyncObjWait(pSyncObj, Timeout)      sem_wait_timed(pSyncObj, Timeout)

/*!
    \brief  This function clears a sync object

    \param  pSyncObj    -   pointer to the sync object control block

    \return upon successful clearing the function should return 0
            Otherwise, a negative value indicating the error code shall be returned
    \note
    \warning
*/
//OsiReturnVal_e osi_SyncObjClear(OsiSyncObj_t* pSyncObj);
#define osi_SyncObjClear(pSyncObj)              sem_wait_timed(pSyncObj, 0)

/*!
    \brief  This function creates a locking object.

    The locking object is used for protecting a shared resources between different
    threads.

    \param  pLockObj    -   pointer to the locking object control block

    \return upon successful creation the function should return 0
            Otherwise, a negative value indicating the error code shall be returned
    \note
    \warning
*/
//OsiReturnVal_e osi_LockObjCreate(OsiLockObj_t* pLockObj);
#define osi_LockObjCreate(pLockObj)         pthread_mutex_init(pLockObj, NULL)

/*!
    \brief  This function deletes a locking object.

    \param  pLockObj    -   pointer to the locking object control block

    \return upon successful deletion the function should return 0
            Otherwise, a negative value indicating the error code shall be returned
    \note
    \warning
*/
//OsiReturnVal_e osi_LockObjDelete(OsiLockObj_t* pLockObj);
#define osi_LockObjDelete(pLockObj)         pthread_mutex_destroy(pLockObj, NULL)

/*!
    \brief  This function locks a locking object.

    All other threads that call this function before this thread calls
    the osi_LockObjUnlock would be suspended

    \param  pLockObj    -   pointer to the locking object control block
    \param  Timeout     -   numeric value specifies the maximum number of mSec to
                            stay suspended while waiting for the locking object
                            Currently, the simple link driver uses only two values:
                                - OSI_WAIT_FOREVER
                                - OSI_NO_WAIT


    \return upon successful reception of the locking object the function should return 0
            Otherwise, a negative value indicating the error code shall be returned
    \note
    \warning
*/
//OsiReturnVal_e osi_LockObjLock(OsiLockObj_t* pLockObj , OsiTime_t Timeout);
#define osi_LockObjLock(pLockObj, Timeout)      mutex_lock_timed(pLockObj, Timeout)

/*!
    \brief  This function unlock a locking object.

    \param  pLockObj    -   pointer to the locking object control block

    \return upon successful unlocking the function should return 0
            Otherwise, a negative value indicating the error code shall be returned
    \note
    \warning
*/
//OsiReturnVal_e osi_LockObjUnlock(OsiLockObj_t* pLockObj);
#define osi_LockObjUnlock(pLockObj)             pthread_mutex_unlock(pLockObj)


/*!
    \brief  This function call the pEntry callback from a different context

    \param  pEntry      -   pointer to the entry callback function

    \param  pValue      -   pointer to any type of memory structure that would be
                            passed to pEntry callback from the execution thread.

    \param  flags       -   execution flags - reserved for future usage

    \return upon successful registration of the spawn the function should return 0
            (the function is not blocked till the end of the execution of the function
            and could be returned before the execution is actually completed)
            Otherwise, a negative value indicating the error code shall be returned
    \note
    \warning
*/
/*!
    \brief  This function creates a Task.

    Creates a new Task and add it to the last of tasks that are ready to run

    \param  pEntry  -   pointer to the Task Function
    \param  pcName  -   Task Name String
    \param  usStackDepth    -   Stack Size Stack Size in 32-bit long words
    \param  pvParameters    -   pointer to structure to be passed to the Task Function
    \param  uxPriority  -   Task Priority

    \return upon successful unlocking the function should return 0
            Otherwise, a negative value indicating the error code shall be returned
    \note
    \warning
*/


//int osi_TaskCreate(P_OSI_TASK_ENTRY pEntry,const signed char * const pcName,unsigned short usStackDepth,void *pvParameters,unsigned long uxPriority,OsiTaskHandle *pTaskHandle);
#define osi_TaskCreate(pEntry, pcName, usStackDepth, pvParameters, uxPriority, pTaskHandle) \
    osi_ThreadCreate(pEntry, usStackDepth, pvParameters, uxPriority, pTaskHandle)


/*!
    \brief  This function Deletes a Task.

    Deletes a  Task and remove it from list of running task

    \param  pTaskHandle -   Task Handle

    \note
    \warning
*/
//void osi_TaskDelete(OsiTaskHandle* pTaskHandle);
#define osi_TaskDelete(pTaskHandle)         pthread_detach(pTaskHandle)

/*!
    \brief  This function used to disable the tasks
    \param  - void
    \return - Key with the suspended tasks
    \note
    \warning
*/
//unsigned long osi_TaskDisable(void);
#define osi_TaskDisable()               SL_ERROR_BSD_EOPNOTSUPP // NOT SUPPORTED
/*!
    \brief  This function used to enable all tasks
    \param  unsigned long
    \return - void
    \note
    \warning
*/
//void osi_TaskEnable(unsigned long);
#define osi_TaskEnable()                SL_ERROR_BSD_EOPNOTSUPP // NOT SUPPORTED


/*!
    \brief  This function call the pEntry callback from a different context

    \param  pEntry      -   pointer to the entry callback function

    \param  pValue      -   pointer to any type of memory structure that would be
                            passed to pEntry callback from the execution thread.

    \param  flags       -   execution flags - reserved for future usage

    \return upon successful registration of the spawn the function should return 0
            (the function is not blocked till the end of the execution of the function
            and could be returned before the execution is actually completed)
            Otherwise, a negative value indicating the error code shall be returned
    \note
    \warning
*/
//OsiReturnVal_e osi_Spawn(P_OSI_SPAWN_ENTRY pEntry , void* pValue , unsigned long flags);
#define osi_Spawn(pEntry, pValue, flags)

/*******************************************************************************

This function creates a message queue that is typically used for inter thread
communication.

Parameters:

    pMsgQ       -   pointer to the message queue control block
    pMsgQName   -   pointer to the name of the message queue
    MsgSize     -   the size of the message.

            NOTICE: THE MESSGAE SIZE MUST BE SMALLER THAN 16

    MaxMsgs     -   maximum number of messages.

Please note that this function allocates the entire memory required
for the maximum number of messages (MsgSize * MaxMsgs).

********************************************************************************/
OsiReturnVal_e osi_MsgQCreate(OsiMsgQ_t*        pMsgQ ,
                              char*             pMsgQName,
                              unsigned long         MsgSize,
                              unsigned long         MaxMsgs);

/*******************************************************************************

This function deletes a specific message queue.
All threads suspended waiting for a message from this queue are resumed with
an error return value.

Parameters:

    pMsgQ       -   pointer to the message queue control block

********************************************************************************/
OsiReturnVal_e osi_MsgQDelete(OsiMsgQ_t* pMsgQ);


/*******************************************************************************

This function writes a message to a specific message queue.

Notice that the message is copied to the queue from the memory area specified
by pMsg pointer.

--------------------------------------------------------------------------------
THIS FUNCTION COULD BE CALLED FROM ISR AS LONG AS THE TIMEOUT PARAMETER IS
SET TO "OSI_NO_WAIT"
--------------------------------------------------------------------------------

Parameters:

    pMsgQ       -   pointer to the message queue control block
    pMsg        -   pointer to the message
    Timeout     -   numeric value specifies the maximum number of mSec to stay
                    suspended while waiting for available space for the message

********************************************************************************/
OsiReturnVal_e osi_MsgQWrite(OsiMsgQ_t* pMsgQ, void* pMsg , OsiTime_t Timeout);


/*******************************************************************************

This function retrieves a message from the specified message queue. The
retrieved message is copied from the queue into the memory area specified by
the pMsg pointer

Parameters:

    pMsgQ       -   pointer to the message queue control block
    pMsg        -   pointer that specify the location where to copy the message
    Timeout     -   numeric value specifies the maximum number of mSec to stay
                    suspended while waiting for a message to be available

********************************************************************************/
OsiReturnVal_e osi_MsgQRead(OsiMsgQ_t* pMsgQ, void* pMsg , OsiTime_t Timeout);

/*!
    \brief  This function starts the OS Scheduler
    \param  - void
    \return - void
    \note
    \warning
*/
//void osi_start();
#ifdef USE_FREERTOS
                                    /* Start the FreeRTOS scheduler */
#define osi_start()                 vTaskStartScheduler();
#else
#define osi_start()                 BIOS_start()
#endif


/*!
    \brief                  Allocates Memory on Heap
    \param  Size        -   Size of the Buffer to be allocated
    \sa
    \note
    \warning
*/
//void * mem_Malloc(unsigned long Size);
#define mem_Malloc(Size)        malloc(Size)

/*!
    \brief              Deallocates Memory
    \param  pMem        -   Pointer to the Buffer to be freed
    \return void
    \sa
    \note
    \warning
*/
//void mem_Free(void *pMem);
#define mem_Free(pMem)              free(pMem)


/*!
    \brief              Set Memory
    \param  pBuf        -   Pointer to the Buffer
    \param  Val         -   Value to be set
    \param  Size        -   Size of the memory to be set
    \sa
    \note
    \warning
*/
//void  mem_set(void *pBuf,int Val, int Size);
#define  mem_set(pBuf, Val, Size)           memset(pBuf, Val, Size)

/*!
    \brief              Copy Memory
    \param  pDst        -   Pointer to the Destination Buffer
    \param  pSrc        -   Pointer to the Source Buffer
    \param  Size        -   Size of the memory to be copied
    \return void
    \note
    \warning
*/
//void  mem_copy(void *pDst, void *pSrc, int Size);
#define mem_copy(pDst, pSrc, Size)          memcpy(pDst, pSrc, Size)
/*!
    \brief          Enter Critical Section
    \sa
    \note
    \warning
*/
//unsigned long osi_EnterCritical(void);
#define osi_EnterCritical()                 HwiP_disable()


/*!
    \brief          Exit Critical Section
    \sa
    \note
    \warning
*/
//void osi_ExitCritical(unsigned long ulKey);
#define osi_ExitCritical(ulKey)             HwiP_restore(ulKey)

/*!
    \brief  This function used to save the os context before sleep
    \param  void
    \return void
    \note
    \warning
*/
//void osi_ContextSave();
#define osi_ContextSave()               SL_ERROR_BSD_EOPNOTSUPP // NOT SUPPORTED
/*!
    \brief  This function used to retrieve the context after sleep
    \param  void
    \return void
    \note
    \warning
*/
//void osi_ContextRestore();
#define osi_ContextRestore()            SL_ERROR_BSD_EOPNOTSUPP // NOT SUPPORTED

/*!
    \brief  This function used to suspend the task for the specified number of milli secs
    \param  MilliSecs   -   Time in millisecs to suspend the task
    \return void
    \note
    \warning
*/
//void osi_Sleep(unsigned int MilliSecs);
#define osi_Sleep(MilliSecs)    {if(MilliSecs >= 1000) { sleep((MilliSecs+500)/1000); } else { usleep(MilliSecs * 1000); } }




/* API for SL Task*/
//OsiReturnVal_e VStartSimpleLinkSpawnTask(unsigned long uxPriority);
extern pthread_t slSpawnThread;
#define SPAWN_TASK_STACK_SIZE       2048
#define VStartSimpleLinkSpawnTask(priority)     osi_ThreadCreate(sl_Task, SPAWN_TASK_STACK_SIZE, NULL, priority, &slSpawnThread)


//void VDeleteSimpleLinkSpawnTask( void );
#define VDeleteSimpleLinkSpawnTask()            pthread_detach(&slSpawnThread)


#ifdef  __cplusplus
}
#endif // __cplusplus


#endif /* OSI_H_ */
