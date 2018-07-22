/*
 * CC1101.c
 *
 *  Created on: Jul 20, 2018
 *      Author: aprey2016
 */

#include "CC1101.h"


//
void spi_init() {
	P3SEL |= BIT3+BIT4;                       // P3.3,4 option select, Data Out (UCA0SIMO), Data In (UCA0SOMI)
	P2SEL |= BIT7;                            // P2.7 option select, Serial Clock Out (UCA0CLK)
	UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**

	UCA0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB;    // 3-pin, 8-bit SPI master
	                                          // Clock polarity high, MSB
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 = 0x02;                           // /2
	UCA0BR1 = 0;                              // High byte

	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}

bool spi_busy() {
	return (UCB0STAT & UCBUSY);    			  //USCI_B0 BUSY?
}

void spi_bus_wait()
{
	while (UCB0STAT & UCBUSY); 		// USCI_B0 BUSY?
}

uint8_t spi_read_bus()
{

	while (!(UCB0IFG & UCRXIFG) && (UCB0STAT & UCBUSY)); 		// USCI_B0 RX buffer/bus busy?
	uint8_t data = UCB0RXBUF; 		// Store received data
	return data;
}

void spi_write_bus(uint8_t data){
	while (!(UCB0IFG & UCTXIFG) && (UCB0STAT & UCBUSY));   		// USCI_B0 TX buffer/bus busy?
	UCB0TXBUF = data;               // Send data
}

