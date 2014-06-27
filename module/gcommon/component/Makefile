all: tagconf tagfilesys tagfunction tagiostream tagnetwork

clean:
	$(MAKE) -C conf clean
	$(MAKE) -C filesys clean
	$(MAKE) -C function clean
	$(MAKE) -C iostream clean
	$(MAKE) -C network clean

cleanall:
	$(MAKE) -C conf cleanall
	$(MAKE) -C filesys cleanall
	$(MAKE) -C function cleanall
	$(MAKE) -C iostream cleanall
	$(MAKE) -C network cleanall

tagconf:
	$(MAKE) -C conf

tagfilesys:
	$(MAKE) -C filesys	

tagfunction:
	$(MAKE) -C function

tagiostream:
	$(MAKE) -C iostream

tagnetwork:
	$(MAKE) -C network