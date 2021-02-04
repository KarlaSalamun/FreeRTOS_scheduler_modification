#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

void TSK_A( void *pvParameters );
void TSK_B( void *pvParameters );

extern volatile double xTardiness;
double total_tardiness;
double unit_tardiness;
int hperiod;
int job_num;
double mean_proctime;
double overload;
double mean_weight;
FILE *output;

#define TSK_A_PERIOD 5
#define TSK_B_PERIOD 8
#define TSK_NUM      5

typedef struct _task {
    int period;
    int duration;
    double weight;
} _task;

int cmp_period( const void *a, const void * b ) {
    return ( (( _task * )(b))->period - (( _task * )(a))->period );
}

int cmp_wpt( const void *a, const void * b ) {
    return ( (( _task * )(b))->duration * (( _task * )(b))->weight - (( _task * )(a))->duration * (( _task * )(a))->weight );
}

int main( int argc, char *argv[] ) {

    char filename[12];
    strcpy( filename, "inputs/" );
    strncat( filename, argv[1], strlen( argv[1] ) );

    vPortEarlyInit();

    _task tasks[TSK_NUM];
    FILE *fd = fopen( filename, "r+" );
    output = fopen( "outputs/wspt", "a+" );

    for( int i=0; i<TSK_NUM; i++ ) {
        fscanf( fd, "%d %d %lf", &tasks[i].period, &tasks[i].duration, &tasks[i].weight );

        

        // assert( tasks[i].weight == 1 );
    }
    qsort( tasks, TSK_NUM, sizeof( _task ), cmp_wpt );
    for( int i=0; i<TSK_NUM; i++ ) {
        char name[TSK_NUM];
        sprintf( name, "%d", i );
        xTaskCreate( TSK_A, name, configMINIMAL_STACK_SIZE, ( void * const )&tasks[i], i, NULL );
    }
    mean_proctime /= TSK_NUM;

    fscanf( fd, "%d", &hperiod );
    fscanf( fd, "%lf", &overload );

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
        taskENTER_CRITICAL();
        if( xLastWakeTimeA + params->period <= xTime ) {
            total_tardiness += params->weight * ( xTime - ( xLastWakeTimeA + params->period ) );
            unit_tardiness += params->weight;
        }
        mean_weight += params->weight;
        mean_proctime += params->duration;
        job_num++;
        count = params->duration;
        xNextWakeTimeA = xLastWakeTimeA + params->period; 
        vTaskDelayUntil(&xLastWakeTimeA, xFrequency);
        assert( xLastWakeTimeA % xFrequency == 0 );
        assert( xNextWakeTimeA == xLastWakeTimeA );
        taskEXIT_CRITICAL();
    }
}

void vApplicationTickHook( void ) {
    int xTime = ( int )xTaskGetTickCount();
    // printf( "TICK: %d\n", xTime );
    if( xTime >= 2 * hperiod ) {
        mean_proctime /= job_num;
        mean_weight /= job_num;
        fprintf( output, "%lf %lf %d %d %lf %lf %lf\n", overload, total_tardiness, hperiod, job_num, mean_proctime, mean_weight, unit_tardiness );
        // if( overload > 1 ) {
        //     assert( total_tardiness != 0 );
        // }
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