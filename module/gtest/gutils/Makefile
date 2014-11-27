SUBDIRS:=gxml gini

all:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR; done

clean:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR clean; done

cleanall:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR cleanall; done
