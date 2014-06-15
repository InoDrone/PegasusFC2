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

#include "inc/Matrix.h"
#include "inc/Math.h"

namespace math {

            template <typename T>
            void Matrix<T>::multiply(const Vector3<T> &gyro)
            {
                /*Matrix<T> tmp;          // eq.17
                tmp.a.x = 0;
                tmp.a.y = -gyro.z;
                tmp.a.z = gyro.y;

                tmp.b.x = gyro.z;
                tmp.b.y = 0;
                tmp.b.z = -gyro.x;

                tmp.c.x = -gyro.y;
                tmp.c.y = gyro.x;
                tmp.c.z = 0;*/

                Matrix<T> tmp;
                tmp.a.x = a.y * gyro.z - a.z * gyro.y; // a.x * 0 + a.y * gyro.z + a.z * -gyro.y;
                tmp.a.y = a.z * gyro.x - a.x * gyro.z; // a.x * -gyro.z + a.y * 0 + a.z * gyro.x;
                tmp.a.z = a.x * gyro.y - a.y * gyro.x; // a.x * gyro.y + a.y * -gyro.x + a.z * 0;

                tmp.b.x = b.y * gyro.z - b.z * gyro.y; // b.x * 0 + b.y * gyro.z + b.z * -gyro.y;
                tmp.b.y = b.z * gyro.x - b.x * gyro.z; // b.x * -gyro.z + b.y * 0 + b.z * gyro.x;
                tmp.b.z = b.x * gyro.y - b.y * gyro.x; // b.x * gyro.y + b.y * -gyro.x + b.z * 0;

                tmp.c.x = c.y * gyro.z - c.z * gyro.y; // c.x * 0 + b.y * gyro.z + c.z * -gyro.y;
                tmp.c.y = c.z * gyro.x - c.x * gyro.z; // c.x * -gyro.z + c.y * 0 + c.z * gyro.x;
                tmp.c.z = c.x * gyro.y - c.y * gyro.x; // c.x * gyro.y + c.y * -gyro.x + c.z * 0;

                (*this) += tmp;
            }

            template <typename T>
            void Matrix<T>::toEULER(float *roll, float* pitch, float *yaw)
            {
                *roll  = RAD2DEG(atan2f(c.y, c.z));
                *pitch = RAD2DEG(-asinf(c.x));
                *yaw   = RAD2DEG(atan2f(b.x, a.x));
            }

            template<typename T>
            void Matrix<T>::toQuat(float q[4]) {
                q[0] = sqrtf(1.0f + a.x + b.y + c.z) / 2.0;
                float w4 = (4.0 * q[0]);
                q[1] = (c.y - b.z) / w4;
                q[2] = (a.z - c.x) / w4;
                q[3] = (b.x - a.y) / w4;
            }

            template <typename T>
            void Matrix<T>::fromEuler(float roll, float pitch, float yaw)
            {
                float cp = cosf(pitch);
                float sp = sinf(pitch);
                float sr = sinf(roll);
                float cr = cosf(roll);
                float sy = sinf(yaw);
                float cy = cosf(yaw);

                a.x = cp * cy;
                a.y = (sr * sp * cy) - (cr * sy);
                a.z = (cr * sp * cy) + (sr * sy);
                b.x = cp * sy;
                b.y = (sr * sp * sy) + (cr * cy);
                b.z = (cr * sp * sy) - (sr * cy);
                c.x = -sp;
                c.y = sr * cp;
                c.z = cr * cp;
            }


            template void Matrix<float>::multiply(const Vector3<float> &g);
            template void Matrix<float>::toEULER(float *roll, float* pitch, float *yaw);
            template void Matrix<float>::fromEuler(float roll, float pitch, float yaw);
            template void Matrix<float>::toQuat(float q[4]);

} /* namespace pegasus */
