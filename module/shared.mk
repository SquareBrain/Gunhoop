INCLUDES+= \
	$(GOHOOP_INC_PATH)/gsystem \
	$(GOHOOP_INC_PATH)/gcommon/conf \
	$(GOHOOP_INC_PATH)/gcommon/filesys \
	$(GOHOOP_INC_PATH)/gcommon/function \
	$(GOHOOP_INC_PATH)/gcommon/iostream \
	$(GOHOOP_INC_PATH)/gcommon/network \
	$(GOHOOP_INC_PATH)/gutils/gini \
	$(GOHOOP_INC_PATH)/gutils/glogger \
	$(GOHOOP_INC_PATH)/gutils/gxml

SLIBS+=

LIBS+= 

LIBS_PATH+=

PRE_DEFINED+=

OUT_HEADS+=

#create dir
OUTPUT:=$(BUILD_PATH)/output
$(shell mkdir -p $(OUTPUT)/lib)
$(shell mkdir -p $(OUTPUT)/obj)

OBJDIR:=$(OUTPUT)/obj
VPATH:=$(SRCDIRS)

PS:=cpp
CC:=g++
CPPFLAGS+=-Wall -g -O0 -march=i686 -std=gnu++0x
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
	@$(CC) -shared -o $(TARGET_FILE) $(OBJS) $(SLIB_FLAGS) $(LIB_FLAGS)
ifdef VERSION
	@cd $(OUTPUT)/lib && ln -snf $(TARGET).so.$(VERSION) $(TARGET).so
endif	
	@echo 'Build $(TARGET_FILE) Success'

$(OBJDIR)/%.o:%.$(PS)
	@echo $(CC) $<, `more $<|wc -l` lines
	@$(CC) -c $(CPPFLAGS) -o $@ $< 	

.PHONY:install clean cleanall 
.IGNORE:clean cleanall

install:
	@echo "Start Install $(TARGET_FILE)"
	@cp -ax $(OUTPUT)/lib/$(TARGET).so* $(GOHOOP_LIB_PATH)
	@mkdir -p $(GOHOOP_INC_PATH)/$(MODULE)
	@cp -ax $(OUT_HEADS) $(GOHOOP_INC_PATH)/$(MODULE)
	@echo "Install $(TARGET_FILE) $(GOHOOP_LIB_PATH) Complete"

clean:
	@rm $(OUTPUT)/obj -rf
ifneq ($(SOURCE),)	
	@touch -a $(SOURCE)
endif

cleanall:clean
	@rm $(OUTPUT) -rf
