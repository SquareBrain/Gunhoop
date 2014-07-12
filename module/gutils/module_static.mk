PRONAME:=gutils
VERSION:=1.0.0
TARGET:=lib$(PRONAME)
PRE_DEFINED:= 
BUILD_PATH:=$(shell pwd)

INCLUDES:=

LIBS_PATH:= 

LIBS:= 

MODULE_PATH:=$(GOHOOP_TOP)/gutils
MODULE:= \
	gini \
	glogger \
	gxml

SLIBS:=$(foreach module, $(MODULE), $(MODULE_PATH)/$(module)/output/lib/lib$(module).a)

include ../static.mk
