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
				while (true) {}
			}

			void MemManage(void) {
				while (true) {}
			}

			void BusFault(void) {

			  while (true) {}
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
