/*
 * Receiver.cpp
 *
 *  Created on: 15 juin 2014
 *      Author: alienx
 */

#include "inc/Receiver.h"

namespace os
{
    uint16_t* Receiver::_values[8];

    uint16_t Receiver::raw(uint8_t channel)
    {
        return *_values[channel];
    }

} /* namespace os */
