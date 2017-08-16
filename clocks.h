// Copyright (c) 2017 Andrej Gelenberg <andrej.gelenberg@udo.edu>

#ifdef MCLK_16MHZ

# define MCLK_F 16000000

#else  // MCLK_16MHZ
# error "no main clock define set"
#endif // MCLK_16MHZ

#if SMCLK_2MHZ
# define SMCLK_F 2000000
#elif SMCLK_4MHZ
# define SMCLK_F 4000000
#elif SMCLK_8MHZ
# define SMCLK_F 8000000
#else
# error "no smclk definiton"
#endif


static void
init_clocks() {

  // init clock
#if MCLK_16MHZ
  DCOCTL  = 0;
  BCSCTL1 = CALBC1_16MHZ;
  DCOCTL  = CALDCO_16MHZ;
#endif

  BCSCTL2 = SELM_0
#if MCLK_16MHZ
    | DIVM_0
#endif

#if SMCLK_2MHZ
    | DIVS_3
#elif SMCLK_4MHZ
    | DIVS_2
#elif SMCLK_8MHZ
    | DIVS_1
#endif
    ;

}
