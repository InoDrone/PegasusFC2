/*
 * ModuleManager.h
 *
 *  Created on: 5 juin 2014
 *      Author: alienx
 */

#ifndef MODULEMANAGER_H_
#define MODULEMANAGER_H_

#include <cinttypes>
#include "Module.h"

namespace os
{
  class ModuleManager
  {
  public:
    static void initAll();
    static void startAll();
  };

} /* namespace os */

#endif /* MODULEMANAGER_H_ */
