/*
 * boards.cpp
 *
 *  Created on: 3 juin 2014
 *      Author: alienx
 */
#include "boards.h"

void boardInit() {

  #ifdef TELEMETRY_ENABLE
      TELEMETRY_PORT::init<TELEMETRY_PORT_RX_PIN, TELEMETRY_PORT_TX_PIN>(TELEMETRY_PORT_BAUDS);
      TELEMETRY_PORT::write("V1\r\n");
  #endif



  /* INIT SPI DRIVERS */
  SPIDRIVER_INIT;

  os::device::MPU6000Config mpu6000Config;
  mpu6000Config.accelScale 	= os::device::MPU6000_ACCEL_SCALE_4G;
  mpu6000Config.gyroScale 	= os::device::MPU6000_GYRO_SCALE_1000;
  mpu6000Config.filter 		= os::device::MPU6000_FILTER_256_HZ;
  mpu6000Config.intPriority	= IRQ_PRIO_HIGH;
  MPU6000::init<MPU6000_INT_PIN>(mpu6000Config);


  MOTOR1::init();

  UAVLinkPeriodic::init();

}

extern "C" {
    int _write(__attribute__((unused)) int file, char *ptr, __attribute__((unused)) int len) {
                int i;
                for(i = 0;i < len;i++) {
                    TELEMETRY_PORT::write(ptr[i]);
                }
                return len;
    };
}
