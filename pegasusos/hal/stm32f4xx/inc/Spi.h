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
	static inline void init();

	static inline void write(uint8_t* buffer, uint8_t addr, uint16_t size);
	static inline void writeByte(uint8_t addr, uint8_t data);
	static inline void writeByte(uint8_t addr);

	static inline void read(uint8_t* buffer, uint8_t addr, uint16_t size);

      private:
	static inline uint8_t sendByte(uint8_t byte);
      };

      template<Address A>
      template<class MISO, class MOSI, class SCK>
      void SpiDriver<A>::init() {
	SPI_InitTypeDef  spi;
	SPI_TypeDef* reg = reinterpret_cast<SPI_TypeDef*>(A);

	SPI_I2S_DeInit(reg);

	MISO::init(GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN );
	MOSI::init(GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN );
	SCK::init(GPIO_Mode_AF, GPIO_Speed_100MHz, GPIO_OType_PP, GPIO_PuPd_DOWN );

	switch (A) {
	  case S1:
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	    MISO::setAlternateFunction(GPIO_AF_SPI1);
	    MOSI::setAlternateFunction(GPIO_AF_SPI1);
	    SCK::setAlternateFunction(GPIO_AF_SPI1);
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

	SPI_Init(reg, &spi);
	SPI_Cmd(reg, ENABLE);
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
      uint8_t SpiDriver<A>::sendByte(uint8_t byte) {
	SPI_TypeDef* reg = reinterpret_cast<SPI_TypeDef*>(A);
	while ((reg->SR & 0x02) == 0); // Wait tx buffer empty
	reg->DR = (uint16_t)byte;
	while ((reg->SR & 0x01) == 0); // Wait for receive byte

	return (uint8_t)reg->DR;
      }



      template<class SPI, class CS>
      class SpiDevice {
      public:
	static inline void init();

	static inline void write(uint8_t* buffer, uint8_t addr, uint16_t size);
	static inline void writeByte(uint8_t addr, uint8_t data);
	static inline void writeByte(uint8_t addr);

	static inline void read(uint8_t* buffer, uint8_t addr, uint16_t size);
      };

      template<class SPI, class CS>
      void SpiDevice<SPI, CS>::init() {
	CS::init(GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL );
	CS::high();
      }

      template<class SPI, class CS>
      void SpiDevice<SPI, CS>::write(uint8_t* buffer, uint8_t addr, uint16_t size) {
	CS::low();
	SPI::write(buffer, addr, size);
	CS::high();
      }

      template<class SPI, class CS>
      void SpiDevice<SPI, CS>::writeByte(uint8_t addr, uint8_t data) {
	CS::low();
	SPI::writeByte(addr, data);
	CS::high();
      }

      template<class SPI, class CS>
      void SpiDevice<SPI, CS>::writeByte(uint8_t addr) {
	CS::low();
	SPI::writeByte(addr);
	CS::high();
      }

      template<class SPI, class CS>
      void SpiDevice<SPI, CS>::read(uint8_t* buffer, uint8_t addr, uint16_t size) {
	CS::low();
	SPI::read(buffer, addr, size);
	CS::high();
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
