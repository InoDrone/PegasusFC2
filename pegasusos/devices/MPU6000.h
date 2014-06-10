/*
 * MPU6000.h
 *
 *  Created on: 7 juin 2014
 *      Author: alienx
 */

#ifndef MPU6000_H_
#define MPU6000_H_

#include "Delay.h"
#include "Exti.h"
#include "Queue.h"

// Remove just for debug
#include "Trace.h"

#define GRAVITY	9.81f

#define MPU6000_RA_XG_OFFS_TC       0x00 //[7] PWR_MODE, [6:1] XG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6000_RA_YG_OFFS_TC       0x01 //[7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6000_RA_ZG_OFFS_TC       0x02 //[7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6000_RA_X_FINE_GAIN      0x03 //[7:0] X_FINE_GAIN
#define MPU6000_RA_Y_FINE_GAIN      0x04 //[7:0] Y_FINE_GAIN
#define MPU6000_RA_Z_FINE_GAIN      0x05 //[7:0] Z_FINE_GAIN
#define MPU6000_RA_XA_OFFS_H        0x06 //[15:0] XA_OFFS
#define MPU6000_RA_XA_OFFS_L_TC     0x07
#define MPU6000_RA_YA_OFFS_H        0x08 //[15:0] YA_OFFS
#define MPU6000_RA_YA_OFFS_L_TC     0x09
#define MPU6000_RA_ZA_OFFS_H        0x0A //[15:0] ZA_OFFS
#define MPU6000_RA_ZA_OFFS_L_TC     0x0B
#define MPU6000_RA_XG_OFFS_USRH     0x13 //[15:0] XG_OFFS_USR
#define MPU6000_RA_XG_OFFS_USRL     0x14
#define MPU6000_RA_YG_OFFS_USRH     0x15 //[15:0] YG_OFFS_USR
#define MPU6000_RA_YG_OFFS_USRL     0x16
#define MPU6000_RA_ZG_OFFS_USRH     0x17 //[15:0] ZG_OFFS_USR
#define MPU6000_RA_ZG_OFFS_USRL     0x18
#define MPU6000_RA_SMPLRT_DIV       0x19
#define MPU6000_RA_CONFIG           0x1A
#define MPU6000_RA_GYRO_CONFIG      0x1B
#define MPU6000_RA_ACCEL_CONFIG     0x1C
#define MPU6000_RA_FF_THR           0x1D
#define MPU6000_RA_FF_DUR           0x1E
#define MPU6000_RA_MOT_THR          0x1F
#define MPU6000_RA_MOT_DUR          0x20
#define MPU6000_RA_ZRMOT_THR        0x21
#define MPU6000_RA_ZRMOT_DUR        0x22
#define MPU6000_RA_FIFO_EN          0x23
#define MPU6000_RA_I2C_MST_CTRL     0x24
#define MPU6000_RA_I2C_SLV0_ADDR    0x25
#define MPU6000_RA_I2C_SLV0_REG     0x26
#define MPU6000_RA_I2C_SLV0_CTRL    0x27
#define MPU6000_RA_I2C_SLV1_ADDR    0x28
#define MPU6000_RA_I2C_SLV1_REG     0x29
#define MPU6000_RA_I2C_SLV1_CTRL    0x2A
#define MPU6000_RA_I2C_SLV2_ADDR    0x2B
#define MPU6000_RA_I2C_SLV2_REG     0x2C
#define MPU6000_RA_I2C_SLV2_CTRL    0x2D
#define MPU6000_RA_I2C_SLV3_ADDR    0x2E
#define MPU6000_RA_I2C_SLV3_REG     0x2F
#define MPU6000_RA_I2C_SLV3_CTRL    0x30
#define MPU6000_RA_I2C_SLV4_ADDR    0x31
#define MPU6000_RA_I2C_SLV4_REG     0x32
#define MPU6000_RA_I2C_SLV4_DO      0x33
#define MPU6000_RA_I2C_SLV4_CTRL    0x34
#define MPU6000_RA_I2C_SLV4_DI      0x35
#define MPU6000_RA_I2C_MST_STATUS   0x36
#define MPU6000_RA_INT_PIN_CFG      0x37
#define MPU6000_RA_INT_ENABLE       0x38
#define MPU6000_RA_DMP_INT_STATUS   0x39
#define MPU6000_RA_INT_STATUS       0x3A
#define MPU6000_RA_ACCEL_XOUT_H     0x3B
#define MPU6000_RA_ACCEL_XOUT_L     0x3C
#define MPU6000_RA_ACCEL_YOUT_H     0x3D
#define MPU6000_RA_ACCEL_YOUT_L     0x3E
#define MPU6000_RA_ACCEL_ZOUT_H     0x3F
#define MPU6000_RA_ACCEL_ZOUT_L     0x40
#define MPU6000_RA_TEMP_OUT_H       0x41
#define MPU6000_RA_TEMP_OUT_L       0x42
#define MPU6000_RA_GYRO_XOUT_H      0x43
#define MPU6000_RA_GYRO_XOUT_L      0x44
#define MPU6000_RA_GYRO_YOUT_H      0x45
#define MPU6000_RA_GYRO_YOUT_L      0x46
#define MPU6000_RA_GYRO_ZOUT_H      0x47
#define MPU6000_RA_GYRO_ZOUT_L      0x48
#define MPU6000_RA_EXT_SENS_DATA_00 0x49
#define MPU6000_RA_EXT_SENS_DATA_01 0x4A
#define MPU6000_RA_EXT_SENS_DATA_02 0x4B
#define MPU6000_RA_EXT_SENS_DATA_03 0x4C
#define MPU6000_RA_EXT_SENS_DATA_04 0x4D
#define MPU6000_RA_EXT_SENS_DATA_05 0x4E
#define MPU6000_RA_EXT_SENS_DATA_06 0x4F
#define MPU6000_RA_EXT_SENS_DATA_07 0x50
#define MPU6000_RA_EXT_SENS_DATA_08 0x51
#define MPU6000_RA_EXT_SENS_DATA_09 0x52
#define MPU6000_RA_EXT_SENS_DATA_10 0x53
#define MPU6000_RA_EXT_SENS_DATA_11 0x54
#define MPU6000_RA_EXT_SENS_DATA_12 0x55
#define MPU6000_RA_EXT_SENS_DATA_13 0x56
#define MPU6000_RA_EXT_SENS_DATA_14 0x57
#define MPU6000_RA_EXT_SENS_DATA_15 0x58
#define MPU6000_RA_EXT_SENS_DATA_16 0x59
#define MPU6000_RA_EXT_SENS_DATA_17 0x5A
#define MPU6000_RA_EXT_SENS_DATA_18 0x5B
#define MPU6000_RA_EXT_SENS_DATA_19 0x5C
#define MPU6000_RA_EXT_SENS_DATA_20 0x5D
#define MPU6000_RA_EXT_SENS_DATA_21 0x5E
#define MPU6000_RA_EXT_SENS_DATA_22 0x5F
#define MPU6000_RA_EXT_SENS_DATA_23 0x60
#define MPU6000_RA_MOT_DETECT_STATUS    0x61
#define MPU6000_RA_I2C_SLV0_DO      0x63
#define MPU6000_RA_I2C_SLV1_DO      0x64
#define MPU6000_RA_I2C_SLV2_DO      0x65
#define MPU6000_RA_I2C_SLV3_DO      0x66
#define MPU6000_RA_I2C_MST_DELAY_CTRL   0x67
#define MPU6000_RA_SIGNAL_PATH_RESET    0x68
#define MPU6000_RA_MOT_DETECT_CTRL      0x69
#define MPU6000_RA_USER_CTRL        0x6A
#define MPU6000_RA_PWR_MGMT_1       0x6B
#define MPU6000_RA_PWR_MGMT_2       0x6C
#define MPU6000_RA_BANK_SEL         0x6D
#define MPU6000_RA_MEM_START_ADDR   0x6E
#define MPU6000_RA_MEM_R_W          0x6F
#define MPU6000_RA_DMP_CFG_1        0x70
#define MPU6000_RA_DMP_CFG_2        0x71
#define MPU6000_RA_FIFO_COUNTH      0x72
#define MPU6000_RA_FIFO_COUNTL      0x73
#define MPU6000_RA_FIFO_R_W         0x74
#define MPU6000_RA_WHO_AM_I         0x75
#define MPU6000_RA_PRODUCT_ID       0x0C


#define MPU6000_WHO_AM_I_BIT        6
#define MPU6000_WHO_AM_I_LENGTH     6

#define MPU6000_PWR1_CLKSEL_BIT         2
#define MPU6000_PWR1_CLKSEL_LENGTH      3

#define MPU6000_CLOCK_INTERNAL          0x00
#define MPU6000_CLOCK_PLL_XGYRO         0x01
#define MPU6000_CLOCK_PLL_YGYRO         0x02
#define MPU6000_CLOCK_PLL_ZGYRO         0x03
#define MPU6000_CLOCK_PLL_EXT32K        0x04
#define MPU6000_CLOCK_PLL_EXT19M        0x05
#define MPU6000_CLOCK_KEEP_RESET        0x07

/* USERCTL REGISTER BIT */
#define MPU6000_USERCTL_FIFO_EN          0X40
#define MPU6000_USERCTL_I2C_MST_EN       0x20
#define MPU6000_USERCTL_DIS_I2C          0X10
#define MPU6000_USERCTL_FIFO_RST         0X04
#define MPU6000_USERCTL_I2C_MST_RST      0X02
#define MPU6000_USERCTL_SIG_RST          0X01

                                            // Product ID Description for MPU6000
                                            // high 4 bits  low 4 bits
                                            // Product Name Product Revision
#define MPU6000ES_REV_C4            0x14    // 0001         0100
#define MPU6000ES_REV_C5            0x15    // 0001         0101
#define MPU6000ES_REV_D6            0x16    // 0001         0110
#define MPU6000ES_REV_D7            0x17    // 0001         0111
#define MPU6000ES_REV_D8            0x18    // 0001         1000
#define MPU6000_REV_C4              0x54    // 0101         0100
#define MPU6000_REV_C5              0x55    // 0101         0101
#define MPU6000_REV_D6              0x56    // 0101         0110
#define MPU6000_REV_D7              0x57    // 0101         0111
#define MPU6000_REV_D8              0x58    // 0101         1000
#define MPU6000_REV_D9              0x59    // 0101         1001


namespace os {
  namespace device {

    enum MPU6000AccelScale {
      MPU6000_ACCEL_SCALE_2G = 0x00,
      MPU6000_ACCEL_SCALE_4G = 0x01,
      MPU6000_ACCEL_SCALE_8G = 0x02,
      MPU6000_ACCEL_SCALE_16G = 0x03
    };

    enum MPU6000GyroScale {
      MPU6000_GYRO_SCALE_250 = 0x00,
      MPU6000_GYRO_SCALE_500 = 0x01,
      MPU6000_GYRO_SCALE_1000 = 0x02,
      MPU6000_GYRO_SCALE_2000 = 0x03
    };

    enum MPU6000Filter {
      MPU6000_FILTER_256_HZ = 0x00,
      MPU6000_FILTER_188_HZ = 0x01,
      MPU6000_FILTER_98_HZ = 0x02,
      MPU6000_FILTER_42_HZ = 0x03,
      MPU6000_FILTER_20_HZ = 0x04,
      MPU6000_FILTER_10_HZ = 0x05,
      MPU6000_FILTER_5_HZ = 0x06
    };

    struct MPU6000Config {
      MPU6000AccelScale accelScale;
      MPU6000GyroScale 	gyroScale;
      MPU6000Filter	filter;
      uint8_t intPriority;
    };

    template<class SPI>
    class MPU6000 {
    public:
	struct Axis {
		int16_t x;
		int16_t y;
		int16_t z;
	};

	struct DatasValues {
	        Axis acc;
	        int16_t temp;
	        Axis gyro;
	};

	union Datas {
		uint8_t bytes[];
		uint16_t word[];
		DatasValues val;
	};

      template<class PIN>
      static void init(const MPU6000Config config);
      static void setRanges(MPU6000AccelScale accScale, MPU6000GyroScale gyroScal, MPU6000Filter filter);
      static bool ping();

      static float getAccelScale();
      static float getGyroScale();

      static bool setReg(uint8_t addr, uint8_t data);
      static int8_t getReg(uint8_t addr);

      static Queue* getQueue();

      static bool irq();
    private:
      static MPU6000Config _config;
      static bool _isReady;
      static Queue queue;
    };

    template<class SPI>
    MPU6000Config MPU6000<SPI>::_config;

    template<class SPI>
    bool MPU6000<SPI>::_isReady;

    template<class SPI>
    Queue MPU6000<SPI>::queue;

    template<class SPI>
    template<class PIN>
    void MPU6000<SPI>::init(const MPU6000Config config) {
      _config = config;

      _isReady = false;
      if (!queue.create(2, sizeof(DatasValues))) {
              return;
      }

      os::hal::Exti::create<PIN>(&irq, config.intPriority);
      SPI::setLowSpeed();
      if (!ping()) {
              os::Trace::debug("MPU6000: PING ERROR");
      }

      // Reset MPU
      while (!setReg(MPU6000_RA_PWR_MGMT_1, 0x80));
      os::hal::Delay::delayMs(50);

      // Reset sensors
      while (!setReg(MPU6000_RA_USER_CTRL, MPU6000_USERCTL_SIG_RST | MPU6000_USERCTL_FIFO_RST));

      // Wait for reset
      while (getReg(MPU6000_RA_USER_CTRL) & (MPU6000_USERCTL_SIG_RST | MPU6000_USERCTL_FIFO_RST));
      os::hal::Delay::delayMs(10);

      // set system clocks
      while (!setReg(MPU6000_RA_PWR_MGMT_1, MPU6000_CLOCK_PLL_ZGYRO));


      /* Enable INT DRDY */
      while (!setReg(MPU6000_RA_INT_PIN_CFG, 0x10)); // 0x10 INT_RD_CLEAR = 1 other 0
      while (!setReg(MPU6000_RA_INT_ENABLE, 0x01)); // Enable DRDY INT

      setRanges(_config.accelScale, _config.gyroScale, _config.filter);

      if ((getReg(MPU6000_RA_INT_ENABLE)) != 0x01) {
          return;
      }

      _isReady = true;

      SPI::setHighSpeed();
    }

    template<class SPI>
    void MPU6000<SPI>::setRanges(MPU6000AccelScale accScale, MPU6000GyroScale gyroScale, MPU6000Filter filter) {
      // TODO: set clocks to prescaler 256

      /* Filter */
      while(!setReg(MPU6000_RA_CONFIG, filter));

      /* Sample Rate */
      while(!setReg(MPU6000_RA_SMPLRT_DIV, filter == MPU6000_FILTER_256_HZ ? 0x08 : 0x00)); // Set to 1Khz ( if DLPF_CGF > 0 else 0x08 )


      /* Gyro scale config */
      while(!setReg(MPU6000_RA_GYRO_CONFIG, gyroScale << 3));

      /* Acc config */
      while(!setReg(MPU6000_RA_ACCEL_CONFIG, accScale << 3));
      // TODO: set clocks to prescaler 8 or 16

      _config.accelScale = accScale;
      _config.gyroScale = gyroScale;
      _config.filter = filter;

    }

    template<class SPI>
    bool MPU6000<SPI>::ping() {
      int8_t id = getReg(MPU6000_RA_WHO_AM_I);

      os::Trace::debug("PING RESPONSE %d", id);
      if (id < 0) return false;

      if ((id & 0x7E) != 0x68) return false;

      return true;
    }


    template<class SPI>
    float MPU6000<SPI>::getAccelScale() {
      switch(_config.accelScale) {
          case MPU6000_ACCEL_SCALE_2G:
              return GRAVITY / 16384.0f;
          case MPU6000_ACCEL_SCALE_4G:
              return GRAVITY / 8192.0f;
          case MPU6000_ACCEL_SCALE_8G:
              return GRAVITY / 4096.0f;
          case MPU6000_ACCEL_SCALE_16G:
              return GRAVITY / 2048.0f;
      }

      return 0.0f;
    }

    template<class SPI>
    float MPU6000<SPI>::getGyroScale() {
      switch(_config.accelScale) {
          case MPU6000_GYRO_SCALE_250:
              return 1.0f / 131.0f;
          case MPU6000_GYRO_SCALE_500:
              return 1.0f / 65.5f;
          case MPU6000_GYRO_SCALE_1000:
              return 1.0f / 32.8f;
          case MPU6000_GYRO_SCALE_2000:
              return 1.0f / 16.4f;
      }

      return 0.0f;
    }

    template<class SPI>
    bool MPU6000<SPI>::setReg(uint8_t addr, uint8_t data) {
      return SPI::writeByte(0x7F & addr, data);
    }

    template<class SPI>
    int8_t MPU6000<SPI>::getReg(uint8_t addr) {
        uint8_t data;
        SPI::read(&data, (addr | 0x80), 1 );
        #ifdef DEBUG
              //os::Trace::debug("SPI [addr:%d, response: %d]", addr, (uint8_t)data);
        #endif
        return data;
    }

    template<class SPI>
    Queue* MPU6000<SPI>::getQueue() {
      return &queue;
    }

    template<class SPI>
    bool MPU6000<SPI>::irq() {
      bool woken = false;

      if (!_isReady) {
	  return false;
      }

      Datas datas;
      uint8_t len = sizeof(Datas) / 2;
      uint8_t i = 0;

      if (!SPI::readFromISR(datas.bytes, MPU6000_RA_ACCEL_XOUT_H | 0x80,(uint16_t) sizeof (Datas), &woken) ) {
              return woken;
      }

      /* SWAP BIT */
      while (len--) {
          uint8_t t = datas.bytes[i];
          datas.bytes[i] = datas.bytes[i+1];
          datas.bytes[i+1] = t;
          i += 2;
      }

      signed portBASE_TYPE higherPriorityTaskWoken;
      queue.sendFromISR(&datas.val, &higherPriorityTaskWoken);

      return woken || higherPriorityTaskWoken == pdTRUE;
    }
  }
}


#endif /* MPU6000_H_ */
