/*
 * Pid.cpp
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */


#include "inc/Pid.h"
#include <string.h>

Pid::Pid()
{
    memset(&level, 0, sizeof(PIDConfig));
    memset(&rate, 0, sizeof(PIDConfig));
}

void Pid::resetIntegral()
{
        level.integral = 0;
        rate.integral = 0;
}

float Pid::calculate(float target, float current, float G_Dt, PIDConfig* pid) {
    return calculate ( (target - current) , G_Dt, pid);
}


float Pid::calculate(float error, float G_Dt, PIDConfig* pid) {

        /* Calculate PTERM */
        pid->pTerm = pid->kP * error;

        /* Calculate I term */
        pid->integral += error * (pid->kI * G_Dt);
        pid->iTerm = pid->integral;
        if (pid->iTerm > pid->ILimit) {
            pid->iTerm = pid->integral = pid->ILimit;
        } else if (pid->iTerm < -pid->ILimit){
            pid->iTerm = pid->integral = -pid->ILimit;
        }

        /* Calculate D Term */
        if (pid->kD) {
            pid->dTerm = pid->kD * (error - pid->lastError);
            pid->lastError = error;
        } else {
            pid->dTerm = 0.0f;
        }

        pid->out = pid->pTerm + pid->iTerm + pid->dTerm;

        if (pid->out > pid->OMax) {
            pid->out = pid->OMax;
        } else if (pid->out < -pid->OMax) {
            pid->out = -pid->OMax;
        }

        return pid->out;
}

float Pid::calculate(float error, float gyro, float G_Dt, uint8_t type) {
    float out = 0.0f;

    if (type == PID_PARRA) {
        out = calculate(error, G_Dt, &level);
        out += calculate(0.0f, gyro, G_Dt, &rate);
    } else if (type == PID_CASCADING) {
        out = calculate (calculate(error, G_Dt, &level), gyro, G_Dt, &rate);
    } else if (type == PID_YAW_MANUAL ) {
        out = calculate (error, gyro, G_Dt, &rate);
    }

    return out;
}

void Pid::setRateConfig(float Kp, float Ki, float Kd, float ILimit)
{
    rate.kP = Kp;
    rate.kI = Ki;
    rate.kD = Kd;
    rate.ILimit = ILimit;
}

void Pid::setLevelConfig(float Kp, float Ki, float ILimit)
{
    level.kP = Kp;
    level.kI = Ki;
    level.kD = 0;
    level.ILimit = ILimit;
}

void Pid::setRateBound(float value)
{
    rate.OMax = value;
}

void Pid::setLevelBound(float value)
{
    level.OMax = value;
}

float Pid::calculate(float target, float current, float G_Dt)
{
    return calculate(target, current, G_Dt, &rate);
}

void Pid::reset()
{
    level.integral = 0;
    rate.integral = 0;
}
