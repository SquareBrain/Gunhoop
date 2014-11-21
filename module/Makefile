SUBDIRS:= \
	gsystem \
	gutils \
	glogger \
	gcommon \
    gtest
	
all:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR; done
	
clean:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR clean; done
	@rm -rf $(GOHOOP_BIN_PATH)/*
	@rm -rf $(GOHOOP_LIB_PATH)/*
	@rm -rf $(GOHOOP_INC_PATH)/*

cleanall:
	@for SUBDIR in $(SUBDIRS); do $(MAKE) -C $$SUBDIR cleanall; done
	@rm -rf $(GOHOOP_BIN_PATH)/*
	@rm -rf $(GOHOOP_LIB_PATH)/*
	@rm -rf $(GOHOOP_INC_PATH)/*
	@rm -rf $(GOHOOP_TOP)/output
