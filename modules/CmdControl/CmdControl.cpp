/*
 * Control.cpp
 *
 *  Created on: 13 juin 2014
 *      Author: alienx
 */

#include "inc/CmdControl.h"

#define TASK_PRIORITY (tskIDLE_PRIORITY + 3)
#define STACK_SIZE_BYTES  1024

using namespace obj;

CmdControl::CmdControl () :
    Thread("Control", TASK_PRIORITY, STACK_SIZE_BYTES / 4) {}

void CmdControl::init()
{
    controlCommand = ControlCommand::instance();
    ControlCommandSettings::instance();
    AttitudeDesired::instance();

    os::Receiver::attach<RC1>(0); // Throttle
    os::Receiver::attach<RC2>(1); // Roll
    os::Receiver::attach<RC3>(2); // Pitch
    os::Receiver::attach<RC4>(3); // Yaw

    settings = ControlCommandSettings::instance()->get();

    ControlCommandSettings::instance()->connect(this);
}

void CmdControl::run()
{
    ControlCommand::Datas cmd;

    lastTime = xTaskGetTickCount();
    float channels[8] = {0};

    while (1) {
        // Update every 20 ms
        delayUntilMS(&lastTime, 20);

        for (uint8_t i = 0; i < 8; i++) {
            cmd.Channel[i] = os::Receiver::raw(i);
            channels[i] = scaleChannel(cmd.Channel[i], settings.ChannelMax[i], settings.ChannelMin[i], settings.ChannelNeutral[i]);
        }

        cmd.Throttle = channels[0];
        cmd.Roll = channels[1];
        cmd.Pitch = channels[2];
        cmd.Yaw = channels[3];

        controlCommand->set(cmd);

        updateAttitudeDesired(&cmd);
    }
}

void  CmdControl::uavlinkHandle(void *params)
{
    UAVLinkObject::UAVLinkEvent* event = reinterpret_cast<UAVLinkObject::UAVLinkEvent*>(params);
    if (event == NULL) {
        return;
    }

    if (event->obj->getId() == CONTROLCOMMANDSETTINGS_ID) {
        settings = ControlCommandSettings::instance()->get();
    }
}

float CmdControl::scaleChannel(int16_t value, int16_t max, int16_t min, int16_t neutral)
{
    float scaledValue;

    if ((max > min && value >= neutral) || (min > max && value <= neutral)) {
        if (max != neutral) {
            scaledValue = (float)(value - neutral) / (float)(max - neutral);
        } else {
            scaledValue = 0;
        }
    } else {
        if (min != neutral) {
            scaledValue = (float)(value - neutral) / (float)(neutral - min);
        } else {
            scaledValue = 0;
        }
    }

    if (scaledValue > 1.0f) {
        scaledValue = 1.0f;
    } else if (scaledValue < -1.0f) {
        scaledValue = -1.0f;
    }

    return scaledValue;
}

void CmdControl::updateAttitudeDesired(ControlCommand::Datas *cmd)
{
    AttitudeDesired::Datas att = AttitudeDesired::instance()->get();
    StabSettings::Datas stabSettings = StabSettings::instance()->get();

    att.Pitch = cmd->Pitch * stabSettings.TiltMax;
    att.Roll = cmd->Roll * stabSettings.TiltMax;
    att.Yaw = cmd->Yaw * 175; // TODO: add to settings

    att.Throttle = (cmd->Throttle < 0) ? -1 : cmd->Throttle;

    AttitudeDesired::instance()->set(att);
}

CmdControl cmdControl;
