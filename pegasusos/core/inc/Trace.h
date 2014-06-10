/*
 * Trace.h
 *
 *  Created on: 7 juin 2014
 *      Author: alienx
 */

#ifndef TRACE_H_
#define TRACE_H_

namespace os
{

  class Trace
  {
  public:
      Trace();

      static void log(const char *fmt, ...);
      static void debug(const char *fmt, ...);
      static void warn(const char *fmt, ...);
      static void error(const char *fmt, ...);
  };

} /* namespace os */

#endif /* TRACE_H_ */
