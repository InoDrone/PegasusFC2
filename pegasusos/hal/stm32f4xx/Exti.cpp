/*
 * EXTI.cpp
 *
 *  Created on: 7 juin 2014
 *      Author: alienx
 */

#include "inc/Exti.h"
#include "FreeRTOS.h"

namespace os
{
  namespace hal
  {
    EXTICallback Exti::_callback[15];

    namespace isr {
      void EXTx_IRQ() {
        portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

        uint32_t state = EXTI->PR & EXTI->IMR;
        uint32_t bit = 0;
        uint8_t j = 0;

        bool woken = false;

        for (j=0; j<15;j++) {
            bit = (1 << j);
            if ((state & bit) && os::hal::Exti::_callback[j]) {
            xHigherPriorityTaskWoken = (os::hal::Exti::_callback[j]() || woken) ? pdTRUE : woken;
            }
        }

        EXTI->PR = 0xFFFFF; // Clear 16 bit

        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
      }
    }

  } /* namespace hal */
} /* namespace os */
