#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

void TSK_A( void *pvParameters );
void TSK_B( void *pvParameters );

uint32_t tardiness = 0;

#define TSK_A_PERIOD 5
#define TSK_B_PERIOD 8

int main( void ) {
    vPortEarlyInit();

    xTaskPeriodicCreate( TSK_A, ( const char * ) "A", configMINIMAL_STACK_SIZE, NULL, 1, NULL, TSK_A_PERIOD, 2, 0.5, 0 );
    xTaskPeriodicCreate( TSK_B, ( const char * ) "B", configMINIMAL_STACK_SIZE, NULL, 1, NULL, TSK_B_PERIOD, 6, 1, 0 );

    vTaskStartScheduler();

    while(1);
    return 0;
}

void TSK_A( void *pvParameters ) {
    TickType_t xLastWakeTimeA;
    const TickType_t xFrequency = TSK_A_PERIOD;
    TickType_t count = 2;
    TickType_t xNextTime;
    TickType_t xTime;
    xLastWakeTimeA = 0;
    int A_Tard = 0;
    for(;;) {
        xTime= xTaskGetTickCount();
        //While loop that simulates capacity
        printf( "task A start\n" );
        while(count != 0) {
            if((xNextTime = xTaskGetTickCount()) > xTime) {
                count--;
                xTime = xNextTime;
            }
        }
        // printf( "task A end \n" );
        // printf( "last wake time: %d\n", xLastWakeTimeA );
        // if( xLastWakeTimeA + xFrequency < xTaskGetTickCount() ) {
        //     A_Tard += xTaskGetTickCount() - xLastWakeTimeA - xFrequency;
        // }
        // printf( "tardiness: %d\n", A_Tard );
        count = 2;  
        vTaskDelayUntil(&xLastWakeTimeA, xFrequency);
    }
}

void TSK_B( void *pvParameters ) {
    TickType_t xLastWakeTimeB;
    const TickType_t xFrequency = TSK_B_PERIOD;
    TickType_t count = 6;
    TickType_t xNextTime;
    TickType_t xTime;
    xLastWakeTimeB = 0;
    int B_Tard = 0;
    for(;;) {
        xTime= xTaskGetTickCount();
        printf( "task B start\n" );
        //While loop that simulates capacity
        while(count != 0) {
            if((xNextTime = xTaskGetTickCount()) > xTime) {
                count--;
                xTime = xNextTime;
            }
        }
        // printf( "task B end\n" );
        // if( xLastWakeTimeB + xFrequency < xTaskGetTickCount() ) {
        //     B_Tard += xTaskGetTickCount() - xLastWakeTimeB - xFrequency;
        // }
        // printf( "tardiness: %d\n", B_Tard );
        count = 6;
        vTaskDelayUntil(&xLastWakeTimeB, xFrequency);
    }
}

void vApplicationTickHook( void ) {
    printf( "TICK: %d ", (int)xTaskGetTickCount() );
}

const struct timespec ts = { .tv_sec = 0, .tv_nsec = 200000 };

void vApplicationIdleHook( void ) {
    /*
     * The idle task must be kept preemptive. Therefore nanosleep mustn't be put
     * inside a critical section.
     * It is safe to use nanosleep without a critical section as it gets
     * aborted by signals which are used by the Linux port to suspend the
     * task.
     */
    nanosleep(&ts, NULL);
}