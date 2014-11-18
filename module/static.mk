INCLUDES+=
	
#LIBS+=

#LIBS_PATH+=

SLIBS+=

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
CPPFLAGS+=-Wall -g -O0 -std=c++11
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

all:$(TARGET) install

$(TARGET):$(OBJS)
	@ar rcs $(TARGET_FILE) $(OBJS) $(SLIB_FLAGS) $(LIB_FLAGS)
ifdef VERSION
	@cd $(OUTPUT)/lib && ln -snf $(TARGET).a.$(VERSION) $(TARGET).a
endif	
	@echo "Build $(TARGET_FILE) Success"

$(OBJDIR)/%.o:%.$(PS)
	@echo $(CC) $<, `more $<|wc -l` lines
	@$(CC) -c $(CPPFLAGS) -o $@ $< 
	
.PHONY:install clean cleanall 
.IGNORE:clean cleanall

install:
ifdef VERSION
	@echo "Start Install $(TARGET_FILE)"
	@cp -ax $(OUTPUT)/lib/$(TARGET).a* $(GOHOOP_LIB_PATH)
	@mkdir -p $(GOHOOP_INC_PATH)/$(MODULE)
	@cp -ax $(OUT_HEADS) $(GOHOOP_INC_PATH)/$(MODULE)
	@echo "Install $(TARGET_FILE) $(GOHOOP_LIB_PATH) Complete"
endif

clean:
	@rm $(OUTPUT)/obj -rf
ifneq ($(SOURCE),)
	@touch -a $(SOURCE)
endif

cleanall:clean
	@rm $(OUTPUT) -rf
