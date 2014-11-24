#third party library include
INCLUDES+=

#third party static library
SLIBS+= 

#third party shared library
LIBS+=pthread dl

#third party shared library path
LIBS_PATH+=$(GOHOOP_LIB_PATH) 

#predefined
PRE_DEFINED+=

#create dir
OUTPUT:=$(BUILD_PATH)/output
$(shell mkdir -p $(OUTPUT)/bin)
$(shell mkdir -p $(OUTPUT)/obj)

OBJDIR:=$(BUILD_PATH)/output/obj
VPATH:=$(SRCDIRS)

PS:=cpp
CC:=g++
CPPFLAGS+=-Wall -g -O0 -std=c++11
CPPFLAGS+=$(addprefix -I, $(INCLUDES))
CPPFLAGS+=$(addprefix -D, $(PRE_DEFINED))

SOURCE:=$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.$(PS)))

CPPSRCS:=$(notdir $(SOURCE))
OBJS:=$(patsubst %.$(PS), $(OBJDIR)/%.o, $(CPPSRCS))

#shared library flags
LIB_FLAGS:=$(addprefix -l, $(LIBS)) $(addprefix -L, $(LIBS_PATH))

#static library flags
SLIB_FLAGS:=-Wl,--whole-archive $(SLIBS) -Wl,--no-whole-archive

TARGET_FILE:=$(OUTPUT)/bin/$(TARGET)

ifdef VERSION
	TARGET_FILE:=$(OUTPUT)/bin/$(TARGET).$(VERSION)
endif

all:$(TARGET)

$(TARGET):$(OBJS)
	@$(CC) $(CPPFLAGS) $(OBJS) -o $(TARGET_FILE) $(SLIB_FLAGS) $(LIB_FLAGS)
	@echo "Build $(TARGET_FILE) Success"

$(OBJDIR)/%.o:%.$(PS)
	@echo $(CC) $<, `more $<|wc -l` lines
	@$(CC) -c $(CPPFLAGS) -o $@ $< 

.PHONY:install clean cleanall
.IGNORE:clean cleanall

install:
ifdef VERSION
	@echo "Start Install $(TARGET_FILE)"
	@mkdir -p $(GOHOOP_BIN_PATH)    
	@cp -ax $(OUTPUT)/bin/$(TARGET) $(GOHOOP_BIN_PATH)
	@echo "Install $(TARGET_FILE) $(GOHOOP_BIN_PATH)/$(TARGET) Complete"
endif

clean:
	@rm $(OUTPUT)/obj -rf
ifneq ($(SOURCE),)	
	@touch -a $(SOURCE)
endif

cleanall:clean
	@rm $(OUTPUT) -rf
