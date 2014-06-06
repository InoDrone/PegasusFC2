/*
 * Telemetry.cpp
 *
 *  Created on: 6 juin 2014
 *      Author: alienx
 */
#include "Telemetry.h"


Telemetry::Telemetry()  :
  Module(),
  rx(),
  tx(),
  uavlink(nullptr) {}

void Telemetry::init()
{
    uavlink = UAVLink::init(&tx);
    tx.init(uavlink);
    rx.init(uavlink);
}

void Telemetry::start()
{
  rx.start();
  tx.start();
}

Telemetry telemetry;
