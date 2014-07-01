PRONAME:=gutils
VERSION:=1.0.0
TARGET:=lib$(PRONAME)
PRE_DEFINED:= 
BUILD_PATH:=$(shell pwd)

INCLUDES:=

LIBS_PATH:= 

LIBS:= 

MODULE_PATH:=$(GOHOOP_TOP)/gutils
SLIBS:= \
	$(MODULE_PATH)/ini/output/lib/libcomponent.a \
	$(MODULE_PATH)/logger/output/lib/libsystem.a \
	$(MODULE_PATH)/xml/output/lib/libsystem.a

include ../static.mk
