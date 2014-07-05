INCLUDES+= \
	$(GOHOOP_TOP)/gcommon/inc \
	$(GOHOOP_TOP)/gcommon/component/conf/inc \
	$(GOHOOP_TOP)/gcommon/component/filesys/inc \
	$(GOHOOP_TOP)/gcommon/component/function/inc \
	$(GOHOOP_TOP)/gcommon/component/iostream/inc \
	$(GOHOOP_TOP)/gcommon/component/network/inc \
	$(GOHOOP_TOP)/gcommon/system/inc \

SLIBS+= 

LIBS+= 

LIBS_PATH+= 

#create dir
OUTPUT:=$(BUILD_PATH)/output
$(shell mkdir -p $(OUTPUT)/lib)
$(shell mkdir -p $(OUTPUT)/obj)

OBJDIR:=$(OUTPUT)/obj
VPATH:=$(SRCDIRS)

PS:=cpp
CC:=g++
CPPFLAGS:=-Wall -g -O0 -march=i686
CPPFLAGS+=$(addprefix -I, $(INCLUDES))
CPPFLAGS+=$(addprefix -D, $(PRE_DEFINED))

SOURCE:=$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.$(PS)))

CPPSRCS:=$(notdir $(SOURCE))
OBJS:=$(patsubst %.$(PS), $(OBJDIR)/%.o, $(CPPSRCS))

LIB_FLAGS:=$(addprefix -l, $(LIBS)) $(addprefix -L, $(LIBS_PATH))
SLIB_FLAGS:=$(SLIBS)

TARGET_FILE:=$(OUTPUT)/lib/$(TARGET).a

ifdef VERSION
	TARGET_FILE:=$(OUTPUT)/lib/$(TARGET).a.$(VERSION)
endif

$(TARGET):$(OBJS) 
	@ar rcs $(TARGET_FILE) $(OBJS) $(SLIB_FLAGS) $(LIB_FLAGS)
ifdef VERSION
	@ln -s $(TARGET_FILE) $(OUTPUT)/lib/$(TARGET).a
endif	
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
ifneq ($(SOURCE),)
	touch -a $(SOURCE)
endif

cleanall:clean
	@rm $(OUTPUT) -rf
