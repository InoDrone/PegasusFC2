/*
 * Telemetry.h
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#ifndef TELEMETRYRX_H_
#define TELEMETRYRX_H_

#include <boards.h>

class TelemetryRX : public os::Thread
{
  public:
    TelemetryRX ();

    void init(UAVLink::Instance* uavlink);
    inline void start() {
      Thread::start();
    }
    void run();

  private:
    UAVLink::Instance* uavlink;
};

#endif /* TELEMETRY_H_ */
