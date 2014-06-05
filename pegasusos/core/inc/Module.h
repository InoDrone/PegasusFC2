/*
 * Module.h
 *
 *  Created on: 5 juin 2014
 *      Author: alienx
 */

#ifndef MODULE_H_
#define MODULE_H_

#include <cinttypes>

extern "C" {
  extern uint32_t __modules_start[];
  extern uint32_t __modules_end[];
}



namespace os {

  class Module {
  public:
    Module();

    virtual void init() = 0;
    virtual void start() = 0;

    static Module* modules[24];
  private:
    static uint16_t pos;

  };
}


#endif /* MODULE_H_ */
