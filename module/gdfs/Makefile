SUBDIRS:= \
	gdfsclient \
	gdfsmaster \
	gdfsserver

all:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR; done

clean:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR clean; done

cleanall:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR cleanall; done

install:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR install; done