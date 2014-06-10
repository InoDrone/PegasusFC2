/*
 * System.cpp
 *
 *  Created on: 3 juin 2014
 *      Author: alienx
 */

#include <pegasusos.h>
#include "inc/System.h"

extern int              main(void) __attribute__((noreturn));
extern "C" {
	void _start() {}
}

namespace os {
namespace hal {


	/**
	 * Reset EntryPoint
	 */
	void System::startup() {

	    /* enable usage, bus and memory faults */
	    SCB->SHCSR  |= SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk;

	    /* configure FP state save behaviour - automatic, lazy save */
	    FPU->FPCCR  |= FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk;

	    /* configure default FPU state */
	    FPU->FPDSCR |= FPU_FPDSCR_DN_Msk; /* enable Default NaN */

	    /* enable the FPU */
	    SCB->CPACR  |= (0xf << 20);      // turn on CP10/11 for FP support on cores that implement it

		initBss(&__bss_start__, &__bss_end__);
		initData(&_sidata, &_sdata, &_edata);

		initArray();


		main();
	}

	void System::init() {
		SystemInit();
		SystemCoreClockUpdate();

        // Timer Prescaler Enable
        RCC->DCKCFGR |= RCC_DCKCFGR_TIMPRE;

		/* 4 bits for Interrupt priorities so no sub priorities */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

		/* Configure HCLK clock as SysTick clock source. */
		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

	    /* Init delay */
	    os::hal::Delay::init();
	}

	void System::reset() {
		portENTER_CRITICAL();

		NVIC_SystemReset();

		while(1) {
		    ;
		}
	}

	/*###############################
	 * PRIVATE INIT
	 *###############################*/
	void System::initBss(u32* section_begin, u32* section_end) {
		u32 *p = section_begin;
		while (p < section_end)
			*p++ = 0;
	}

	void System::initData(u32* from, u32* section_begin, u32* section_end)
	{
		u32 *p = section_begin;
		while (p < section_end)
			*p++ = *from++;
	}

	void System::initArray()
	{
		int count;
		int i;

		count =  __preinit_array_end - __preinit_array_start;
		for (i=0; i < count; i++)
			 __preinit_array_start[i]();

		count = __init_array_end - __init_array_start;
		for (i = 0; i < count; i++)
		__init_array_start[i]();
	}

	void System::finiArray()
	{
		  int count;
		  int i;

		  count = __fini_array_end - __fini_array_start;
		  for (i = count; i > 0; i--)
		    __fini_array_start[i - 1]();
	}

	/* Link FreeRTOS function */
	extern "C" {
		void vApplicationIdleHook() {

		}

		/**
		 * stack overflow is detected.
		 */
		void vApplicationStackOverflowHook(__attribute__((unused)) xTaskHandle *pxTask,
		                                   __attribute__((unused)) signed portCHAR *pcTaskName)
		{
		}

		/**
		 * Called by the RTOS when a malloc call fails.
		 */
		void vApplicationMallocFailedHook(void)
		{
		}
	}

} /* namespace hal */
} /* namespace os */
