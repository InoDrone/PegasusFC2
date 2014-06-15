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

#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <inttypes.h>
#include <cmath>

namespace math {

            template <typename T>
            class Vector3
            {
                public:
                    T x,y,z;

                    Vector3<T>() {
                        x = y = z = 0;
                    }

                    Vector3<T>(const T _x, const T _y, const T _z) :
                            x(_x),
                            y(_y),
                            z(_z) {}

                    float length(void) const;
                    // vector dot product
                    T operator *(const Vector3<T> &v) const;
                    // vector cross product
                    Vector3<T> operator %(const Vector3<T> &v) const;

                    // add
                    Vector3<T> operator +(const Vector3<T> &v) const;
                    Vector3<T> &operator +=(const Vector3<T> &v);

                    // sub
                    Vector3<T> operator -(const Vector3<T> &v) const;
                    Vector3<T> &operator -=(const Vector3<T> &v);

                    //mul
                    Vector3<T> operator *(const T num) const;
                    Vector3<T> &operator *=(const T num);

                    //div
                    Vector3<T> operator /(const T num) const;
                    Vector3<T> &operator /=(const T num);

                    void normalize()
                    {
                        float norm = sqrtf ( x*x + y*y + z*z);
                        *this /= norm;
                    }

                    void zero()
                    {
                        x = y = z = 0;
                    }


            };

            typedef Vector3<int16_t>      Vector3i;
            typedef Vector3<uint16_t>      Vector3ui;
            typedef Vector3<float>        Vector3f;

} /* namespace pegasus */

#endif /* VECTOR3_H_ */
