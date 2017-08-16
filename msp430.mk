# Copyright (c) 2013-2017 Andrej Gelenberg <andrej.gelenberg@udo.edu>

ifndef CPU
$(error CPU ist not set)
endif

-include *.d

CFLAGS += -mmcu=$(CPU) -MMD
CHOST  = msp430-elf
CC     = $(CHOST)-gcc

CLEAN += *.o *.d

%.o: CFLAGS += -MMD
%.o: %.c
	$(CC) -c $(CFLAGS) -o '$@' '$<'

%.o: %.s
	$(CC) -c $(CFLAGS) -o '$@' '$<'

%.s: %.c
	$(CC) -S -c $(CFLAGS) -o '$@' '$<'

%: %.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(filter %.o,$^) $(LIBS) -o '$@'

.PHONY: clean
clean:
	-rm -rf $(CLEAN)
