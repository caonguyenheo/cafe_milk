/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,

 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== main_tirtos.c ========
 */
#include <stdint.h>

/* POSIX Header files */
#include <pthread.h>

/* RTOS header files */
#include <ti/sysbios/BIOS.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>

#include <ti/drivers/SPI.h>

//#include <ti/display/Display.h>
/* Example/Board Header files */
#include "Board.h"
#include "uart_terminal.h"
#include "main_thread.h"
#include "spi_handler.h"


extern void * mainThread1(void *arg0);
extern void * ShellThread(void *param);
extern void * TcpThread(void *param);
extern void * spiSlaveHandleTask(void * param);
extern void *receive(void * param);
extern  void *client_group2(void * param);
extern void *SlaveHandleTask(void *param);


#define TASK_STACK_SIZE         (1024)


/*
//#define TASK_PRIORITY_MAINTHREAD		(15)
#define SPAWN_TASK_PRIORITY				(14)
#define TASK_PRIORITY_TCP				(13)
#define TASK_PRIORITY_RECEIVE			(10)
#define TASK_PRIORITY_CLIENTGROUP		(10)
#define TASK_PRIORITY_SPI_SLAVE			(11)
#define TASK_PRIORITY_CLI				(2)
*/

#define SPAWN_TASK_PRIORITY				(2)
#define TASK_PRIORITY_TCP				(8)
#define TASK_PRIORITY_SPI_SLAVE			(4)
#define TASK_PRIORITY_CLI				(1)



#define ENABLE_CLI_TASK     			(1)
#define ENABLE_TCP_TASK          		(1)
#define ENABLE_SPI_TASK					(1)
#define ENABLE_MAIN_THREAD_TASK			(0)
#define INIT_HARDWARE					(0)
#define ENABLE_CLIENTGROUP				(0)
#define ENABLE_SL_TASK					(1)
#define ENABLE_RECEIVE					(0)



/* Stack size in bytes */
#define THREADSTACKSIZE     1024
sem_t guserconfig_init;
sem_t slave;
//sem_t masterSem;
sem_t clientsem;

pthread_mutex_t clientMutex;

/*pthread_t thread = (pthread_t)NULL;
pthread_t thread_tcp = (pthread_t)NULL;
pthread_t thread_mainthread = (pthread_t)NULL;
pthread_t thread_spi = (pthread_t)NULL;
pthread_t thread_cli = (pthread_t)NULL;
pthread_t thread_receivetcp = (pthread_t)NULL;
pthread_t spawnThread = (pthread_t)NULL;*/

pthread_t thread_tcp = (pthread_t)NULL;
pthread_t thread_spi = (pthread_t)NULL;
pthread_t thread_cli = (pthread_t)NULL;
pthread_t spawnThread = (pthread_t)NULL;
/*
 *  ======== main ========
 */
int main(void)
{

    pthread_attr_t	pAttrs;
    pthread_attr_t	attrs_spawn;

    struct	sched_param priParam;
    int	retc;
    struct timespec     ts = {0};
//    int 				detachState;
//    void *thread_result;


    /* Call board init functions */
    Board_initGeneral();
    InitTerm();
    GPIO_init();
    SPI_init();
//    PinMuxConfig();
    /* Init Application variables */
//    RetVal = initAppVariables();

    /* Init Terminal UART */


    /* initialize the realtime clock */
    clock_settime(CLOCK_REALTIME, &ts);

    /* Switch off all LEDs on boards */
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_OFF);
    GPIO_write(Board_GPIO_LED1, Board_GPIO_LED_OFF);
    GPIO_write(Board_GPIO_LED2, Board_GPIO_LED_OFF);

    retc = sem_init(&guserconfig_init, 0, 0);
    if (retc < 0)
    {
        while (1);
    }
//    retc = sem_init(&clientsem, 0, 0);
//    if (retc < 0)
//    {
//        while (1);
//    }


    retc = sem_init(&slave, 0, 0);

    if (retc != 0)
    {
    	UART_PRINT("Error creating slaveSem line [%d] function [%d]\n\r",__LINE__, __FUNCTION__);

        while(1);
    }

/*    retc = sem_init(&masterSem, 0, 0);
    if (retc != 0)
    {
    	UART_PRINT("Error creating masterSem line [%d] function [%d]\n\r",__LINE__, __FUNCTION__);
        while(1);
    }*/
    retc = pthread_mutex_init(&clientMutex, NULL);
    if (retc != 0)
    {
        /* pthread_mutex_init() failed */
    	UART_PRINT("Unable to init pthread_mutex_init\n\r");
        while (1);
    }
    /* Set priority and stack size attributes */
    pthread_attr_init(&pAttrs);
    pthread_attr_init(&attrs_spawn);
#if 0
    priParam.sched_priority = 1;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&pAttrs, detachState);
    if(retc != 0)
    {
        /* pthread_attr_setdetachstate() failed */
        while(1)
        {
            ;
        }
    }

    pthread_attr_setschedparam(&pAttrs, &priParam);

    retc |= pthread_attr_setstacksize(&pAttrs, THREADSTACKSIZE);
    if(retc != 0)
    {
        /* pthread_attr_setstacksize() failed */
        while(1)
        {
            ;
        }
    }
#endif

    /* create the sl_Task */


#if (ENABLE_MAIN_THREAD_TASK)

    priParam.sched_priority = TASK_PRIORITY_MAINTHREAD;
    retc = pthread_attr_setschedparam(&pAttrs, &priParam);
//    detachState = 1;
//    retc |= pthread_attr_setdetachstate(&pAttrs, detachState);
    retc |= pthread_attr_setstacksize(&pAttrs, TASK_STACK_SIZE * 3);

    if(retc != 0)
    {
        /* error handling */
        while(1)
        {
            UART_PRINT("Unable to create mainThread1 thread \n");
            return(NULL);
        }
    }
	retc = pthread_create(&thread_mainthread, &pAttrs, mainThread1, NULL);
	if(retc != 0)
	{
		/* pthread_create() failed */
        UART_PRINT("Unable to create Task mainThread1\n\r");
		while(1)
		{
			;
		}
	}

#endif

#if (ENABLE_SL_TASK)

    priParam.sched_priority = SPAWN_TASK_PRIORITY;
    retc = pthread_attr_setschedparam(&attrs_spawn, &priParam);
    retc |= pthread_attr_setstacksize(&attrs_spawn, TASK_STACK_SIZE * 2);

    retc = pthread_create(&spawnThread, &attrs_spawn, sl_Task, NULL);
    if(retc != 0)
    {
        /* Handle Error */
        UART_PRINT("Unable to create sl_Task thread\r\n");
        while(1);
    }

#endif


#if INIT_HARDWARE
    struct timespec     ts = {0};

    SPI_init();
    GPIO_init();
    InitTerm();
    clock_settime(CLOCK_REALTIME, &ts);
    /* Switch off all LEDs on boards */
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_OFF);
    GPIO_write(Board_GPIO_LED1, Board_GPIO_LED_OFF);
    GPIO_write(Board_GPIO_LED2, Board_GPIO_LED_OFF);
#endif

#if (ENABLE_SPI_TASK)

    priParam.sched_priority = TASK_PRIORITY_SPI_SLAVE;
    retc = pthread_attr_setschedparam(&pAttrs, &priParam);
    retc |= pthread_attr_setstacksize(&pAttrs, TASK_STACK_SIZE * 5);

    if(retc != 0)
    {
        /* error handling */
        while(1)
        {
            UART_PRINT("Unable to create SPI thread \n");
            return(NULL);
        }
    }

//    retc = pthread_create(&thread_spi, &pAttrs, spiSlaveHandleTask, NULL);
    retc = pthread_create(&thread_spi, &pAttrs, SlaveHandleTask, NULL);

    if(retc != 0)
    {
    	UART_PRINT("Unable to create Task spiSlaveHandleTask\n\r");
        while(1)
        {
            ;
        }
    }

#endif


#if (ENABLE_TCP_TASK)
    //create thread TCP
//    pthread_attr_init(&pAttrs_tcp);
    priParam.sched_priority = TASK_PRIORITY_TCP;
    retc = pthread_attr_setschedparam(&pAttrs, &priParam);
    retc |= pthread_attr_setstacksize(&pAttrs, TASK_STACK_SIZE * 5);

    if(retc != 0)
    {
        /* error handling */
        while(1)
        {
            UART_PRINT("Unable to create TCP thread \n");
            return(NULL);
        }
    }

    retc = pthread_create(&thread_tcp, &pAttrs, TcpThread, NULL);

    if(retc != 0)
    {
    	UART_PRINT("Unable to create Task TcpThread\n\r");
        while(1)
        {
            ;
        }
    }
#endif

#if (ENABLE_CLIENTGROUP)

    priParam.sched_priority = TASK_PRIORITY_CLIENTGROUP;
    retc = pthread_attr_setschedparam(&pAttrs, &priParam);
    retc |= pthread_attr_setstacksize(&pAttrs, TASK_STACK_SIZE * 3);

    if(retc != 0)
    {
         //error handling
        while(1)
        {
            UART_PRINT("Unable to create CLIENTGROUP thread \n");
            return(NULL);
        }
    }

    retc = pthread_create(&thread_receivetcp, &pAttrs, client_group2, NULL);

    if(retc != 0)
    {
    	UART_PRINT("Unable to create Task client_group2\n\r");
        while(1)
        {
            ;
        }
    }
#endif

#if (ENABLE_RECEIVE)

    priParam.sched_priority = TASK_PRIORITY_RECEIVE;
    retc = pthread_attr_setschedparam(&pAttrs, &priParam);
    retc |= pthread_attr_setstacksize(&pAttrs, TASK_STACK_SIZE * 2);

    if(retc != 0)
    {
         //error handling
        while(1)
        {
            UART_PRINT("Unable to create RECEIVE thread \n");
            return(NULL);
        }
    }

    retc = pthread_create(&thread_receivetcp, &pAttrs, receive, NULL);

    if(retc != 0)
    {
    	UART_PRINT("Unable to create Task receive\n\r");
        while(1)
        {
            ;
        }
    }
#endif
//#if (ENABLE_SPI_TASK)
//
//    priParam.sched_priority = TASK_PRIORITY_SPI_SLAVE;
//    retc = pthread_attr_setschedparam(&pAttrs, &priParam);
//    retc |= pthread_attr_setstacksize(&pAttrs, TASK_STACK_SIZE * 7);
//
//    if(retc != 0)
//    {
//        /* error handling */
//        while(1)
//        {
//            UART_PRINT("Unable to create SPI thread \n");
//            return(NULL);
//        }
//    }
//
////    retc = pthread_create(&thread_spi, &pAttrs, spiSlaveHandleTask, NULL);
//    retc = pthread_create(&thread_spi, &pAttrs, SlaveHandleTask, NULL);
//
//    if(retc != 0)
//    {
//    	UART_PRINT("Unable to create Task spiSlaveHandleTask\n\r");
//        while(1)
//        {
//            ;
//        }
//    }
//
//#endif

#if (ENABLE_CLI_TASK)
    //create thread shell
//    pthread_attr_init(&pAttrs_shell);
    priParam.sched_priority = TASK_PRIORITY_CLI;
    retc = pthread_attr_setschedparam(&pAttrs, &priParam);
    retc |= pthread_attr_setstacksize(&pAttrs, TASK_STACK_SIZE * 3);

    if(retc != 0)
    {
        /* error handling */
        while(1)
        {
            UART_PRINT("Unable to create Shell thread \n");
            return(NULL);
        }
    }

    retc = pthread_create(&thread_cli, &pAttrs, ShellThread, NULL);

    if(retc != 0)
    {
    	UART_PRINT("Unable to create Task ShellThread\n\r");
        while(1)
        {
            ;
        }
    }

#endif







    BIOS_start();

    return (0);
}

/*
 *  ======== dummyOutput ========
 *  Dummy SysMin output function needed for benchmarks and size comparison
 *  of FreeRTOS and TI-RTOS solutions.
 */
void dummyOutput(void)
{
}
