/*
 * Telemetry.cpp
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#include "inc/TelemetryTX.h"

void TelemetryTX::init()
{

}

void TelemetryTX::run()
{
    while(1) {
	TELEMETRY_PORT::write("HELLO PEGASUS\r\n");
	delay(1000);
    }
}

TelemetryTX tmtx;
