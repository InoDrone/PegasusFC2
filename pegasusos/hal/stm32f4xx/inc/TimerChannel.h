/*
 * TimerChannel.h
 *
 *  Created on: 5 juin 2014
 *      Author: alienx
 */

#ifndef TIMERCHANNEL_H_
#define TIMERCHANNEL_H_

#include "Timer.h"

namespace os {
  namespace hal {
    namespace timer {

      template<class TIM, Channel C>
      class TimerChannel {
          public:
            static inline void init(Mode mode, uint16_t Hz, uint8_t priority = 128);

            static inline void setTimePulse(uint16_t value);
            static inline TIM_TypeDef* getTimer();
            static inline uint8_t getChannel();

      };

      /**
       *
       */
      template<class TIM, Channel C>
      void TimerChannel<TIM,C>::init(Mode mode, uint16_t Hz, uint8_t priority) {
          TIM::init();
          TIM::setFreq(Hz);
          TIM::setMode(C, mode);
          TIM::enable();

          if (mode == Mode::MODE_IC) {
              TIM::enableInterrupt(priority);
          }
      }

      /**
       *
       */
      template<class TIM, Channel C>
      void TimerChannel<TIM,C>::setTimePulse(uint16_t value) {
          TIM::setTimePulse(C, value);
      }

      /**
       *
       */
      template<class TIM, Channel C>
          TIM_TypeDef* TimerChannel<TIM,C>::getTimer() {
          return TIM::getReg();
      }

      template<class TIM, Channel C>
      uint8_t TimerChannel<TIM,C>::getChannel() {
              uint8_t channel = 4;
              switch (C) {
                  case CHANNEL1:
                      channel = 0;
                  break;
                  case CHANNEL2:
                      channel = 1;
                  break;
                  case CHANNEL3:
                      channel = 2;
                  break;
                  case CHANNEL4:
                      channel = 3;
                  break;
              }

              return channel;
      }
    }

    /* TIM1 */
    typedef timer::TimerChannel<HAL_TIM1, timer::CHANNEL1> TIM1_CHANNEL1;
    typedef timer::TimerChannel<HAL_TIM1, timer::CHANNEL2> TIM1_CHANNEL2;
    typedef timer::TimerChannel<HAL_TIM1, timer::CHANNEL3> TIM1_CHANNEL3;
    typedef timer::TimerChannel<HAL_TIM1, timer::CHANNEL4> TIM1_CHANNEL4;

    /* TIM2 */
    typedef timer::TimerChannel<HAL_TIM2, timer::CHANNEL1> TIM2_CHANNEL1;
    typedef timer::TimerChannel<HAL_TIM2, timer::CHANNEL2> TIM2_CHANNEL2;
    typedef timer::TimerChannel<HAL_TIM2, timer::CHANNEL3> TIM2_CHANNEL3;
    typedef timer::TimerChannel<HAL_TIM2, timer::CHANNEL4> TIM2_CHANNEL4;

    /* TIM3 */
    typedef timer::TimerChannel<HAL_TIM3, timer::CHANNEL1> TIM3_CHANNEL1;
    typedef timer::TimerChannel<HAL_TIM3, timer::CHANNEL2> TIM3_CHANNEL2;
    typedef timer::TimerChannel<HAL_TIM3, timer::CHANNEL3> TIM3_CHANNEL3;
    typedef timer::TimerChannel<HAL_TIM3, timer::CHANNEL4> TIM3_CHANNEL4;

    /* TIM4 */
    typedef timer::TimerChannel<HAL_TIM4, timer::CHANNEL1> TIM4_CHANNEL1;
    typedef timer::TimerChannel<HAL_TIM4, timer::CHANNEL2> TIM4_CHANNEL2;
    typedef timer::TimerChannel<HAL_TIM4, timer::CHANNEL3> TIM4_CHANNEL3;
    typedef timer::TimerChannel<HAL_TIM4, timer::CHANNEL4> TIM4_CHANNEL4;

    /* TIM5 */
    typedef timer::TimerChannel<HAL_TIM5, timer::CHANNEL1> TIM5_CHANNEL1;
    typedef timer::TimerChannel<HAL_TIM5, timer::CHANNEL2> TIM5_CHANNEL2;
    typedef timer::TimerChannel<HAL_TIM5, timer::CHANNEL3> TIM5_CHANNEL3;
    typedef timer::TimerChannel<HAL_TIM5, timer::CHANNEL4> TIM5_CHANNEL4;

    /* TIM8 */
    typedef timer::TimerChannel<HAL_TIM8, timer::CHANNEL1> TIM8_CHANNEL1;
    typedef timer::TimerChannel<HAL_TIM8, timer::CHANNEL2> TIM8_CHANNEL2;
    typedef timer::TimerChannel<HAL_TIM8, timer::CHANNEL3> TIM8_CHANNEL3;
    typedef timer::TimerChannel<HAL_TIM8, timer::CHANNEL4> TIM8_CHANNEL4;

    /* TIM9 */
    typedef timer::TimerChannel<HAL_TIM9, timer::CHANNEL1> TIM9_CHANNEL1;
    typedef timer::TimerChannel<HAL_TIM9, timer::CHANNEL2> TIM9_CHANNEL2;
    typedef timer::TimerChannel<HAL_TIM9, timer::CHANNEL3> TIM9_CHANNEL3;
    typedef timer::TimerChannel<HAL_TIM9, timer::CHANNEL4> TIM9_CHANNEL4;

    /* TIM10 */
    typedef timer::TimerChannel<HAL_TIM10, timer::CHANNEL1> TIM10_CHANNEL1;
    typedef timer::TimerChannel<HAL_TIM10, timer::CHANNEL2> TIM10_CHANNEL2;
    typedef timer::TimerChannel<HAL_TIM10, timer::CHANNEL3> TIM10_CHANNEL3;
    typedef timer::TimerChannel<HAL_TIM10, timer::CHANNEL4> TIM10_CHANNEL4;

    /* TIM11 */
    typedef timer::TimerChannel<HAL_TIM11, timer::CHANNEL1> TIM11_CHANNEL1;
    typedef timer::TimerChannel<HAL_TIM11, timer::CHANNEL2> TIM11_CHANNEL2;
    typedef timer::TimerChannel<HAL_TIM11, timer::CHANNEL3> TIM11_CHANNEL3;
    typedef timer::TimerChannel<HAL_TIM11, timer::CHANNEL4> TIM11_CHANNEL4;

    /* TIM12 */
    typedef timer::TimerChannel<HAL_TIM12, timer::CHANNEL1> TIM12_CHANNEL1;
    typedef timer::TimerChannel<HAL_TIM12, timer::CHANNEL2> TIM12_CHANNEL2;
    typedef timer::TimerChannel<HAL_TIM12, timer::CHANNEL3> TIM12_CHANNEL3;
    typedef timer::TimerChannel<HAL_TIM12, timer::CHANNEL4> TIM12_CHANNEL4;

    /* TIM13 */
    typedef timer::TimerChannel<HAL_TIM13, timer::CHANNEL1> TIM13_CHANNEL1;
    typedef timer::TimerChannel<HAL_TIM13, timer::CHANNEL2> TIM13_CHANNEL2;
    typedef timer::TimerChannel<HAL_TIM13, timer::CHANNEL3> TIM13_CHANNEL3;
    typedef timer::TimerChannel<HAL_TIM13, timer::CHANNEL4> TIM13_CHANNEL4;

    /* TIM14 */
    typedef timer::TimerChannel<HAL_TIM14, timer::CHANNEL1> TIM14_CHANNEL1;
    typedef timer::TimerChannel<HAL_TIM14, timer::CHANNEL2> TIM14_CHANNEL2;
    typedef timer::TimerChannel<HAL_TIM14, timer::CHANNEL3> TIM14_CHANNEL3;
    typedef timer::TimerChannel<HAL_TIM14, timer::CHANNEL4> TIM14_CHANNEL4;
  }
}


#endif /* TIMERCHANNEL_H_ */
