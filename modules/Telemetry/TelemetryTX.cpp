/*
 * Telemetry.cpp
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#include "inc/TelemetryTX.h"

#define QUEUE_SIZE         80
#define TASK_PRIORITY	   (tskIDLE_PRIORITY + 2)

#define MAX_RETRIES	   2

using namespace obj;

TelemetryTX::TelemetryTX()  :
  Thread("TelemetryTX", TASK_PRIORITY),
  queue(QUEUE_SIZE, sizeof(UAVLinkEvent)) {}

void TelemetryTX::init(UAVLink::Instance* ulink)
{
  this->uavlink = ulink;

  UAVLinkObject::UAVLinkObjectList* objects = UAVLinkObject::getRegistredObjects();
  objects->start();
  while (objects->hasNext()) {
      UAVLinkObject* object = objects->next();
      TELEMETRY_PORT::write("REGISTER OBJECT: ");
      TELEMETRY_PORT::write(object->getName());
      if (object->telemetryUpdateMode() == UAVLinkObject::UPDATEMODE_ONCHANGE) {
          object->connect(&queue, UAVLinkObject::EVENT_UPDATED);
          TELEMETRY_PORT::write(" (ONCHANGE)");
      } else if (object->telemetryUpdateMode() == UAVLinkObject::UPDATEMODE_PERIODIC) {
          UAVLinkEvent event;
          event.obj = object;
          event.event = UAVLinkObject::EVENT_PERIODIC;
          TELEMETRY_PORT::write(" (PERIODIC)");
          if(UAVLinkPeriodic::create(&event, &queue, object->telemetryUpdatePeriod())) {
              TELEMETRY_PORT::write("-CREATED");
          }
      }

      TELEMETRY_PORT::write("\r\n");
  }
}

void TelemetryTX::run()
{
    UAVLinkEvent event;
    while(1) {
        if (queue.receive(&event, portMAX_DELAY)) {
            processEvent(&event);
        }
    }
}

void TelemetryTX::processEvent(obj::UAVLinkEvent* event) {
  obj::UAVLinkObject* obj = event->obj;

  bool success = false;
  uint8_t retries = 0;

  if (event->event == UAVLinkObject::EVENT_UPDATED || event->event == UAVLinkObject::EVENT_PERIODIC) {
          while(retries < MAX_RETRIES && !success) {
          success = UAVLink::sendObject(uavlink, obj, obj->telemetryIsAcked(), 250);
          if (!success) {
              retries++;
          }
      }
  }
}

int32_t TelemetryTX::transmitData(uint8_t *data, int32_t length) {
  TELEMETRY_PORT::write(data, length);

  return length;
}
