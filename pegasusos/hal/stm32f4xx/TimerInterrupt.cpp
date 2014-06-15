/*
 * TimerInterrupt.cpp
 *
 *  Created on: 13 juin 2014
 *      Author: alienx
 */

#include "inc/TimerInterrupt.h"

namespace os
{
    namespace hal
    {

        util::List<TimerInterrupt::TimerEvent*> TimerInterrupt::_eventList;

        bool TimerInterrupt::attach(TIM_TypeDef* tim, uint8_t channel, TimerISRCallback callback) {

            TimerEvent* ev = new TimerEvent;
            ev->channel = channel;
            ev->reg = tim;
            ev->callback = callback;

            _eventList.append(ev);

            return true;
        }

        void TimerInterrupt::isr(TIM_TypeDef* reg)
        {
            TimerEvent *ev;
            _eventList.start();

            uint32_t state = ((reg->DIER  & reg->SR) & 0x1F);

            while (_eventList.hasNext()) {
                ev = _eventList.next();

                if (ev->reg == reg && (state & (1 << (ev->channel + 1)))) {
                    if (ev->callback) {
                        ev->callback();
                    }
                }
            }

            reg->SR &= ~(0x1F); // Clear Timer Interrupt Flag
        }

        namespace isr {

            void TIM1_CC_Irq()
            {
                TimerInterrupt::isr(TIM1);
            }

            void TIM2_Irq()
            {
                TimerInterrupt::isr(TIM2);
            }

            void TIM3_Irq()
            {
                TimerInterrupt::isr(TIM3);
            }

            void TIM4_Irq()
            {
                TimerInterrupt::isr(TIM4);
            }

            void TIM5_Irq()
            {
                TimerInterrupt::isr(TIM5);
            }

            void TIM6_Irq()
            {
                TimerInterrupt::isr(TIM6);
            }

            void TIM7_Irq()
            {
                TimerInterrupt::isr(TIM7);
            }

            // TODO: ---
        }

    } /* namespace hal */
} /* namespace os */
