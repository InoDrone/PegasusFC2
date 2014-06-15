/*
 * Pid.h
 *
 *  Created on: 28 nov. 2013
 *      Author: alienx
 */

#ifndef PID_H_
#define PID_H_

#define PID_PARRA            0
#define PID_CASCADING        1
#define PID_YAW_MANUAL       2
#define PID_YAW_HEADING      3

#include <cinttypes>

class Pid {
    public:
        Pid();
        ~Pid() {};

        void resetIntegral();


        float calculate(float error, float gyro, float G_Dt, uint8_t type=PID_PARRA);

        void setRateConfig(float Kp, float Ki, float Kd, float ILimit);
        void setLevelConfig(float Kp, float Ki, float ILimit);

        void setRateBound(float value);
        void setLevelBound(float value);

        float calculate(float target, float current, float G_Dt);

        void reset();

    private:

        struct PIDConfig {
            float kP;
            float kI;
            float kD;
            float ILimit;
            float integral;
            float lastError;

            float OMax;

            float pTerm;
            float iTerm;
            float dTerm;

            float out;
        };

        PIDConfig level;
        PIDConfig rate;


        float calculate(float error, float G_Dt, PIDConfig* pid);
        float calculate(float target, float current, float G_Dt, PIDConfig* pid);
};


#endif /* PID_H_ */
