MODULE:=gutils
VERSION:=1.0.0
TARGET:=lib$(MODULE)
PRE_DEFINED:= 
BUILD_PATH:=$(shell pwd)

SUBDIRS:= \
	gini \
	glogger \
	gxml

INCLUDES:=

LIBS_PATH:= 

LIBS:= 

SLIBS:=

OUT_HEADS:=$(wildcard $(BUILD_PATH)/*.h) \
	$(foreach dir, $(SUBDIRS), $(BUILD_PATH)/$(dir)/inc/*.h)

include ../static.mk
