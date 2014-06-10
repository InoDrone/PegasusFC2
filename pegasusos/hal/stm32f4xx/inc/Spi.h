/*
 * Spi.h
 *
 *  Created on: 5 juin 2014
 *      Author: alienx
 */

#ifndef SPI_H_
#define SPI_H_

#include <pegasusos.h>
#include <stm32f4xx_spi.h>
#include "Trace.h"

#define SPI_MODE_MASTER ((uint16_t)0x0104)

#define SPI_DATASIZE_16B                ((uint16_t)0x0800)
#define SPI_DATASIZE_8B                 ((uint16_t)0x0000)

#define SPI_CPOL_LOW                   ((uint16_t)0x0000)
#define SPI_CPOL_HIGH                  ((uint16_t)0x0002)

#define SPI_CPHA_1EDGE                  ((uint16_t)0x0000)
#define SPI_CPHA_2EDGE                  ((uint16_t)0x0001)

#define SPI_NSS_SOFT                    ((uint16_t)0x0200)
#define SPI_NSS_HARD                    ((uint16_t)0x0000)

#define SPI_BAUDPSC_2         ((uint16_t)0x0000)
#define SPI_BAUDPSC_4         ((uint16_t)0x0008)
#define SPI_BAUDPSC_8         ((uint16_t)0x0010)
#define SPI_BAUDPSC_16        ((uint16_t)0x0018)
#define SPI_BAUDPSC_32        ((uint16_t)0x0020)
#define SPI_BAUDPSC_64        ((uint16_t)0x0028)
#define SPI_BAUDPSC_128       ((uint16_t)0x0030)
#define SPI_BAUDPSC_256       ((uint16_t)0x0038)

#define SPI_LSBFIRST_MSB                ((uint16_t)0x0000)
#define SPI_LSBFIRST_LSB                ((uint16_t)0x0080)

namespace os {
  namespace hal {
    namespace spi {

      enum Address {
	S1 = SPI1_BASE,
	S2 = SPI2_BASE,
	S3 = SPI3_BASE,
	S4 = SPI4_BASE,
	S5 = SPI5_BASE,
	S6 = SPI6_BASE
      };

      template<Address A>
      class SpiDriver {
          public:
            template<class MOSI, class MISO, class SCK>
            static void init();

            static void setSpeed(uint8_t speed);
            static inline void setLowSpeed();
            static inline void setHighSpeed();


            static inline void write(uint8_t* buffer, uint8_t addr, uint16_t size);
            static inline void writeByte(uint8_t addr, uint8_t data);
            static inline void writeByte(uint8_t addr);

            static inline void read(uint8_t* buffer, uint8_t addr, uint16_t size);
            static inline int8_t readByte(uint8_t addr);

            static inline bool claimBus();
            static inline bool releaseBus();

            static inline bool claimBusISR(bool*);
            static inline bool releaseBusISR(bool*);

          private:
            static inline uint8_t sendByte(uint8_t byte);
            static xSemaphoreHandle deviceBusy;
      };

      template<Address A>
      xSemaphoreHandle SpiDriver<A>::deviceBusy;


      template<Address A>
      template<class MISO, class MOSI, class SCK>
      void SpiDriver<A>::init() {
        SPI_InitTypeDef  spi;
        SPI_TypeDef* reg = reinterpret_cast<SPI_TypeDef*>(A);

        MISO::init(GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN );
        MOSI::init(GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN );
        SCK::init(GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN );

        deviceBusy = xSemaphoreCreateBinary();
        xSemaphoreGive(deviceBusy);

        switch (A) {
          case S1:
            MISO::setAlternateFunction(GPIO_AF_SPI1);
            MOSI::setAlternateFunction(GPIO_AF_SPI1);
            SCK::setAlternateFunction(GPIO_AF_SPI1);

            RCC->APB2ENR &= ~(RCC_APB2Periph_SPI1); // Reset
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
            //os::Trace::debug("SPI1 CONFIG : %d, %d, %d", MISO::getPinNumber(), MOSI::getPinNumber(), SCK::getPinNumber());
            break;
          case S2:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
            MISO::setAlternateFunction(GPIO_AF_SPI2);
            MOSI::setAlternateFunction(GPIO_AF_SPI2);
            SCK::setAlternateFunction(GPIO_AF_SPI2);
            break;
          case S3:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
            MISO::setAlternateFunction(GPIO_AF_SPI3);
            MOSI::setAlternateFunction(GPIO_AF_SPI3);
            SCK::setAlternateFunction(GPIO_AF_SPI3);
            break;
          case S4:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
            MISO::setAlternateFunction(GPIO_AF_SPI4);
            MOSI::setAlternateFunction(GPIO_AF_SPI4);
            SCK::setAlternateFunction(GPIO_AF_SPI4);
            break;
          case S5:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);
            MISO::setAlternateFunction(GPIO_AF_SPI5);
            MOSI::setAlternateFunction(GPIO_AF_SPI5);
            SCK::setAlternateFunction(GPIO_AF_SPI5);
            break;
          case S6:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI6, ENABLE);
            MISO::setAlternateFunction(GPIO_AF_SPI6);
            MOSI::setAlternateFunction(GPIO_AF_SPI6);
            SCK::setAlternateFunction(GPIO_AF_SPI6);
            break;
        }

        spi.SPI_Mode = SPI_Mode_Master;
        spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
        spi.SPI_DataSize = SPI_DataSize_8b;
        spi.SPI_CPOL = SPI_CPOL_Low;
        spi.SPI_CPHA = SPI_CPHA_1Edge;
        spi.SPI_NSS = SPI_NSS_Soft;
        spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
        spi.SPI_FirstBit = SPI_FirstBit_MSB;
        spi.SPI_CRCPolynomial = 7;

        SPI_DeInit(reg);
        SPI_Init(reg, &spi);
        SPI_Cmd(reg, ENABLE);

      }

      template<Address A>
      void SpiDriver<A>::setSpeed(uint8_t speed) {
          SPI_TypeDef* reg = reinterpret_cast<SPI_TypeDef*>(A);
          uint16_t tmpReg;
          tmpReg = reg->CR1;
          tmpReg &= ~(0x38); // Reset
          tmpReg |= speed;

          reg->CR1 = tmpReg;
      }

      template<Address A>
      void SpiDriver<A>::setLowSpeed() {
          setSpeed(SPI_BaudRatePrescaler_256);
      }

      template<Address A>
      void SpiDriver<A>::setHighSpeed() {
          setSpeed(SPI_BaudRatePrescaler_8);
      }


      template<Address A>
      void SpiDriver<A>::write(uint8_t* buffer, uint8_t addr, uint16_t size) {
        sendByte(addr);
        while(size >=0x01) {
            sendByte(*buffer);
            buffer++;
            size--;
        }
      }

      template<Address A>
      void SpiDriver<A>::writeByte(uint8_t addr, uint8_t data) {
        sendByte(addr);
        sendByte(data);
      }

      template<Address A>
      void SpiDriver<A>::writeByte(uint8_t addr) {
        sendByte(addr);
      }

      template<Address A>
      void SpiDriver<A>::read(uint8_t* buffer, uint8_t addr, uint16_t size) {
        sendByte(addr);
        while (size > 0x00) {
            *buffer = sendByte(0x0);
            buffer++;
            size--;
        }
      }

      template<Address A>
      int8_t SpiDriver<A>::readByte(uint8_t addr) {
	  sendByte(addr);
	  return sendByte(0x0);
      }

      template<Address A>
      uint8_t SpiDriver<A>::sendByte(uint8_t byte) {
        SPI_TypeDef* reg = reinterpret_cast<SPI_TypeDef*>(A);
        uint8_t rxByte;
        while (!(reg->SR & SPI_I2S_FLAG_TXE)); // Wait tx buffer empty
        reg->DR = (uint16_t)byte;
        while (!(reg->SR & SPI_I2S_FLAG_RXNE)); // Wait for receive byte
        rxByte = (uint8_t)reg->DR;
        while (!(reg->SR & SPI_I2S_FLAG_TXE)); // Wait tx buffer empty
        while (reg->SR & SPI_I2S_FLAG_BSY); // Completed

        return rxByte;
      }

      template<Address A>
      bool SpiDriver<A>::claimBus() {
        if (xSemaphoreTake(deviceBusy, 0xffff) != pdTRUE) {
            return false;
        }

        return true;
      }

      template<Address A>
      bool SpiDriver<A>::releaseBus() {
        xSemaphoreGive(deviceBusy);
        return true;
      }

      template<Address A>
      bool SpiDriver<A>::claimBusISR(bool *woken) {
        signed portBASE_TYPE higherPriorityTaskWoken = pdFALSE;

        if (xSemaphoreTakeFromISR(deviceBusy, &higherPriorityTaskWoken) != pdTRUE) {
            return false;
        }

        if (woken) {
            *woken = *woken || (higherPriorityTaskWoken == pdTRUE);
        }

        return true;
      }

      template<Address A>
      bool SpiDriver<A>::releaseBusISR(bool *woken) {
        signed portBASE_TYPE higherPriorityTaskWoken = pdFALSE;

        xSemaphoreGiveFromISR(deviceBusy, &higherPriorityTaskWoken);
        if (woken) {
            *woken = *woken || (higherPriorityTaskWoken == pdTRUE);
        }
        return true;
      }




      template<class SPI, class CS>
      class SpiDevice {
          public:
            static inline void init();

            static inline void setLowSpeed();
            static inline void setHighSpeed();

            static inline bool write(uint8_t* buffer, uint8_t addr, uint16_t size);
            static inline bool writeByte(uint8_t addr, uint8_t data);
            static inline bool writeByte(uint8_t addr);

            static inline bool read(uint8_t* buffer, uint8_t addr, uint16_t size);
            static inline bool readFromISR(uint8_t* buffer, uint8_t addr, uint16_t size, bool* woken);
            static inline int8_t readByte(uint8_t addr);

            static inline bool claimBus();
            static inline bool releaseBus();

            static inline bool claimBusISR(bool*);
            static inline bool releaseBusISR(bool*);
      };

      template<class SPI, class CS>
      void SpiDevice<SPI, CS>::init() {
        CS::init(GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL );
        CS::high();
      }

      template<class SPI, class CS>
      void SpiDevice<SPI, CS>::setLowSpeed() {
          SPI::setLowSpeed();
      }

      template<class SPI, class CS>
      void SpiDevice<SPI, CS>::setHighSpeed() {
          SPI::setHighSpeed();
      }

      template<class SPI, class CS>
      bool SpiDevice<SPI, CS>::write(uint8_t* buffer, uint8_t addr, uint16_t size) {
        if (!claimBus()) {
            return false;
        }
        SPI::write(buffer, addr, size);
        releaseBus();

        return true;
      }

      template<class SPI, class CS>
      bool SpiDevice<SPI, CS>::writeByte(uint8_t addr, uint8_t data) {
        if (!claimBus()) {
            return false;
        }
        SPI::writeByte(addr, data);
        releaseBus();

        return true;
      }

      template<class SPI, class CS>
      bool SpiDevice<SPI, CS>::writeByte(uint8_t addr) {
        if (!claimBus()) {
            return false;
        }
        SPI::writeByte(addr);
        releaseBus();

        return true;
      }

      template<class SPI, class CS>
      bool SpiDevice<SPI, CS>::read(uint8_t* buffer, uint8_t addr, uint16_t size) {
        if (!claimBus()) {
            return false;
        }

        SPI::read(buffer, addr, size);
        releaseBus();

        return true;
      }

      template<class SPI, class CS>
      bool SpiDevice<SPI, CS>::readFromISR(uint8_t* buffer, uint8_t addr, uint16_t size, bool* woken) {
        if (!claimBusISR(woken)) {
            return false;
        }

        SPI::read(buffer, addr, size);
        releaseBusISR(woken);

        return true;
      }

      template<class SPI, class CS>
      int8_t SpiDevice<SPI, CS>::readByte(uint8_t addr) {
        int8_t data;
        if (!claimBus()) {
            return -2;
        }

        data = SPI::readByte(addr);
        releaseBus();

        return data;
      }

      template<class SPI, class CS>
      bool SpiDevice<SPI, CS>::claimBus() {
        if (!SPI::claimBus()) {
            return false;
        }
        CS::low();
        return true;
      }

      template<class SPI, class CS>
      bool SpiDevice<SPI, CS>::releaseBus() {
        CS::high();
        return SPI::releaseBus();
      }

      template<class SPI, class CS>
      bool SpiDevice<SPI, CS>::claimBusISR(bool* woken) {
        if (!SPI::claimBusISR(woken)) {
            return false;
        }
        CS::low();
        return true;
      }

      template<class SPI, class CS>
      bool SpiDevice<SPI, CS>::releaseBusISR(bool *woken) {
        CS::high();
        return SPI::releaseBusISR(woken);
      }

    }

    typedef spi::SpiDriver<spi::S1> HAL_SPI1;
    typedef spi::SpiDriver<spi::S2> HAL_SPI2;
    typedef spi::SpiDriver<spi::S3> HAL_SPI3;
    typedef spi::SpiDriver<spi::S4> HAL_SPI4;
    typedef spi::SpiDriver<spi::S5> HAL_SPI5;
    typedef spi::SpiDriver<spi::S6> HAL_SPI6;
  }
}


#endif /* SPI_H_ */
