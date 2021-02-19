#pragma once

#include <stdint.h>
#include <assert.h>

/* Port related definitions */
#define configPORT_USE_REINIT                       ( 0 )
#define configPORT_MAX_TASKS                        ( 10 )

#define configUSE_PREEMPTION                        ( 1 )
#define configUSE_PORT_OPTIMISED_TASK_SELECTION     ( 0 )
#define configUSE_TICKLESS_IDLE                     ( 0 )
#define configCPU_CLOCK_HZ                          ( 60000000 )
#define configTICK_RATE_HZ                          ( 10000 )
#define configMAX_PRIORITIES                        ( 12 )
#define configMINIMAL_STACK_SIZE                    ( 128 )
#define configMAX_TASK_NAME_LEN                     ( 16 )
#define configUSE_16_BIT_TICKS                      ( 0 )
#define configIDLE_SHOULD_YIELD                     ( 1 )
#define configUSE_TASK_NOTIFICATIONS                ( 1 )
#define configUSE_MUTEXES                           ( 1 )
#define configUSE_RECURSIVE_MUTEXES                 ( 1 )
#define configUSE_COUNTING_SEMAPHORES               ( 1 )
#define configUSE_ALTERNATIVE_API                   ( 0 ) /* Deprecated! */
#define configQUEUE_REGISTRY_SIZE                   ( 10 )
#define configUSE_QUEUE_SETS                        ( 0 )
#define configUSE_TIME_SLICING                      ( 1 )
#define configUSE_NEWLIB_REENTRANT                  ( 0 )
#define configENABLE_BACKWARD_COMPATIBILITY         ( 0 )
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS     ( 5 )
#define configSTACK_DEPTH_TYPE                  uint16_t
#define configMESSAGE_BUFFER_LENGTH_TYPE        size_t

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION         0
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configTOTAL_HEAP_SIZE                   10240
#define configAPPLICATION_ALLOCATED_HEAP        0

/* Hook function related definitions. */
#define configUSE_IDLE_HOOK                     1
#define configUSE_TICK_HOOK                     1
#define configCHECK_FOR_STACK_OVERFLOW          0
#define configUSE_MALLOC_FAILED_HOOK            0
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                1
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         1

/* Software timer related definitions. */
#define configUSE_TIMERS                        0
#define configTIMER_TASK_PRIORITY               12
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE

/* Interrupt nesting behaviour configuration. */
#define configKERNEL_INTERRUPT_PRIORITY         [dependent of processor]
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    [dependent on processor and application]
#define configMAX_API_CALL_INTERRUPT_PRIORITY   [dependent on processor and application]

/* Define to trap errors during development. */
#define configASSERT( x ) assert( x )

/* FreeRTOS MPU specific definitions. */
#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0

#define configUSE_EDF_SCHEDULER 				0
#define configUSE_GP_SCHEDULER					1

#define configTRACE_TARDINESS					1

/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xResumeFromISR                  1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     0
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_eTaskGetState                   0
#define INCLUDE_xEventGroupSetBitFromISR        1
#define INCLUDE_xTimerPendFunctionCall          0
#define INCLUDE_xTaskAbortDelay                 0
#define INCLUDE_xTaskGetHandle                  0
#define INCLUDE_xTaskResumeFromISR              1

extern volatile double tardiness;
/*
#define traceTASK_INCREMENT_TICK( xTickCount )		\
	taskENTER_CRITICAL();					\
	printf( "TICK: %d\n", (int)xTickCount + 1 );	\
	taskEXIT_CRITICAL();

#define traceTASK_CREATE(xTask)					\
	printf("task %s is successfully created\n", pxNewTCB->pcTaskName );

#define traceTASK_DELAY_UNTIL( xTimeToWake )			\
	printf( "task %s delayed until %d\n", pxCurrentTCB->pcTaskName, (int)xTimeToWake );

*/
/*
#define traceTASK_SWITCHED_OUT()			\
	taskENTER_CRITICAL();					\
	printf( "task %s is switched OUT\n", pxCurrentTCB->pcTaskName );	\
	taskEXIT_CRITICAL();
*/
/*
#define traceTASK_SWITCHED_IN()				\
	taskENTER_CRITICAL();					\
	printf( "task %s switched IN\n", pxCurrentTCB->pcTaskName );	\
	taskEXIT_CRITICAL();
*/