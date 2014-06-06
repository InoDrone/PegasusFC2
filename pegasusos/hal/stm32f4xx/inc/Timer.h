/*
 * Timer.h
 *
 *  Created on: 5 juin 2014
 *      Author: alienx
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <pegasusos.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_rcc.h>

namespace os {
  namespace hal {
    namespace timer {

      enum Address {
	TIMER1 = TIM1_BASE,
	TIMER2 = TIM2_BASE,
	TIMER3 = TIM3_BASE,
	TIMER4 = TIM4_BASE,
	TIMER5 = TIM5_BASE,
	TIMER6 = TIM6_BASE,
	TIMER7 = TIM7_BASE,
	TIMER8 = TIM8_BASE,
	TIMER9 = TIM9_BASE,
	TIMER10 = TIM10_BASE,
	TIMER11 = TIM11_BASE,
	TIMER12 = TIM12_BASE,
	TIMER13 = TIM13_BASE,
	TIMER14 = TIM14_BASE
      };

      enum Mode {
	MODE_OC,
	MODE_IC
      };

      enum Channel {
	CHANNEL1 = TIM_Channel_1,
	CHANNEL2 = TIM_Channel_2,
	CHANNEL3 = TIM_Channel_3,
	CHANNEL4 = TIM_Channel_4
      };

      template <Address A>
      class Timer {
      public:
	static inline void init();

	static inline void setFreq(uint32_t hz);

	static inline void setMode(Channel channel, Mode);

	static inline void setTimePulse(Channel channel, uint16_t value);

	static inline void enable();
	static inline void disable();

	static inline TIM_TypeDef* getReg();

      private:
	static bool isInit;
	static TIM_TimeBaseInitTypeDef config;

      };

      template<Address A>
      bool Timer<A>::isInit = false;

      template<Address A>
      TIM_TimeBaseInitTypeDef Timer<A>::config;

      template<Address A>
      void Timer<A>::init() {
	TIM_TypeDef* reg = reinterpret_cast<TIM_TypeDef*>(A);

	if (isInit) {
	    return;
	}

	TIM_TimeBaseStructInit(&config);
	TIM_DeInit(reg);

	switch (A) {
	  case TIMER1:
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	    break;
	  case TIMER2:
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	    break;
	  case TIMER3:
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	    break;
	  case TIMER4:
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	    break;
	  case TIMER5:
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	    break;
	  case TIMER6:
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	    break;
	  case TIMER7:
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	    break;
	  case TIMER8:
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	    break;
	  case TIMER9:
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	    break;
	  case TIMER10:
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	    break;
	  case TIMER11:
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
	    break;
	  case TIMER12:
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	    break;
	  case TIMER13:
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	    break;
	  case TIMER14:
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	    break;
	}

	isInit = true;
      }

      template<Address A>
      TIM_TypeDef* Timer<A>::getReg() {
	return reinterpret_cast<TIM_TypeDef*>(A);
      }

      template<Address A>
      void Timer<A>::setFreq(uint32_t hz) {

	TIM_TypeDef* reg = reinterpret_cast<TIM_TypeDef*>(A);
	RCC_ClocksTypeDef clocks;
	RCC_GetClocksFreq(&clocks);

	uint16_t psc = 0;
	if (A == TIMER1 || A == TIMER8 || A == TIMER9 || A == TIMER10 || A == TIMER11) {
	    psc = (uint16_t)(( (clocks.PCLK2_Frequency*2) / 1000000) - 1);
	} else {
	    psc = (uint16_t)(( (clocks.PCLK1_Frequency*2) / 1000000) - 1);
	}

	uint16_t arr = 0xFFFF;
	if (hz < 0xFFFF) {
	    arr = (uint16_t)( ( 1000000 / hz ) -1 );
	}

	config.TIM_Prescaler = psc;
	config.TIM_Period = arr;

	TIM_TimeBaseInit(reg, &config);
	TIM_ARRPreloadConfig(reg, ENABLE);
      }

      template<Address A>
      void Timer<A>::setMode(Channel channel, Mode mode) {
	TIM_TypeDef* reg = reinterpret_cast<TIM_TypeDef*>(A);
	TIM_ICInitTypeDef  icConfig;
	TIM_OCInitTypeDef  ocConfig;

	switch(mode) {
	  case MODE_IC: // Input compare
	    icConfig.TIM_Channel = channel;
	    icConfig.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
	    icConfig.TIM_ICSelection = TIM_ICSelection_DirectTI;
	    icConfig.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	    icConfig.TIM_ICFilter = 0x0;

	    TIM_ICInit(reg, &icConfig);

	    break;
	  case MODE_OC: // Output Compare
	    ocConfig.TIM_OCMode 	= TIM_OCMode_PWM1;
	    ocConfig.TIM_OutputState 	= TIM_OutputState_Enable;
	    ocConfig.TIM_OutputNState	= TIM_OutputNState_Disable;
	    ocConfig.TIM_Pulse		= 0x0;
	    ocConfig.TIM_OCPolarity 	= TIM_OCPolarity_High;
	    ocConfig.TIM_OCNPolarity	= TIM_OCNPolarity_High;
	    ocConfig.TIM_OCIdleState	= TIM_OCIdleState_Reset;
	    ocConfig.TIM_OCNIdleState	= TIM_OCNIdleState_Reset;

	    switch(channel) {
	      case CHANNEL1:
		    TIM_OC1Init(reg, &ocConfig);
	            TIM_OC1PreloadConfig(reg, TIM_OCPreload_Enable);
		break;
	      case CHANNEL2:
		    TIM_OC2Init(reg, &ocConfig);
	            TIM_OC2PreloadConfig(reg, TIM_OCPreload_Enable);
		break;
	      case CHANNEL3:
		    TIM_OC3Init(reg, &ocConfig);
	            TIM_OC3PreloadConfig(reg, TIM_OCPreload_Enable);
		break;
	      case CHANNEL4:
		    TIM_OC4Init(reg, &ocConfig);
	            TIM_OC4PreloadConfig(reg, TIM_OCPreload_Enable);
		break;
	    }

	    TIM_CtrlPWMOutputs(reg, ENABLE);
	    break;
	}
      }

      template<Address A>
      void Timer<A>::setTimePulse(Channel channel, uint16_t value) {
	TIM_TypeDef* reg = reinterpret_cast<TIM_TypeDef*>(A);

	    switch(channel) {
	      case CHANNEL1:
		  TIM_SetCompare1(reg, value);
		break;
	      case CHANNEL2:
		  TIM_SetCompare2(reg, value);
		break;
	      case CHANNEL3:
		  TIM_SetCompare3(reg, value);
		break;
	      case CHANNEL4:
		  TIM_SetCompare4(reg, value);
		break;
	    }
      }

      template<Address A>
      void Timer<A>::enable() {
	TIM_TypeDef* reg = reinterpret_cast<TIM_TypeDef*>(A);
	TIM_Cmd(reg, ENABLE);
      }

      template<Address A>
      void Timer<A>::disable() {
	TIM_TypeDef* reg = reinterpret_cast<TIM_TypeDef*>(A);
	TIM_Cmd(reg, DISABLE);
      }

    } /* end namespace timer */

    typedef timer::Timer<timer::TIMER1> HAL_TIM1;
    typedef timer::Timer<timer::TIMER2> HAL_TIM2;
    typedef timer::Timer<timer::TIMER3> HAL_TIM3;
    typedef timer::Timer<timer::TIMER4> HAL_TIM4;
    typedef timer::Timer<timer::TIMER5> HAL_TIM5;
    typedef timer::Timer<timer::TIMER6> HAL_TIM6;
    typedef timer::Timer<timer::TIMER7> HAL_TIM7;
    typedef timer::Timer<timer::TIMER8> HAL_TIM8;
    typedef timer::Timer<timer::TIMER9> HAL_TIM9;
    typedef timer::Timer<timer::TIMER10> HAL_TIM10;
    typedef timer::Timer<timer::TIMER11> HAL_TIM11;
    typedef timer::Timer<timer::TIMER12> HAL_TIM12;
    typedef timer::Timer<timer::TIMER13> HAL_TIM13;
    typedef timer::Timer<timer::TIMER14> HAL_TIM14;
  }
}



#endif /* TIMER_H_ */
