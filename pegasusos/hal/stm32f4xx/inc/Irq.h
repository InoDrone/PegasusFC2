/*
 * NVIC.h
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#ifndef IRQ_H_
#define IRQ_H_

#include <stm32f4xx.h>

namespace os {
  namespace hal {

    namespace nvic {

      class Functions {
      public:
	  template<IRQn irq>
	  static inline void enable();
	  static inline void enable(IRQn irq);

	  template<IRQn irq>
	  static inline void disable();
	  static inline void disable(IRQn irq);

	  template<IRQn irq>
	  static inline void setPriority(uint8_t priority, uint8_t subpriority = 0);
	  static inline void setPriority(IRQn irq, uint8_t priority, uint8_t subpriority = 0);
      };

	  template<IRQn irq>
	  void Functions::enable()
	  {
	    enable(irq);
	  }

	  void Functions::enable(IRQn irq)
	  {
	    NVIC->ISER[ irq >> 5 ] = (uint32_t)0x01 << (irq & (uint8_t)0x1F);
	  }

	  template<IRQn irq>
	  void Functions::disable()
	  {
	    disable(irq);
	  }

	  void Functions::disable(IRQn irq)
	  {
	    NVIC->ICER[ irq >> 5 ] = (uint32_t)0x01 << (irq & (uint8_t)0x1F);
	  }

	  template<IRQn irq>
	  void Functions::setPriority(uint8_t priority, uint8_t subpriority)
	  {
	    setPriority(irq, priority, subpriority);
	  }

	  void Functions::setPriority(IRQn irq, uint8_t priority, uint8_t subpriority)
	  {
	    uint8_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;
	    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
	    tmppre = (0x4 - tmppriority);
	    tmpsub = tmpsub >> tmppriority;

	    tmppriority = priority << tmppre;
	    tmppriority |=  (uint8_t)(subpriority & tmpsub);

	    tmppriority = tmppriority << 0x04;

	    NVIC->IP[irq] = tmppriority;
	  }
    } /* end nic*/

    typedef nvic::Functions IRQ;
  }
}


#endif /* NVIC_H_ */
