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
int active;

#define TSK_A_PERIOD 5
#define TSK_B_PERIOD 8
#define TSK_NUM      3

typedef struct _task {
    int period;
    int duration;
    double weight;
    int id;
    int remaining;
    int instance;
} _task;

struct _task tasks[TSK_NUM];

int main( int argc, char *argv[] ) {

    char filename[12];
    strcpy( filename, "inputs/" );
    strncat( filename, argv[1], strlen( argv[1] ) );

    vPortEarlyInit();

    FILE *fd = fopen( filename, "r+" );
    output = fopen( "outputs/test", "a+" );

    for( int i=0; i<TSK_NUM; i++ ) {
        fscanf( fd, "%d %d %lf", &tasks[i].period, &tasks[i].duration, &tasks[i].weight );

        tasks[i].id = i;
        tasks[i].instance = 1;
        tasks[i].remaining = tasks[i].duration;

        char name[TSK_NUM];
        sprintf( name, "%d", i );

        xTaskPeriodicCreate( TSK_A, name, configMINIMAL_STACK_SIZE, ( void * const )&tasks[i], 1, 
            NULL, tasks[i].period, tasks[i].duration, tasks[i].weight, 0 );
    }
    mean_proctime /= TSK_NUM;

    fscanf( fd, "%d", &hperiod );
    fscanf( fd, "%lf", &overload );

    vTaskStartScheduler();

    while(1);
    return 0;
}

void TSK_A( void *pvParameters ) {
    int index = (( _task * )pvParameters)->id;
    TickType_t xLastWakeTimeA;
    TickType_t xNextWakeTimeA;
    // const TickType_t xFrequency = TSK_A_PERIOD;
    const TickType_t xFrequency = tasks[index].period;
    TickType_t xNextTime;
    TickType_t xTime;
    xLastWakeTimeA = 0;
    xNextWakeTimeA = tasks[index].period;
    int count = tasks[index].duration;
    for(;;) {
        xTime= xTaskGetTickCount();
        taskENTER_CRITICAL();
        printf( "[task %d] START\n", index );
        active = index;
        taskEXIT_CRITICAL();
        //While loop that simulates capacity
        // printf( "task A start\n" );
        while(1) {
            if((xNextTime = xTaskGetTickCount()) > xTime) {
                taskENTER_CRITICAL();
                active = index;
                xTime = xNextTime;
                // vTaskSuspendAll();
                count--;
                if( count == 0 ) {
                    break;
                }
                else {
                    // xTaskResumeAll();
                    taskEXIT_CRITICAL();
                }
            }
        }
        // if( xLastWakeTimeA + tasks[index].period < xTime ) {
        //     total_tardiness += tasks[index].weight * ( xTime - ( xLastWakeTimeA + tasks[index].period ) );
        //     // assert(overload > 1);
        //     unit_tardiness += tasks[index].weight;
        //     printf( "[task %d] tardiness %d\n", index, ( xTime - ( xLastWakeTimeA + params->period ) ));
        // }
        active = index;
        count = tasks[index].duration;
        xNextWakeTimeA = xLastWakeTimeA + tasks[index].period; 
        vTaskDelayUntil(&xLastWakeTimeA, xFrequency);
        taskEXIT_CRITICAL();
        // xTaskResumeAll();
    }
}

void vApplicationTickHook( void ) {
    int xTime = ( int )xTaskGetTickCount();
    // printf( "TICK: %d\n", xTime );
    for( int i=0; i<TSK_NUM; i++ ) {
        if( i == active ) {
            tasks[i].remaining--;
            taskENTER_CRITICAL();
            printf( "[task %d] remaining %d\n", i, tasks[i].remaining );
            taskEXIT_CRITICAL();
        }
        if( tasks[i].remaining == 0 ) {
            if( tasks[i].instance * tasks[i].period < xTime ) {
                total_tardiness += tasks[i].weight * ( xTime - tasks[i].instance * tasks[i].period );
                taskENTER_CRITICAL();
                printf( "task %d: tardiness %d\n", i, ( xTime - tasks[i].instance * tasks[i].period ) );
                taskEXIT_CRITICAL();
            }
            mean_weight += tasks[i].weight;
            mean_proctime += tasks[i].duration;
            job_num++;
            tasks[i].remaining = tasks[i].duration;
            tasks[i].instance++;
        }
    }
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