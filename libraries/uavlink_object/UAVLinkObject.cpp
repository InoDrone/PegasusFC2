/*
 * UAVLinkObject.cpp
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#include "inc/UAVLinkObject.h"

namespace obj {

	void UAVLinkObject::dispatchEvent(EVENT_TYPE type) {
		UAVLinkEvent event = {
			.obj = this,
			.event = type
		};
	}

	// Connect with UAVLinkListener
	bool UAVLinkObject::connect(UAVLinkListener* listener)
	{
	  return false;
	}

	// Connect with xQueueHandle
	bool UAVLinkObject::connect(Queue* queue) {
	  return false;
	}

} /* namespace obj */
