SUBDIRS:= \
	gsystem \
	gutils \
	gcommon \
	gbase \
	gdata \
	gdfs \
	gmapreduce
	
all:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR; done
	
clean:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR clean; done

cleanall:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR cleanall; done
