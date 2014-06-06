/*
 * Telemetry.h
 *
 *  Created on: 5 juin 2014
 *      Author: alienx
 */

#ifndef TELEMETRY_H_
#define TELEMETRY_H_

#include <boards.h>
#include "TelemetryRX.h"
#include "TelemetryTX.h"

class Telemetry : public os::Module {
public:
    Telemetry();

    void init();
    void start();


private:
    TelemetryRX rx;
    TelemetryTX tx;
    UAVLink::Instance* uavlink;
};

#endif /* TELEMETRY_H_ */
