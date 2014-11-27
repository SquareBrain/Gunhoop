PRONAME:=g_logger_test
TARGET:=$(PRONAME)
PRE_DEFINED:= 
BUILD_PATH:=$(shell pwd)

INCLUDES:=$(BUILD_PATH)

SRCDIRS:=$(BUILD_PATH) \
    $(GOHOOP_INC_PATH)/gsystem \
    $(GOHOOP_INC_PATH)/gutils \
    $(GOHOOP_INC_PATH)/glogger

LIBS_PATH:=$(GOHOOP_LIB_PATH)

LIBS:=gsystem gutils glogger

SLIBS:=

include $(GOHOOP_TOP)/app.mk
