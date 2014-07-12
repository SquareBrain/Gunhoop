INCLUDES+= \
	$(GOHOOP_TOP)/gsystem \
	$(GOHOOP_TOP)/gsystem/inc \
	$(GOHOOP_TOP)/gcommon/ \
	$(GOHOOP_TOP)/gcommon/inc \
	$(GOHOOP_TOP)/gcommon/component/conf/inc \
	$(GOHOOP_TOP)/gcommon/component/filesys/inc \
	$(GOHOOP_TOP)/gcommon/component/function/inc \
	$(GOHOOP_TOP)/gcommon/component/iostream/inc \
	$(GOHOOP_TOP)/gcommon/component/network/inc

SLIBS+=

LIBS+= 

LIBS_PATH+=

PRE_DEFINED+=

#create dir
OUTPUT:=$(BUILD_PATH)/output
$(shell mkdir -p $(OUTPUT)/lib)
$(shell mkdir -p $(OUTPUT)/obj)

OBJDIR:=$(OUTPUT)/obj
VPATH:=$(SRCDIRS)

PS:=cpp
CC:=g++
CPPFLAGS+=-Wall -g -O0 -march=i686
CPPFLAGS+=-fPIC
CPPFLAGS+=$(addprefix -I, $(INCLUDES))
CPPFLAGS+=$(addprefix -D, $(PRE_DEFINED))

SOURCE:=$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.$(PS)))

CPPSRCS:=$(notdir $(SOURCE))
OBJS:=$(patsubst %.$(PS), $(OBJDIR)/%.o, $(CPPSRCS))

LIB_FLAGS:=$(addprefix -l, $(LIBS)) $(addprefix -L, $(LIBS_PATH))
SLIB_FLAGS:=-Wl,--whole-archive $(SLIBS) -Wl,--no-whole-archive

TARGET_FILE:=$(OUTPUT)/lib/$(TARGET).so
ifdef VERSION
	TARGET_FILE:=$(OUTPUT)/lib/$(TARGET).so.$(VERSION)
endif

$(TARGET):$(OBJS)
	$(CC) -shared -o $(TARGET_FILE) $(OBJS) $(SLIB_FLAGS) $(LIB_FLAGS)
ifdef VERSION
	@ln -sf $(TARGET_FILE) $(OUTPUT)/lib/$(TARGET).so
endif	
	@echo 'Build $(TARGET_FILE) Success'

$(OBJDIR)/%.o:%.$(PS)
	@echo $(CC) $<, `more $<|wc -l` lines
	@$(CC) -c $(CPPFLAGS) -o $@ $< 	

.PHONY:install clean cleanall 
.IGNORE:clean cleanall

install:
	@echo "Start Install $(TARGET_FILE)"
	@cp -f $(TARGET_FILE) /usr/lib
ifdef VERSION	
	@ln -sf /usr/lib/$(TARGET).so.$(VERSION) /usr/lib/$(TARGET).so
endif	
	@echo "Install $(TARGET_FILE) Complete"

clean:
	@rm $(OUTPUT)/obj -rf
ifneq ($(SOURCE),)	
	@touch -a $(SOURCE)
endif

cleanall:clean
	@rm $(OUTPUT) -rf
