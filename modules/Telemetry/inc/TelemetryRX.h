/*
 * Telemetry.h
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#ifndef TELEMETRYRX_H_
#define TELEMETRYRX_H_

#include <pegasusos.h>

class TelemetryRX : public os::Thread
{
  public:
    TelemetryRX () :
      Thread("TelemetryRX", 1) {

	init();

	/* Start Task */
	start();
    }

  private:
	  void init();
	  void run();
};

#endif /* TELEMETRY_H_ */
