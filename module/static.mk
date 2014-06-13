INCLUDES+=$(GOHOOP_ROOT) 	

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

$(TARGET) : $(OBJS)
	ar rcs $(OUTPUT)/lib/$(TARGET).a $(OBJS)
	@echo "++++++++++Build $(TARGET).a Success++++++++++"

$(OBJDIR)/%.o:$(SRCDIR)/%.cpp
	@echo compile file $<, `more $<|wc -l` lines ....
	$(CC) -c $(CPPFLAGS) -o $@ $< 

.PHONY : all install clean cleanall 

install:
	@echo "start install $(TARGET).a ..."
	@echo "install $(TARGET).a complete ..."

clean :
	@rm $(OUTPUT)/obj -rf

cleanall :
	@rm $(OUTPUT) -rf
