/*
 * Math.cpp
 *
 *  Created on: 12 juin 2014
 *      Author: alienx
 */
#include "inc/Math.h"

    uint16_t Math::constrain(uint16_t value, uint16_t min, uint16_t max) {
        if (value > max) {
            return max;
        } else if (value < min) {
            return min;
        }

        return value;
    }

    int16_t Math::constrain(int16_t value, int16_t min, int16_t max) {
        if (value > max) {
            return max;
        } else if (value < min) {
            return min;
        }

        return value;
    }

    float Math::constrain(float value, float min, float max) {
        if (value > max) {
            return max;
        } else if (value < min) {
            return min;
        }

        return value;
    }

    void Math::filterInit(filter_t *f, float fe)
    {
        f->fe = fe;
        f->x0 = 0.0;
    }

    float Math::filter(filter_t *f, float v, float G_Dt)
    {
        float x1;

        x1 = f->x0 + (v-f->x0) * (G_Dt / f->fe);
        f->x0 = x1;

        return x1;
    }

    float Math::filter(float current, float previous, float factor)
    {
        if (factor != 1.0) {
            return (previous * (1.0f - factor) + (current * factor));
        }

        return current;
    }

    // ****** find quaternion from roll, pitch, yaw ********
    void Math::RPY2Quaternion(const float rpy[3], float q[4])
    {
        float phi, theta, psi;
        float cphi, sphi, ctheta, stheta, cpsi, spsi;

        phi    = DEG2RAD(rpy[0] / 2);
        theta  = DEG2RAD(rpy[1] / 2);
        psi    = DEG2RAD(rpy[2] / 2);
        cphi   = cosf(phi);
        sphi   = sinf(phi);
        ctheta = cosf(theta);
        stheta = sinf(theta);
        cpsi   = cosf(psi);
        spsi   = sinf(psi);

        q[0]   = cphi * ctheta * cpsi + sphi * stheta * spsi;
        q[1]   = sphi * ctheta * cpsi - cphi * stheta * spsi;
        q[2]   = cphi * stheta * cpsi + sphi * ctheta * spsi;
        q[3]   = cphi * ctheta * spsi - sphi * stheta * cpsi;

        if (q[0] < 0) { // q0 always positive for uniqueness
            q[0] = -q[0];
            q[1] = -q[1];
            q[2] = -q[2];
            q[3] = -q[3];
        }
    }

    // ****** find roll, pitch, yaw from quaternion ********
    void Math::quaternion2RPY(const float q[4], float rpy[3])
    {
        float R13, R11, R12, R23, R33;
        float q0s = q[0] * q[0];
        float q1s = q[1] * q[1];
        float q2s = q[2] * q[2];
        float q3s = q[3] * q[3];

        R13    = 2.0f * (q[1] * q[3] - q[0] * q[2]);
        R11    = q0s + q1s - q2s - q3s;
        R12    = 2.0f * (q[1] * q[2] + q[0] * q[3]);
        R23    = 2.0f * (q[2] * q[3] + q[0] * q[1]);
        R33    = q0s - q1s - q2s + q3s;

        rpy[1] = RAD2DEG(asinf(-R13)); // pitch always between -pi/2 to pi/2
        rpy[2] = RAD2DEG(atan2f(R12, R11));
        rpy[0] = RAD2DEG(atan2f(R23, R33));

        // TODO: consider the cases where |R13| ~= 1, |pitch| ~= pi/2
    }
