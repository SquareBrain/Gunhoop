PS=cpp
PRONAME:=dutil
VERSION:=1.0.0
TARGET:=lib$(PRONAME)
PRE_DEFINED:= 
BUILD_PATH:=$(shell pwd)

INCLUDES:=\
        ../ini/inc \
		../xml/inc

SLIBS:=dini dxml

SLIBS:=$(addprefix ../lib/lib, $(SLIBS))
SLIBS:=$(addsuffix .a, $(SLIBS))

LIBS:=

LIBS_PATH:=

include ../../shared.mk
