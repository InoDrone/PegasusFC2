/*
 * Telemetry.h
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#ifndef TELEMETRYTX_H_
#define TELEMETRYTX_H_

#include <pegasusos.h>

class TelemetryTX : public os::Thread, public os::Module
{
  public:
    TelemetryTX () :
      Thread("TelemetryTX", 1),
      Module() {}

    void init();
    inline void start() {
      Thread::start();
    }
    void run();
};

#endif /* TELEMETRY_H_ */
