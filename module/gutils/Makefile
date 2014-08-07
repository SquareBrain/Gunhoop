SUBDIRS:= \
	gini \
	glogger \
	#gxml \

all:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR; done
	$(MAKE) -f module_shared.mk
	$(MAKE) -f module_static.mk

clean:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR clean; done
	$(MAKE) -f module_shared.mk clean
	$(MAKE) -f module_static.mk clean

cleanall:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR cleanall; done
	$(MAKE) -f module_shared.mk cleanall
	$(MAKE) -f module_static.mk cleanall

install:
	$(MAKE) -f module_shared.mk install
	$(MAKE) -f module_static.mk install	
