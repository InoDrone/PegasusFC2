/*
 * Stablization.h
 *
 *  Created on: 15 juin 2014
 *      Author: alienx
 */

#ifndef STABLIZATION_H_
#define STABLIZATION_H_

#include <boards.h>
#include <Pid.h>


class Stablization : public os::Module, public os::Thread, public UAVLinkCallbackListener
{
    public:
        Stablization ();

        void init();
        void start() {
          Thread::start();
        }
        void run();

        void uavlinkHandle(void *params);
    private:
        obj::StabSettings::Datas settings;

        Pid rollPID;
        Pid pitchPID;
        Pid yawPID;

        os::Queue gyroQueue;

        int16_t scaleChannel(float value, int16_t max, int16_t min, int16_t neutral);
};

#endif /* STABLIZATION_H_ */
