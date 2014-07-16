SUBDIRS:= \
	gsystem \
	gutils \
	gcommon
	
all:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR; done
	
clean:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR clean; done
	@rm ./output/bin/* -rf
	@rm ./output/include/* -rf
	@rm ./output/lib/* -rf

cleanall:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR cleanall; done
	@rm ./output -rf