/*
 * Delay.cpp
 *
 *  Created on: 7 juin 2014
 *      Author: alienx
 */

#include "inc/Delay.h"
#include "Trace.h"

#define DWT_CTRL   (*(volatile uint32_t *)0xe0001000)
#define CYCCNTENA  (1 << 0)
#define DWT_CYCCNT (*(volatile uint32_t *)0xe0001004)

namespace os
{
  namespace hal
  {
    uint32_t Delay::usTicks;

    void Delay::init() {
      RCC_ClocksTypeDef clocks;
      RCC_GetClocksFreq(&clocks);
      usTicks = clocks.SYSCLK_Frequency / 1000000;

#ifdef DEBUG
      os::Trace::debug("SYSCLK: %d", clocks.SYSCLK_Frequency);
      os::Trace::debug("PB1CLK: %d", clocks.PCLK1_Frequency);
      os::Trace::debug("PB2CLK: %d", clocks.PCLK2_Frequency);
#endif
      CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

      DWT_CTRL |= CYCCNTENA;
    }

    void Delay::delayUs(uint32_t us) {
      uint32_t elapsed    = 0;
      uint32_t last_count = DWT_CYCCNT;

      for (;;) {
          uint32_t current_count = DWT_CYCCNT;
          uint32_t elapsed_uS;

          /* measure the time elapsed since the last time we checked */
          elapsed   += current_count - last_count;
          last_count = current_count;

          /* convert to microseconds */
          elapsed_uS = elapsed / usTicks;
          if (elapsed_uS >= us) {
              break;
          }

          /* reduce the delay by the elapsed time */
          us -= elapsed_uS;

          /* keep fractional microseconds for the next iteration */
          elapsed %= usTicks;
      }
    }

    void Delay::delayMs(uint32_t ms) {
      while (ms--) {
	  delayUs(1000);
      }
    }

    uint32_t Delay::micros() {
      return DWT_CYCCNT / usTicks;
    }

    uint32_t Delay::millis() {
      return DWT_CYCCNT / usTicks / 1000;
    }

    uint32_t Delay::ticks() {
      return DWT_CYCCNT;
    }

    uint32_t Delay::diff(uint32_t raw) {
      uint32_t diff = DWT_CYCCNT - raw;

      return diff / usTicks;
    }
  } /* namespace hal */
} /* namespace os */
