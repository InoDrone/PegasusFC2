/*
 * Uart.h
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#ifndef UART_H_
#define UART_H_

#include <stm32f4xx.h>
#include <stm32f4xx_usart.h>

#include "Gpio.h"
#include "Dma.h"
#include "Print.h"

#include "FreeRTOS.h"
#include "semphr.h"

#define USART1_DMA_RX 		os::hal::DMA2_STREAM5
#define USART1_DMA_RX_CHANNEL	DMA_Channel_4
#define USART1_DMA_TX 		os::hal::DMA2_STREAM7
#define USART1_DMA_TX_CHANNEL	DMA_Channel_4

#define USART2_DMA_RX 		os::hal::DMA1_STREAM5
#define USART2_DMA_RX_CHANNEL	DMA_Channel_4
#define USART2_DMA_TX 		os::hal::DMA1_STREAM6
#define USART2_DMA_TX_CHANNEL	DMA_Channel_4

#define USART3_DMA_RX 		os::hal::DMA1_STREAM1
#define USART3_DMA_RX_CHANNEL	DMA_Channel_4
#define USART3_DMA_TX 		os::hal::DMA1_STREAM3
#define USART3_DMA_TX_CHANNEL	DMA_Channel_4

namespace os {
  namespace hal {
    namespace uart {

      enum Address {
	  U1 = USART1_BASE,
	  U2 = USART2_BASE,
	  U3 = USART3_BASE,
	  U4 = UART4_BASE,
	  U5 = UART5_BASE,
	  U6 = USART6_BASE,
	  U7 = UART7_BASE,
	  U8 = UART8_BASE
      };

      template <Address A>
      class Uart {
	public:
	  //template <class RX, class TX>
	  //static inline void init(USART_InitTypeDef init);
	  //static inline void init(uint16_t word, uint16_t stopbits, uint16_t parity, uint16_t hardware = USART_HardwareFlowControl_None);

	  template<class RX, class TX>
	  static inline void init(uint32_t bauds, bool enabled=true);


	  static inline void enable();
	  static inline void disable();

	  static inline void enableTxInterrupt();
	  static inline void enableRxInterrupt();

	  static void enableDMA();
	  static bool dmaInterrupt();

	  static inline bool bytesAvailable();
	  static inline void write(uint8_t c);
	  static inline uint16_t write(const uint8_t* buffer, uint16_t size);
	  static inline uint16_t write(const char* buffer);

	  static inline uint8_t read();

	  struct DMAFifo {
            uint8_t buffer[1024];
            uint16_t pos;
            uint16_t tail;
            bool enabled;
            DMA_Stream_TypeDef* stream;
	  };


	private:

	  static bool isrEnabled;
	  static DMAFifo tx;
	  static DMAFifo rx;

	  static void startTx();
	  static void writeDMA(uint8_t c);

	  static xSemaphoreHandle txMutex;
	  static xSemaphoreHandle rxMutex;
	  static xSemaphoreHandle bufferSendSem;

      };

      /* Variables init */
      ////////////////////
      template<Address A>
      bool Uart<A>::isrEnabled = false;

      template<Address A>
      typename Uart<A>::DMAFifo Uart<A>::tx;

      template<Address A>
      typename Uart<A>::DMAFifo Uart<A>::rx;

      template<Address A>
      xSemaphoreHandle Uart<A>::rxMutex;

      template<Address A>
      xSemaphoreHandle Uart<A>::txMutex;

      template<Address A>
      xSemaphoreHandle Uart<A>::bufferSendSem;

      /* Class functions */
      /////////////////////
      template<Address A>
      template<class RX, class TX>
      void Uart<A>::init(uint32_t bauds, bool enabled) {
	USART_InitTypeDef init;
	USART_TypeDef* reg = reinterpret_cast<USART_TypeDef*>(A);

	bufferSendSem = xSemaphoreCreateMutex();
	//rxMutex = xSemaphoreCreateBinary();

	RX::init(GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP );
	TX::init(GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_UP );

	switch (A) {
	  case U1:
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	    RX::setAlternateFunction(GPIO_AF_USART1);
	    TX::setAlternateFunction(GPIO_AF_USART1);
	    break;
	  case U2:
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	    RX::setAlternateFunction(GPIO_AF_USART2);
	    TX::setAlternateFunction(GPIO_AF_USART2);
	    break;
	  case U3:
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	    RX::setAlternateFunction(GPIO_AF_USART3);
	    TX::setAlternateFunction(GPIO_AF_USART3);
	    break;
	  case U4:
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	    RX::setAlternateFunction(GPIO_AF_UART4);
	    TX::setAlternateFunction(GPIO_AF_UART4);
	    break;
	  case U5:
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	    RX::setAlternateFunction(GPIO_AF_UART5);
	    TX::setAlternateFunction(GPIO_AF_UART5);
	    break;
	  case U6:
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	    RX::setAlternateFunction(GPIO_AF_USART6);
	    TX::setAlternateFunction(GPIO_AF_USART6);
	    break;
	  case U7:
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);
	    RX::setAlternateFunction(GPIO_AF_UART7);
	    TX::setAlternateFunction(GPIO_AF_UART7);
	    break;
	  case U8:
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE);
	    RX::setAlternateFunction(GPIO_AF_UART8);
	    TX::setAlternateFunction(GPIO_AF_UART8);
	    break;
	}

	/* Init uart variables */
	init.USART_BaudRate = bauds;
	init.USART_WordLength = USART_WordLength_8b;
	init.USART_StopBits = USART_StopBits_1;
	init.USART_Parity = USART_Parity_No;
	init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Init uart variables */
	USART_Init(reg, &init);

	if (enabled) {
	    enable();
	}
      }

      template<Address A>
      void Uart<A>::enable() {
	USART_TypeDef* reg = reinterpret_cast<USART_TypeDef*>(A);

	enableDMA();
	USART_Cmd(reg, ENABLE);
      }

      template<Address A>
      void Uart<A>::disable() {
	USART_TypeDef* reg = reinterpret_cast<USART_TypeDef*>(A);

	USART_Cmd(reg, DISABLE);
      }


      template<Address A>
      void Uart<A>::enableTxInterrupt() {
	NVIC_InitTypeDef nvic;
	USART_TypeDef* reg = reinterpret_cast<USART_TypeDef*>(A);


	/* Enable the USARTx Interrupt */
	/*nvic.NVIC_IRQChannel = EVAL_COM1_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nvic);*/

	USART_ITConfig(reg, USART_IT_TXE, ENABLE);


      }

      template<Address A>
      void Uart<A>::enableRxInterrupt() {
	USART_TypeDef* reg = reinterpret_cast<USART_TypeDef*>(A);

	USART_ITConfig(reg, USART_IT_RXNE, ENABLE);

      }


      template<Address A>
      void Uart<A>::enableDMA() {
	DMA_InitTypeDef dmaRX;
	DMA_InitTypeDef dmaTX;

	USART_TypeDef* reg = reinterpret_cast<USART_TypeDef*>(A);

        memset(&tx, 0, sizeof(DMAFifo));
        memset(&rx, 0, sizeof(DMAFifo));

        // INIT DMA STRUCTURE
        DMA_StructInit(&dmaRX);
        dmaRX.DMA_PeripheralBaseAddr = (uint32_t)&reg->DR;
        dmaRX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        dmaRX.DMA_MemoryInc = DMA_MemoryInc_Enable;
        dmaRX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        dmaRX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
        dmaRX.DMA_Priority = DMA_Priority_Low;
        dmaRX.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;

        /* CONFIGURE DMA RX */
        dmaRX.DMA_Memory0BaseAddr = (uint32_t)rx.buffer;
        dmaRX.DMA_DIR = DMA_DIR_PeripheralToMemory;
        dmaRX.DMA_BufferSize = sizeof(rx.buffer);
        dmaRX.DMA_Mode = DMA_Mode_Circular;
        dmaRX.DMA_FIFOMode = DMA_FIFOMode_Disable;
        dmaRX.DMA_MemoryBurst = DMA_MemoryBurst_Single;
        dmaRX.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

        DMA_StructInit(&dmaTX);
        dmaTX.DMA_PeripheralBaseAddr = (uint32_t)&reg->DR;
        dmaTX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        dmaTX.DMA_MemoryInc = DMA_MemoryInc_Enable;
        dmaTX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        dmaTX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
        dmaTX.DMA_Priority = DMA_Priority_Low;
        dmaTX.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
        dmaTX.DMA_DIR = DMA_DIR_MemoryToPeripheral;
        dmaTX.DMA_BufferSize = sizeof(tx.buffer);
        dmaTX.DMA_Mode = DMA_Mode_Normal;
        dmaTX.DMA_FIFOMode = DMA_FIFOMode_Enable;
        dmaTX.DMA_MemoryBurst = DMA_MemoryBurst_INC4;
        dmaTX.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

        switch(A) {
          case U1:
            dmaRX.DMA_Channel = USART1_DMA_RX_CHANNEL;
            USART1_DMA_RX::init(dmaRX);
            USART1_DMA_RX::enable();
            rx.pos = USART1_DMA_RX::getCurrDataCounter();
            rx.stream = USART1_DMA_RX::getReg();

            dmaTX.DMA_Channel = USART1_DMA_TX_CHANNEL;
            USART1_DMA_TX::init(dmaTX);
            USART1_DMA_TX::setCurrDataCounter(0);
            USART1_DMA_TX::enableInterupt(&dmaInterrupt, DMA_IT_TC, 2);

            tx.stream = USART1_DMA_TX::getReg();
            break;
          case U2:
            dmaRX.DMA_Channel = USART2_DMA_RX_CHANNEL;
            USART2_DMA_RX::init(dmaRX);
            USART2_DMA_RX::enable();
            rx.pos = USART2_DMA_RX::getCurrDataCounter();
            rx.stream = USART2_DMA_RX::getReg();

            dmaTX.DMA_Channel = USART2_DMA_TX_CHANNEL;
            USART2_DMA_TX::init(dmaTX);
            USART2_DMA_TX::setCurrDataCounter(0);
            USART2_DMA_TX::enableInterupt(&dmaInterrupt, DMA_IT_TC, 2);

            tx.stream = USART2_DMA_TX::getReg();
            break;
          case U3:
            dmaRX.DMA_Channel = USART3_DMA_RX_CHANNEL;
            USART3_DMA_RX::init(dmaRX);
            USART3_DMA_RX::enable();
            rx.pos = USART3_DMA_RX::getCurrDataCounter();
            rx.stream = USART3_DMA_RX::getReg();

            dmaTX.DMA_Channel = USART3_DMA_TX_CHANNEL;
            USART3_DMA_TX::init(dmaTX);
            USART3_DMA_TX::setCurrDataCounter(0);
            USART3_DMA_TX::enableInterupt(&dmaInterrupt, DMA_IT_TC, 2);

            tx.stream = USART3_DMA_TX::getReg();
            break;
        }

        USART_DMACmd(reg, USART_DMAReq_Tx, ENABLE);
        USART_DMACmd(reg, USART_DMAReq_Rx, ENABLE);

      }

      template<Address A>
      bool Uart<A>::bytesAvailable() {
	return rx.stream->NDTR != rx.pos;
      }

      template<Address A>
      bool Uart<A>::dmaInterrupt() {
	  tx.enabled = false;
	  startTx();

	  /* No ISR switch required */
	  return false;
      }

      /**
       * Start DMA TX
       */
      template<Address A>
      void Uart<A>::startTx() {
	int size;
	uint32_t tail = tx.tail;

	if (!tx.enabled && tx.pos != tail) {
	    if (tx.pos > tail){
		size = tx.pos - tail;
		tx.tail = tx.pos;
	    } else {
		size = sizeof(tx.buffer) - tail;
		tx.tail = 0;
	    }

	    tx.enabled = true;
	    tx.stream->M0AR = (uint32_t)&tx.buffer[tail];
	    tx.stream->NDTR = size;

	    // Enable TX DMA
	    tx.stream->CR |= (uint32_t)DMA_SxCR_EN;
	}
      }

      template<Address A>
      void Uart<A>::writeDMA(uint8_t c) {
	  tx.buffer[tx.pos] = c;
          tx.pos = (tx.pos + 1) % sizeof(tx.buffer);

          startTx();
      }

      template<Address A>
      void Uart<A>::write(uint8_t c) {
	  writeDMA(c);
      }

      template<Address A>
      uint16_t Uart<A>::write(const uint8_t* buffer, uint16_t size) {
	  uint8_t *ch = (uint8_t*)buffer;

	  if (xSemaphoreTake(bufferSendSem, 5) != pdTRUE) {
	      return -1;
	  }

	  while (size--) {
	      writeDMA(*ch++);
	  }

	  xSemaphoreGive(bufferSendSem);
	  return size;
      }

      template<Address A>
      uint16_t Uart<A>::write(const char* buffer) {
	uint16_t size = 0;

	  if (xSemaphoreTake(bufferSendSem, 5) != pdTRUE) {
	      return -1;
	  }

        while (*buffer) {
            size++;
            writeDMA(*buffer++);
        }

	xSemaphoreGive(bufferSendSem);
        return size;
      }

      template<Address A>
      uint8_t Uart<A>::read()
      {
          uint8_t c;
          uint16_t bufferSize = sizeof(rx.buffer);
          c = rx.buffer[bufferSize - rx.pos];
          if (--rx.pos == 0) {
              rx.pos = bufferSize;
          }

          return c;
      }

    }

    typedef uart::Uart<uart::U1> SERIAL1;
    typedef uart::Uart<uart::U2> SERIAL2;
    typedef uart::Uart<uart::U3> SERIAL3;
    typedef uart::Uart<uart::U4> SERIAL4;
    typedef uart::Uart<uart::U5> SERIAL5;
    typedef uart::Uart<uart::U6> SERIAL6;
    typedef uart::Uart<uart::U7> SERIAL7;
    typedef uart::Uart<uart::U7> SERIAL8;

  }
}


#endif /* UART_H_ */
