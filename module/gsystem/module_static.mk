MODULE:=gsystem

VERSION:=1.0.0

TARGET:=lib$(MODULE)

BUILD_PATH:=$(shell pwd)

PRE_DEFINED:=

INCLUDES:=$(BUILD_PATH)/inc

SRCDIRS:=$(BUILD_PATH)/src

LIBS_PATH:= 

LIBS:=

SLIBS:= 

OUT_HEADS:=$(wildcard $(BUILD_PATH)/*.h) \
	$(wildcard $(BUILD_PATH)/inc/*.h)

include $(GOHOOP_TOP)/static.mk
