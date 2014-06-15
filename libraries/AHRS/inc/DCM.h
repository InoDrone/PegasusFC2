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
 */

#ifndef DCM_H_
#define DCM_H_

#include "Vector3.h"
#include "Matrix.h"
#include "Math.h"

#include "GyroSensor.h"
#include "AccelSensor.h"
#include "AttitudeState.h"


namespace AHRS
{
    class DCM
    {
            typedef obj::AccelSensor::Datas AccDatas;
            typedef obj::GyroSensor::Datas GyroDatas;
            typedef obj::AttitudeState::Datas AtitudeDatas;

        public:
            DCM();

            void update(AccDatas*, GyroDatas*, AtitudeDatas* att, float G_Dt);
            void matrixUpdate(float G_Dt);
            void normalize();
            void driftCorrection(float G_Dt);
            float pGain(float sRate);

            void reset();
            void setKp(float kp);
            void setKi(float ki);

        private:
            AccDatas* _acc;
            GyroDatas* _gyro;
            AtitudeDatas* _att;

            float _mKp;
            float _mKi;

            math::Vector3f _mOmegaP; // acc proportional
            math::Vector3f _mOmegaI; // Integral
            math::Vector3f _mOmega;  // Gyro Vector

            math::Vector3f _mAccelEF;

            math::Matrix3f _mDCM;

            math::Vector3f omegaISum;
            uint16_t omegaISumTime;
            float rateDelta;
    };

} /* namespace pegasus */

#endif /* DCM_H_ */
