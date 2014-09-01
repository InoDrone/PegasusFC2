/*
 * Altitude.cpp
 *
 *  Created on: 20 juin 2014
 *      Author: alienx
 */

#include "inc/Altitude.h"

#define STACK_SIZE_BYTES 550
#define TASK_PRIORITY (tskIDLE_PRIORITY + 1)

#define BARO_LPF_TAU    0.6f

Altitude::Altitude () :
Thread("Altitude", TASK_PRIORITY, STACK_SIZE_BYTES / 4) {}


void Altitude::init()
{
    obj::BaroSensor::instance();
    _mAltitude = 0;
    _baroAvg = 0;
    _baroIdx = 0;
    memset(_baroHistory, 0, sizeof(_baroHistory));
}

void Altitude::run()
{
    obj::BaroSensor::Datas baro;

    while (1) {

        // Read temperature
        MS5611::startADC(MS5611::TempConversion);
        delay(MS5611::getWaitingDelay());
        MS5611::readADC();

        // Read Pressure
        MS5611::startADC(MS5611::PressureConversion);
        delay(MS5611::getWaitingDelay());
        MS5611::readADC();

        temp = MS5611::getTemp();
        pressure = MS5611::getPressure();

        float altitude = 44330.0f * (1.0f - powf((pressure / MS5611_SEALEVEL_PRESS), (1.0f / 5.255f)));

        if (!std::isnan(altitude)) {

            _baroHistory[_baroIdx] = altitude;
            _baroAvg += _baroHistory[_baroIdx];
            _baroAvg -= _baroHistory[(_baroIdx + 1) % BARO_AVG_SIZE];
            _baroIdx++;
            if (_baroIdx == BARO_AVG_SIZE) _baroIdx = 0;

            _mAltitude = (_mAltitude * BARO_LPF_TAU) + (altitude * (1 - BARO_LPF_TAU)); // LPF Filter


            baro.Altitude = _mAltitude;
            baro.Pressure = pressure;
            baro.Temperature = temp;

            obj::BaroSensor::instance()->set(baro);
        }
    }
}

float Altitude::calculateVelocity(float G_Dt)
{
    static float velocity = 0.0f;
    static float lastAlt;
    float baroVelocity = (_mAltitude - lastAlt) * 100.0f / G_Dt; // to cm/s
    lastAlt = _mAltitude;

    // Integrator velocity cm/sec
    // accZ in m/s/s
    //velocity += ((accZ * 100.0f) * G_Dt);

    // Complementary Filter for correct accZ velocity drift
    velocity = (velocity * 0.985f) + (baroVelocity * 0.015f);

    return velocity;

}

#ifdef USE_BARO
Altitude _altitude;
#endif
