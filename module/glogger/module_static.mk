MODULE:=glogger
VERSION:=1.0.0
TARGET:=lib$(MODULE)
PRE_DEFINED:= 
BUILD_PATH:=$(shell pwd)

INCLUDES:=$(BUILD_PATH)/inc \
	$(GOHOOP_INC_PATH)/gsystem \
	$(GOHOOP_INC_PATH)/gutils

SRCDIRS:=$(BUILD_PATH)/src

SUBDIRS:=

LIBS_PATH:= 

LIBS:= 

SLIBS:=

OUT_HEADS:=$(wildcard $(BUILD_PATH)/inc/*.h)

include $(GOHOOP_TOP)/static.mk
