/*
 * ModuleManager.cpp
 *
 *  Created on: 5 juin 2014
 *      Author: alienx
 */

#include "inc/ModuleManager.h"

namespace os
{

  void ModuleManager::initAll() {
    uint8_t i;
    for (i = 0; i < 24; i++) {
	if (Module::modules[i]) {
	    (Module::modules[i])->init();
	}
    }
  }

  void ModuleManager::startAll() {
    uint8_t i;
    for (i = 0; i < 24; i++) {
	if (Module::modules[i]) {
	    (Module::modules[i])->start();
	}
    }
  }

} /* namespace os */
