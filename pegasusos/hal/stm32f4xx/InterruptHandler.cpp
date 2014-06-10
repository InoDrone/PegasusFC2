/*
 * InterruptHandler.cpp
 *
 *  Created on: 3 juin 2014
 *      Author: alienx
 */

#include "inc/InterruptHandler.h"

namespace os {
	namespace hal {
		namespace isr {
			void NMI(void) {
			  while (true) {}
			}

			void HardFault(void) {
				PE1::high();
				__asm volatile
				(
				    " tst lr, #4                                                \n"
				    " ite eq                                                    \n"
				    " mrseq r0, msp                                             \n"
				    " mrsne r0, psp                                             \n"
				    " ldr r1, [r0, #24]                                         \n"
				    " ldr r2, handler2_address_const                            \n"
				    " bx r2                                                     \n"
				    " handler2_address_const: .word prvGetRegistersFromStack    \n"
				);
			}

			void MemManage(void) {
				while (true) {}
			}

			void BusFault(void) {

                PE1::high();
                __asm volatile
                (
                    " tst lr, #4                                                \n"
                    " ite eq                                                    \n"
                    " mrseq r0, msp                                             \n"
                    " mrsne r0, psp                                             \n"
                    " ldr r1, [r0, #24]                                         \n"
                    " ldr r2, handler2_address_const1                            \n"
                    " bx r2                                                     \n"
                    " handler2_address_const1: .word prvGetRegistersFromStack    \n"
                );
			}

			void UsageFault(void) {

			  while (true) {}
			}

			void DebugMon(void) {
			  while (true) {}
			}

			void DefaultHandler() {
				while(true) {}
			}

			void DMA1_Stream3_IRQ(void) {
			  DMA1_STREAM3::handle();
			}

			void DMA1_Stream6_IRQ(void) {
			  DMA1_STREAM6::handle();
			}
		}
	} /* namespace hal */
} /* namespace os */
