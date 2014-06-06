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
    os::hal::PE2::init(GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN );
    os::hal::PE2::high();
}

void TelemetryRX::run()
{
    while(1) {

	while(TELEMETRY_PORT::bytesAvailable()) {
	    UAVLink::receive(uavlink, TELEMETRY_PORT::read());
	    os::hal::PE2::toggle();
	}
    }
}
