/*
 * Telemetry.cpp
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#include "inc/TelemetryRX.h"

#define TASK_PRIORITY	   (tskIDLE_PRIORITY + 2)

TelemetryRX::TelemetryRX()  :
  Thread("TelemetryRX", TASK_PRIORITY) {}

void TelemetryRX::init(UAVLink::Instance* ulink)
{
    this->uavlink = ulink;
}

void TelemetryRX::run()
{
    while(1) {

	while(TELEMETRY_PORT::bytesAvailable()) {
	    UAVLink::receive(uavlink, TELEMETRY_PORT::read());
	}
    }
}
