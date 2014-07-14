INCLUDES+= \
	$(GOHOOP_TOP)/gsystem \
	$(GOHOOP_TOP)/gsystem/inc \
	$(GOHOOP_TOP)/gcommon/ \
	$(GOHOOP_TOP)/gcommon/inc \
	$(GOHOOP_TOP)/gcommon/conf/inc \
	$(GOHOOP_TOP)/gcommon/filesys/inc \
	$(GOHOOP_TOP)/gcommon/function/inc \
	$(GOHOOP_TOP)/gcommon/iostream/inc \
	$(GOHOOP_TOP)/gcommon/network/inc \
	$(GOHOOP_TOP)/gutils \
	$(GOHOOP_TOP)/gutils/gini/inc \
	$(GOHOOP_TOP)/gutils/glogger/inc \
	$(GOHOOP_TOP)/gutils/gxml/inc

SLIBS+= 

LIBS+= 

LIBS_PATH+= 

PRE_DEFINED+=

#create dir
OUTPUT:=$(BUILD_PATH)/output
$(shell mkdir -p $(OUTPUT)/bin)
$(shell mkdir -p $(OUTPUT)/obj)

OBJDIR:=$(BUILD_PATH)/output/obj
VPATH:=$(SRCDIRS)

PS:=cpp
CC:=g++
CPPFLAGS+=-Wall -g -O0 -march=i686 -std=gnu++0x
CPPFLAGS+=$(addprefix -I, $(INCLUDES))
CPPFLAGS+=$(addprefix -D, $(PRE_DEFINED))

SOURCE:=$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.$(PS)))

CPPSRCS:=$(notdir $(SOURCE))
OBJS:=$(patsubst %.$(PS), $(OBJDIR)/%.o, $(CPPSRCS))

LIB_FLAGS:=$(addprefix -l, $(LIBS)) $(addprefix -L, $(LIBS_PATH))
SLIB_FLAGS:=-Wl,--whole-archive $(SLIBS) -Wl,--no-whole-archive

TARGET_FILE:=$(OUTPUT)/bin/$(TARGET)

ifdef VERSION
	TARGET_FILE:=$(OUTPUT)/bin/$(TARGET).$(VERSION)
endif

$(TARGET):$(OBJS)
	@$(CC) $(CPPFLAGS) $(OBJS) -o $(TARGET_FILE) $(SLIB_FLAGS) $(LIB_FLAGS)
	@echo "Build $(TARGET_FILE) Success"

$(OBJDIR)/%.o:%.$(PS)
	@echo $(CC) $<, `more $<|wc -l` lines
	@$(CC) -c $(CPPFLAGS) -o $@ $< 

.PHONY:install clean cleanall
.IGNORE:clean cleanall

install:
	@echo "Start Install $(TARGET_FILE)"
	@echo "Install $(TARGET_FILE) Complete"

clean:
	@rm $(OUTPUT)/obj -rf
	@touch -a $(SOURCE)

cleanall:clean
	@rm $(OUTPUT) -rf
