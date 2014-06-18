all: dnetwork dcomponent dlogger dposix dutil ut_app

clean:
	$(MAKE) -C network clean
	$(MAKE) -C component clean
	$(MAKE) -C logger clean
	$(MAKE) -C posix clean
	$(MAKE) -C util clean
	$(MAKE) -C ut clean

cleanall:
	$(MAKE) -C network cleanall
	$(MAKE) -C component cleanall
	$(MAKE) -C logger cleanall
	$(MAKE) -C posix cleanall
	$(MAKE) -C util cleanall
	$(MAKE) -C ut cleanall
	rm $(DUYE_LIB) -rf
	rm $(DUYE_BIN) -rf

dnetwork:
	$(MAKE) -C network 

dcomponent:
	$(MAKE) -C component

dlogger:    
	$(MAKE) -C logger    

dposix:    
	$(MAKE) -C posix

dutil:    
	$(MAKE) -C util

ut_app:
	$(MAKE) -C ut
