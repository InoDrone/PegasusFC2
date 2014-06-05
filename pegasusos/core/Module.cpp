/*
 * Module.cpp
 *
 *  Created on: 5 juin 2014
 *      Author: alienx
 */

#include "inc/Module.h"

namespace os {
  uint16_t Module::pos = 0;
  Module* Module::modules[24] = {0};

  Module::Module ()
  {
    modules[pos++] = this;
  }
}

