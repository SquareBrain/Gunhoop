#third party library include
INCLUDES+=

#third party static library
SLIBS+=

#third party shared library
LIBS+=

#third party shared library path
LIBS_PATH+=$(GOHOOP_LIB_PATH)

#predefined
PRE_DEFINED+=

#output head files
OUT_HEADS+=

#create dir
OUTPUT:=$(BUILD_PATH)/output
$(shell mkdir -p $(OUTPUT)/lib)
$(shell mkdir -p $(OUTPUT)/obj)

OBJDIR:=$(OUTPUT)/obj
VPATH:=$(SRCDIRS)

PS:=cpp
CC:=g++
CPPFLAGS+=-Wall -ggdb -O0 -std=c++11
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

all:$(TARGET) install

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
ifdef VERSION
	@echo "Start Install $(TARGET_FILE)"
	@mkdir -p $(GOHOOP_INC_PATH)/$(MODULE)  
	@mkdir -p $(GOHOOP_LIB_PATH)     
	@cp -ax $(OUTPUT)/lib/$(TARGET).so* $(GOHOOP_LIB_PATH)
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
