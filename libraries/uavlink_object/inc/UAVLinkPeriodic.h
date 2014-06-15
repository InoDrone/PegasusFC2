/*
 * UAVLinkPeriodic.h
 *
 *  Created on: 6 juin 2014
 *      Author: alienx
 */

#ifndef UAVLINKPERIODIC_H_
#define UAVLINKPERIODIC_H_

#include <pegasusos.h>
#include "UAVLinkObject.h"

  class UAVLinkPeriodic
  {
  public:

    static void init();
    static bool create(obj::UAVLinkObject::UAVLinkEvent *ev, os::Queue* queue, uint16_t period);

    static void run(void *params);

  private:
    struct UAVLinkPeriodicEvent {
      obj::UAVLinkObject::UAVLinkEvent ev;
      os::Queue* queue;
      uint16_t updatePeriod;
      uint32_t nextUpdateAt;
    };

    static util::List<UAVLinkPeriodicEvent*> _mObjectList;
    static xSemaphoreHandle mutex;

    static int32_t periodicUpdate();
    static uint16_t randomizePeriod(uint16_t period);
  };

#endif /* UAVLINKPERIODIC_H_ */
