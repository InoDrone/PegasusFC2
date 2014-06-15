/*
 * Sensors.h
 *
 *  Created on: 6 juin 2014
 *      Author: alienx
 */

#ifndef SENSORS_H_
#define SENSORS_H_

#include <boards.h>
#include <Math.h>


class Sensors : public os::Thread, public os::Module, public UAVLinkCallbackListener
{
public:
  Sensors ();

  void init();
  void start() {
    Thread::start();
  }
  void run();

  void uavlinkHandle(void *params);

private:
  obj::AccelSensor* accel;
  obj::GyroSensor* gyro;
  obj::AttitudeSettings* attitudeSettings;

  obj::AttitudeSettings::Datas settings;
};

#endif /* SENSORS_H_ */
