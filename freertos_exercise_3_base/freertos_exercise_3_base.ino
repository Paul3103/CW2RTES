#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <FreeRTOSConfig.h>

#define SERIAL_PORT Serial

QueueHandle_t xQueue;

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
  bool firstTime = true;
  int32_t lValueToSend;
  BaseType_t xStatus;
  TickType_t  xLastWakeTime = xTaskGetTickCount();


 lValueToSend = ( int32_t ) pvParameters;
 /* As per most tasks, this task is implemented within an infinite loop. */
 for( ;; )
 {
   if (lValueToSend == 100) {
     if (!firstTime) {
      xStatus = xQueueSendToBack( xQueue, &lValueToSend, pdMS_TO_TICKS( 1000 ) );
     }
     if( xStatus != pdPASS )
     {
        vPrintString( "Could not send to the queue.\r\n" );
     }
      
   } else {
     if(firstTime) {
      xStatus = xQueueSendToBack( xQueue, &lValueToSend, pdMS_TO_TICKS( 1000 ) );
     }
     if( xStatus != pdPASS )
     {
        vPrintString( "Could not send to the queue.\r\n" );
     }
   }
   firstTime = !firstTime;
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
   
     }
     else
     {
    
        vPrintString( "Could not receive from the queue.\r\n" );
     }
   }
}


void setup() {

   xQueue = xQueueCreate( 5, sizeof( int32_t ) );
   if( xQueue != NULL )
   {
  
   xTaskCreate( vSenderTask, "Sender1", 1000, ( void * ) 100, 1, NULL );
   xTaskCreate( vSenderTask, "Sender2", 1000, ( void * ) 200, 1, NULL );

   xTaskCreate( vReceiverTask, "Receiver", 1000, NULL, 2, NULL );
   
   }
   else
   {
 
   }

}

void loop() {
 
}
