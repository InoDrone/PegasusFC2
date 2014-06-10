/*
 * UAVLinkObject.cpp
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#include "inc/UAVLinkObject.h"

namespace obj {

	UAVLinkObject::UAVLinkObjectList UAVLinkObject::_objRegistred;

	UAVLinkObject::UAVLinkObject(const char* name,uint32_t objid, bool isSettings, uint16_t size) :
	    _mId(objid),
	    _mIsSettings(isSettings),
	    _mName(name),
	    _mSize(size) {

	  mutex = xSemaphoreCreateRecursiveMutex();

	  xSemaphoreTakeRecursive(mutex, portMAX_DELAY);
	  _objRegistred.append(this);
	  xSemaphoreGiveRecursive(mutex);
	}

	void UAVLinkObject::dispatchEvent(EVENT_TYPE type) {
	  UAVLinkEvent eventMsg;
	  eventMsg.event = type;
	  eventMsg.obj = this;

	  _mEvents.start();
	  while(_mEvents.hasNext()) {
		ObjectEvent *event = _mEvents.next();
		if ((event->event & type) != 0) {
		    event->queue->send(&eventMsg, 0);
		}
	  }
	}

	// Connect with UAVLinkListener
	/*bool UAVLinkObject::connect(UAVLinkListener* listener)
	{
	  return false;
	}*/

	// Connect with xQueueHandle
	bool UAVLinkObject::connect(os::Queue* queue, uint8_t evMask) {
	  ObjectEvent* event = new ObjectEvent;
	  event->queue = queue;
	  event->event = evMask;

	  xSemaphoreTakeRecursive(mutex, portMAX_DELAY);
	  if (!_mEvents.append(event)) {
	      xSemaphoreGiveRecursive(mutex);
	      return false;
	  }

	  xSemaphoreGiveRecursive(mutex);
	  return true;
	}

	UAVLinkObject::UAVLinkObjectList* UAVLinkObject::getRegistredObjects() {
	  return &_objRegistred;
	}

	UAVLinkObject* UAVLinkObject::getById(uint32_t id) {
        UAVLinkObject* object = (UAVLinkObject *)NULL;

        //xSemaphoreTakeRecursive(mutex, portMAX_DELAY);
        _objRegistred.start();
        while (_objRegistred.hasNext()) {
          UAVLinkObject* obj = _objRegistred.next();
          if (obj->_mId == id) {
              object = obj;
          }
        }
        //xSemaphoreGiveRecursive(mutex);

        return object;
    }

	bool UAVLinkObject::telemetryIsAcked() {
	  return ((meta.flags >> TELEMETRY_ACKED_SHIFT) & 1) == 1;
	}

	UAVLinkObject::UPDATE_MODE UAVLinkObject::telemetryUpdateMode() {
	  return (UAVLinkObject::UPDATE_MODE)((meta.flags >> TELEMETRY_UPDATEON_SHIFT) & 0x03);
	}

	uint16_t UAVLinkObject::telemetryUpdatePeriod() {
	  return meta.updatePeriod;
	}

	void UAVLinkObject::pack(uint8_t *out) {
	  xSemaphoreTakeRecursive(mutex, portMAX_DELAY);
	  toBytes(out);
	  xSemaphoreGiveRecursive(mutex);
	}

	void UAVLinkObject::unpack(const uint8_t *in) {
	  xSemaphoreTakeRecursive(mutex, portMAX_DELAY);
	  fromBytes(in);
	  dispatchEvent(EVENT_RECEIVED);
	  xSemaphoreGiveRecursive(mutex);
	}

} /* namespace obj */
