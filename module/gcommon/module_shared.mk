PRONAME:=gcommon
VERSION:=1.0.0
TARGET:=lib$(PRONAME)
PRE_DEFINED:= 
BUILD_PATH:=$(shell pwd)

SUBDIRS:= \
	conf \
	filesys \
	function \
	iostream \
	network

INCLUDES:=$(foreach dir, $(SUBDIRS), $(BUILD_PATH)/$(dir)/inc)

SRCDIRS:=$(foreach dir, $(SUBDIRS), $(BUILD_PATH)/$(dir)/src)

LIBS_PATH:= 

LIBS:= 

SLIBS:=

include ../shared.mk
