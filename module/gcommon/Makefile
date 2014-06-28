all:tagcomponent tagsystem taglibmk

clean:
	$(MAKE) -C component clean
	$(MAKE) -C system clean
	$(MAKE) -f module.mk clean

cleanall:
	$(MAKE) -C component cleanall
	$(MAKE) -C system cleanall
	$(MAKE) -f module.mk cleanall
	
tagcomponent:
	$(MAKE) -C component 

tagsystem:    
	$(MAKE) -C system

taglibmk:    
	$(MAKE) -f module.mk
