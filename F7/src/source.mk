#List of all the LASMO-related sources

USRSRC = src/main_F7.c   \
				 src/sd.c        \
				 src/control.c   \
				 src/galva.c     \
				 src/adc.c       \
				 src/max5105.c   \
				 src/lasers.c    \
				 src/decoder.c   \
				 src/uart.c      \
				 src/converter.c \
				 src/shell_rtt.c \
				 src/eth_F7.c

# Required includes directory
USRINC = includes

# Shared variables
ALLCSRC += $(USRSRC)
ALLINC  += $(USRINC)
