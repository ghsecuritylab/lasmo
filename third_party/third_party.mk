THIRD_PARTY_PATH = $(PROJECT_PATH)/third_party

ALLINC += $(THIRD_PARTY_PATH)/ $(THIRD_PARTY_PATH)/ilda_decoder/

include $(THIRD_PARTY_PATH)/RTT/RTT.mk

ALLCSRC += $(THIRD_PARTY_PATH)/ilda_decoder/ilda-decoder.c
