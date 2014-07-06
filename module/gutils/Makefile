SUBDIRS:= \
	ini \
	logger \
	xml \

all:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR; done
	$(MAKE) -f module_shared.mk
	$(MAKE) -f module_static.mk
#$(MAKE) -C utest

clean:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR clean; done
	$(MAKE) -f module_shared.mk clean
	$(MAKE) -f module_static.mk clean
#$(MAKE) -C utest clean

cleanall:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR cleanall; done
	$(MAKE) -f module_shared.mk cleanall
	$(MAKE) -f module_static.mk cleanall
#$(MAKE) -C utest cleanall
