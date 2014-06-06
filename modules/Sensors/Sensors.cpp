/*
 * Sensors.cpp
 *
 *  Created on: 6 juin 2014
 *      Author: alienx
 */

#include "inc/Sensors.h"

#define TASK_PRIORITY (tskIDLE_PRIORITY + 3)

using namespace obj;

Sensors::Sensors () :
  Thread("Sensors", TASK_PRIORITY) {}

void Sensors::init() {

  accel = AccelSensor::instance();
  Attitude::instance();

  os::hal::PE3::init(GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN );
}

void Sensors::run() {

    AccelSensor::Datas accelDatas = accel->get();

    while(1) {
	accelDatas.x = 0.1f;
	accel->set(accelDatas);

	os::hal::PE3::toggle();
	delay(100);
    }
}

Sensors sensors;

