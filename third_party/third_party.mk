THIRD_PARTY_PATH = $(PROJECT_PATH)/third_party

ALLINC += $(THIRD_PARTY_PATH)/

include $(THIRD_PARTY_PATH)/RTT/RTT.mk
include $(THIRD_PARTY_PATH)/ShellRTT/shell_rtt.mk
