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

int main( void ) {
    vPortEarlyInit();

    xTaskEdfCreate( TSK_A, ( const char * ) "A", configMINIMAL_STACK_SIZE, NULL, 1, NULL, 5 );
    xTaskEdfCreate( TSK_B, ( const char * ) "B", configMINIMAL_STACK_SIZE, NULL, 1, NULL, 5 );

    vTaskStartScheduler();

    while(1);
    return 0;
}

void TSK_A( void *pvParameters ) {
    TickType_t xLastWakeTimeA;
    const TickType_t xFrequency = 5; //task A frequeny
    TickType_t count = 2;
    TickType_t xNextTime;
    TickType_t xTime;
    xLastWakeTimeA = 0;
    for(;;) {
        xTime= xTaskGetTickCount();
        //While loop that simulates capacity
        while(count != 0) {
            if((xNextTime = xTaskGetTickCount()) > xTime) {
                count--;
                xTime = xNextTime;
            }
        }
        printf( "task A \n" );
        count = 2;
        vTaskDelayUntil(&xLastWakeTimeA, xFrequency);
    }
}

void TSK_B( void *pvParameters ) {
    TickType_t xLastWakeTimeB;
    const TickType_t xFrequency = 5; //task A frequeny
    TickType_t count = 2;
    TickType_t xNextTime;
    TickType_t xTime;
    xLastWakeTimeB = 0;
    for(;;) {
        xTime= xTaskGetTickCount();
        //While loop that simulates capacity
        while(count != 0) {
            if((xNextTime = xTaskGetTickCount()) > xTime) {
                count--;
                xTime = xNextTime;
                
            }
        }
        printf( "task B \n" );
        count = 2;
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