PRONAME:=gsystem
VERSION:=1.0.0
TARGET:=lib$(PRONAME)
PRE_DEFINED:= 
BUILD_PATH:=$(shell pwd)

INCLUDES:=

LIBS_PATH:= 

LIBS:= 

MODULE_PATH:=$(GOHOOP_TOP)/gsystem
SLIBS:= \
	$(MODULE_PATH)/posix/output/lib/libposix.a

include ../shared.mk
