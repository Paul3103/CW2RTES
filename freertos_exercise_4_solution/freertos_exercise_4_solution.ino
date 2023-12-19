#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>


#define SERIAL_PORT Serial
#define TASK1_PERIOD_MS 5000
#define TASK2_PERIOD_MS 10000


SemaphoreHandle_t xSemaphore;
int VeryUsefulSum; //This is going to be added to by Task1 and decremented by Task2

void setup() {

    //For the actual stuff being done the different tasks add or subtract to the same sum, so it can be seen in the SERIAL port
    //That the value of VeryUsefulSum ranges from 1 to -1
    xSemaphore = xSemaphoreCreateMutex();
    VeryUsefulSum = 0;
    xTaskCreate(Task1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xTaskCreate(Task2, "Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

 
}

void loop() {
    
}

void Task1(void *pvParameters) {
    bool FirstTime = true;
    vTaskDelay(pdMS_TO_TICKS(1000));

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
        // Task 1 code here
        xSemaphoreTake(xSemaphore,pdMS_TO_TICKS(1000));
        if(FirstTime){
        VeryUsefulSum += 1;

          SERIAL_PORT.print("Task 1 doing useful things at t=");
          SERIAL_PORT.println(millis()/1000);
          SERIAL_PORT.print("The New Value of the very useful sum is: ");
          SERIAL_PORT.println(VeryUsefulSum);
        FirstTime = !FirstTime;

        } else{
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TASK1_PERIOD_MS));
        VeryUsefulSum += 1;

        SERIAL_PORT.print("Task 1 doing useful things at t=");
        SERIAL_PORT.println(millis()/1000);
        SERIAL_PORT.print("The New Value of the very useful sum is: ");
        SERIAL_PORT.println(VeryUsefulSum);
        //SERIAL_PORT.println("THERE IS REASON FOR THIS TO BE DELAYED HAPPEN");
       
        }

        
    }
}

void Task2(void *pvParameters) {
    int TaskExecTime = 2;
    vTaskDelay(pdMS_TO_TICKS(1000));
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
        xSemaphoreTake(xSemaphore,pdMS_TO_TICKS(1000));
        VeryUsefulSum -=1;

        SERIAL_PORT.print("Task 2 doing useful things at t=");
        SERIAL_PORT.println(millis()/1000);
        SERIAL_PORT.print("The New Value of the very useful sum is: ");
        SERIAL_PORT.println(VeryUsefulSum);           
        
        TaskExecTime -= 1;
        if (TaskExecTime <= 0) {
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(TASK2_PERIOD_MS));
          TaskExecTime = 2;
        }
        // Then waits for the remaining time in the 10-second period
    }
}
