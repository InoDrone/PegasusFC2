/*
 * Delay.h
 *
 *  Created on: 7 juin 2014
 *      Author: alienx
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stm32f4xx.h>

namespace os
{
  namespace hal
  {

    class Delay
    {
    public:
      static void init();
      static void delayUs(uint32_t us);
      static void delayMs(uint32_t ms);
      static uint32_t micros();
      static uint32_t millis();

      static uint32_t ticks();

      static uint32_t diff(uint32_t raw);

    private:
      static uint32_t usTicks;
    };

  } /* namespace hal */
} /* namespace os */

#endif /* DELAY_H_ */
