#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>


#define SERIAL_PORT Serial

SemaphoreHandle_t xSemaphore;
// Function prototypes
void Task1(void *pvParameters);
void Task2(void *pvParameters);

void setup() {
    // Setup Task 1
    xSemaphore = xSemaphoreCreateMutex();
    xTaskCreate(Task1, "Task1", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

    // Setup Task 2
    xTaskCreate(Task2, "Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Start the scheduler
    //vTaskStartScheduler();
}

void loop() {
    // This shouldn't be reached, as the scheduler takes over
}

void Task1(void *pvParameters) {
    bool FirstTime = true;
    vTaskDelay(pdMS_TO_TICKS(1000));

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
        // Task 1 code here
        xSemaphoreTake(xSemaphore,pdMS_TO_TICKS(1000));
        if(FirstTime){
          SERIAL_PORT.print("Task 1 doing useful things at t=");
          SERIAL_PORT.println(millis()/1000);
        //SERIAL_PORT.println("THERE IS REASON FOR THIS TO BE DELAYED HAPPEN");
        FirstTime = !FirstTime;

        } else{
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(5000));

        SERIAL_PORT.print("Task 1 doing useful things at t=");
        SERIAL_PORT.println(millis()/1000);
        //SERIAL_PORT.println("THERE IS REASON FOR THIS TO BE DELAYED HAPPEN");
       
        }

        
    }
}

void Task2(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(1000));
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
        xSemaphoreTake(xSemaphore,pdMS_TO_TICKS(1000));
        SERIAL_PORT.print("Task 2 doing useful things at t=");
        SERIAL_PORT.println(millis()/1000);
            
        

        // Then waits for the remaining time in the 10-second period
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10000));
    }
}
