/*
 * UAVLink.h
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#ifndef UAVLINK_H_
#define UAVLINK_H_


#include "UAVLinkObject.h"

#include "UAVLinkListener.h"

namespace obj {
	typedef struct UAVLinkEvent {
		UAVLinkObject* obj;
		UAVLinkObject::EVENT_TYPE event;
	};
}


#endif /* UAVLINK_H_ */
