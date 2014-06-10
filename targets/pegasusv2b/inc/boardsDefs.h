/*
 * boardsDefs.h
 *
 *  Created on: 5 juin 2014
 *      Author: alienx
 */

#ifndef BOARDSDEFS_H_
#define BOARDSDEFS_H_

#include <pegasushal.h>
#include "devices/MPU6000.h"

#define IRQ_PRIO_LOW            12              // lower than RTOS
#define IRQ_PRIO_MID            8               // higher than RTOS
#define IRQ_PRIO_HIGH           5               // for SPI, ADC, I2C etc...
#define IRQ_PRIO_HIGHEST        4               // for USART etc...

/////////////////////////////////////////////////////////
// MOTOR CONFIGURATION
/////////////////////////////////////////////////////////
#define MOTOR_PWM_HZ	400
typedef os::hal::PC6 MOTOR1_PIN;
typedef os::hal::PC7 MOTOR2_PIN;
typedef os::hal::PC8 MOTOR3_PIN;
typedef os::hal::PC9 MOTOR4_PIN;
typedef os::hal::PA0 MOTOR5_PIN;
typedef os::hal::PA1 MOTOR6_PIN;
typedef os::hal::PA2 MOTOR7_PIN;
typedef os::hal::PA3 MOTOR8_PIN;

typedef os::hal::pwm::Output<os::hal::TIM8_CHANNEL1, MOTOR1_PIN, MOTOR_PWM_HZ> MOTOR1;
typedef os::hal::pwm::Output<os::hal::TIM8_CHANNEL2, MOTOR1_PIN, MOTOR_PWM_HZ> MOTOR2;
typedef os::hal::pwm::Output<os::hal::TIM8_CHANNEL3, MOTOR1_PIN, MOTOR_PWM_HZ> MOTOR3;
typedef os::hal::pwm::Output<os::hal::TIM8_CHANNEL4, MOTOR1_PIN, MOTOR_PWM_HZ> MOTOR4;
typedef os::hal::pwm::Output<os::hal::TIM5_CHANNEL1, MOTOR1_PIN, MOTOR_PWM_HZ> MOTOR5;
typedef os::hal::pwm::Output<os::hal::TIM5_CHANNEL2, MOTOR1_PIN, MOTOR_PWM_HZ> MOTOR6;
typedef os::hal::pwm::Output<os::hal::TIM5_CHANNEL3, MOTOR1_PIN, MOTOR_PWM_HZ> MOTOR7;
typedef os::hal::pwm::Output<os::hal::TIM5_CHANNEL4, MOTOR1_PIN, MOTOR_PWM_HZ> MOTOR8;

#define MOTOR_INIT_ALL MOTOR1::init(); \
  MOTOR2::init(); \
  MOTOR3::init(); \
  MOTOR3::init(); \
  MOTOR4::init(); \
  MOTOR5::init(); \
  MOTOR6::init(); \
  MOTOR7::init(); \
  MOTOR8::init();

/////////////////////////////////////////////////////////
// SERVO CONFIGURATION
/////////////////////////////////////////////////////////
#define SERVO_PWM_HZ	50
typedef os::hal::PE5 	SERVO1_PIN;
typedef os::hal::PE6 	SERVO2_PIN;
typedef os::hal::PB10 	SERVO3_PIN;
typedef os::hal::PB11 	SERVO4_PIN;

typedef os::hal::pwm::Output<os::hal::TIM9_CHANNEL1,  SERVO1_PIN, SERVO_PWM_HZ> SERVO1;
typedef os::hal::pwm::Output<os::hal::TIM9_CHANNEL2,  SERVO2_PIN, SERVO_PWM_HZ> SERVO2;
typedef os::hal::pwm::Output<os::hal::TIM2_CHANNEL3,  SERVO3_PIN, SERVO_PWM_HZ> SERVO3;
typedef os::hal::pwm::Output<os::hal::TIM2_CHANNEL4,  SERVO4_PIN, SERVO_PWM_HZ> SERVO4;

#define SERVO_INIT_ALL SERVO1::init(); \
  SERVO2::init(); \
  SERVO3::init(); \
  SERVO4::init();

/////////////////////////////////////////////////////////
// RC INPUT
/////////////////////////////////////////////////////////
typedef os::hal::PE9 	RC1_PIN;
typedef os::hal::PE11	RC2_PIN;
typedef os::hal::PE13 	RC3_PIN;
typedef os::hal::PE14 	RC4_PIN;
typedef os::hal::PD12 	RC5_PIN;
typedef os::hal::PD13	RC6_PIN;
typedef os::hal::PD14 	RC7_PIN;
typedef os::hal::PD15 	RC8_PIN;

/*typedef os::hal::pwm::Input<os::hal::TIM1_CHANNEL1,  RC1_PIN> RC1;
typedef os::hal::pwm::Input<os::hal::TIM1_CHANNEL2,  RC1_PIN> RC2;
typedef os::hal::pwm::Input<os::hal::TIM1_CHANNEL3,  RC1_PIN> RC3;
typedef os::hal::pwm::Input<os::hal::TIM1_CHANNEL4,  RC1_PIN> RC4;
typedef os::hal::pwm::Input<os::hal::TIM4_CHANNEL1,  RC1_PIN> RC5;
typedef os::hal::pwm::Input<os::hal::TIM4_CHANNEL2,  RC1_PIN> RC6;
typedef os::hal::pwm::Input<os::hal::TIM4_CHANNEL3,  RC1_PIN> RC7;
typedef os::hal::pwm::Input<os::hal::TIM4_CHANNEL4,  RC1_PIN> RC8;

#define RCINPUT_INIT_ALL RC1::init(); \
    RC2::init(); \
    RC3::init(); \
    RC4::init(); \
    RC5::init(); \
    RC6::init(); \
    RC7::init(); \
    RC8::init();
*/
/////////////////////////////////////////////////////////
// COM CONFIGURATION
/////////////////////////////////////////////////////////
#define TELEMETRY_ENABLE
typedef os::hal::SERIAL2	TELEMETRY_PORT;
#define TELEMETRY_PORT_RX_PIN	os::hal::PA3
#define TELEMETRY_PORT_TX_PIN	os::hal::PA2
#define TELEMETRY_PORT_BAUDS	57600

/////////////////////////////////////////////////////////
// SPI CONFIGURATION
/////////////////////////////////////////////////////////
typedef os::hal::HAL_SPI1 SPI1_DRIVER;
typedef os::hal::spi::SpiDevice<SPI1_DRIVER, os::hal::PE12> MPU6000_SPIDEVICE;
typedef os::hal::spi::SpiDevice<SPI1_DRIVER, os::hal::PE15> MS5611_SPIDEVICE;

#define SPIDRIVER_INIT SPI1_DRIVER::init<os::hal::PA6, os::hal::PA7, os::hal::PA5>(); \
		MPU6000_SPIDEVICE::init(); \
		MS5611_SPIDEVICE::init();


/////////////////////////////////////////////////////////
// MPU6000 CONFIGURATION
/////////////////////////////////////////////////////////
#define USE_MPU6000
typedef os::hal::PE8 MPU6000_INT_PIN;
typedef os::device::MPU6000<MPU6000_SPIDEVICE> MPU6000;


#endif /* BOARDSDEFS_H_ */
