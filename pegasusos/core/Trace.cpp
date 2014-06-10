/*
 * Trace.cpp
 *
 *  Created on: 7 juin 2014
 *      Author: alienx
 */

#include "Trace.h"
#include <cstdio>
#include <cstdarg>

namespace os
{
  using namespace std;

  Trace::Trace() { }

  void Trace::debug(const char *fmt, ...) {
      va_list ap;

      va_start(ap, fmt);
      (void)vprintf(fmt, ap);
      va_end(ap);

      printf("\r\n");
  }

  void Trace::log(const char *fmt, ...) {
      va_list ap;

      va_start(ap, fmt);
      (void)vprintf(fmt, ap);
      va_end(ap);

      printf("\r\n");
  }

  void Trace::warn(const char *fmt, ...) {
      va_list ap;
      printf("[WARN] ");

      va_start(ap, fmt);
      (void)vprintf(fmt, ap);
      va_end(ap);

      printf("\r\n");
  }

  void Trace::error(const char *fmt, ...) {
      va_list ap;
      printf("[ERROR] ");

      va_start(ap, fmt);
      (void)vprintf(fmt, ap);
      va_end(ap);

      printf("\r\n");
  }
} /* namespace os */
