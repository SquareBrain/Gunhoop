PRONAME:=gsystem

VERSION:=1.0.0

TARGET:=lib$(PRONAME)

BUILD_PATH:=$(shell pwd)

PRE_DEFINED:= 

INCLUDES:=$(BUILD_PATH)/inc \
	$(BUILD_PATH)/posix/inc

SRCDIRS:=$(BUILD_PATH)/posix/src

LIBS_PATH:= 

LIBS:= 

SLIBS:=

include ../shared.mk
