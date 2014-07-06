PRONAME:=component
TARGET:=lib$(PRONAME)
PRE_DEFINED:= 
BUILD_PATH:=$(shell pwd)

SUBDIRS:= \
	conf \
	filesys \
	function \
	iostream \
	network

INCLUDES:=$(foreach dir, $(SUBDIRS), $(BUILD_PATH)/$(dir)/inc) \
	$(GOHOOP_TOP)/gcommon/system/inc
	
SRCDIRS:=$(foreach dir, $(SUBDIRS), $(BUILD_PATH)/$(dir)/src)

LIBS_PATH:= 

LIBS:= 

SLIBS:=

include ../../static.mk
