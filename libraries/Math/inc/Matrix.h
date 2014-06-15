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

#ifndef MATRIX_H_
#define MATRIX_H_

#include "Vector3.h"

namespace math {

            template <typename T>
            class Matrix
            {
                public:
                    Vector3<T> a,b,c;

                    Matrix<T>() {}

                    Matrix<T>(const Vector3<T> va, const Vector3<T> vb, const Vector3<T> vc) :
                        a(va),
                        b(vb),
                        c(vc) {
                    }

                    void setup() {
                        a.x = b.y = b.z = 1;
                        a.y = a.z = 0;
                        b.x = b.z = 0;
                        c.x = c.y = 0;
                    }

                    void multiply(const Vector3<T> &gyro);
                    void toEULER(float *roll, float* pitch, float *yaw);
                    void fromEuler(float roll, float pitch, float yaw);
                    void toQuat(float q[4]);

                    // add
                    Matrix<T> operator        + (const Matrix<T> &m) const
                    {
                        return Matrix<T>(a+m.a, b+m.b, c+m.c);
                    }
                    Matrix<T> &operator        += (const Matrix<T> &m)
                    {
                        return *this = *this + m;
                    }

                    // sub
                    Matrix<T> operator        - (const Matrix<T> &m) const
                    {
                        return Matrix<T>(a-m.a, b-m.b, c-m.c);
                    }
                    Matrix<T> &operator        -= (const Matrix<T> &m)
                    {
                        return *this = *this - m;
                    }
            };

            typedef Matrix<int16_t>   Matrix3i;
            typedef Matrix<float>     Matrix3f;


} /* namespace pegasus */

#endif /* MATRIX_H_ */
