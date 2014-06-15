#include <pegasusos.h>

#include "boards.h"

using namespace os::hal;

#define INIT_TASK_PRIORITY (tskIDLE_PRIORITY + configMAX_PRIORITIES - 1)

static void initTask(void *params);

int main()
{
    System::init();


    PE1::init(GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN );
    PE1::low();

    PE2::init(GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN );
    PE2::low();

    PE3::init(GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN );
    PE3::low();

    PE0::init(GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN );
    PE0::low();


    xTaskCreate(&initTask, "initTask", configMINIMAL_STACK_SIZE * 4, NULL, INIT_TASK_PRIORITY, NULL);

    /* Start FreeRTOS scheduler */
    vTaskStartScheduler();

    while (1) {
        PE1::toggle();
        os::hal::Delay::delayMs(100);
    }
    
    return 0;
}

void initTask(__attribute__((unused)) void *parameters) {
  boardInit();

  os::ModuleManager::initAll();
  os::ModuleManager::startAll();

  vTaskDelete(NULL);
}
