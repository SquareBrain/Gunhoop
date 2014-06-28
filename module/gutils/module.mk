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
	$(MODULE_PATH)/ini/output/lib/libini.a \
	$(MODULE_PATH)/logger/output/lib/liblogger.a \
	$(MODULE_PATH)/xml/output/lib/libxml.a \

include ../shared.mk
