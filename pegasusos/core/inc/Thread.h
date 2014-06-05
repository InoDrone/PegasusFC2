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
		Thread(const char *name, UBaseType_t priority) :
		  mName(name),
		  mPriority(priority) { }

		void start() {
		  xTaskCreate( &call, mName, configMINIMAL_STACK_SIZE, this, mPriority, &mHandle );
		}

		inline static void delay(uint16_t ms);


		static void call(void* task) {
		    static_cast<Thread*>(task)->run();
		}

	protected:
		xTaskHandle mHandle;
		const char* mName;
		UBaseType_t mPriority;

		virtual void run() = 0;
	};

	 // Inline Methods
	 ///////////////////
	inline void Thread::delay (uint16_t ms) {
	    vTaskDelay(ms / portTICK_PERIOD_MS);
	}
}



#endif /* TASKBASE_H_ */
