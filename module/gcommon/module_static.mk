PRONAME:=gcommon
VERSION:=1.0.0
TARGET:=lib$(PRONAME)
PRE_DEFINED:= 
BUILD_PATH:=$(shell pwd)

INCLUDES:=

LIBS_PATH:= 

LIBS:= 

MODULE_PATH:=$(GOHOOP_TOP)/gcommon
SLIBS:= \
	$(MODULE_PATH)/component/output/lib/libcomponent.a \

include ../static.mk
