/*
 * Control.h
 *
 *  Created on: 13 juin 2014
 *      Author: alienx
 */

#ifndef CMDCONTROL_H_
#define CMDCONTROL_H_
#include <boards.h>

class CmdControl : public os::Thread,
        public os::Module,
        public UAVLinkCallbackListener
{
    public:
        CmdControl ();

        void init();
        void start() {
          Thread::start();
        }
        void run();

        void uavlinkHandle(void *params);
    private:
        obj::ControlCommand* controlCommand;
        uint32_t lastTime;
        obj::ControlCommandSettings::Datas settings;

        float scaleChannel(int16_t value, int16_t max, int16_t min, int16_t neutral);
        void updateAttitudeDesired(obj::ControlCommand::Datas *cmd);
};

#endif /* CMDCONTROL_H_ */
