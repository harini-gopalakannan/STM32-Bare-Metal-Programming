#include "stm32f4xx.h"
#include "adc.h"

#define ADC1EN (1U<<8)
#define ADC_CH1 (1U<<0)
#define GPIOAEN (1U<<0)
#define ADC_SEQ_LEN_1 0x00
#define CR2_ADON (1U<<0)
#define CR2_SWSTART (1U<<30)
#define SR_EOC (1U<<1)

/*---------- DMA2 CHANNEL 0 STREAM 0 --------------------*/
#define DMA2EN (1U<<22)
#define DMA_S0_EN (1U<<0)
#define CHSEL0_MASK (7U<<25)
#define DMA_MEM_INC (1U<<10)
#define DMA_DIR_PERIPH_TO_MEM_MASK (3U<<6)
#define DMA_CR_TCIE (1U<<4)
#define DMA_PSIZE_16BIT (1U << 11)
#define DMA_MSIZE_16BIT (1U << 13)

#define ADC_DMA_EN (1U<<8)

#define ADC_DMA_EXTEN (1U<<28)
#define ADC_DDS (1U<<9)

/*for DMA to start automatically = circular buffer (no need to press nrst everytime)*/
//#define DMA_CR_CIRC (1U << 8)

/*----------------------------------------*/
volatile uint16_t data;

void dma2_stream0_init()
{
	/*enable clock access to dma - connect to ahb1*/
	RCC->AHB1ENR |= DMA2EN;

	/*disable dma2 stream0*/
	DMA2_Stream0->CR &=~ DMA_S0_EN;

	/*wait until DMA2 stream0 is disabled*/
	while (DMA2_Stream0->CR & DMA_S0_EN){};

	/*clear all interrupt flags of stream0*/
	DMA2->LIFCR |= (1U<<0);
	DMA2->LIFCR |= (1U<<2);
	DMA2->LIFCR |= (1U<<3);
	DMA2->LIFCR |= (1U<<4);
	DMA2->LIFCR |= (1U<<5);

	/*set the destination buffer*/
	DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;

	/*set the source buffer*/
	DMA2_Stream0->M0AR =(uint32_t) &data;

	/*set the length*/
	DMA2_Stream0->NDTR = 1;

	/*SELECT STREAM0 AND CHANNEL 0*/
	DMA2_Stream0->CR &=~(CHSEL0_MASK);

	/*enable memory to increment*/
	DMA2_Stream0->CR &=~ DMA_MEM_INC;

	/*configure the transfer direction (RX) - here, data moving from PERIPHERAL TO MEMORY*/
	DMA2_Stream0->CR &=~(DMA_DIR_PERIPH_TO_MEM_MASK);

	/*ENABLE DMA TX COMPLETE INTERRUPT*/
	DMA2_Stream0->CR |= DMA_CR_TCIE;

	/*enable direct mode, disable FIFO*/
	DMA2_Stream0->FCR = 0;

	// Clear PSIZE and MSIZE
	DMA2_Stream0->CR &= ~((3U << 11) | (3U << 13));

	// Set peripheral size = 16-bit
	DMA2_Stream0->CR |= DMA_PSIZE_16BIT;

	// Set memory size = 16-bit
	DMA2_Stream0->CR |= DMA_MSIZE_16BIT;

	/*enable circular mode*/
	//DMA2_Stream0->CR |= DMA_CR_CIRC;

	/*ENABLE DMA2 STREAM0*/
	DMA2_Stream0->CR |= DMA_S0_EN;

	/*ENABLE ADC1 DMA*/
	ADC1->CR2 |= ADC_DMA_EN;

	/*ENABLE EXTERNAL TRIGGER AT RISING EDGE*/
	ADC1->CR2 &= ~(3U << 28);  // clear EXTEN
	ADC1->CR2 |= ADC_DMA_EXTEN;

	/*select the external event used to trigger the start of conversion: TIM2 TRGO */
	ADC1->CR2 &=~ (1U<<24);
	ADC1->CR2 |= (1U<<25);
	ADC1->CR2 |= (1U<<26);
	ADC1->CR2 &=~ (1U<<27);

	/*SET DDS TO ENABLE CONTINOUS DMA REQUESTS*/
	ADC1->CR2 |= ADC_DDS;

	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void pa1_adc_init(void){
	/**Configure the ADC GPIO pin****/
	/*Enable clock access to gpioA*/
	RCC->AHB1ENR|=GPIOAEN;
	/*Set the mode of PA1 to analog*/
	GPIOA->MODER |=(1U<<2);
	GPIOA->MODER |=(1U<<3);
	/*Configure the ADC module*/
	/*Enable clock acces to ADC*/
	RCC->APB2ENR |= ADC1EN;
	/*Configure adc parameters*/
	/*CONVERSION seqn start*/
	ADC1->SQR3 = ADC_CH1;
	/*COnversion seqn length*/
	ADC1->SQR1 = ADC_SEQ_LEN_1;

	/*set sampling time*/
	//ADC1->SMPR2 &= ~(7U << 3);
	//ADC1->SMPR2 |=  (7U << 3);

	/*Enable ADC module*/
	ADC1->CR2 |= CR2_ADON;

	for(volatile int i = 0; i < 1000; i++);
}


