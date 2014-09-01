/*
 * Altitude.h
 *
 *  Created on: 20 juin 2014
 *      Author: alienx
 */

#ifndef ALTITUDE_H_
#define ALTITUDE_H_

#define BARO_AVG_SIZE 16

#include <boards.h>
#include <devices/MS5611.h>
#include <cmath>

class Altitude : public os::Module, public os::Thread
{
    public:
        Altitude ();

        void init();
        void start() {
          Thread::start();
        }
        void run();
    private:
        float temp;
        float pressure;
        float _mAltitude;
        float _baroHistory[BARO_AVG_SIZE];
        float _baroAvg;
        uint8_t _baroIdx;

        float calculateVelocity(float G_Dt);
};

#endif /* ALTITUDE_H_ */
