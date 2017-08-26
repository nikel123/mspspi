// Copyright (c) 2017 Andrej Gelenberg <andrej.gelenberg@udo.edu>

#include <stdint.h>
#include <msp430g2553.h>
#include <time.h>

#define MCLK_16MHZ 1
#define SMCLK_8MHZ 1
#define UART_9600 1
//#define UART_115200 1 // not supported by launchpad uart

#define enable_interrupt()  asm("EINT")
#define disable_interrupt() asm("DINT")
#define lpm0_eint() _BIS_SR(LPM0_bits | GIE)

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

}

#define print(msg) uart_tx(msg, sizeof(msg))
#define println(msg) print(msg "\n\r")

void
main() {

  char cmd;

  init();

  println("\n\rmspspi v0.0 Copyright (c) 2017 Andrej Gelenberg <andrej.gelenberg@udo.edu>");

  for(;;) {

    print("\n\r> ");

    uart_rx(&cmd, 1);

    print("\n\rcmd: ");
    uart_tx(&cmd, 1);
    print("\n\r");

    switch(cmd) {

      case '?':
      case 'h':
      case 'H':
        println("Help TODO");
        break;

      default:
        println("unrecognized command");

    }

  }

}
