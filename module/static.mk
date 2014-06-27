INCLUDES+= \
	$(GOHOOP_TOP)/gcommon/inc	\
	$(GOHOOP_TOP)/gcommon/component/conf/inc \
	$(GOHOOP_TOP)/gcommon/component/filesys/inc \
	$(GOHOOP_TOP)/gcommon/component/function/inc \
	$(GOHOOP_TOP)/gcommon/component/iostream/inc \
	$(GOHOOP_TOP)/gcommon/component/network/inc \
	$(GOHOOP_TOP)/gcommon/system/inc \
	$(GOHOOP_TOP)/gutils/logger/inc

SLIBS+= 

LIBS+= 

LIBS_PATH+= 

#create dir
OUTPUT:=$(BUILD_PATH)/output
$(shell mkdir -p $(OUTPUT)/lib)
$(shell mkdir -p $(OUTPUT)/obj)

OBJDIR:=$(OUTPUT)/obj
SRCDIR:=$(BUILD_PATH)/src

PS:=cpp
CC:=g++
CPPFLAGS:=-Wall -g -O0 -march=i686
CPPFLAGS+=$(addprefix -I, $(INCLUDES))
CPPFLAGS+=$(addprefix -D, $(PRE_DEFINED))

SOURCE:=$(wildcard $(BUILD_PATH)/src/*.$(PS))

CPPSRCS:=$(notdir $(SOURCE))
OBJS:=$(patsubst %.$(PS), $(OBJDIR)/%.o, $(CPPSRCS))

LIB_FLAGS:=$(addprefix -l, $(LIBS)) $(addprefix -L, $(LIBS_PATH))
SLIB_FLAGS:=$(SLIBS)
TARGET_FILE:=$(OUTPUT)/lib/$(TARGET).a

$(TARGET) : $(OBJS)
	ar rcs $(TARGET_FILE) $(OBJS) $(SLIB_FLAGS) $(LIB_FLAGS)
	@echo "++++++++++Build $(TARGET_FILE) Success++++++++++"

$(OBJDIR)/%.o:$(SRCDIR)/%.cpp
	@echo compile file $<, `more $<|wc -l` lines ....
	$(CC) -c $(CPPFLAGS) -o $@ $< 

.PHONY : all install clean cleanall 

install:
	@echo "start install $(TARGET_FILE) ..."
	@echo "install $(TARGET_FILE) complete ..."

clean :
	@rm $(OUTPUT)/obj -rf

cleanall :
	@rm $(OUTPUT) -rf
