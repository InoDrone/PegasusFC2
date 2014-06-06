/*
 * Sensors.h
 *
 *  Created on: 6 juin 2014
 *      Author: alienx
 */

#ifndef SENSORS_H_
#define SENSORS_H_

#include <boards.h>

class Sensors : public os::Thread, public os::Module
{
public:
  Sensors ();

  void init();
  void start() {
    Thread::start();
  }
  void run();

private:
  obj::AccelSensor* accel;
};

#endif /* SENSORS_H_ */
