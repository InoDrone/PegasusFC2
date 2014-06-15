/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Author: Marc Jacquier <marc@inodrone.com>
 *  Project: InoDronePegasus
 *
 *  Direction Cosine Matrix
 *  by William Premerlani and Paul Bizard
 *  (cf: http://gentlenav.googlecode.com/files/DCMDraft2.pdf)
 */

#include "inc/DCM.h"

namespace AHRS {

    using namespace math;

    DCM::DCM() :
    _mKp(0.2f),
    _mKi(0.00087f),//0.0087f
    _mOmegaP(0.0f, 0.0f, 0.0f),
    _mOmegaI(0.0f, 0.0f, 0.0f),
    _mDCM(),
    omegaISum(0.0f, 0.0f, 0.0f),
    omegaISumTime(0),
    rateDelta(0) {

        _mDCM.setup();

    }

    void DCM::update(AccDatas* acc, GyroDatas* gyro, AtitudeDatas* att, float G_dt)
    {
        this->_acc = acc;
        this->_gyro = gyro;
        this->_att = att;

        matrixUpdate(G_dt);

        normalize();

        driftCorrection(G_dt);

        _mDCM.toEULER(&att->Roll, &att->Pitch, &att->Yaw);
        _mDCM.toQuat(&att->q1);

    }

    void DCM::matrixUpdate(float G_dt)
    {
        _mOmega.zero();

        Vector3f vGyro;
        vGyro.x = DEG2RAD(_gyro->x); // in rad/s
        vGyro.y = DEG2RAD(_gyro->y); // in rad/s
        vGyro.z = DEG2RAD(_gyro->z); // in rad/s

        _mOmega = vGyro + _mOmegaI; // Add P I term


        _mDCM.multiply((_mOmega + _mOmegaP) * G_dt);
    }

    void DCM::normalize()
    {
        float error;
        float renorm;
        Vector3f tmp0,tmp1,tmp2;

        error = _mDCM.a * _mDCM.b;                          // eq.18 (X * Y)
        tmp0 = _mDCM.a - (_mDCM.b * (0.5f * error));        // eq.19 (X - ((error/2) * Y)
        tmp1 = _mDCM.b - (_mDCM.a * (0.5f * error));        // eq.19 (Y - ((error/2) * X)
        tmp2 = tmp0 % tmp1;                                 // eq.20 Z = X * Y (Cross product)

        // Renorm eq.21
        renorm = 1.0f / tmp0.length();//0.5f * (3 - (tmp0 * tmp0));
        _mDCM.a = tmp0 * renorm;

        renorm = 1.0f / tmp1.length();//0.5f * (3 - (tmp1 * tmp1));
        _mDCM.b = tmp1 * renorm;

        renorm = 1.0f / tmp2.length();//0.5f * (3 - (tmp2 * tmp2));
        _mDCM.c = tmp2 * renorm;

    }

    void DCM::driftCorrection(float G_Dt)
    {

        Matrix3f tmpDCM = _mDCM;
        Vector3f error;
        //Vector3f tmpScaledI;

        rateDelta += G_Dt;

        Vector3f vAcc;
        vAcc.x = _acc->x; // in m/s²
        vAcc.y = _acc->y; // in m/s²
        vAcc.z = _acc->z; // in m/s²
        vAcc.normalize();

        error = vAcc % tmpDCM.c; // Cross Product

        float spinRate = _mOmega.length();
        _mOmegaP = error * pGain(spinRate) * _mKp;

        if (spinRate < DEG2RAD(20)) {
            omegaISum += error * _mKi * rateDelta;
            omegaISumTime += rateDelta;
        }

        if (omegaISumTime >= 5) {
            // limit the rate of change of omega_I to the hardware
            // reported maximum gyro drift rate. This ensures that
            // short term errors don't cause a buildup of omega_I
            // beyond the physical limits of the device
            // _gyro_drift_limit 0.5 degrees/second/minute
            float change_limit = DEG2RAD(0.5/60) * omegaISumTime;
            omegaISum.x = Math::constrain(omegaISum.x, -change_limit, change_limit);
            omegaISum.y = Math::constrain(omegaISum.y, -change_limit, change_limit);
            omegaISum.z = Math::constrain(omegaISum.z, -change_limit, change_limit);
            _mOmegaI += omegaISum;
            omegaISum.zero();
            omegaISumTime = 0;
        }

        /*tmpScaledI = error * _mKi;
        _mOmegaI += tmpScaledI;*/

        rateDelta=0;
    }

    float DCM::pGain(float sRate)
    {
        if (sRate < DEG2RAD(50)) {
            return 1.0f;
        }
        if (sRate > DEG2RAD(500)) {
            return 10.0f;
        }
        return sRate/DEG2RAD(50);
    }

    void DCM::reset()
    {
        _mOmegaP.zero();
        _mOmegaI.zero();
        _mOmega.zero();

        _mDCM.fromEuler(0,0,0);

    }

    void DCM::setKp(float kp)
    {
        _mKp = kp;
    }

    void DCM::setKi(float ki)
    {
        _mKi = ki;
    }

} /* namespace AHRS */
