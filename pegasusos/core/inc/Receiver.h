/*
 * Receiver.h
 *
 *  Created on: 15 juin 2014
 *      Author: alienx
 */

#ifndef RECEIVER_H_
#define RECEIVER_H_

#include <cinttypes>

namespace os
{

    class Receiver
    {
        public:
            template<class RC>
            static bool attach(uint8_t channel);
            static uint16_t raw(uint8_t channel);

        private:
            static uint16_t* _values[8];
    };

    template<class RC>
    bool Receiver::attach(uint8_t channel)
    {
            _values[channel] = RC::getValuePtr();

            return true;
    }

} /* namespace os */

#endif /* RECEIVER_H_ */
