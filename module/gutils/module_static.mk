MODULE:=gutils
VERSION:=1.0.0
TARGET:=lib$(MODULE)
PRE_DEFINED:= 
BUILD_PATH:=$(shell pwd)

SUBDIRS:= \
	gini \
	gxml

INCLUDES:=

LIBS_PATH:= 

LIBS:= 

SLIBS:=$(foreach dir, $(SUBDIRS), $(BUILD_PATH)/$(dir)/output/lib/lib$(dir).a)

OUT_HEADS:=$(wildcard $(BUILD_PATH)/*.h) \
	$(foreach dir, $(SUBDIRS), $(BUILD_PATH)/$(dir)/inc/*.h)

include ../static.mk
