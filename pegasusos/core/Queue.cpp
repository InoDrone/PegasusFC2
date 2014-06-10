/*
 * Queue.cpp
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#include "inc/Queue.h"

namespace os
{

  /**
   * Constructor
   */
  Queue::Queue (const UBaseType_t uxQueueLength, const UBaseType_t uxItemSize) :
      isCreated(false)
  {
      queue = xQueueCreate(uxQueueLength, uxItemSize);
      if (queue != NULL) {
	  isCreated = true;
      }
  }


  /**
   * Create Queue
   */
  bool Queue::create(const UBaseType_t uxQueueLength, const UBaseType_t uxItemSize)
  {
    if (isCreated) return false;

    queue = xQueueCreate(uxQueueLength, uxItemSize);
    if (queue == NULL) return false;

    isCreated = true;

    return true;
  }

  /**
   * Send item to Queue
   */
  bool Queue::send(const void * const pvItemToQueue, TickType_t xTicksToWait)
  {
    if (!isCreated) return false;

    return xQueueSend(queue, pvItemToQueue, xTicksToWait) == pdTRUE;
  }

  /**
   * Send item to Queue
   */
  bool Queue::sendFromISR(const void * const pvItemToQueue, BaseType_t * const woken)
  {
    if (!isCreated) return false;

    return xQueueSendToBackFromISR(queue, pvItemToQueue, woken) == pdTRUE;
  }

  /**
   * Receive item to Queue
   */
  bool Queue::receive(void *buffer, TickType_t xTicksToWait)
  {
    if (!isCreated) return false;

    return xQueueReceive(queue, buffer, xTicksToWait) == pdTRUE;
  }

} /* namespace os */
