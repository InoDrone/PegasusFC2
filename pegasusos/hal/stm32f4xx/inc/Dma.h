/*
 * Dma.h
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#ifndef DMA_H_
#define DMA_H_

#include <stm32f4xx.h>
#include <stm32f4xx_dma.h>

#include "Irq.h"

namespace os {
  namespace hal {

    namespace dma {

      typedef void (*DMAISRCallback)();

      enum DMA {
	DMA_1 = DMA1_BASE,
	DMA_2 = DMA2_BASE
      };
      enum StreamAddress {
	  STREAM0 = 0x010,
	  STREAM1 = 0x028,
	  STREAM2 = 0x040,
	  STREAM3 = 0x058,
	  STREAM4 = 0x070,
	  STREAM5 = 0x088,
	  STREAM6 = 0x0A0,
	  STREAM7 = 0x0B8
      };

      template<DMA D, StreamAddress S>
      class Stream {
	public:
	  static inline void init(DMA_InitTypeDef);
	  static inline uint16_t getCurrDataCounter();
	  static inline void setCurrDataCounter(uint16_t);

	  static inline void enable();
	  static inline void disable();

	  static inline void enableInterupt(DMAISRCallback, uint32_t interruptMask, uint8_t priority);

	  static void handle();

	  static inline DMA_Stream_TypeDef* getReg();

	private:
	  static DMA_InitTypeDef _sInit;
	  static volatile DMAISRCallback _sISRCallback;
      };

      template<DMA D, StreamAddress S>
      DMA_InitTypeDef Stream<D,S>::_sInit;

      template<DMA D, StreamAddress S>
      volatile DMAISRCallback Stream<D,S>::_sISRCallback;

      template<DMA D, StreamAddress S>
      void Stream<D,S>::init(DMA_InitTypeDef init) {
	DMA_Stream_TypeDef* reg = reinterpret_cast<DMA_Stream_TypeDef*>(D + S);
	switch(D) {
	  case DMA_1:
	    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	    break;
	  case DMA_2:
	    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	    break;
	}

	DMA_DeInit(reg);
	DMA_Init(reg, &init);

	_sInit = init;
      }

      template<DMA D, StreamAddress S>
	uint16_t Stream<D,S>::getCurrDataCounter() {
	return ((uint16_t)(reinterpret_cast<DMA_Stream_TypeDef*>(D + S)->NDTR));
      }

      template<DMA D, StreamAddress S>
      void Stream<D,S>::setCurrDataCounter(uint16_t c) {
	reinterpret_cast<DMA_Stream_TypeDef*>(D + S)->NDTR = c;
      }

      template<DMA D, StreamAddress S>
      void Stream<D,S>::enable() {
	DMA_Stream_TypeDef* reg = reinterpret_cast<DMA_Stream_TypeDef*>(D + S);
	DMA_Cmd(reg, ENABLE);
      }

      template<DMA D, StreamAddress S>
      void Stream<D,S>::disable() {
	DMA_Stream_TypeDef* reg = reinterpret_cast<DMA_Stream_TypeDef*>(D + S);
	DMA_Cmd(reg, DISABLE);
      }

      template<DMA D, StreamAddress S>
      void Stream<D,S>::enableInterupt(DMAISRCallback cb, uint32_t interruptMask, uint8_t priority) {
	DMA_Stream_TypeDef* reg = reinterpret_cast<DMA_Stream_TypeDef*>(D + S);

	/* Set DMA IRQ callback */
	_sISRCallback = cb;

        DMA_ITConfig(reg, interruptMask, ENABLE);
        switch (D) {
          case DMA_1:
            switch(S) {
              case STREAM0:
                os::hal::IRQ::setPriority(DMA1_Stream0_IRQn, priority);
                os::hal::IRQ::enable(DMA1_Stream0_IRQn);
        	break;
              case STREAM1:
                os::hal::IRQ::setPriority(DMA1_Stream1_IRQn, priority);
                os::hal::IRQ::enable(DMA1_Stream1_IRQn);
        	break;
              case STREAM2:
                os::hal::IRQ::setPriority(DMA1_Stream2_IRQn, priority);
                os::hal::IRQ::enable(DMA1_Stream2_IRQn);
        	break;
              case STREAM3:
                os::hal::IRQ::setPriority(DMA1_Stream3_IRQn, priority);
                os::hal::IRQ::enable(DMA1_Stream3_IRQn);
        	break;
              case STREAM4:
                os::hal::IRQ::setPriority(DMA1_Stream4_IRQn, priority);
                os::hal::IRQ::enable(DMA1_Stream4_IRQn);
        	break;
              case STREAM5:
                os::hal::IRQ::setPriority(DMA1_Stream5_IRQn, priority);
                os::hal::IRQ::enable(DMA1_Stream5_IRQn);
        	break;
              case STREAM6:
                os::hal::IRQ::setPriority(DMA1_Stream6_IRQn, priority);
                os::hal::IRQ::enable(DMA1_Stream6_IRQn);
        	break;
              case STREAM7:
                os::hal::IRQ::setPriority(DMA1_Stream7_IRQn, priority);
                os::hal::IRQ::enable(DMA1_Stream7_IRQn);
        	break;
            }
            break;
          case DMA_2:
            switch(S) {
              case STREAM0:
                os::hal::IRQ::setPriority(DMA2_Stream0_IRQn, priority);
                os::hal::IRQ::enable(DMA2_Stream0_IRQn);
        	break;
              case STREAM1:
                os::hal::IRQ::setPriority(DMA2_Stream1_IRQn, priority);
                os::hal::IRQ::enable(DMA2_Stream1_IRQn);
        	break;
              case STREAM2:
                os::hal::IRQ::setPriority(DMA2_Stream2_IRQn, priority);
                os::hal::IRQ::enable(DMA2_Stream2_IRQn);
        	break;
              case STREAM3:
                os::hal::IRQ::setPriority(DMA2_Stream3_IRQn, priority);
                os::hal::IRQ::enable(DMA2_Stream3_IRQn);
        	break;
              case STREAM4:
                os::hal::IRQ::setPriority(DMA2_Stream4_IRQn, priority);
                os::hal::IRQ::enable(DMA2_Stream4_IRQn);
        	break;
              case STREAM5:
                os::hal::IRQ::setPriority(DMA2_Stream5_IRQn, priority);
                os::hal::IRQ::enable(DMA2_Stream5_IRQn);
        	break;
              case STREAM6:
                os::hal::IRQ::setPriority(DMA2_Stream6_IRQn, priority);
                os::hal::IRQ::enable(DMA2_Stream6_IRQn);
        	break;
              case STREAM7:
                os::hal::IRQ::setPriority(DMA2_Stream7_IRQn, priority);
                os::hal::IRQ::enable(DMA2_Stream7_IRQn);
        	break;
            }
            break;
        }
      }

      template<DMA D, StreamAddress S>
      void Stream<D,S>::handle() {
	DMA_Stream_TypeDef* reg = reinterpret_cast<DMA_Stream_TypeDef*>(D + S);
	switch(S) {
	  case STREAM3:
	    DMA_ClearFlag(reg, DMA_FLAG_TCIF3 | DMA_FLAG_HTIF3 | DMA_FLAG_TEIF3 | DMA_FLAG_DMEIF3 | DMA_FLAG_FEIF3 );
	    break;
	  case STREAM6:
	    DMA_ClearFlag(reg, DMA_FLAG_TCIF6 | DMA_FLAG_HTIF6 | DMA_FLAG_TEIF6 | DMA_FLAG_DMEIF6 | DMA_FLAG_FEIF6 );
	    break;
	  default:
	    break;
	}


	if (_sISRCallback) {
	    _sISRCallback();
	}
      }

      template<DMA D, StreamAddress S>
      DMA_Stream_TypeDef* Stream<D,S>::getReg() {
	return reinterpret_cast<DMA_Stream_TypeDef*>(D + S);
      }

    } /* end namespace dma */

    /* Define DMAx */
    typedef dma::Stream<dma::DMA_1, dma::STREAM0> DMA1_STREAM0;
    typedef dma::Stream<dma::DMA_1, dma::STREAM1> DMA1_STREAM1;
    typedef dma::Stream<dma::DMA_1, dma::STREAM2> DMA1_STREAM2;
    typedef dma::Stream<dma::DMA_1, dma::STREAM3> DMA1_STREAM3;
    typedef dma::Stream<dma::DMA_1, dma::STREAM4> DMA1_STREAM4;
    typedef dma::Stream<dma::DMA_1, dma::STREAM5> DMA1_STREAM5;
    typedef dma::Stream<dma::DMA_1, dma::STREAM6> DMA1_STREAM6;
    typedef dma::Stream<dma::DMA_1, dma::STREAM7> DMA1_STREAM7;


    typedef dma::Stream<dma::DMA_2, dma::STREAM0> DMA2_STREAM0;
    typedef dma::Stream<dma::DMA_2, dma::STREAM1> DMA2_STREAM1;
    typedef dma::Stream<dma::DMA_2, dma::STREAM2> DMA2_STREAM2;
    typedef dma::Stream<dma::DMA_2, dma::STREAM3> DMA2_STREAM3;
    typedef dma::Stream<dma::DMA_2, dma::STREAM4> DMA2_STREAM4;
    typedef dma::Stream<dma::DMA_2, dma::STREAM5> DMA2_STREAM5;
    typedef dma::Stream<dma::DMA_2, dma::STREAM6> DMA2_STREAM6;
    typedef dma::Stream<dma::DMA_2, dma::STREAM7> DMA2_STREAM7;



    /*namespace isr {
      void DMA1_Stream0_IRQ() {
	DMA1_STREAM0::handle();
      }

      void DMA1_Stream1_IRQ() {
	DMA1_STREAM1::handle();
      }

      void DMA1_Stream2_IRQ() {
	DMA1_STREAM2::handle();
      }

      void DMA1_Stream3_IRQ() {
	DMA1_STREAM3::handle();
      }

      void DMA1_Stream4_IRQ() {
	DMA1_STREAM4::handle();
      }

      void DMA1_Stream5_IRQ() {
	DMA1_STREAM5::handle();
      }

      void DMA1_Stream6_IRQ() {
	DMA1_STREAM6::handle();

      }

      void DMA1_Stream7_IRQ() {
	DMA1_STREAM7::handle();
      }

    }*/
  }
}




#endif /* DMA_H_ */
