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
  Thread("Sensors", TASK_PRIORITY, configMINIMAL_STACK_SIZE * 4) {}

void Sensors::init() {

  accel = AccelSensor::instance();
  gyro = GyroSensor::instance();
  attitudeSettings = AttitudeSettings::instance();
  attitudeSettings->connect(this);

  settings = attitudeSettings->get();


  //os::hal::PE3::init(GPIO_Mode_OUT, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN );
}

void Sensors::run() {

    AccelSensor::Datas accelDatas;
    GyroSensor::Datas gyroDatas;
    uint8_t mpuSample = 0;

    int32_t accAccum[3] = { 0, 0, 0 };
    int32_t gyroAccum[3] = {0, 0, 0 };

    float gyroScaling = 0;
    float accScaling = 0;

    while(1) {
            mpuSample = 0;
            memset(accAccum, 0, sizeof(accAccum));
            memset(gyroAccum, 0, sizeof(gyroAccum));

            {
                MPU6000::DatasValues mpuDatas;
                os::Queue* mpuQueue = MPU6000::getQueue();
                while (mpuQueue->receive(&mpuDatas, mpuSample == 0 ? 10 : 0)) {
                        accAccum[0] += mpuDatas.acc.x;
                        accAccum[1] += mpuDatas.acc.y;
                        accAccum[2] += mpuDatas.acc.z;

                        gyroAccum[0] += mpuDatas.gyro.x;
                        gyroAccum[1] += mpuDatas.gyro.y;
                        gyroAccum[2] += mpuDatas.gyro.z;

                    mpuSample++;
                }

                if (mpuSample == 0) {
                    continue;
                }

                gyroScaling = MPU6000::getGyroScale();
                accScaling = MPU6000::getAccelScale();

                gyroDatas.temperature = accelDatas.temperature = 35.0f + ((float)mpuDatas.temp + 512.0f) / 340.0f;
            }

            /* Accel values scaling - offset */
            accelDatas.x = (float)accAccum[0] / mpuSample;
            accelDatas.x = accelDatas.x * accScaling - settings.AccelBias.X;
            //accelDatas.x = -accelDatas.x;

            accelDatas.y = (float)accAccum[1] / mpuSample;
            accelDatas.y = accelDatas.y * accScaling - settings.AccelBias.Y;

            accelDatas.z = (float)accAccum[2] / mpuSample;
            accelDatas.z = accelDatas.z * accScaling - settings.AccelBias.Z;


            accel->set(accelDatas);

            /* Gyro values scaling - offset */
            gyroDatas.x = (float)gyroAccum[0] / mpuSample;
            gyroDatas.x = (gyroDatas.x * gyroScaling) - settings.GyroBias.X;

            gyroDatas.y = (float)gyroAccum[1] / mpuSample;
            gyroDatas.y = (gyroDatas.y * gyroScaling) - settings.GyroBias.Y;

            gyroDatas.z = (float)gyroAccum[2] / mpuSample;
            gyroDatas.z = (gyroDatas.z * gyroScaling) - settings.GyroBias.Z;
            //gyroDatas.z = -gyroDatas.z;

            gyro->set(gyroDatas);

    }
}

void Sensors::uavlinkHandle(void *params) {
    UAVLinkObject::UAVLinkEvent* event = reinterpret_cast<UAVLinkObject::UAVLinkEvent*>(params);
    if (event == NULL) {
        return;
    }

    if (event->obj->getId() == ATTITUDESETTINGS_ID) {
        os::hal::PE1::high();
        settings = attitudeSettings->get();
    }
}

Sensors sensors;

