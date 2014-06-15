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

#include "inc/Vector3.h"

namespace math {

            // Vector dot product
            template <typename T>
            T Vector3<T>::operator *(const Vector3<T> &v) const
            {
                return x*v.x + y*v.y + z*v.z;                                  // eq.8
            }

            template <typename T>
            float Vector3<T>::length() const
            {
                return sqrtf(x*x + y*y + z*z);
            }

            // Vector cross product
            template <typename T>
            Vector3<T> Vector3<T>::operator %(const Vector3<T> &v) const
            {
                Vector3<T> tmp(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);  // eq.10
                return tmp;
            }


            //add
            template <typename T>
            Vector3<T> Vector3<T>::operator +(const Vector3<T> &v) const
            {
                return Vector3<T>(x+v.x, y+v.y, z+v.z);
            }
            template <typename T>
            Vector3<T> &Vector3<T>::operator +=(const Vector3<T> &v)
            {
                x+=v.x; y+=v.y; z+=v.z;
                return *this;
            }

            //sub
            template <typename T>
            Vector3<T> Vector3<T>::operator -(const Vector3<T> &v) const
            {
                return Vector3<T>(x-v.x, y-v.y, z-v.z);
            }
            template <typename T>
            Vector3<T> &Vector3<T>::operator -=(const Vector3<T> &v)
            {
                x -= v.x; y -= v.y; z -= v.z;
                return *this;
            }

            //mul
            template <typename T>
            Vector3<T> Vector3<T>::operator *(const T num) const
            {
                return Vector3<T>(x*num, y*num, z*num);
            }
            template <typename T>
            Vector3<T> &Vector3<T>::operator *=(const T num)
            {
                x*=num; y*=num; z*=num;
                return *this;
            }

            //div
            template <typename T>
            Vector3<T> &Vector3<T>::operator /=(const T num)
            {
                x /= num; y /= num; z /= num;
                return *this;
            }

            template <typename T>
            Vector3<T> Vector3<T>::operator /(const T num) const
            {
                return Vector3<T>(x/num, y/num, z/num);
            }



            template Vector3<float> Vector3<float>::operator %(const Vector3<float> &v) const;
            template float Vector3<float>::length() const;
            template float Vector3<float>::operator *(const Vector3<float> &v) const;
            template Vector3<float> &Vector3<float>::operator *=(const float num);
            template Vector3<float> Vector3<float>::operator *(const float num) const;
            template Vector3<float> &Vector3<float>::operator -=(const Vector3<float> &v);
            template Vector3<float> Vector3<float>::operator -(const Vector3<float> &v) const;
            template Vector3<float> &Vector3<float>::operator +=(const Vector3<float> &v);
            template Vector3<float> Vector3<float>::operator +(const Vector3<float> &v) const;
            template Vector3<float> &Vector3<float>::operator /=(const float num);
            template Vector3<float> Vector3<float>::operator /(const float num) const;

} /* namespace pegasus */
