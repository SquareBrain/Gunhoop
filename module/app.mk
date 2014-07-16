INCLUDES+= \
	$(GOHOOP_INC_PATH)/gsystem \
	$(GOHOOP_INC_PATH)/gcommon \
	$(GOHOOP_INC_PATH)/gutils

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

all:$(TARGET) install

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
	@mkdir -p $(GOHOOP_BIN_PATH)/$(MODULE)
	@cp -ax $(OUTPUT)/bin/$(TARGET) $(GOHOOP_BIN_PATH)/$(MODULE)
	@echo "Install $(TARGET_FILE) $(GOHOOP_BIN_PATH)/$(TARGET) Complete"
endif

clean:
	@rm $(OUTPUT)/obj -rf
	@touch -a $(SOURCE)

cleanall:clean
	@rm $(OUTPUT) -rf
