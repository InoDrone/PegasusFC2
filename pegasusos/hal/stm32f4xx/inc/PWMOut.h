/*
 * PWMOut.h
 *
 *  Created on: 5 juin 2014
 *      Author: alienx
 */

#ifndef PWMOUT_H_
#define PWMOUT_H_

#include <stm32f4xx.h>
#include "TimerChannel.h"

namespace os {
  namespace hal {
    namespace pwm {

      template<class C, class PIN, uint16_t Hz>
      class Output {
      public:
	static inline void init();
	static inline void write(uint16_t value);
      };

      template<class C, class PIN, uint16_t Hz>
      void Output<C,PIN,Hz>::init()
      {
	PIN::init(GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP);
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

	C::init(os::hal::timer::MODE_OC, Hz);
	C::setTimePulse(1000);
      }

      template<class C, class PIN, uint16_t Hz>
      void Output<C,PIN,Hz>::write(uint16_t value)
      {
	C::setTimePulse(value);
      }
    }
  }
}


#endif /* PWMOUT_H_ */
