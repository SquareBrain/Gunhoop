all: tagini taglogger tagxml taglibmk

clean:
	$(MAKE) -C ini clean
	$(MAKE) -C logger clean
	$(MAKE) -C xml clean
	$(MAKE) -f module.mk clean

cleanall:
	$(MAKE) -C ini cleanall
	$(MAKE) -C logger cleanall
	$(MAKE) -C xml cleanall
	$(MAKE) -f module.mk cleanall

tagini:    
	$(MAKE) -C ini 

taglogger:	  
	$(MAKE) -C logger	

tagxml:    
	$(MAKE) -C xml 

taglibmk:    
	$(MAKE) -f module.mk 
