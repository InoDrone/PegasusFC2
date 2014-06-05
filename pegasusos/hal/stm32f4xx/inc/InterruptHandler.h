/*
 * InterruptHandler.h
 *
 *  Created on: 3 juin 2014
 *      Author: alienx
 */

#ifndef INTERRUPTHANDLER_H_
#define INTERRUPTHANDLER_H_

#include <pegasusos.h>

extern "C" {
  extern unsigned int _estack;

  extern void SVC_Handler();
  extern void PendSV_Handler();
  extern void SysTick_Handler();
}

namespace os {
  namespace hal {
	namespace isr {
		extern void Reset(void);
		extern void NMI(void);
		extern void HardFault(void);
		extern void MemManage(void);
		extern void BusFault(void);
		extern void UsageFault(void);

		/*extern void ServiceCall(void);
		extern void PendService(void);
		extern void SystemTick(void);*/

		extern void DebugMon(void);
		extern void DefaultHandler(void);

		extern void DMA1_Stream3_IRQ(void);
	}

	__attribute__((section(".cpu_vectors")))
	uint32_t interruptVectors[] = {
		// Top
		reinterpret_cast<uint32_t>(&_estack),
		// Reset
		reinterpret_cast<uint32_t>(os::hal::System::startup),
		// NMI
		reinterpret_cast<uint32_t>(os::hal::isr::NMI),
		// HardFault
		reinterpret_cast<uint32_t>(os::hal::isr::HardFault),
		// MemManage
		reinterpret_cast<uint32_t>(os::hal::isr::MemManage),
		// BusFault
		reinterpret_cast<uint32_t>(os::hal::isr::BusFault),
		// UsageFault
		reinterpret_cast<uint32_t>(os::hal::isr::UsageFault),
		0, 0, 0, 0,
		// Service handler
		reinterpret_cast<uint32_t>(SVC_Handler),
		// Debug Monitor
		reinterpret_cast<uint32_t>(os::hal::isr::DebugMon),
		0,
		// PendService
		reinterpret_cast<uint32_t>(PendSV_Handler),
		// System Tick Handler
		reinterpret_cast<uint32_t>(SysTick_Handler)
	};

	__attribute__((section(".periph_vectors")))
	uint32_t interruptPeriVectors[] = {
			/* WWDG_IRQHandler          Window WatchDog */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* PVD_IRQHandler           PVD through EXTI Line detection */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* TAMP_STAMP_IRQHandler    Tamper and TimeStamps through the EXTI line */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* RTC_WKUP_IRQHandler      RTC Wakeup through the EXTI line */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* FLASH_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* RCC_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* EXTI0_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* EXTI1_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* EXTI2_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* EXTI3_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* EXTI4_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* DMA1_Stream0_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* DMA1_Stream1_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* DMA1_Stream2_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* DMA1_Stream3_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DMA1_Stream3_IRQ),
			/* DMA1_Stream4_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* DMA1_Stream5_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* DMA1_Stream6_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* ADC_IRQHandler ADC1-3  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* CAN1_TX_IRQHandler CAN1 TX  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* CAN1_RX0_IRQHandler CAN1 RX0  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* CAN1_RX1_IRQHandler CAN1 RX1  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* CAN1_SCE_IRQHandler CAN1 SCE  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* EXTI9_5_IRQHandler        External Line[9:5]s  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* TIM1_BRK_TIM9_IRQHandler         TIM1 Break and TIM9     */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* TIM1_UP_TIM10_IRQHandler         TIM1 Update and TIM10     */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* TIM1_TRG_COM_TIM11_IRQHandler    TIM1 Trigger and Commutation and TIM11     */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* TIM1_CC_IRQHandler               TIM1 Capture/Compare INT    */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* TIM2_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* TIM3_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* TIM4_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* I2C1_EV_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* I2C1_ER_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* I2C2_EV_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* I2C2_ER_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* SPI1_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* SPI2_IRQHandler */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* UART1 */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* UART2 */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* UART3 */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* EXTI15_10_IRQHandler             External Line[15:10]s */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* RTC_Alarm_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* OTG_FS_WKUP_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* TIM8_BRK_TIM12_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* TIM8_UP_TIM13_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* TIM8_TRG_COM_TIM14_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* TIM8_CC_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* DMA1_Stream7_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* FSMC_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* SDIO_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* TIM5_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* SPI3_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* UART4_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* UART5_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* TIM6_DAC_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* TIM7_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* DMA2_Stream0_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* DMA2_Stream1_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* DMA2_Stream2_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* DMA2_Stream3_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* DMA2_Stream4_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* ETH_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* ETH_WKUP_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* CAN2_TX_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* CAN2_RX0_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* CAN2_RX1_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* CAN2_SCE_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* OTG_FS_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* DMA2_Stream5_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* DMA2_Stream6_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* DMA2_Stream7_IRQHandler  */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* USART6_IRQHandler   */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* I2C3_EV_IRQHandler    */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* I2C3_ER_IRQHandler    */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* OTG_HS_EP1_OUT_IRQHandler    */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* OTG_HS_EP1_IN_IRQHandler    */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* OTG_HS_WKUP_IRQHandler     */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* OTG_HS_IRQHandler     */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* DCMI_IRQHandler     */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* CRYP_IRQHandler     */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* HASH_RNG_IRQHandler     */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler),
			/* FPU_IRQHandler      */
			reinterpret_cast<uint32_t>(os::hal::isr::DefaultHandler)
	};

} /* namespace hal */
} /* namespace os */

#endif /* INTERRUPTHANDLER_H_ */
