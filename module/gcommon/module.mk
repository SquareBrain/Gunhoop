PRONAME:=gcommon
VERSION:=1.0.0
TARGET:=lib$(PRONAME)
PRE_DEFINED:= 
BUILD_PATH:=$(shell pwd)

INCLUDES:=

LIBS_PATH:= 

LIBS:= 

GCOMMON_PATH:=$(GOHOOP_TOP)/gcommon
SLIBS:= \
	$(GCOMMON_PATH)/component/output/lib/libcomponent.a \
	$(GCOMMON_PATH)/system/output/lib/libsystem.a \

include ../shared.mk
