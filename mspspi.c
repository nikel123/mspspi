// Copyright (c) 2017 Andrej Gelenberg <andrej.gelenberg@udo.edu>

#include <stdint.h>
#include <msp430g2553.h>
#include <time.h>

#define MCLK_16MHZ 1
#define SMCLK_8MHZ 1
#define UART_9600 1
//define UART_115200

#include "clocks.h"
#include "sleep.h"
#include "uart.h"

static void
init() {

  IE1 = 0;
  IE2 = 0;

  // init IO Ports

  // enable uart
  P1SEL = BIT1 | BIT2;
  P1SEL2= BIT1 | BIT2;

  P1DIR = 0xff;
  P1OUT = 0;

  P2SEL = 0;
  P2SEL2= 0;
  P2DIR = 0xff;
  P2OUT = 0;

  init_clocks();
  init_uart();
  init_sleep_timer();

  // disable whatchdog
  WDTCTL = WDTPW | WDTHOLD;

  __eint();

}

void
main() {

  init();

  while(1) {

    P1OUT ^= BIT6;
    msleep(1000);
    static const char msg[] = "Hello World!\n\r";
    uart_tx(msg, sizeof(msg));

  }

}
