/*
 * Attitude.h
 *
 *  Created on: 11 juin 2014
 *      Author: alienx
 */

#ifndef MODULE_ATTITUDE_H_
#define MODULE_ATTITUDE_H_

#include <boards.h>
#include <DCM.h>
#include <Vector3.h>
#include <Math.h>

class Attitude : public os::Thread, public os::Module, public UAVLinkCallbackListener
{
public:
  Attitude ();

  void init();
  void start() {
    Thread::start();
  }
  void run();

  void uavlinkHandle(void *params);

private:
  obj::AccelSensor* accel;
  obj::GyroSensor* gyro;
  obj::AttitudeState* attitudeState;
  obj::AttitudeSettings* attitudeSettings;

  obj::AttitudeSettings::Datas settings;
  obj::AttitudeState::Datas attitude;

  os::Queue accQueue;
  os::Queue gyroQueue;

  AHRS::DCM dcm;

  float gyroCorrectBias[3];
  float gyroBiasRate;
  float yawBiasRate;
  float accelKi;
  float accelKp;
  float q[4];

  bool updateSensors(obj::AccelSensor::Datas *accelDatas, obj::GyroSensor::Datas *gyroDatas);
  bool update(obj::AccelSensor::Datas *accelDatas, obj::GyroSensor::Datas *gyroDatas, float G_Dt);
};

#endif /* ATTITUDE_H_ */
