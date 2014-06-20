all: tagcomponent taglogger tagnetwork tagsystem

clean:
	$(MAKE) -C component clean
	$(MAKE) -C logger clean
	$(MAKE) -C network clean
	$(MAKE) -C system clean

cleanall:
	$(MAKE) -C component cleanall
	$(MAKE) -C logger cleanall
	$(MAKE) -C network cleanall
	$(MAKE) -C system cleanall

tagcomponent:
	$(MAKE) -C component

taglogger:
	$(MAKE) -C logger 

tagnetwork:    
	$(MAKE) -C network    

tagsystem:    
	$(MAKE) -C system
