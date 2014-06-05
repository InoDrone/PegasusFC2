/*
 * Gpio.h
 *
 *  Created on: 3 juin 2014
 *      Author: alienx
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <pegasusos.h>

#include <stm32f4xx_gpio.h>

namespace os {
	namespace hal {
		namespace gpio {

			enum Address {
				A = GPIOA_BASE,
				B = GPIOB_BASE,
				C = GPIOC_BASE,
				D = GPIOD_BASE,
				E = GPIOE_BASE,
				F = GPIOF_BASE,
				G = GPIOG_BASE
			};


			template<Address P>
			class Port {
				public:

				private:
					Port();
			};

			template<Address P, u8 N>
			class Pin {
				public:
					static inline void init(GPIO_InitTypeDef);
					static inline void init(GPIOMode_TypeDef, GPIOSpeed_TypeDef, GPIOOType_TypeDef, GPIOPuPd_TypeDef);
					static inline void setAlternateFunction(uint8_t);
					static inline void high();
					static inline void low();
					static inline void toggle();
				private:
					static inline uint16_t PIN(u8 pin) {
						return 1 << pin;
					}
					Pin();
			};


			template<Address P, u8 N>
			void Pin<P, N>::init(GPIO_InitTypeDef init) {
				GPIO_TypeDef* reg = reinterpret_cast<GPIO_TypeDef*>(P);
				switch (P) {
				case Address::A:
		            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
					break;
				case Address::B:
		            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
					break;
				case Address::C:
		            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
					break;
				case Address::D:
		            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
					break;
				case Address::E:
		            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
					break;
				case Address::F:
		            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
					break;
				case Address::G:
		            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
					break;
				}

				init.GPIO_Pin = PIN(N);
				GPIO_Init(reg, &init);

				low();
			}

			template<Address P, u8 N>
			void Pin<P, N>::init(GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed, GPIOOType_TypeDef type, GPIOPuPd_TypeDef pupd) {
				GPIO_InitTypeDef GPIOStructInit;
				GPIO_StructInit(&GPIOStructInit);

				GPIOStructInit.GPIO_Pin = PIN(N);
				GPIOStructInit.GPIO_Mode = mode;
				GPIOStructInit.GPIO_Speed = speed;
				GPIOStructInit.GPIO_OType = type;
				GPIOStructInit.GPIO_PuPd = pupd;

				init (GPIOStructInit);

			}

			template<Address P, u8 N>
			void Pin<P, N>::setAlternateFunction(uint8_t AF) {
				GPIO_TypeDef* reg = reinterpret_cast<GPIO_TypeDef*>(P);

				GPIO_PinAFConfig(reg, N, AF);

			}

			template<Address P, u8 N>
			void Pin<P, N>::high() {

				GPIO_TypeDef* reg = reinterpret_cast<GPIO_TypeDef*>(P);

				GPIO_SetBits(reg, PIN(N));
			}

			template<Address P, u8 N>
			void Pin<P, N>::low() {
				GPIO_TypeDef* reg = reinterpret_cast<GPIO_TypeDef*>(P);

				GPIO_ResetBits(reg, PIN(N));
			}

			template<Address P, u8 N>
			void Pin<P, N>::toggle() {
				GPIO_TypeDef* reg = reinterpret_cast<GPIO_TypeDef*>(P);

				GPIO_ToggleBits(reg, PIN(N));
			}

		}

		typedef gpio::Port<gpio::A> HAL_GPIOA;
		typedef gpio::Port<gpio::B> HAL_GPIOB;
		typedef gpio::Port<gpio::C> HAL_GPIOC;
		typedef gpio::Port<gpio::D> HAL_GPIOD;
		typedef gpio::Port<gpio::E> HAL_GPIOE;
		typedef gpio::Port<gpio::F> HAL_GPIOF;
		typedef gpio::Port<gpio::G> HAL_GPIOG;

		/* Port A Pins */
		typedef gpio::Pin<gpio::A, 0> PA0;
		typedef gpio::Pin<gpio::A, 1> PA1;
		typedef gpio::Pin<gpio::A, 2> PA2;
		typedef gpio::Pin<gpio::A, 3> PA3;
		typedef gpio::Pin<gpio::A, 4> PA4;
		typedef gpio::Pin<gpio::A, 5> PA5;
		typedef gpio::Pin<gpio::A, 6> PA6;
		typedef gpio::Pin<gpio::A, 7> PA7;
		typedef gpio::Pin<gpio::A, 8> PA8;
		typedef gpio::Pin<gpio::A, 9> PA9;
		typedef gpio::Pin<gpio::A, 10> PA10;
		typedef gpio::Pin<gpio::A, 11> PA11;
		typedef gpio::Pin<gpio::A, 12> PA12;
		typedef gpio::Pin<gpio::A, 13> PA13;
		typedef gpio::Pin<gpio::A, 14> PA14;
		typedef gpio::Pin<gpio::A, 15> PA15;

		/* Port B Pins */
		typedef gpio::Pin<gpio::B, 0> PB0;
		typedef gpio::Pin<gpio::B, 1> PB1;
		typedef gpio::Pin<gpio::B, 2> PB2;
		typedef gpio::Pin<gpio::B, 3> PB3;
		typedef gpio::Pin<gpio::B, 4> PB4;
		typedef gpio::Pin<gpio::B, 5> PB5;
		typedef gpio::Pin<gpio::B, 6> PB6;
		typedef gpio::Pin<gpio::B, 7> PB7;
		typedef gpio::Pin<gpio::B, 8> PB8;
		typedef gpio::Pin<gpio::B, 9> PB9;
		typedef gpio::Pin<gpio::B, 10> PB10;
		typedef gpio::Pin<gpio::B, 11> PB11;
		typedef gpio::Pin<gpio::B, 12> PB12;
		typedef gpio::Pin<gpio::B, 13> PB13;
		typedef gpio::Pin<gpio::B, 14> PB14;
		typedef gpio::Pin<gpio::B, 15> PB15;

		/* Port C Pins */
		typedef gpio::Pin<gpio::C, 0> PC0;
		typedef gpio::Pin<gpio::C, 1> PC1;
		typedef gpio::Pin<gpio::C, 2> PC2;
		typedef gpio::Pin<gpio::C, 3> PC3;
		typedef gpio::Pin<gpio::C, 4> PC4;
		typedef gpio::Pin<gpio::C, 5> PC5;
		typedef gpio::Pin<gpio::C, 6> PC6;
		typedef gpio::Pin<gpio::C, 7> PC7;
		typedef gpio::Pin<gpio::C, 8> PC8;
		typedef gpio::Pin<gpio::C, 9> PC9;
		typedef gpio::Pin<gpio::C, 10> PC10;
		typedef gpio::Pin<gpio::C, 11> PC11;
		typedef gpio::Pin<gpio::C, 12> PC12;
		typedef gpio::Pin<gpio::C, 13> PC13;
		typedef gpio::Pin<gpio::C, 14> PC14;
		typedef gpio::Pin<gpio::C, 15> PC15;

		/* Port D Pins */
		typedef gpio::Pin<gpio::D, 0> PD0;
		typedef gpio::Pin<gpio::D, 1> PD1;
		typedef gpio::Pin<gpio::D, 2> PD2;
		typedef gpio::Pin<gpio::D, 3> PD3;
		typedef gpio::Pin<gpio::D, 4> PD4;
		typedef gpio::Pin<gpio::D, 5> PD5;
		typedef gpio::Pin<gpio::D, 6> PD6;
		typedef gpio::Pin<gpio::D, 7> PD7;
		typedef gpio::Pin<gpio::D, 8> PD8;
		typedef gpio::Pin<gpio::D, 9> PD9;
		typedef gpio::Pin<gpio::D, 10> PD10;
		typedef gpio::Pin<gpio::D, 11> PD11;
		typedef gpio::Pin<gpio::D, 12> PD12;
		typedef gpio::Pin<gpio::D, 13> PD13;
		typedef gpio::Pin<gpio::D, 14> PD14;
		typedef gpio::Pin<gpio::D, 15> PD15;


		/* Port E Pins */
		typedef gpio::Pin<gpio::E, 0> PE0;
		typedef gpio::Pin<gpio::E, 1> PE1;
		typedef gpio::Pin<gpio::E, 2> PE2;
		typedef gpio::Pin<gpio::E, 3> PE3;
		typedef gpio::Pin<gpio::E, 4> PE4;
		typedef gpio::Pin<gpio::E, 5> PE5;
		typedef gpio::Pin<gpio::E, 6> PE6;
		typedef gpio::Pin<gpio::E, 7> PE7;
		typedef gpio::Pin<gpio::E, 8> PE8;
		typedef gpio::Pin<gpio::E, 9> PE9;
		typedef gpio::Pin<gpio::E, 10> PE10;
		typedef gpio::Pin<gpio::E, 11> PE11;
		typedef gpio::Pin<gpio::E, 12> PE12;
		typedef gpio::Pin<gpio::E, 13> PE13;
		typedef gpio::Pin<gpio::E, 14> PE14;
		typedef gpio::Pin<gpio::E, 15> PE15;

	} /* namespace hal */
} /* namespace os */

#endif /* GPIO_H_ */
