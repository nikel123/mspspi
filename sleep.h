// Copyright (c) 2017 Andrej Gelenberg <andrej.gelenberg@udo.edu>
static volatile uint16_t sleep_counter;

static void
init_sleep_timer() {
  
  // init timer
  // CLK -> SMCLK / 8 (250kHz)
  TACTL   = TASSEL_2 | ID_3 | TACLR;
  // make it trigger after 1ms
  TACCR0  = SMCLK_F / 8 / 1000;
  TACCTL0 = 0;

}

__interrupt_vec(TIMER0_A0_VECTOR) void
timer_ie() {

  if ( ! --sleep_counter ) {
    TACCTL0 &= ~CCIE;
    TACTL &= ~MC_1;
    LPM0_EXIT;
  }

}

static void
msleep(
    uint16_t msec) {

  sleep_counter = msec;
  TACTL |= MC_1 | TACLR;
  TACCTL0 = CCIE;
  LPM0;

}
