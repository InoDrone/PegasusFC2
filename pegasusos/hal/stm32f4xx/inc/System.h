/*
 * System.h
 *
 *  Created on: 3 juin 2014
 *      Author: alienx
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stm32f4xx.h>

extern "C" {
	extern u32    _etext;
	extern u32    _sdata;
	extern u32    _sidata;
	extern u32    _edata;
	extern u32    __bss_start__;
	extern u32    __bss_end__;

	// These magic symbols are provided by the linker.
	extern void
	(*__preinit_array_start[])(void) __attribute__((weak));
	extern void
	(*__preinit_array_end[])(void) __attribute__((weak));
	extern void
	(*__init_array_start[])(void) __attribute__((weak));
	extern void
	(*__init_array_end[])(void) __attribute__((weak));
	extern void
	(*__fini_array_start[])(void) __attribute__((weak));
	extern void
	(*__fini_array_end[])(void) __attribute__((weak));
}

namespace os {
	namespace hal {

	class System {
	public:
		static void init();
		static void startup();
		static void reset();

	private:
		static void initBss(u32* section_begin, u32* section_end);
		static void initData(u32* from, u32* section_begin, u32* section_end);
		static void initArray();
		static void finiArray();

	};

	} /* namespace hal */
} /* namespace os */

#endif /* SYSTEM_H_ */
