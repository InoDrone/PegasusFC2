/*
 * Telemetry.cpp
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#include "inc/TelemetryTX.h"

#define QUEUE_SIZE         80
#define TASK_PRIORITY	   (tskIDLE_PRIORITY + 2)

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
      object->connect(&queue, UAVLinkObject::EVENT_UPDATED);
      TELEMETRY_PORT::write(object->getName());
      TELEMETRY_PORT::write("\r\n");
  }
}

void TelemetryTX::run()
{
    UAVLinkEvent event;
    while(1) {
        if (queue.receive(&event, portMAX_DELAY)) {
//            /UAVLinkObject* obj = event.obj;
            /*TELEMETRY_PORT::write("OBJECT : ");
            TELEMETRY_PORT::write(obj->getName());
            TELEMETRY_PORT::write("\r\n");*/
        }
    }
}

int32_t TelemetryTX::transmitData(uint8_t *data, int32_t length) {
  TELEMETRY_PORT::write(data, length);

  return length;
}
