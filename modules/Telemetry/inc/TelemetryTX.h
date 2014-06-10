/*
 * Telemetry.h
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#ifndef TELEMETRYTX_H_
#define TELEMETRYTX_H_

#include <boards.h>

class TelemetryTX : public os::Thread, public UAVLinkListener
{
  public:
    TelemetryTX ();

    void init(UAVLink::Instance*);
    inline void start() {
      Thread::start();
    }
    void run();

    int32_t transmitData(uint8_t *data, int32_t length);

  private:
    os::Queue queue;
    UAVLink::Instance* uavlink;

    void processEvent(obj::UAVLinkEvent* event);
};

#endif /* TELEMETRY_H_ */
