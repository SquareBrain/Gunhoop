SUBDIRS:= \
	gsystem \
	gutils \
	glogger \
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
	@rm -rf $(GOHOOP_BIN_PATH)/*
	@rm -rf $(GOHOOP_LIB_PATH)/*
	@rm -rf $(GOHOOP_INC_PATH)/*