MODULE:=gsystem

VERSION:=1.0.0

TARGET:=lib$(MODULE)

BUILD_PATH:=$(shell pwd)

PRE_DEFINED:=

INCLUDES:=

SRCDIRS:=

LIBS_PATH:= 

LIBS:= 

SLIBS:= 

OUT_HEADS:=$(wildcard $(BUILD_PATH)/inc/*.h)

include ../static.mk
