#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <FreeRTOSConfig.h>

#define SERIAL_PORT Serial

QueueHandle_t xQueue;
SemaphoreHandle_t xSemaphore;

void vPrintString( const char *pcString )
{

 taskENTER_CRITICAL();
 {
  SERIAL_PORT.println(pcString );
   fflush( stdout );
 }
 taskEXIT_CRITICAL();
}



static void vSenderTask( void *pvParameters )
{
  int32_t lValueToSend = ( int32_t ) pvParameters;
  BaseType_t xStatus;
  TickType_t  xLastWakeTime = xTaskGetTickCount();


  for( ;; )
  {
  if (lValueToSend == 100) {
    vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 1000 ) );  
  }
    xSemaphoreTake( xSemaphore, pdMS_TO_TICKS(portMAX_DELAY) );

    xStatus = xQueueSendToBack( xQueue, &lValueToSend, pdMS_TO_TICKS( 1000 ) );
    if( xStatus != pdPASS )
    {
      vPrintString( "Could not send to the queue.\r\n" );
    }
    xSemaphoreGive( xSemaphore );
    if (lValueToSend == 200) {
      vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 1000 ) );  
      
    }
    vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 1000 ) );  
  }
}





static void vReceiverTask( void *pvParameters )
{
  /* Declare the variable that will hold the values received from the queue. */
  int32_t lReceivedValue;
  BaseType_t xStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS( 1200 );
  
   for( ;; )
   {
  
     if( uxQueueMessagesWaiting( xQueue ) != 0 )
     {
     vPrintString( "Queue should have been empty!\r\n" );
     }
   
     xStatus = xQueueReceive( xQueue, &lReceivedValue, xTicksToWait );
     if( xStatus == pdPASS )
     {
       SERIAL_PORT.println(String(lReceivedValue) +"At this time: "+String(millis()) );
       if(lReceivedValue ==100) {
          digitalWrite(LED_BUILTIN, HIGH);
       } else {
        digitalWrite(LED_BUILTIN,LOW);
       }
     }
     else
     {
    
        vPrintString( "Could not receive from the queue.\r\n" );
     }
   }
}


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  xQueue = xQueueCreate( 5, sizeof( int32_t ) );
  xSemaphore = xSemaphoreCreateMutex();
  if( xQueue != NULL && xSemaphore != NULL )
  {
    xSemaphoreGive( xSemaphore );

    xTaskCreate( vSenderTask, "Sender1", 1000, ( void * ) 100, 1, NULL );
    xTaskCreate( vSenderTask, "Sender2", 1000, ( void * ) 200, 1, NULL );

    xTaskCreate( vReceiverTask, "Receiver", 1000, NULL, 2, NULL );
    //vTaskStartScheduler();
  }
  else
  {
    // Error creating queue or semaphore
  }
}

void loop() {
 
}
