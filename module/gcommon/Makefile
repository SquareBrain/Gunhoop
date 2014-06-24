all: tagcomponent tagsystem taglibmk

clean:
	$(MAKE) -C component clean
	$(MAKE) -C system clean
	$(MAKE) -C libmk clean

cleanall:
	$(MAKE) -C component cleanall
	$(MAKE) -C system cleanall
	$(MAKE) -C libmk cleanall
	
tagcomponent:
	$(MAKE) -C component 

tagsystem:    
	$(MAKE) -C system

taglibmk:	  
	$(MAKE) -C libmk
