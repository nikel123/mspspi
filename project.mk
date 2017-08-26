AUTHOR  = Andrej Gelenberg <andrej.gelenberg@udo.edu>
VERSION = 0.0

CPU := msp430g2553

all: mspspi
CLEAN += mspspi

mspspi: mspspi.o
mspspi.s: mspspi.o

.PHONY: falsh
flash: mspspi
	mspdebug rf2500 "prog $<"
