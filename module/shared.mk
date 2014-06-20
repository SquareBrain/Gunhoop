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
CPPFLAGS+=-fPIC
CPPFLAGS+=$(addprefix -I, $(INCLUDES))
CPPFLAGS+=$(addprefix -D, $(PRE_DEFINED))

SOURCE:=$(wildcard $(BUILD_PATH)/src/*.$(PS))

CPPSRCS:=$(notdir $(SOURCE))
OBJS:=$(patsubst %.$(PS), $(OBJDIR)/%.o, $(CPPSRCS))

$(TARGET) : $(OBJS)
	$(CC) -shared -o $(OUTPUT)/lib/$(TARGET).so.$(VERSION) $(OBJS) -Wl,--whole-archive $(SLIBS) -Wl,--no-whole-archive $(addprefix -l, $(LIBS)) $(addprefix -L, $(LIBS_PATH))
	@echo '++++++++++Build $(TARGET).$(VERSION).so Success++++++++++'

$(OBJDIR)/%.o:$(SRCDIR)/%.cpp
	@echo $<, `more $<|wc -l` lines
	$(CC) -c $(CPPFLAGS) -o $@ $< 

.PHONY : all install clean cleanall 

install :
	@echo 'start install $(TARGET).so.$(VERSION) ...'
	@echo 'install $(TARGET).so.$(VERSION) complete ...'

clean :
	@rm $(OUTPUT)/obj -rf

cleanall :
	@rm $(OUTPUT)/output -rf
