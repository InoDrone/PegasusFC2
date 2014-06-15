/*
 * TimerInterrupt.h
 *
 *  Created on: 13 juin 2014
 *      Author: alienx
 */

#ifndef TIMERINTERRUPT_H_
#define TIMERINTERRUPT_H_

#include <stm32f4xx.h>
#include <stm32f4xx_tim.h>
#include <List.h>

namespace os
{
    namespace hal
    {
        typedef void (*TimerISRCallback)(void);

        class TimerInterrupt
        {
            public:
                struct TimerEvent {
                    TIM_TypeDef* reg;
                    uint8_t channel;
                    TimerISRCallback callback;
                };

                static bool attach(TIM_TypeDef* tim, uint8_t channel, TimerISRCallback callback);
                static void isr(TIM_TypeDef* tim);

            private:
                static util::List<TimerEvent*> _eventList;
        };

    } /* namespace hal */
} /* namespace os */

#endif /* TIMERINTERRUPT_H_ */
