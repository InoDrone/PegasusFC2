/*
 * MS5611.h
 *
 *  Created on: 20 juin 2014
 *      Author: alienx
 */

#ifndef MS5611_H_
#define MS5611_H_

#include <cinttypes>
#include <Trace.h>

#define MS561101BA_RESET        0x1E
/* D1 */
#define MS561101BA_D1_256       0x40
#define MS561101BA_D1_512       0x42
#define MS561101BA_D1_1024      0x44
#define MS561101BA_D1_2048      0x46
#define MS561101BA_D1_4096      0x48
/* D2 */
#define MS561101BA_D2_256       0x50
#define MS561101BA_D2_512       0x52
#define MS561101BA_D2_1024      0x54
#define MS561101BA_D2_2048      0x56
#define MS561101BA_D2_4096      0x58

/* ADC */
#define MS561101BA_ADC          0x00

/* PROM */
#define MS5611_PROM_SETUP 0xA0
#define MS5611_PROM_C1 0xA2
#define MS5611_PROM_C2 0xA4
#define MS5611_PROM_C3 0xA6
#define MS5611_PROM_C4 0xA8
#define MS5611_PROM_C5 0xAA
#define MS5611_PROM_C6 0xAC
#define MS5611_PROM_CRC 0xAE

#define MS5611_SEALEVEL_PRESS 101.325f //kPa

#define MS5611_POW2(x) (1 << x)

namespace os {
  namespace device {

      template<class SPI>
      class MS5611 {
          public:
              enum State {
                  TempConversion,
                  PressureConversion
              };

              static bool init();
              static void startADC(State state);
              static bool readADC();
              static inline int32_t getWaitingDelay();

              static inline float getTemp();
              static inline float getPressure();

          private:

              // Definitions
              union PROMDatas {
                   uint8_t  bytes[];
                   uint16_t word[];
                   struct {
                           uint16_t MSSETUP;
                           uint16_t C1;
                           uint16_t C2;
                           uint16_t C3;
                           uint16_t C4;
                           uint16_t C5;
                           uint16_t C6;
                           uint16_t MSCRC;
                   } val;
              };

              // Variables
              static PROMDatas _PROMDatas;
              static State _state;
              static uint32_t _rawTemp;
              static uint32_t _rawPressure;
              static int64_t _temperature;
              static int64_t _pressure;
              static int64_t T2;


              // Methodes
              static bool checkCRC();

      };

      // Static variable definitions
      template<class SPI>
      typename MS5611<SPI>::PROMDatas MS5611<SPI>::_PROMDatas;

      template<class SPI>
      typename MS5611<SPI>::State MS5611<SPI>::_state;

      template<class SPI>
      uint32_t MS5611<SPI>::_rawTemp;

      template<class SPI>
      uint32_t MS5611<SPI>::_rawPressure;

      template<class SPI>
      int64_t MS5611<SPI>::_temperature;

      template<class SPI>
      int64_t MS5611<SPI>::_pressure;

      template<class SPI>
      int64_t MS5611<SPI>::T2;

      // Class Methodes
      template<class SPI>
      bool MS5611<SPI>::init() {
              bool isInit = false;

              SPI::setLowSpeed();

              /* Reset MS5611 */
              SPI::writeByte(MS561101BA_RESET);
              os::hal::Delay::delayMs(10);

              /* Read PROM Datas calibration */
              memset(_PROMDatas.bytes, 0, sizeof(_PROMDatas));
              SPI::read(&_PROMDatas.bytes[0], MS5611_PROM_SETUP, 2);
              SPI::read(&_PROMDatas.bytes[2], MS5611_PROM_C1, 2);
              SPI::read(&_PROMDatas.bytes[4], MS5611_PROM_C2, 2);
              SPI::read(&_PROMDatas.bytes[6], MS5611_PROM_C3, 2);
              SPI::read(&_PROMDatas.bytes[8], MS5611_PROM_C4, 2);
              SPI::read(&_PROMDatas.bytes[10], MS5611_PROM_C5, 2);
              SPI::read(&_PROMDatas.bytes[12], MS5611_PROM_C6, 2);
              SPI::read(&_PROMDatas.bytes[14], MS5611_PROM_CRC, 2);

              /* SWAP BIT */
              uint8_t len = sizeof(_PROMDatas) / 2;
              uint8_t i = 0;

              while (len--) {
                  uint8_t t = _PROMDatas.bytes[i];
                  _PROMDatas.bytes[i] = _PROMDatas.bytes[i+1];
                  _PROMDatas.bytes[i+1] = t;
                  i += 2;
              }

              if (!checkCRC()) {
                  os::Trace::error("[MS5611] CRC Error.");
                  isInit = false;
              } else {
                  os::Trace::debug("[MS5611] CRC OK.");
                  isInit = true;
              }

              SPI::setHighSpeed();

              T2 = 0;

              return isInit;
      }

      // from AN520
      template<class SPI>
      bool MS5611<SPI>::checkCRC()
      {
          uint16_t crc = 0x00;
          uint8_t i, n;
          uint8_t crcRead = (uint8_t)(_PROMDatas.val.MSCRC & 0xF); // Save 4 bit
          _PROMDatas.val.MSCRC = 0xFF00 & _PROMDatas.val.MSCRC;

          for (i=0;i < 16;i++)
          {
              if (i%2 == 1) crc ^= (_PROMDatas.word[i>>1] & 0x00FF);
              else crc ^= (_PROMDatas.word[i>>1] >> 8);
              for (n=8; n > 0; n--) {
                     if (crc & 0x8000) {
                         crc = (crc << 1) ^ 0x3000;
                     } else {
                         crc = (crc << 1);
                     }
              }
          }

          crc = (0x000F & (crc >> 12));
          _PROMDatas.val.MSCRC = crcRead;
          return (crcRead == (uint8_t)crc);
      }

      template<class SPI>
      void MS5611<SPI>::startADC(State state) {
          if (state == TempConversion) {
              while (!SPI::writeByte(MS561101BA_D2_4096));
          } else if (state == PressureConversion) {
              while (!SPI::writeByte(MS561101BA_D1_4096));
          }

          _state = state;
      }

      template<class SPI>
      int32_t MS5611<SPI>::getWaitingDelay() {
              // 4096 -> ~10ms
              return 10;
      }

      template<class SPI>
      bool MS5611<SPI>::readADC()
      {
              bool success = true;
              static int64_t dTemp;
              uint8_t datas[3];
              memset(datas, 0, sizeof(datas));

              /* Read TEMP ADC */
              if (_state == TempConversion) {
                  if (!SPI::read(datas, MS561101BA_ADC, 3)) {
                      return false;
                  }

                  _rawTemp = (((uint32_t)datas[0]) << 16) | (((uint32_t)datas[1]) << 8) | datas[2];

                  // D2 - C5 * 2^8
                  dTemp = ((int32_t)_rawTemp) - (_PROMDatas.val.C5 * MS5611_POW2(8));
                  // TEMP = 20°C + dT * TEMPSENS = 2000 + dT * C6 / 2^23
                  _temperature = 2000l + ((dTemp * _PROMDatas.val.C6) / MS5611_POW2(23));


              } else if (_state == PressureConversion) { /* Read Pressure ADC */
                  int64_t OFFSET;
                  int64_t SENS;
                  int64_t offset2 = 0;
                  int64_t sens2 = 0;

                  if (!SPI::read(datas, MS561101BA_ADC, 3)) {
                      return false;
                  }

                  if (_temperature < 2000) {
                      int64_t corr = (_temperature - 2000) * (_temperature - 2000);
                      offset2 = (5 * corr) / 2;
                      sens2 = (5 * corr) / 4;
                      T2 = (dTemp * dTemp) >> 31;
                      if (_temperature < 1500) {
                          int64_t corr2 = (_temperature - 1500) * (_temperature - 1500);
                          offset2 += 7 * corr2;
                          sens2 += (11 * corr2) / 2;
                      }
                  } else {
                      T2 = 0;
                      offset2 = 0;
                      sens2 = 0;
                  }

                  _rawPressure = (((uint32_t)datas[0]) << 16) | (((uint32_t)datas[1]) << 8) | datas[2];

                  // OFF = OFF_T1 + TCO * dT = C2 * 2^16 + (C4 * dT ) / 2^7
                  OFFSET = ((int64_t)_PROMDatas.val.C2 * MS5611_POW2(16) ) + (((int64_t)dTemp) * _PROMDatas.val.C4) /  MS5611_POW2(7) - offset2;

                  // SENS = SENS_T1 + TCS * dT = C1 * 2^15 + (C3 * dT ) / 2^8
                  SENS = ((int64_t)_PROMDatas.val.C1 * MS5611_POW2(15)) + (((int64_t)dTemp) * _PROMDatas.val.C3) / MS5611_POW2(8) - sens2;

                  // P = D1 * SENS - OFF = (D1 * SENS / 2^21 - OFF) / 2^15
                  _pressure = (((((int64_t)_rawPressure) * SENS ) / MS5611_POW2(21)) - OFFSET) / MS5611_POW2(15);
              } else {
                  success = false;
              }

              return success;
      }

      template<class SPI>
      float MS5611<SPI>::getTemp()
      {
              return ((float)_temperature - T2) / 100.0f;
      }

      /**
       * Get pressure in KPa
       */
      template<class SPI>
      float MS5611<SPI>::getPressure()
      {
              return ((float)_pressure) / 1000.0f;
      }
  }
}


#endif /* MS5611_H_ */
