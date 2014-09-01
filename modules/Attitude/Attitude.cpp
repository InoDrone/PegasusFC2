/*
 * Attitude.cpp
 *
 *  Created on: 11 juin 2014
 *      Author: alienx
 */

#include "inc/Attitude.h"

#define STACK_SIZE_BYTES 2048
#define TASK_PRIORITY (tskIDLE_PRIORITY + 3)
#define FAILSAFE_TIMEOUT_MS       10

#define GRAVITY 9.81f

using namespace obj;

Attitude::Attitude () :
  Thread("Attitude", TASK_PRIORITY, STACK_SIZE_BYTES / 4),
  dcm(),
  accFiltered() {}

void Attitude::init()
{
    accel = AccelSensor::instance();
    gyro = GyroSensor::instance();
    attitudeState = AttitudeState::instance();
    attitudeSettings = AttitudeSettings::instance();

    attitude = attitudeState->get();
    attitude.q1 = 1;
    attitude.q2 = 0;
    attitude.q3 = 0;
    attitude.q4 = 0;
    attitudeState->set(attitude);

    q[0] = 1;
    q[1] = 0;
    q[2] = 0;
    q[3] = 0;

    gyroCorrectBias[0] = 0;
    gyroCorrectBias[1] = 0;
    gyroCorrectBias[2] = 0;

    zVelocity = 0;


    /*accQueue.create(1, sizeof(UAVLinkObject::UAVLinkEvent));
    gyroQueue.create(1, sizeof(UAVLinkObject::UAVLinkEvent));

    accel->connect(&accQueue, UAVLinkObject::EVENT_UPDATED);
    gyro->connect(&gyroQueue, UAVLinkObject::EVENT_UPDATED);*/

    attitudeSettings->connect(this);

    UAVLinkObject::UAVLinkEvent event;
    event.obj = AttitudeSettings::instance();
    uavlinkHandle(&event);
}

void Attitude::run()
{

    uint32_t timeval;
    //UAVLinkObject::UAVLinkEvent event;
    AccelSensor::Datas accelDatas;
    GyroSensor::Datas gyroDatas;
    //AttitudeState::Datas attitudeDatas;
    float G_Dt;
    bool initialized = false;
    delay(100);

    while (1) {

        // Calibration / INIT
        if ( (xTaskGetTickCount() < 7000) && (xTaskGetTickCount() > 1000)) {
            gyroBiasRate = 0.01f;
            yawBiasRate = 0.01f;
            accelKp = 1.0f;
            accelKi = 0.0f;
            initialized = false;
        } else if (!initialized) {
            gyroBiasRate = 0.0f;
            yawBiasRate = 0.00001f;
            accelKp = settings.accelKp;
            accelKi = settings.accelKi;
            initialized = true;
        }


        if (updateSensors(&accelDatas, &gyroDatas)) {
            G_Dt = os::hal::Delay::diff(timeval) / 1000000.0f;
            timeval = os::hal::Delay::ticks();

            update(&accelDatas, &gyroDatas, G_Dt);
        }
    }
}

bool Attitude::updateSensors(AccelSensor::Datas *accelDatas, GyroSensor::Datas *gyroDatas)
{
    uint8_t mpuSample = 0;

    int32_t accAccum[3] = { 0, 0, 0 };
    int32_t gyroAccum[3] = {0, 0, 0 };

    float gyroScaling = 0;
    float accScaling = 0;

    mpuSample = 0;
    memset(accAccum, 0, sizeof(accAccum));
    memset(gyroAccum, 0, sizeof(gyroAccum));

    {
        MPU6000::DatasValues mpuDatas;
        os::Queue* mpuQueue = MPU6000::getQueue();
        while (mpuQueue->receive(&mpuDatas, mpuSample == 0 ? 4 : 0)) {
                accAccum[0] += mpuDatas.acc.x;
                accAccum[1] += mpuDatas.acc.y;
                accAccum[2] += mpuDatas.acc.z;

                gyroAccum[0] += mpuDatas.gyro.x;
                gyroAccum[1] += mpuDatas.gyro.y;
                gyroAccum[2] += mpuDatas.gyro.z;

            mpuSample++;
        }

        if (mpuSample == 0) {
            return false;
        }

        gyroScaling = MPU6000::getGyroScale();
        accScaling = MPU6000::getAccelScale();

        gyroDatas->temperature = accelDatas->temperature = 35.0f + ((float)mpuDatas.temp + 512.0f) / 340.0f;
    }

    /* Accel values scaling - offset */
    accelDatas->x = (float)accAccum[0] / mpuSample;
    accelDatas->x =  ((accelDatas->x * accScaling) * -1);

    accelDatas->y = (float)accAccum[1] / mpuSample;
    accelDatas->y = accelDatas->y * accScaling;
    //accelDatas->y *= -1; // Reverse

    accelDatas->z = (float)accAccum[2] / mpuSample;
    accelDatas->z = accelDatas->z * accScaling;


    /* Gyro values scaling - offset */
    gyroDatas->x = (float)gyroAccum[0] / mpuSample;
    gyroDatas->x = (gyroDatas->x * gyroScaling);// - settings.GyroBias.X;
    gyroDatas->x *= -1;

    gyroDatas->y = (float)gyroAccum[1] / mpuSample;
    gyroDatas->y = (gyroDatas->y * gyroScaling);// - settings.GyroBias.Y;
    //gyroDatas->y *= -1;

    gyroDatas->z = (float)gyroAccum[2] / mpuSample;
    gyroDatas->z = (gyroDatas->z * gyroScaling);// - settings.GyroBias.Z;


    if (settings.enableBias == 1) {
        accelDatas->x -= settings.AccelBias.X;
        accelDatas->y -= settings.AccelBias.Y;
        accelDatas->z -= settings.AccelBias.Z;

        gyroDatas->x += gyroCorrectBias[0];
        gyroDatas->y += gyroCorrectBias[1];
        gyroDatas->z += gyroCorrectBias[2];
    }

    gyroCorrectBias[0] += -gyroDatas->x * gyroBiasRate;
    gyroCorrectBias[1] += -gyroDatas->y * gyroBiasRate;

    gyroCorrectBias[2] += -gyroDatas->z * yawBiasRate;

    accel->set(*accelDatas);
    gyro->set(*gyroDatas);

    return true;

}

bool Attitude::update(obj::AccelSensor::Datas *accelDatas, obj::GyroSensor::Datas *gyroDatas, float G_Dt)
{
    math::Vector3f grot, accErr;//, accEarth;

    math::Vector3f vAccel;
    vAccel.x = accelDatas->x;
    vAccel.y = accelDatas->y;
    vAccel.z = accelDatas->z;
    //vAccel.normalize();


    if (G_Dt > 0.01f) {
        G_Dt = 0.01f;
    } else if (G_Dt <= 0.001f) {
        G_Dt = 0.001f;
    }

    // Filter Accel
    float accAlpha = (settings.AccFilter == 0 || settings.AccFilter < 0.0001f) ? 0 : expf(-G_Dt / settings.AccFilter);
    Math::filter(vAccel, &accFiltered, accAlpha);

    // Rotate Gravity
    grot.x = -(2 * (q[1] * q[3] - q[0] * q[2]));
    grot.y = -(2 * (q[2] * q[3] + q[0] * q[1]));
    grot.z = -(q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);

    // TODO: grot filter
    accErr = accFiltered % grot; // Cross Product Vector3

    //accEarth = grot * vAccel; // Dot product


    float accMag = vAccel.length();
    if (accMag < 1.0e-3f) {
        return false;
    }

    accErr.x /= accMag;
    accErr.y /= accMag;
    accErr.z /= accMag;

    zVelocity += (accErr.z * G_Dt);

    gyroCorrectBias[0] += accErr.x * accelKi;
    gyroCorrectBias[1] += accErr.y * accelKi;

    gyroDatas->x += accErr.x * accelKp / G_Dt;
    gyroDatas->y += accErr.y * accelKp / G_Dt;
    gyroDatas->z += accErr.z * accelKp / G_Dt;

    {
        float qdot[4];
        qdot[0] = DEG2RAD(-q[1] * gyroDatas->x - q[2] * gyroDatas->y - q[3] * gyroDatas->z) * G_Dt / 2;
        qdot[1] = DEG2RAD(q[0] * gyroDatas->x - q[3] * gyroDatas->y + q[2] * gyroDatas->z) * G_Dt / 2;
        qdot[2] = DEG2RAD(q[3] * gyroDatas->x + q[0] * gyroDatas->y - q[1] * gyroDatas->z) * G_Dt / 2;
        qdot[3] = DEG2RAD(-q[2] * gyroDatas->x + q[1] * gyroDatas->y + q[0] * gyroDatas->z) * G_Dt / 2;

        // Take a time step
        q[0]    = q[0] + qdot[0];
        q[1]    = q[1] + qdot[1];
        q[2]    = q[2] + qdot[2];
        q[3]    = q[3] + qdot[3];

        if (q[0] < 0) {
            q[0] = -q[0];
            q[1] = -q[1];
            q[2] = -q[2];
            q[3] = -q[3];
        }
    }

    // Renomalize
    float qmag = sqrtf(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
    q[0] = q[0] / qmag;
    q[1] = q[1] / qmag;
    q[2] = q[2] / qmag;
    q[3] = q[3] / qmag;

    // If quaternion has become inappropriately short or is nan reinit.
    // THIS SHOULD NEVER ACTUALLY HAPPEN
    if ((fabsf(qmag) < 1.0e-3f) || std::isnan(qmag)) {
        q[0] = 1.0f;
        q[1] = 0.0f;
        q[2] = 0.0f;
        q[3] = 0.0f;
    }

    attitude.q1 = q[0];
    attitude.q2 = q[1];
    attitude.q3 = q[2];
    attitude.q4 = q[3];
    Math::quaternion2RPY(&attitude.q1, &attitude.Roll);

    attitude.zVelocity = zVelocity;
    attitude.G_Dt = G_Dt;
    attitudeState->set(attitude);

    return true;
}

void Attitude::uavlinkHandle(void *params) {
    UAVLinkObject::UAVLinkEvent* event = reinterpret_cast<UAVLinkObject::UAVLinkEvent*>(params);
    if (event == NULL) {
        return;
    }

    if (event->obj->getId() == ATTITUDESETTINGS_ID) {
        settings = attitudeSettings->get();

        accelKp = settings.accelKp;
        accelKi = settings.accelKi;

        gyroCorrectBias[0] = settings.GyroBias.X;
        gyroCorrectBias[1] = settings.GyroBias.Y;
        gyroCorrectBias[2] = settings.GyroBias.Z;

        /*dcm.reset();
        dcm.setKp(settings.DCMkP);
        dcm.setKi(settings.DCMkI);*/
    }
}

Attitude attitude;
