/*
 * Print.h
 *
 *  Created on: 6 juin 2014
 *      Author: alienx
 */

#ifndef PRINT_H_
#define PRINT_H_

#include <inttypes.h>
#include <stdio.h> // for size_t

namespace os
{
   enum {
       BYTE = 0,
       BIN  = 2,
       OCT  = 8,
       DEC  = 10,
       HEX  = 16
   };

   class Print {
   public:
       static void write(__attribute__((unused)) uint8_t ch) {};
       static void write(const char *str);
       static void write(const void *buf, uint32_t len);
       static void print(char);
       static void print(const char[]);
       static void print(uint8_t, int=DEC);
       static void print(int, int=DEC);
       static void print(unsigned int, int=DEC);
       static void print(long, int=DEC);
       static void print(unsigned long, int=DEC);
       static void print(long long, int=DEC);
       static void print(unsigned long long, int=DEC);
       static void print(double, int=2);
       static void println(void);
       static void println(char);
       static void println(const char[]);
       static void println(uint8_t, int=DEC);
       static void println(int, int=DEC);
       static void println(unsigned int, int=DEC);
       static void println(long, int=DEC);
       static void println(unsigned long, int=DEC);
       static void println(long long, int=DEC);
       static void println(unsigned long long, int=DEC);
       static void println(double, int=2);
   private:
       static void printNumber(unsigned long long, uint8_t);
       static void printFloat(double, uint8_t);
   };

} /* namespace os */

#endif /* PRINT_H_ */
