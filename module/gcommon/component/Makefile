all: tagfunction tagnetwork

clean:
	$(MAKE) -C function clean
	$(MAKE) -C network clean

cleanall:
	$(MAKE) -C function cleanall
	$(MAKE) -C network cleanall

tagfunction:
	$(MAKE) -C component

tagnetwork:
	$(MAKE) -C logger
