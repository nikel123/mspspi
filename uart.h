static volatile struct {
  const char *pos;
  uint16_t    count;
} uart_tx_state;

static volatile struct {
  char     *pos;
  uint16_t  count;
} uart_rx_state;

static void
init_uart() {

  uart_rx_state.count = 0;

  // init uart
  // 115200 8o2
  UCA0CTL1 = UCSSEL_3 | UCRXEIE | UCSWRST;
  UCA0CTL0 = UCPEN | UCSPB;

#if SMCLK_8MHZ

# if UART_9600

  UCA0BR0 = 833 & 0xff;
  UCA0BR1 = 833 >> 8;
  UCA0MCTL = UCBRS_2 | UCBRF_0;

# elif UART_115200

  UCA0BR0 = 69;
  UCA0BR1 = 0;
  UCA0MCTL = UCBRS_4 | UCBRF_0;

# else
#   error "unhandled uart baud rate"
# endif

#else
# error "unhandled SMCLK frequency"
#endif

  UCA0CTL1 &= ~UCSWRST;

}

__interrupt_vec(USCIAB0TX_VECTOR) void
uart_tx_ie() {

  if ( !uart_tx_state.count ) {

    UC0IE &= ~UCA0TXIE;
    LPM0_EXIT;
    return;

  }

  UCA0TXBUF = *(uart_tx_state.pos++);
  --uart_tx_state.count;

}

__interrupt_vec(USCIAB0RX_VECTOR) void
uart_rx_ie() {

  P1OUT ^= BIT0;

  if ( uart_rx_state.count ) {
    *(uart_rx_state.pos++) = UCA0RXBUF;
    --uart_rx_state.count;
  }

  if ( !uart_rx_state.count ) {
    UC0IE &= ~UCA0RXIE;
    LPM0_EXIT;
  }

}

static void
uart_tx(
    const char *buf,
    uint16_t    len) {

  P1OUT |= BIT0;

  uart_tx_state.pos   = buf;
  uart_tx_state.count = len;

  disable_interrupt();
  UC0IE    |= UCA0TXIE;
  lpm0_eint();

  P1OUT &= ~BIT0;

}

static void
uart_rx(
    char     *buf,
    uint16_t  len) {

  P1OUT |= BIT6;

  uart_rx_state.pos   = buf;
  uart_rx_state.count = len;

  register volatile int8_t d = UCA0RXBUF;

  disable_interrupt();
  UC0IE |= UCA0RXIE;
  lpm0_eint();

  P1OUT &= ~BIT6;

}
