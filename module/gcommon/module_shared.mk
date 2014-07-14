MODULE:=gcommon
VERSION:=1.0.0
TARGET:=lib$(MODULE)
PRE_DEFINED:= 
BUILD_PATH:=$(shell pwd)

SUBDIRS:= \
	conf \
	filesys \
	function \
	iostream \
	network

INCLUDES:=$(BUILD_PATH)/inc \
	$(foreach dir, $(SUBDIRS), $(BUILD_PATH)/$(dir)/inc)

SRCDIRS:=$(foreach dir, $(SUBDIRS), $(BUILD_PATH)/$(dir)/src)

LIBS_PATH:= 

LIBS:= 

SLIBS:=

OUT_HEADS:=$(wildcard $(BUILD_PATH)/*.h) \
	$(wildcard $(BUILD_PATH)/inc/*.h) \
	$(foreach dir, $(SUBDIRS), $(wildcard $(BUILD_PATH)/$(dir)/inc/*.h))

include ../shared.mk
