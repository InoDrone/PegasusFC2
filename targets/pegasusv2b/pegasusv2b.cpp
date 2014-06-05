#include <pegasusos.h>

#include "boardsDefs.h"

void task1(void *pvParameters);
void task2(void *pvParameters);

using namespace os::hal;

#define INIT_TASK_PRIORITY (tskIDLE_PRIORITY + configMAX_PRIORITIES - 1)

static void initTask(void *params);

int main()
{
    System::init();
    
   // PegasusOSBoardInit();
#ifdef TELEMETRY_PORT
    TELEMETRY_PORT::init<TELEMETRY_PORT_RX_PIN, TELEMETRY_PORT_TX_PIN>(TELEMETRY_PORT_BAUDS);
    TELEMETRY_PORT::write("V1\r\n");
#endif

    PE1::init(GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN );
    PE1::hight();

   /* PE0::init(GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN );
    PE0::high();


    PE2::init(GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN );
    PE3::init(GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN );

    xTaskCreate( task1, "BlinkLed", configMINIMAL_STACK_SIZE, ( void * ) NULL, tskIDLE_PRIORITY, NULL );
    //xTaskCreate( task2, "BlinkLed2", configMINIMAL_STACK_SIZE, ( void * ) NULL, tskIDLE_PRIORITY, NULL );*/

    xTaskCreate(initTask, "initTask", configMINIMAL_STACK_SIZE, NULL, INIT_TASK_PRIORITY, NULL);

    /* Start FreeRTOS scheduler */
    vTaskStartScheduler();

    while (1) {
        PE1::high();
    }
    
    return 0;
}

void initTask(__attribute__((unused)) void *parameters) {
  //InitBoard();

  os::ModuleManager::initAll();
  os::ModuleManager::startAll();

  vTaskDelete(NULL);
}
