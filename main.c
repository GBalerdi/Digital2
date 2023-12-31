// vi: ts=4 shiftwidth=4
//																			  //
// Author(s):																  //
//	 Miguel Angel Sagreras
// Edited By:
// 	Balerdi German
//	Deodato Lucila
//
// Copyright (C) 2021														  //
//	  Miguel Angel Sagreras													  //
//																			  //
// This source file may be used and distributed without restriction provided  //
// that this copyright statement is not removed from the file and that any	  //
// derivative work contains  the original copyright notice and the associated //
// disclaimer.																  //
//																			  //
// This source file is free software; you can redistribute it and/or modify   //
// it under the terms of the GNU General Public License as published by the   //
// Free Software Foundation, either version 3 of the License, or (at your	  //
// option) any later version.												  //
//																			  //
// This source is distributed in the hope that it will be useful, but WITHOUT //
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or	  //
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for   //
// more details at http://www.gnu.org/licenses/.							  //
//																			  //

#define SRAM_SIZE		((uint32_t) 0x00005000)
#define SRAM_BASE		((uint32_t) 0x20000000)
#define STACKINIT		((interrupt_t)(SRAM_BASE+SRAM_SIZE))

typedef int			   int32_t;
typedef short		   int16_t;
typedef char		   int8_t;
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

typedef void(*interrupt_t)(void);

typedef union {
	uint8_t  byte[4];
	uint16_t hword[2];
	uint32_t word;
} word_t;

typedef word_t page[0x400/sizeof(uint32_t)];

// MEMORY MAP

enum {TIM2	= 0, TIM3  = 1, TIM4  = 2 };
enum {GPIOA = 0, GPIOB = 1, GPIOC = 2, GPIOD = 3, GPIOE = 4, GPIOF = 5 };
enum {DMA1	= 0 };
enum {CHN1	= 0, CHN2  = 1, CHN3  = 2, CHN4  = 3, CHN5	= 4, CHN6  = 5, CHN7 = 6, CHN8 = 7 };
enum {ADC1	= 0 };

struct {
	union {
		struct {
			uint32_t CR1;
			uint32_t CR2;
			uint32_t SMCR;
			uint32_t DIER;
			uint32_t SR;
			uint32_t EGR;
			uint32_t CCMR1;
			uint32_t CCMR2;
			uint32_t CCER;
			uint32_t CNT;
			uint32_t PSC;
			uint32_t ARR;
			uint32_t RES1;
			uint32_t CCR1;
			uint32_t CCR2;
			uint32_t CCR3;
			uint32_t CCR4;
			uint32_t BDTR;
			uint32_t DCR;
			uint32_t DMAR;
		} REGs;
		page reserved;
	} TIMs[3];

	word_t reserved1[(0x40002800-0x40000c00)/sizeof(word_t)];
	page RTC;
	page WWDG;
	page IWDG;
	word_t reserved2[(0x40003800-0x40003400)/sizeof(word_t)];
	page SPI2;
	word_t reserved3[(0x40004400-0x40003c00)/sizeof(word_t)];
	page USART[2];
	word_t reserved4[(0x40005400-0x40004c00)/sizeof(word_t)];
	page I2C[2];
	page USB;
	page USBCAN_SRAM;
	page bxCAN;
	word_t reserved5[(0x40006c00-0x40006800)/sizeof(word_t)];
	page BKP;
	page PWR;
	word_t reserved6[(0x40010000-0x40007400)/sizeof(word_t)];

	page AFIO;
	page EXTI;

	union {
		struct {
			uint32_t CRL;
			uint32_t CRH;
			uint32_t IDR;
			uint32_t ODR;
			uint32_t BSRR;
			uint32_t BRR;
			uint32_t LCKR;
		} REGs;
		page reserved;
	} GPIOs[5];
	word_t reserved7[(0x40012400-0x40011C00)/sizeof(word_t)];
	
	union {
		struct {
			uint32_t SR;
			uint32_t CR1;
			uint32_t CR2;
			uint32_t SMPR1;
			uint32_t SMPR2;
			uint32_t JOFR[4];
			uint32_t HTR;
			uint32_t LTR;
			uint32_t SQR1;
			uint32_t SQR2;
			uint32_t SQR3;
			uint32_t JSQR;
			uint32_t JDR[4];
			uint32_t DR;
		} REGs;
		page reserved;
	} ADC[2];
	page TIM1;
	page SPI1;
	word_t reserved8[(0x40013800-0x40013400)/sizeof(word_t)];
	
	union  {
		struct {
			uint32_t SR;
			uint32_t DR;
			uint32_t BRR;
			uint32_t CR1;
			uint32_t CR2;
			uint32_t CR3;
			uint32_t GTPR;
		} REGs;
		page reserved;
	} USART1;
	
	word_t reserved9[(0x40020000-0x40013C00)/sizeof(word_t)];
	union {
		struct {
			uint32_t ISR;
			uint32_t IFCR;
			struct {
				uint32_t CCR;
				uint32_t CNDTR;
				uint32_t CPAR;
				uint32_t CMAR;
				uint32_t RESERVED;
			} CHN[8];
		} REGs;
		page reserved;
	} DMAs[1];
	word_t reservedA[(0x40021000-0x40020400)/sizeof(word_t)];

	union {
		struct {
			uint32_t CR;
			uint32_t CFGR;
			uint32_t CIR;
			uint32_t APB2RSTR;
			uint32_t APB1RSTR;
			uint32_t AHBENR;
			uint32_t APB2ENR;
			uint32_t APB1ENR;
			uint32_t BDCR;
			uint32_t CSR;
			uint32_t AHBRSTR;
			uint32_t CFGR2;
		} REGs;
		page reserved;
	} RCC;

} volatile *const DEVMAP = (void *) 0x40000000;

#define ENA_IRQ(IRQ) {CTX->NVIC.REGs.ISER[((uint32_t)(IRQ) >> 5)] = (1 << ((uint32_t)(IRQ) & 0x1F));}
#define DIS_IRQ(IRQ) {CTX->NVIC.REGs.ICER[((uint32_t)(IRQ) >> 5)] = (1 << ((uint32_t)(IRQ) & 0x1F));}
#define CLR_IRQ(IRQ) {CTX->NVIC.REGs.ICPR[((uint32_t)(IRQ) >> 5)] = (1 << ((uint32_t)(IRQ) & 0x1F));}

struct {
	word_t reversed0[(0xe000e010-0xe0000000)/sizeof(word_t)];
	union {
		struct {
			uint32_t CSR;
			uint32_t RVR;
			uint32_t CVR;
			uint32_t CALIB;
		} REGs;
	} SYSTICK;
	word_t reversed1[(0xe000e100-0xe000e020)/sizeof(word_t)];
	
	union {
		struct {
			uint32_t ISER[8];
			uint32_t RES0[24];
			uint32_t ICER[8];
			uint32_t RES1[24];
			uint32_t ISPR[8];
			uint32_t RES2[24];
			uint32_t ICPR[8];
			uint32_t RES3[24];
			uint32_t IABR[8];
			uint32_t RES4[56];
			uint8_t  IPR[240];
			uint32_t RES5[644];
			uint32_t STIR;
		} REGs;
	} NVIC;
} volatile *const CTX = ((void *) 0xE0000000);

enum IRQs {
	IRQ_TIMER4	  = 30,	//pag 202
	IRQ_USART1	  = 37,
};

int  main(void);
void handler_systick(void);
void handler_timer4(void);
void handler_usart1(void);

// INTERRUPT VECTOR NVIC
const interrupt_t vector_table[] __attribute__ ((section(".vtab"))) = {
	STACKINIT,			// 0x0000_0000 Stack Pointer
	(interrupt_t) main, // 0x0000_0004 Reset
	0,					// 0x0000_0008
	0,					// 0x0000_000C
	0,					// 0x0000_0010
	0,					// 0x0000_0014
	0,					// 0x0000_0018
	0,					// 0x0000_001C
	0,					// 0x0000_0020
	0,					// 0x0000_0024
	0,					// 0x0000_0028
	0,					// 0x0000_002C
	0,					// 0x0000_0030
	0,					// 0x0000_0034
	0,					// 0x0000_0038
	handler_systick,	// 0x0000_003C Systick timer
	0,					// 0x0000_0040
	0,					// 0x0000_0044
	0,					// 0x0000_0048
	0,					// 0x0000_004C
	0,					// 0x0000_0050
	0,					// 0x0000_0054
	0,					// 0x0000_0058
	0,					// 0x0000_005C
	0,					// 0x0000_0060
	0,					// 0x0000_0064
	0,					// 0x0000_0068
	0,					// 0x0000_006C
	0,					// 0x0000_0070
	0,					// 0x0000_0074
	0,					// 0x0000_0078
	0,					// 0x0000_007C
	0,					// 0x0000_0080
	0,					// 0x0000_0084
	0,					// 0x0000_0088
	0,					// 0x0000_008C
	0,					// 0x0000_0090
	0,					// 0x0000_0094
	0,					// 0x0000_0098
	0,					// 0x0000_009C
	0,					// 0x0000_00A0
	0,					// 0x0000_00A4
	0,					// 0x0000_00A8
	0,					// 0x0000_00AC
	0,					// 0x0000_00B0 
	0,					// 0x0000_00B4
	handler_timer4,		// 0x0000_00B8 TIM4	pag 202
	0,					// 0x0000_00BC
	0,					// 0x0000_00C0
	0,					// 0x0000_00C4
	0,					// 0x0000_00C8
	0,					// 0x0000_00CC
	0,					// 0x0000_00D0
	handler_usart1,		// 0x0000_00D4 USART1
	0,					// 0x0000_00D8
	0,					// 0x0000_00DC
};


volatile uint32_t tick;
void handler_systick(void)
{
	tick++;
}

volatile uint32_t capture;
volatile uint32_t aux_cap;
void handler_timer4(void){
	if (DEVMAP->TIMs[TIM4].REGs.CCER & (1<<1)){				// Falling edge capture
		capture = DEVMAP->TIMs[TIM4].REGs.CCR1 - aux_cap;
		DEVMAP->TIMs[TIM4].REGs.CCER &= ~(1<<1);			// Set rising edge Interrupt
	}
	else{													// Rising Edge capture
		aux_cap = DEVMAP->TIMs[TIM4].REGs.CCR1;				// Access register value
		DEVMAP->TIMs[TIM4].REGs.CCER |= (1<<1);				// Set Falling edge interrupt
	}
	DEVMAP->GPIOs[GPIOC].REGs.ODR ^= (1<<13);
	DEVMAP->TIMs[TIM4].REGs.SR &= ~((1 << 9) | (1 << 1)); 	// Clear interrupt flag 1 & overflag 1 Bits
	CLR_IRQ(IRQ_TIMER4);
}

volatile uint32_t tx_rdy;
volatile uint32_t tx_req;
void handler_usart1(void)
{
	tx_rdy = tx_req;
	DEVMAP->USART1.REGs.CR1 &= ~(1 << 7);					 // Disable TXE Interrupt
	CLR_IRQ(IRQ_USART1);
}

int main(void)
{
	// PCLK code
	DEVMAP->RCC.REGs.CR   |= (1 << 16);						// Enable HSE
	while (!(DEVMAP->RCC.REGs.CR & (1 << 17)));				// Wait for HSE is locked

	DEVMAP->RCC.REGs.CR   &= ~(1 << 24);					// Disable PLL
	DEVMAP->RCC.REGs.CFGR |= (0b0100 << 18);				// Set PLLMULL to 6. Set PLL output clock to 48 Mhz
	DEVMAP->RCC.REGs.CFGR |=  (1 << 16);					// Select HSE as the PLL source clock
	DEVMAP->RCC.REGs.CR   |=  (1 << 24);					// Enable PLL
	while (!(DEVMAP->RCC.REGs.CR & (1 << 25)));				// Wait for PLL is locked

	DEVMAP->RCC.REGs.CFGR |= (0b1000 << 4);					// Set AHB HPRE division to 2. Set AHB clock to 24 Mhz

	DEVMAP->RCC.REGs.CFGR |= (0b10 << 0);					// Select PLL clock as the system clock
	while (!(DEVMAP->RCC.REGs.CFGR & (0b10 << 2)));			// Wait for PLL clock to be selected

	// SYSTICK Code
	DEVMAP->RCC.REGs.APB2ENR |= (1 << 3);					// Enable GPIOB clock
	DEVMAP->RCC.REGs.APB2ENR |= (1 << 4);

	DEVMAP->GPIOs[GPIOC].REGs.CRH  = 0x33333333;

	DEVMAP->GPIOs[GPIOB].REGs.CRH &= 0xFF0FFFFF;
	DEVMAP->GPIOs[GPIOB].REGs.CRH |= 0x00300000;			// Port B13 output


	CTX->SYSTICK.REGs.CSR  = 0x00000;						// Clear register, set to run at AHB/8 -> 24 Mhz/8 = 3 Mhz
	CTX->SYSTICK.REGs.CSR |= (1 << 1);						// Enable interrupt
	CTX->SYSTICK.REGs.RVR = 1000000;						// Set 1/3 second tick
	CTX->SYSTICK.REGs.CSR |= (1 << 0);						// Enable SysTick
	CTX->SYSTICK.REGs.CVR = 0;								// Clear register to start

	// TIMER code

	// Port Configuration
	DEVMAP->RCC.REGs.APB1ENR |= (1<<2);						// Enable Timer 4 pag 115

	DEVMAP->GPIOs[GPIOB].REGs.CRL &= 0xF0FFFFFF;
	DEVMAP->GPIOs[GPIOB].REGs.CRL |= 0x04000000;			// Set port B6 to input floating. pag.166

	// Timer basic function
	DEVMAP->TIMs[TIM4].REGs.ARR = 0xFFFE;					// Period
	DEVMAP->TIMs[TIM4].REGs.PSC = 23;						// Prescaler = 23
	//DEVMAP->TIMs[TIM4].REGs.CR1 &= ~(3<<8);
	//DEVMAP->TIMs[TIM4].REGs.CR1 &= ~(1<<4);
	DEVMAP->TIMs[TIM4].REGs.CR1 =0;

	// Input Capture configuration
	DEVMAP->TIMs[TIM4].REGs.CCMR1 = 0;						// Reset reg
	DEVMAP->TIMs[TIM4].REGs.CCMR1 |= (1<<0);				// Set input capture
	DEVMAP->TIMs[TIM4].REGs.CCER   = 0;						// Reset reg,rising edge active
	DEVMAP->TIMs[TIM4].REGs.CCER  |= (1<<0);				// Input capture channel 1
	DEVMAP->TIMs[TIM4].REGs.DIER  |= (1<<1);				// Interrupt enable input capture
	//DEVMAP->TIMs[TIM4].REGs.DIER |= (1<<0);				// Update interrupt enable

	ENA_IRQ(IRQ_TIMER4);									// Enable TIMER4 NVIC IRQ
	DEVMAP->TIMs[TIM4].REGs.CR1 |= (1<<0);					// start timer

	// USART CODE
	DEVMAP->RCC.REGs.APB2ENR |= (1 << 2);					// Enable GPIOA clock
	DEVMAP->RCC.REGs.APB2ENR |= (1 << 14);					// Enable UART1 clock

	DEVMAP->GPIOs[GPIOA].REGs.CRH &= 0xFFFFF00F;
	DEVMAP->GPIOs[GPIOA].REGs.CRH |= 0x000008B0;			// PA10 : RX, PA9 TX

	DEVMAP->USART1.REGs.CR1 = 0;							// Reset del CR1
	DEVMAP->USART1.REGs.CR1 &= ~(1 << 12);					// Word length - leave default (8 data)
	DEVMAP->USART1.REGs.CR2 |= (0b00 << 12);				// Number of stop bits - leave default (1 stop)
	DEVMAP->USART1.REGs.BRR =  2500;						// Set BRR to (24 Mhz/9600 bdps) = 2500
	DEVMAP->USART1.REGs.CR1 |= (1 << 3);					// Transmitter enable
//	DEVMAP->USART1.REGs.CR1 |= (1 << 5);					// RXNEIE : Enable RXNE Interrupt
//	DEVMAP->USART1.REGs.CR1 |= (1 << 2);					// Receiver enable
	DEVMAP->USART1.REGs.CR1 |= (1 << 13);					// Enable USART1
	ENA_IRQ(IRQ_USART1);									// Enable USART1 NVIC IRQ

	/* ******** */
	/* PROCESS  */ {
	/* ******** */
		enum states { S_SYSTICK, S_INPUT, S_TXDATA };
		enum states state;
		uint8_t tx_data[7];
		uint8_t *tx_ptr = tx_data;

		tx_data[3] = 'C';
		tx_data[4] = 'm';
		tx_data[5] = '\r';
		tx_data[6] = '\n';

		DEVMAP->GPIOs[GPIOC].REGs.ODR |= (1<<13);
		tick = 0;
		capture=0;
		state = S_SYSTICK;
		for(;;) {
			switch (state) {
				case S_SYSTICK :
					if (tick >1) {
						DEVMAP->GPIOs[GPIOB].REGs.ODR &= ~(1<<13);
						tick=0;
						capture=0;
						DEVMAP->TIMs[TIM4].REGs.EGR |= (1<<0);	//update event (reset timer)
						state = S_INPUT;
					}
				break;
				case S_INPUT:
					if(capture!=0) {
						static const uint8_t hextab[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
						uint32_t dr;
						uint8_t *dst;

						dr	= capture/55;
						dst = tx_data+sizeof(tx_data)-4;
				  		while (tx_data < dst--) {
							*dst = hextab[(dr%10)];
							dr /= 10;
						}
						tx_req = tx_rdy;
						tx_ptr = tx_data;
						state = S_TXDATA;
					}
					if (tick>1){
						DEVMAP->GPIOs[GPIOB].REGs.ODR |= (1<<13);
						tick=0;
						state = S_SYSTICK;
					}
				break;
				case S_TXDATA:
					if (tx_ptr < tx_data+sizeof(tx_data)) {
						if (tx_req == tx_rdy) {
							tx_req = !tx_rdy;
							DEVMAP->USART1.REGs.DR = *tx_ptr++;
							DEVMAP->USART1.REGs.CR1 |= (1 << 7);	// TXEIE : Enable TXE Interrupt
						}
					}
					else {
						DEVMAP->GPIOs[GPIOB].REGs.ODR |= (1<<13);
						tick=0;
						state = S_SYSTICK;
					}
				}
			}
		}
	return 0;
}
