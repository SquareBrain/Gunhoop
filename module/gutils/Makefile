all: dini dxml target

clean:
	$(MAKE) -C ini clean
	$(MAKE) -C xml clean
	$(MAKE) -C util clean

cleanall:
	$(MAKE) -C ini cleanall
	$(MAKE) -C xml cleanall
	$(MAKE) -C util cleanall
	rm ./lib -rf

dini:    
	$(MAKE) -C ini 

dxml:    
	$(MAKE) -C xml 

target:    
	$(MAKE) -C util 
