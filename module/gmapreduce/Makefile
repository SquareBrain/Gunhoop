all: tagmaster tagslave 

clean:
	$(MAKE) -C gmaster clean
	$(MAKE) -C gslave clean

cleanall:
	$(MAKE) -C gmaster cleanall
	$(MAKE) -C gslave cleanall

tagmaster:
	$(MAKE) -C gmaster

tagslave:
	$(MAKE) -C gslave
