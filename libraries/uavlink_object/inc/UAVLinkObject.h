/*
 * UAVLinkObject.h
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#ifndef UAVLINKOBJECT_H_
#define UAVLINKOBJECT_H_

#include <pegasusos.h>

namespace obj {

class UAVLinkObject {
public:
	typedef util::List<UAVLinkObject*> UAVLinkObjectList;
	enum EVENT_TYPE {
		EVENT_NONE,
		EVENT_UPDATED,
		EVENT_RECEIVED,
		EVENT_PERIODIC
	};

	enum UPDATE_MODE {
	    UPDATEMODE_MANUAL    = 0, /** Manually update object, by calling the updated() function */
	    UPDATEMODE_PERIODIC  = 1, /** Automatically update object at periodic intervals */
	    UPDATEMODE_ONCHANGE  = 2, /** Only update object when its data changes */
	};

	enum META_FLAGS {
	  TELEMETRY_ACKED_SHIFT,
	  TELEMETRY_UPDATEON_SHIFT
	};

	struct MetaDef {
	  uint8_t flags;
	  uint16_t updatePeriod;
	};
	//virtual UAVLinkObject* instance() = 0;

	//bool connect(UAVLinkListener* listener);
	bool connect(os::Queue* queue, uint8_t evMask);

	bool telemetryIsAcked();
	UPDATE_MODE telemetryUpdateMode();
	uint16_t telemetryUpdatePeriod();

	void pack(uint8_t *out);
	void unpack(const uint8_t *in);

	static UAVLinkObjectList* getRegistredObjects();
	static UAVLinkObject* getById(uint32_t id);

	uint16_t length() {
	  return _mSize;
	}

	const char* getName() {
	  return _mName;
	}

	uint32_t getId() {
	  return _mId;
	}

protected:
	UAVLinkObject(const char* name, uint32_t objid, bool isSettings, uint16_t size);
	void dispatchEvent(EVENT_TYPE type);

	virtual void toBytes(uint8_t *out) = 0;
	virtual void fromBytes(const uint8_t *in) = 0;

	xSemaphoreHandle mutex;
	MetaDef meta;

private:
	struct ObjectEvent {
	  os::Queue* queue;
	  uint8_t event;
	};

	util::List<ObjectEvent*> _mEvents;
	uint32_t _mId;
	bool _mIsSettings;
	const char* _mName;
	uint16_t _mSize;

	static UAVLinkObjectList _objRegistred;

};

struct UAVLinkEvent {
	UAVLinkObject* obj;
	UAVLinkObject::EVENT_TYPE event;
};

} /* namespace obj */

#endif /* UAVLINKOBJECT_H_ */
