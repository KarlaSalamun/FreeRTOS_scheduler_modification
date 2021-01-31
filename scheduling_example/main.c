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

extern volatile double xTardiness;
uint32_t hperiod;

#define TSK_A_PERIOD 5
#define TSK_B_PERIOD 8

typedef struct _task {
    int period;
    int duration;
    double weight;
} _task;

FILE *output;
double overload;

int main( int argc, char *argv[] ) {

    char filename[10];
    strcpy( filename, "inputs/" );
    strncat( filename, argv[1], strlen( argv ) );

    vPortEarlyInit();

    _task tasks[5];
    FILE *fd = fopen( filename, "r+" );
    output = fopen( "outputs/edf", "a+" );

    fscanf( fd, "%lf", &overload );

    for( int i=0; i<5; i++ ) {
        fscanf( fd, "%d %d %lf", &tasks[i].period, &tasks[i].duration, &tasks[i].weight );

        const char name[2];
        sprintf( name, "%d", i );

        xTaskPeriodicCreate( TSK_A, name, configMINIMAL_STACK_SIZE, (void const *)&tasks[i], 1, NULL,  tasks[i].period, tasks[i].duration, tasks[i].weight, 0 );
        // assert( tasks[i].weight == 1 );
    }

    fscanf( fd, "%d", &hperiod );

    vTaskStartScheduler();

    while(1);
    return 0;
}

void TSK_A( void *pvParameters ) {
    _task *params = ( _task * )pvParameters;
    TickType_t xLastWakeTimeA;
    TickType_t xNextWakeTimeA;
    // const TickType_t xFrequency = TSK_A_PERIOD;
    const TickType_t xFrequency = params->period;
    volatile int count = params->duration;
    TickType_t xNextTime;
    TickType_t xTime;
    xLastWakeTimeA = 0;
    xNextWakeTimeA = params->period;
    int A_Tard = 0;
    for(;;) {
        xTime= xTaskGetTickCount();
        //While loop that simulates capacity
        // printf( "task A start\n" );
        while(count != 0) {
            if((xNextTime = xTaskGetTickCount()) > xTime) {
                count--;
                xTime = xNextTime;
            }
        }
        count = params->duration;
        xNextWakeTimeA = xLastWakeTimeA + params->period; 
        vTaskDelayUntil(&xLastWakeTimeA, xFrequency);
    }
}

void vApplicationTickHook( void ) {
    int xTime = ( int )xTaskGetTickCount();
    // printf( "TICK: %d\n", xTime );
    if( xTime >= 2 * hperiod ) {
        fprintf( output, "%lf %lf %d\n", overload, 0.5 * xTardiness / (double) hperiod, hperiod );
        fclose( output );
        exit(0);
    }
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