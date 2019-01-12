#List of all the LASMO-related sources
USRSRC = src/main_F7.c \
				 src/sd.c			 \
				 src/galva.c   \
				 src/max5105.c \
				 src/lasers.c

# Required includes directory
USRINC = includes

# Shared variables
ALLCSRC += $(USRSRC)
ALLINC  += $(USRINC)
