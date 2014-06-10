/*
 * EXTI.h
 *
 *  Created on: 7 juin 2014
 *      Author: alienx
 */

#ifndef HAL_EXTI_H_
#define HAL_EXTI_H_

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include "Irq.h"

namespace os
{
  namespace hal
  {

    typedef bool (*EXTICallback)(void);

    class Exti
    {
    public:
      template<class PIN>
      static bool create(EXTICallback cb, uint8_t priority);

      static EXTICallback _callback[15];
    };

    template<class PIN>
    bool Exti::create(EXTICallback cb, uint8_t priority) {
      EXTI_InitTypeDef config;
      IRQn irq;
      uint8_t bit = PIN::getPinNumber();

      if (_callback[bit] != 0) {
              return false;
      }

      RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
      PIN::init(GPIO_Mode_IN, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL);

      SYSCFG_EXTILineConfig(PIN::getPortNumber(), bit);
      config.EXTI_Line    = (1 <<  bit);
      config.EXTI_Mode 	  = EXTI_Mode_Interrupt;
      config.EXTI_Trigger = EXTI_Trigger_Rising;
      config.EXTI_LineCmd = ENABLE;
      EXTI_Init(&config);

      _callback[bit] = cb;

      if (bit > 4 && bit < 10) {
          irq = EXTI9_5_IRQn;
      } else if (bit > 9) {
          irq = EXTI15_10_IRQn;
      } else {
          irq = (IRQn)(EXTI0_IRQn + bit);
      }

      IRQ::enable(irq);
      IRQ::setPriority(irq, priority);

      return true;
    }

  } /* namespace hal */
} /* namespace os */

#endif /* EXTI_H_ */
