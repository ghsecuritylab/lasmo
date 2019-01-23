#List of all the LASMO-related sources
USRSRC = src/main_F7.c \
				 src/sd.c      \
				 src/control.c \
				 src/galva.c   \
				 src/adc.c     \
				 src/max5105.c \
				 src/lasers.c  \
				 src/decoder.c \
				 src/shell_rtt.c

# Required includes directory
USRINC = includes

# Shared variables
ALLCSRC += $(USRSRC)
ALLINC  += $(USRINC)
