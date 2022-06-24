/*
Utilizando como base el programa en freeRTOS visto en clase (MAIN_BLINKY), crear una
tarea nueva llamada “adc_task”, y un semáforo binario denominado “adc_sem”. La tarea
adc_task debe estar bloqueada por el semaforo adc_sem. Una vez que se desbloquee, debe
llamar a la máquina de estados desarrollada en el punto anterior. El semáforo debe
desbloquearse en el callback del timer
 */


#include <stdio.h>
#include <conio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Priorities at which the tasks are created. */
//#define mainQUEUE_RECEIVE_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
//#define	mainQUEUE_SEND_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define ADC_Task_Priority (tskIDLE_PRIORITY + 2 )



/* The rate at which data is sent to the queue.  The times are converted from
milliseconds to ticks using the pdMS_TO_TICKS() macro. */
#define mainTASK_SEND_FREQUENCY_MS			pdMS_TO_TICKS( 200UL )
#define mainTIMER_SEND_FREQUENCY_MS			pdMS_TO_TICKS( 2000UL )

/* The number of items the queue can hold at once. */
#define mainQUEUE_LENGTH					( 2 )

/* The values sent to the queue receive task from the queue send task and the
queue send software timer respectively. */
#define mainVALUE_SENT_FROM_TASK			( 100UL )
#define mainVALUE_SENT_FROM_TIMER			( 200UL )

/*-----------------------------------------------------------*/

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvQueueReceiveTask( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );

/*
 * The callback function executed when the software timer expires. se ejecuta la funcion callback cuando finaliza el timer
 */
//static void prvQueueSendTimerCallback( TimerHandle_t xTimerHandle );
static void ADC_Task(void* pvParameters);

static void prvADC_Task_TimerCallback(TimerHandle_t xTimerHandle);
xSemaphoreHandle sem; 
/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
//static QueueHandle_t xQueue = NULL;

/* A software timer that is started from the tick hook. */
static TimerHandle_t xTimer = NULL;

uint16_t sgn[] =
{
    #include "sgn.h" 

};
enum
{
    ACUMULAR, PROMEDIAR

};

static void Acumular();
static void Promediar();
static void maquina_estado();

static uint32_t llamados = 0;
static uint32_t Acumulador = 0;
uint8_t N = 5;

void main_blinky( void )
{
const TickType_t xTimerPeriod = mainTIMER_SEND_FREQUENCY_MS;

	/* Create the queue. */
	//xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( uint32_t ) );

	/*/if (xQueue != NULL)
	{
		/* Start the two tasks as described in the comments at the top of this
		file. */
		//xTaskCreate( prvQueueReceiveTask,			/* The function that implements the task. */
			//		"Rx", 							/* The text name assigned to the task - for debug only as it is not used by the kernel. */
			//		configMINIMAL_STACK_SIZE, 		/* The size of the stack to allocate to the task. */
			//		NULL, 							/* The parameter passed to the task - not used in this simple case. */
			//		mainQUEUE_RECEIVE_TASK_PRIORITY,/* The priority assigned to the task. */
			//		NULL );							/* The task handle is not required, so NULL is passed. */

		//xTaskCreate( prvQueueSendTask, "TX", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK_PRIORITY, NULL );

		/* Create the software timer, but don't start it yet. */
	//	xTimer = xTimerCreate( "Timer",				/* The text name assigned to the software timer - for debug only as it is not used by the kernel. */
		//						xTimerPeriod,		/* The period of the software timer in ticks. */
			//					pdFALSE,			/* xAutoReload is set to pdFALSE, so this is a one-shot timer. */
				//				NULL,				/* The timer's ID is not used. */
					//			prvQueueSendTimerCallback );/* The function executed when the timer expires. */

//		xTimerStart( xTimer, 0 ); /* The scheduler has not started so use a block time of 0. */

		/* Start the tasks and timer running. */
	//	vTaskStartScheduler();
	//}*/

    vSemaphoreCreateBinary(sem);
    if (sem != NULL)
    {
        // entra si se crea correctamente el semaforo
        xTaskCreate(ADC_Task,			/* The function that implements the task. */
            "Vx", 							/* The text name assigned to the task - for debug only as it is not used by the kernel. */
            configMINIMAL_STACK_SIZE, 		/* The size of the stack to allocate to the task. */
            NULL, 							/* The parameter passed to the task - not used in this simple case. */
            ADC_Task_Priority,/* The priority assigned to the task. */
            NULL);							/* The task handle is not required, so NULL is passed. */
        

        //creo timer pero no lo inicializo aun
        xTimer = xTimerCreate("Timer",				/* The text name assigned to the software timer - for debug only as it is not used by the kernel. */
            xTimerPeriod,		/* The period of the software timer in ticks. */
            pdFALSE,			/* xAutoReload is set to pdFALSE, so this is a one-shot timer. */
            NULL,				/* The timer's ID is not used. */
            prvADC_Task_TimerCallback);/* funcion a utilizar cuando espira el timer. */
        xTimerStart(xTimer, 0); /* bloqueo tiempo a 0 pq aun no se ejecuto el scheduler*/

        xSemaphoreTake(sem, portMAX_DELAY); //semaforo demorara la tarea hasta que finalice el timer
        /* comienzan las tareas y el timer */
        vTaskStartScheduler();

    }

	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the idle and/or
	timer tasks	to be created.  See the memory management section on the
	FreeRTOS web site for more details. */
	for( ;; );
}
/*-----------------------------------------------------------*/


//siempre que creo un timer debe tener su callback para saber que hacer cuando finalice 
// XTimer es para saber que timer genero el call back
static void ADC_Task(void* pvParameters)
{
    xSemaphoreTake(sem, portMAX_DELAY);
    printf("-----------Ejecucion Maquina de Estado ---------------");
    maquina_estado();
    xSemaphoreGive(sem);
}
static void prvADC_Task_TimerCallback(TimerHandle_t xTimerHandle)
{
    
    const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TIMER;

    (void)xTimerHandle;

    xSemaphoreGive(sem);
}


static void maquina_estado()
{
    uint8_t estado = ACUMULAR; // por default arranco con el acumulador

    printf("--------inicio de programa------- \n");

    while (1)
    {

        switch (estado)
        {

        case ACUMULAR:
            Acumular();
            if (llamados >= N)
            {

                estado = PROMEDIAR;

            }
            else
            {
                estado = ACUMULAR;
            }
            break;

        case PROMEDIAR:
            Promediar();
            estado = ACUMULAR;
            break;


        }

    }

    printf("------ Fin del programa------");

    return 0;
}

static void Acumular()
{

    Acumulador = Acumulador + sgn[llamados];
    llamados++;
    return;
}



static void Promediar()
{

    float Prom = Acumulador / N;
    printf("--------- PROMEDIO ----------");

    printf("%f\n", Prom);

    Acumulador = 0;
    llamados = 0;

    printf("--------Elija un Distinto N------- \n");
    scanf("%d", &N);
    if (N > 64)
    {
        N = 0; // como se encuentra fuera del largo seteo un valor por default 
    }

    return;
}





/*static void prvQueueSendTask(void* pvParameters)
{
    TickType_t xNextWakeTime;
    const TickType_t xBlockTime = mainTASK_SEND_FREQUENCY_MS;
    const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TASK;

    /* Prevent the compiler warning about the unused parameter. 
(void)pvParameters;

/* Initialise xNextWakeTime - this only needs to be done once. 
xNextWakeTime = xTaskGetTickCount();

for (;; )
{
    /* Place this task in the blocked state until it is time to run again.
    The block time is specified in ticks, pdMS_TO_TICKS() was used to
    convert a time specified in milliseconds into a time specified in ticks.
    While in the Blocked state this task will not consume any CPU time. 
    vTaskDelayUntil(&xNextWakeTime, xBlockTime);

    /* Send to the queue - causing the queue receive task to unblock and
    write to the console.  0 is used as the block time so the send operation
    will not block - it shouldn't need to block as the queue should always
    have at least one space at this point in the code. 
    xQueueSend(xQueue, &ulValueToSend, 0U);
}
}
/*-----------------------------------------------------------

static void prvQueueSendTimerCallback(TimerHandle_t xTimerHandle)
{
    const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TIMER;

    /* This is the software timer callback function.  The software timer has a
    period of two seconds and is reset each time a key is pressed.  This
    callback function will execute if the timer expires, which will only happen
    if a key is not pressed for two seconds. 

    /* Avoid compiler warnings resulting from the unused parameter. 
    (void)xTimerHandle;

    /* Send to the queue - causing the queue receive task to unblock and
    write out a message.  This function is called from the timer/daemon task, so
    must not block.  Hence the block time is set to 0. 
    xQueueSend(xQueue, &ulValueToSend, 0U);
}
/*-----------------------------------------------------------

static void prvQueueReceiveTask(void* pvParameters)
{
    uint32_t ulReceivedValue;

    /* Prevent the compiler warning about the unused parameter. 
    (void)pvParameters;

    for (;; )
    {
        /* Wait until something arrives in the queue - this task will block
        indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
        FreeRTOSConfig.h.  It will not use any CPU time while it is in the
        Blocked state. 
        xQueueReceive(xQueue, &ulReceivedValue, portMAX_DELAY);

        /*  To get here something must have been received from the queue, but
        is it an expected value?  Normally calling printf() from a task is not
        a good idea.  Here there is lots of stack space and only one task is
        using console IO so it is ok.  However, note the comments at the top of
        this file about the risks of making Windows system calls (such as
        console output) from a FreeRTOS task. 
        if (ulReceivedValue == mainVALUE_SENT_FROM_TASK)
        {
            printf("Message received from task\r\n");
        }
        else if (ulReceivedValue == mainVALUE_SENT_FROM_TIMER)
        {
            printf("Message received from software timer\r\n");
        }
        else
        {
            printf("Unexpected message\r\n");
        }

        /* Reset the timer if a key has been pressed.  The timer will write
        mainVALUE_SENT_FROM_TIMER to the queue when it expires. 
        if (_kbhit() != 0)
        {
            /* Remove the key from the input buffer. 
            (void)_getch();

            /* Reset the software timer. 
            xTimerReset(xTimer, portMAX_DELAY);
        }
    }
}
/*-----------------------------------------------------------*/



