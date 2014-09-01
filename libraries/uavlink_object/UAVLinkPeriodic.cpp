/*
 * UAVLinkPeriodic.cpp
 *
 *  Created on: 6 juin 2014
 *      Author: alienx
 */

#include "UAVLinkPeriodic.h"

#define TASK_PRIORITY	   (tskIDLE_PRIORITY + 3)

using namespace obj;

  util::List<UAVLinkPeriodic::UAVLinkPeriodicEvent*> UAVLinkPeriodic::_mObjectList;
  xSemaphoreHandle UAVLinkPeriodic::mutex;

  void UAVLinkPeriodic::init ()
  {
    mutex = xSemaphoreCreateRecursiveMutex();
    //Thread::create(&run, "UAVLinkEvent", TASK_PRIORITY);
    xTaskCreate( &run, "UAVLinkEvent", configMINIMAL_STACK_SIZE * 4, NULL, TASK_PRIORITY, NULL );
  }

  bool UAVLinkPeriodic::create(UAVLinkObject::UAVLinkEvent *ev, os::Queue* queue, uint16_t period) {
    UAVLinkPeriodicEvent* entry;
    xSemaphoreTakeRecursive(mutex, portMAX_DELAY);

    _mObjectList.start();
    while(_mObjectList.hasNext()) {
	UAVLinkPeriodicEvent* pev = _mObjectList.next();

        if (pev->queue == queue &&
            pev->ev.event == ev->event &&
            pev->ev.obj == ev->obj) {

            xSemaphoreGiveRecursive(mutex);

            return false;
        }
    }

    entry = new UAVLinkPeriodicEvent;
    entry->ev.event = ev->event;
    entry->ev.obj = ev->obj;
    entry->queue = queue;
    entry->updatePeriod = period;
    entry->nextUpdateAt = randomizePeriod(period);
    _mObjectList.append(entry);


    xSemaphoreGiveRecursive(mutex);

    return true;
  }

  int32_t UAVLinkPeriodic::periodicUpdate() {
    UAVLinkPeriodicEvent *entry;
    uint32_t now;
    uint32_t nextUpdateAt;
    int32_t offset;

    xSemaphoreTakeRecursive(mutex, portMAX_DELAY);
    nextUpdateAt = xTaskGetTickCount() * portTICK_RATE_MS + 1000;

    _mObjectList.start();
    while(_mObjectList.hasNext()) {
        entry = _mObjectList.next();

        if (entry->updatePeriod > 0) {
            now = xTaskGetTickCount() * portTICK_RATE_MS;
            /* Object periodic update lauch */
            if (entry->nextUpdateAt <= now) {
                os::hal::PE2::toggle();
                offset = (now - entry->nextUpdateAt) % entry->updatePeriod;
                entry->nextUpdateAt = now + entry->updatePeriod - offset;
                entry->queue->send(&entry->ev, 0);
            }

            if (entry->nextUpdateAt < nextUpdateAt) {
                    nextUpdateAt = entry->nextUpdateAt;
            }
        }

    }

    xSemaphoreGiveRecursive(mutex);

    return nextUpdateAt;
  }


  void UAVLinkPeriodic::run (__attribute__((unused)) void *params)
  {
    static uint32_t nextUpdateAt = 0;
    os::hal::PE2::high();

    while(1) {
        if ( (xTaskGetTickCount() * portTICK_RATE_MS) >= nextUpdateAt) {
            nextUpdateAt = periodicUpdate();
        }

        vTaskDelay(nextUpdateAt / portTICK_RATE_MS - xTaskGetTickCount());
    }
  }

  /**
   * Park-Miller-Carta Pseudo-Random Number Generator and OpenPilot implementation
   * http://www.firstpr.com.au/dsp/rand31/
   */
  uint16_t UAVLinkPeriodic::randomizePeriod(uint16_t period)
  {
      static uint32_t seed = 1;
      uint32_t hi, lo;

      lo  = 16807 * (seed & 0xFFFF);
      hi  = 16807 * (seed >> 16);
      lo += (hi & 0x7FFF) << 16;
      lo += hi >> 15;
      if (lo > 0x7FFFFFFF) {
          lo -= 0x7FFFFFFF;
      }
      seed = lo;
      return (uint16_t)(((float)period * (float)lo) / (float)0x7FFFFFFF);
  }
