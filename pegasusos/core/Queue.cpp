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
  Queue::Queue (const UBaseType_t uxQueueLength, const UBaseType_t uxItemSize)
  {
      queue = xQueueCreate(uxQueueLength, uxItemSize);
  }

  /**
   * Send item to Queue
   */
  bool Queue::send(const void * const pvItemToQueue, TickType_t xTicksToWait)
  {
    return xQueueSend(queue, pvItemToQueue, xTicksToWait) != pdTRUE;
  }

  /**
   * Receive item to Queue
   */
  bool Queue::receive(void *buffer, TickType_t xTicksToWait)
  {
    return xQueueReceive(queue, buffer, xTicksToWait) != pdTRUE;
  }

} /* namespace os */
