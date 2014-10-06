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
	$(foreach dir, $(SUBDIRS), $(BUILD_PATH)/$(dir)/inc) \
	$(GOHOOP_INC_PATH)/gsystem \
	$(GOHOOP_INC_PATH)/gutils \
	$(GOHOOP_INC_PATH)/glogger

SRCDIRS:=$(foreach dir, $(SUBDIRS), $(BUILD_PATH)/$(dir)/src)

LIBS_PATH:= 

LIBS:= 

SLIBS:=

OUT_HEADS:=$(wildcard $(BUILD_PATH)/*.h) \
	$(wildcard $(BUILD_PATH)/inc/*.h) \
	$(foreach dir, $(SUBDIRS), $(wildcard $(BUILD_PATH)/$(dir)/inc/*.h))

include ../static.mk
