/*
 * TaskBase.h
 *
 *  Created on: 3 juin 2014
 *      Author: alienx
 */

#ifndef TASKBASE_H_
#define TASKBASE_H_

#include "FreeRTOS.h"
#include "task.h"

namespace os {

	class Thread {
	public:
		Thread(const char *name, UBaseType_t priority, uint16_t stacksize = configMINIMAL_STACK_SIZE ) :
		  mName(name),
		  mPriority(priority),
		  _mStackSize(stacksize) { }

		void start() {
		  xTaskCreate( &call, mName, _mStackSize, this, mPriority, &mHandle );
		}

		/*static void create(TaskFunction_t cb, const char*name, UBaseType_t priority, uint16_t stacksize = configMINIMAL_STACK_SIZE, xTaskHandle* handle = NULL) {
		  xTaskCreate( cb, name, stacksize, NULL, priority, &handle );
		}*/

		inline static void delay(uint16_t ms);


		static void call(void* task) {
		    static_cast<Thread*>(task)->run();
		}

	protected:
		xTaskHandle mHandle;
		const char* mName;
		UBaseType_t mPriority;
		uint16_t _mStackSize;

		virtual void run() = 0;
	};

	 // Inline Methods
	 ///////////////////
	inline void Thread::delay (uint16_t ms) {
	    vTaskDelay(ms / portTICK_PERIOD_MS);
	}
}



#endif /* TASKBASE_H_ */
