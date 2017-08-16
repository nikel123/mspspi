static volatile struct {
  const char *pos;
  uint16_t    count;
} uart_tx_state;

static void
init_uart() {

  // init uart
  // 115200 8o2
  //UCA0CTL0 = UCPEN | UCSPB;
  UCA0CTL0 = 0;
  UCA0CTL1 = UCSSEL_3 | UCSWRST;

#if SMCLK_8MHZ

# if UART_9600

  UCA0BR0 = 0x41;
  UCA0BR1 = 0x3;
  UCA0MCTL = UCBRS_2 | UCBRF_0;

# elif UART_115200
  UCA0BR0 = 69;
  UCA0BR1 = 0;
  UCA0MCTL = UCBRF_0 | UCBRS_4;
# else
#   error "unhandled uart baud rate"
# endif

#else
# error "unhandled SMCLK frequency"
#endif
 
  //UCA0CTL1 |= UCBRKIE;

}

__interrupt_vec(USCIAB0TX_VECTOR) void
uart_tx_ie() {

  if ( !uart_tx_state.count ) {
    UC0IE &= ~UCA0TXIE;
    UCA0CTL1 |= UCSWRST;
    LPM0_EXIT;
  }

  UCA0TXBUF = *(uart_tx_state.pos++);
  --uart_tx_state.count;

}

__interrupt_vec(USCIAB0RX_VECTOR) void
uart_rx_ie() {
}

static void
uart_tx(
    const char *buf,
    uint16_t    len) {

  UCA0CTL1 &= ~UCSWRST;

  UCA0TXBUF = buf[0];

  uart_tx_state.pos          = &(buf[1]);
  uart_tx_state.count        = len - 1;

  UC0IE |= UCA0TXIE;

  LPM0;

}
