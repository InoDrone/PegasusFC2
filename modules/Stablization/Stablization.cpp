/*
 * Stablization.cpp
 *
 *  Created on: 15 juin 2014
 *      Author: alienx
 */

#include "inc/Stablization.h"

#define TASK_PRIORITY (tskIDLE_PRIORITY + 3)
#define STACK_SIZE_BYTES  1024

using namespace obj;

float quadX[4][3] = {
        //ROLL | PITCH | YAW
        {  0.5, -0.5, -0.5 }, // Front Left , M1 , 0
        { -0.5, -0.5,  0.5 }, // Front right, M2 , 1
        { -0.5, 0.5, -0.5 }, // Rear Right , M3 , 2
        {  0.5, 0.5,  0.5 } // Rear Left   , M4 , 3
};

Stablization::Stablization () :
Thread("Stablization", TASK_PRIORITY, STACK_SIZE_BYTES / 4),
rollPID(),
pitchPID(),
yawPID() {}

void Stablization::init()
{
    StabDesired::instance(); // Init
    settings = StabSettings::instance()->get();

    gyroQueue.create(1, sizeof(UAVLinkObject::UAVLinkEvent));
    GyroSensor::instance()->connect(&gyroQueue, UAVLinkObject::EVENT_UPDATED);

    StabSettings::instance()->connect(this);

    UAVLinkObject::UAVLinkEvent event;
    event.obj = StabSettings::instance();
    uavlinkHandle(&event);
}

void Stablization::run()
{
    uint32_t timeval;
    AttitudeDesired::Datas attDesired;
    AttitudeState::Datas attitude;
    GyroSensor::Datas gyro;
    StabDesired::Datas stabDesired;
    UAVLinkObject::UAVLinkEvent ev;
    float G_Dt;

    while (1) {

        if (!gyroQueue.receive(&ev, 30 / portTICK_RATE_MS)) {
            os::hal::PE3::low();
            continue;
        }


        G_Dt = os::hal::Delay::diff(timeval) / 1000000.0f;
        timeval = os::hal::Delay::ticks();

        if (G_Dt > 0.01f) {
            G_Dt = 0.01f;
        } else if (G_Dt <= 0.001f) {
            G_Dt = 0.001f;
        }

        attDesired = AttitudeDesired::instance()->get();
        attitude = AttitudeState::instance()->get();
        gyro = GyroSensor::instance()->get();

        float attError[3] = {
                attDesired.Roll - attitude.Roll,
                attDesired.Pitch - attitude.Pitch,
                attDesired.Yaw - attitude.Yaw
        };

        float modx = fmodf(attError[2] + 180.0f, 360.0f);
        if (modx < 0) {
            attError[2] = modx + 180.0f;
        } else {
            attError[2] = modx - 180.0f;
        }

        /* Filter gyro datas */
        float filteredGyro[3];
        float gyroAlpha = (settings.GyroTau < 0.0001f) ? 0 : expf(-G_Dt / settings.GyroTau);
        filteredGyro[0] = filteredGyro[0] * gyroAlpha + gyro.x * ( 1 - gyroAlpha);
        filteredGyro[1] = filteredGyro[1] * gyroAlpha + gyro.y * ( 1 - gyroAlpha);
        filteredGyro[2] = filteredGyro[2] * gyroAlpha + gyro.z * ( 1 - gyroAlpha);


        stabDesired.Roll = rollPID.calculate(attError[0], filteredGyro[0], G_Dt, PID_CASCADING);
        stabDesired.Pitch = pitchPID.calculate(attError[1], filteredGyro[1], G_Dt, PID_CASCADING);
        stabDesired.Yaw = 0;//yawPID.calculate(attError[2], filteredGyro[2], G_Dt, PID_CASCADING);


        stabDesired.Throttle = attDesired.Throttle;
        stabDesired.UpdateTime = G_Dt * 1000.0f;

        StabDesired::instance()->set(stabDesired);


        bool isArmed = stabDesired.Throttle >= 0.00f;
        float M1, M2, M3, M4;
        M1 = M2 = M3 = M4 = 1000; // Min PWM VAL

        if (!isArmed) {
            MOTOR1::write(1000);
            MOTOR2::write(1000);
            MOTOR3::write(1000);
            MOTOR4::write(1000);
        } else {
            float C[4];
            C[0] = stabDesired.Throttle +
                    (quadX[0][0] * stabDesired.Roll) +
                    (quadX[0][1] * stabDesired.Pitch) +
                    (quadX[0][2] * stabDesired.Yaw);
            C[1] = stabDesired.Throttle +
                    (quadX[1][0] * stabDesired.Roll) +
                    (quadX[1][1] * stabDesired.Pitch) +
                    (quadX[1][2] * stabDesired.Yaw);
            C[2] = stabDesired.Throttle +
                    (quadX[2][0] * stabDesired.Roll) +
                    (quadX[2][1] * stabDesired.Pitch) +
                    (quadX[2][2] * stabDesired.Yaw);
            C[3] = stabDesired.Throttle +
                    (quadX[3][0] * stabDesired.Roll) +
                    (quadX[3][1] * stabDesired.Pitch) +
                    (quadX[3][2] * stabDesired.Yaw);

            M1 = scaleChannel(C[0], 1700, 1000, 1300);
            M2 = scaleChannel(C[1], 1700, 1000, 1300);
            M3 = scaleChannel(C[2], 1700, 1000, 1300);
            M4 = scaleChannel(C[3], 1700, 1000, 1300);


            MOTOR1::write(M1);
            MOTOR2::write(M2);
            MOTOR3::write(M3);
            MOTOR4::write(M4);
        }



    }
}

void Stablization::uavlinkHandle(void *params) {
    UAVLinkObject::UAVLinkEvent* event = reinterpret_cast<UAVLinkObject::UAVLinkEvent*>(params);
    if (event == NULL) {
        return;
    }

    if (event->obj->getId() == STABSETTINGS_ID) {
        os::hal::PE3::toggle();
        settings = StabSettings::instance()->get();


        rollPID.setLevelBound(settings.MaxRate.X);
        pitchPID.setLevelBound(settings.MaxRate.Y);
        yawPID.setLevelBound(settings.MaxRate.Z);

        rollPID.setRateBound(1.0f);
        pitchPID.setRateBound(1.0f);
        yawPID.setRateBound(1.0f);

        /* Level/Attitude PID */
        rollPID.setLevelConfig(settings.TiltPI.Kp,
                               settings.TiltPI.Ki,
                               settings.TiltPI.ILimit);

        pitchPID.setLevelConfig(settings.TiltPI.Kp,
                                settings.TiltPI.Ki,
                                settings.TiltPI.ILimit);

        yawPID.setLevelConfig(settings.YawPI.Kp,
                              settings.YawPI.Ki,
                              settings.YawPI.ILimit);

        /* Rate PID */
        rollPID.setRateConfig(settings.TiltRatePID.Kp,
                              settings.TiltRatePID.Ki,
                              settings.TiltRatePID.Kd,
                              settings.TiltRatePID.ILimit);

        pitchPID.setRateConfig(settings.TiltRatePID.Kp,
                              settings.TiltRatePID.Ki,
                              settings.TiltRatePID.Kd,
                              settings.TiltRatePID.ILimit);

        yawPID.setRateConfig(settings.YawRatePID.Kp,
                              settings.YawRatePID.Ki,
                              settings.YawRatePID.Kd,
                              settings.YawRatePID.ILimit);


    }
}

int16_t Stablization::scaleChannel(float value, int16_t max, int16_t min, int16_t neutral)
{
    int16_t valueScaled;

    // Scale
    if (value >= 0.0f) {
        valueScaled = (int16_t)(value * ((float)(max - neutral))) + neutral;
    } else {
        valueScaled = (int16_t)(value * ((float)(neutral - min))) + neutral;
    }

    if (max > min) {
        if (valueScaled > max) {
            valueScaled = max;
        }
        if (valueScaled < min) {
            valueScaled = min;
        }
    } else {
        if (valueScaled < max) {
            valueScaled = max;
        }
        if (valueScaled > min) {
            valueScaled = min;
        }
    }

    return valueScaled;
}


Stablization stablization;
