/*
 * UAVLinkObject.h
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#ifndef UAVLINKOBJECT_H_
#define UAVLINKOBJECT_H_

#include <Queue.h>

namespace obj {

class UAVLinkObject {
public:
	enum EVENT_TYPE {
		OBJ_UPDATED,
		OBJ_RECEIVED
	};

	virtual static bool init() = 0;

	bool connect(UAVLinkListener* listener);
	bool connect(Queue* queue);

protected:
	void static dispatchEvent(EVENT_TYPE type);

};

} /* namespace obj */

#endif /* UAVLINKOBJECT_H_ */
