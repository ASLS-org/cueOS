#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
void TIM2_IRQHandler(void);
void SDIO_IRQHandler(void);
void DMA2_Stream3_IRQHandler(void);
void ETH_IRQHandler(void);
void DMA2_Stream6_IRQHandler(void);

#endif
