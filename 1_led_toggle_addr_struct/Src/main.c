#include <stdint.h>

#define periph_base (0x40000000UL)
#define ahb1_periph_offset (0x00020000UL)
#define ahb1_periph_base (periph_base + ahb1_periph_offset)
#define gpioa_offset (0x0000000UL)
//#define gpioc_offset (0x800UL)

#define gpioa_base (ahb1_periph_base+gpioa_offset)
//#define gpioc_base (ahb1_periph_base+gpioc_offset)

#define rcc_offset (0x3800UL)
#define rcc_base (rcc_offset+ahb1_periph_base)

typedef struct
{
  volatile uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
  uint32_t DUMMY[4];
  volatile uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */

} GPIO_TypeDef;

typedef struct
{
  uint32_t DUMMY[12];
  volatile uint32_t AHB1ENR;       /*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */

} RCC_TypeDef;

#define gpioaen (1U<<0)
//#define gpioc_en (1U<<2)

#define pin5 (1U<<5)
#define led_pin pin5
//#define pin13 (1U<<5)
//#define led_pin pin13

#define RCC ((RCC_TypeDef *)rcc_base)
#define gpioa ((GPIO_TypeDef *)gpioa_base)

int main(void){
	/* 1. enable clock access to gpioa */
	RCC ->AHB1ENR|= gpioaen;
	//rcc_ahb1en_r |=gpiocen;
	/* 2. set pa5 as output pin */
	gpioa->MODER |= (1U<<10);
	gpioa->MODER &=~(1U<<11);
	while (1){
		/* 3. set pa5 high */
		gpioa->ODR ^= led_pin;

		for (int i=0; i<100000; i++){
				}
	}
}



