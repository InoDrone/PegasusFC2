/*
 * PWMInput.h
 *
 *  Created on: 13 juin 2014
 *      Author: alienx
 */

#ifndef PWMIN_H_
#define PWMIN_H_

#include <stm32f4xx.h>
#include "TimerChannel.h"
#include "TimerInterrupt.h"

namespace os {
    namespace hal {
        namespace pwm {

            template<class C, class PIN>
            class Input
            {
                public:
                    static void init(uint8_t priority);
                    static void handleISR();
                    static inline uint16_t read();

                    static uint16_t *getValuePtr();

                private:
                    static uint16_t _value;
                    static uint32_t _startTime;

            };

            template<class C, class PIN>
            uint16_t Input<C, PIN>::_value;

            template<class C, class PIN>
            uint32_t Input<C, PIN>::_startTime;

            template<class C, class PIN>
            void Input<C, PIN>::init(uint8_t priority)
            {
                    PIN::init(GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN);
                    _value = 1000;

                    TIM_TypeDef* tim = C::getTimer();
                    if (tim == TIM1) {
                        PIN::setAlternateFunction(GPIO_AF_TIM1);
                    } else if (tim == TIM2) {
                        PIN::setAlternateFunction(GPIO_AF_TIM2);
                    } else if (tim == TIM3) {
                        PIN::setAlternateFunction(GPIO_AF_TIM3);
                    } else if (tim == TIM4) {
                        PIN::setAlternateFunction(GPIO_AF_TIM4);
                    } else if (tim == TIM5) {
                        PIN::setAlternateFunction(GPIO_AF_TIM5);
                    } else if (tim == TIM8) {
                        PIN::setAlternateFunction(GPIO_AF_TIM8);
                    } else if (tim == TIM9) {
                        PIN::setAlternateFunction(GPIO_AF_TIM9);
                    } else if (tim == TIM10) {
                        PIN::setAlternateFunction(GPIO_AF_TIM10);
                    } /* TODO: 11 -> 14 */

                    C::init(os::hal::timer::MODE_IC, 0xFFFF, priority);
                    TimerInterrupt::attach(C::getTimer(), C::getChannel(), &handleISR);
            }

            template<class C, class PIN>
            void Input<C,PIN>::handleISR()
            {
                    TIM_TypeDef* reg = C::getTimer();
                    uint8_t channel = C::getChannel();
                    __IO uint32_t* ccrx = &reg->CCR1 + channel;
                    uint16_t time = *ccrx;
                    bool isRising = (reg->CCER & (0x02 << (channel * 4))) == 0;

                    if (isRising) {
                        _startTime = time;
                    } else {
                        if (_startTime > time) {
                            _value = ((time + reg->ARR) - _startTime); // Overflow timer
                        } else {
                            _value = (time - _startTime);
                        }
                    }

                    reg->CCER ^= (0x02 << (channel * 4));
            }

            template<class C, class PIN>
            uint16_t Input<C,PIN>::read()
            {
                    return _value;
            }

            template<class C, class PIN>
            uint16_t* Input<C,PIN>::getValuePtr()
            {
                    return &_value;
            }

        } /* namespace pwm */
    } /* namespace hal */
} /* namespace os */

#endif /* PWMINPUT_H_ */
