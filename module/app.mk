INCLUDES+=$(GOHOOP_ROOT) 	

SLIBS+= 

LIBS+= 

LIBS_PATH+= 

#create dir
OUTPUT:=$(BUILD_PATH)/output
$(shell mkdir -p $(OUTPUT)/bin)
$(shell mkdir -p $(OUTPUT)/obj)

OBJDIR:=$(BUILD_PATH)/output/obj
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
	$(CC) $(CPPFLAGS) $(OBJS) -o $(OUTPUT)/bin/$(TARGET).$(VERSION) $(SLIBS) $(addprefix -l, $(LIBS)) $(addprefix -L, $(LIBS_PATH))  
	@echo "++++++++++Build $(TARGET).$(VERSION) Success++++++++++"

$(OBJDIR)/%.o:$(SRCDIR)/%.cpp
	@echo $<, `more $<|wc -l` lines
	$(CC) -c $(CPPFLAGS) -o $@ $< 

.PHONY : all install clean 

install :
	@echo "start install $(TARGET).$(VERSION) ..."
	@echo 'install $(TARGET).$(VERSION) complete ...'

clean :
	$rm $(OUTPUT)/obj -rf

cleanall :
	$rm $(OUTPUT) -rf
