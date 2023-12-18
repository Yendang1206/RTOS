/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*					WIN32 PORT & LINUX PORT
*                          (c) Copyright 2004, Werner.Zimmermann@fht-esslingen.de
*                 (Similar to Example 1 of the 80x86 Real Mode port by Jean J. Labrosse)
*                                           All Rights Reserved
** *****************************************************************************************************
*		Further modified by mikael.jakas@puv.fi, jukka.matila@vamk.fi
* *****************************************************************************************************
*                                               EXAMPLE #1
*********************************************************************************************************
*/

#include "includes.h"


/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE                 512       /* Size of each task's stacks (# of WORDs)            */
#define  N_TASKS                        4       /* Number of identical tasks                          */


/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
OS_STK        TaskStartStk[TASK_STK_SIZE];

OS_EVENT *SharedMemSem;

INT16U  counter=1;
INT16U  check_1;
INT16U  check_2;

INT8U counter1Str[20];
INT8U counter2Str[20];


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void TaskStart(void* pdata);        /* Function prototype of Startup task           */
void Player_1(void* data);     /* Function prototype of Player_1 task     */
void Player_2(void* data);     /* Function prototype of Player_2 task     */
void GameMaster(void* data);         /* Function prototype of  GameMaster   */

/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/

int  main(void)
{
    PC_DispClrScr(DISP_FGND_BLUE + DISP_BGND_BLUE);      /* Clear the screen                         */

    OSInit();                                              /* Initialize uC/OS-II                      */

    SharedMemSem = OSSemCreate(1);
    OSTaskCreate(TaskStart, (void*)0, &TaskStartStk[TASK_STK_SIZE - 1], 3);

    OSStart();                                             /* Start multitasking                       */

    return 0;
}


/*
*********************************************************************************************************
*                                              STARTUP TASK
*********************************************************************************************************
*/
void  TaskStart(void* pdata)
{
    INT16U key;
    INT8U  err;

    pdata = pdata;                                         /* Prevent compiler warning                 */

    OSTaskCreate(Player_1, (void*)counter1Str, &TaskStk[0][TASK_STK_SIZE - 1], 0);
    OSTaskCreate(Player_2, (void*)counter2Str, &TaskStk[1][TASK_STK_SIZE - 1], 1);
    OSTaskCreate(GameMaster, (void*)0, &TaskStk[2][TASK_STK_SIZE - 1], 2);

    for (;;) {
        if (PC_GetKey(&key) == TRUE) {                     /* See if key has been pressed              */
            if (key == 0x1B) {                             /* Yes, see if it's the ESCAPE key          */
                exit(0);  	                           /* End program                              */
            }
        }
        OSTimeDlyHMSM(0, 0, 1, 0);                         /* Wait one second                          */
    }

}

/*
*********************************************************************************************************
*                                                  TASKS
*********************************************************************************************************
*/

void Player_1(void* data)
{
    INT8U err;

    srand(GetCurrentThreadId());

    for (;;) {

        OSSemPend(SharedMemSem, 0, &err);
        
        counter = rand() % 50;
        check_1 = counter;
        if(counter>=1 && counter <=50) sprintf(counter1Str, "Player 1: %d ", counter);
        
        PC_DispStr(3, 3, counter1Str, DISP_FGND_WHITE+ DISP_BGND_BLUE);

        OSSemPost(SharedMemSem);
        OSTimeDlyHMSM(0, 0, 1, 0);
    }
} /* end of Player_1 */

void Player_2(void* data)
{
    INT8U err;
    INT8U x = 50;

    srand(GetCurrentThreadId());

    for (;;) {

        OSSemPend(SharedMemSem, 0, &err);

        counter = rand() % 50;
        check_2 = counter;
        if (counter >= 1 && counter <= 50) sprintf(counter2Str, "Player 2: %d ", counter);
        
        PC_DispStr(3, 5, counter2Str, DISP_FGND_WHITE+ DISP_BGND_BLUE);

        OSSemPost(SharedMemSem);
        OSTimeDlyHMSM(0, 0, 1, 0);
    }

}/*end of Player_2 */

void GameMaster(void* data)
{
    INT8U err;

    for (;;) {

        OSSemPend(SharedMemSem, 0, &err);
        
        if (data = counter1Str) check_1;
        if (data = counter2Str) check_2;

        if (check_1 > check_2) {
           PC_DispStr(3, 8, "Winner is: Player 1", DISP_FGND_WHITE+ DISP_BGND_BLUE);
        }
        if (check_2 > check_1) {
            PC_DispStr(3, 8, "Winner is: Player 2", DISP_FGND_WHITE+ DISP_BGND_BLUE);
        }
        OSSemPost(SharedMemSem);
        OSTimeDlyHMSM(0, 0, 1, 0);
    }

}/*end of  GameMaster */ 

/*
*********************************************************************************************************
*                                      NON-TASK FUNCTIONS
*********************************************************************************************************
*/

