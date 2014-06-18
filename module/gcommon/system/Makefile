all: dipc dmemory dstdio dsocket dsystem dthread dposix 

clean:
	$(MAKE) -C ipc clean
	$(MAKE) -C memory clean
	$(MAKE) -C stdio clean
	$(MAKE) -C socket clean
	$(MAKE) -C system clean
	$(MAKE) -C thread clean
	$(MAKE) -C posix clean

cleanall:
	$(MAKE) -C ipc cleanall
	$(MAKE) -C memory cleanall
	$(MAKE) -C stdio cleanall
	$(MAKE) -C socket cleanall
	$(MAKE) -C system cleanall
	$(MAKE) -C thread cleanall
	$(MAKE) -C posix cleanall
	rm ./lib -rf

dipc:    
	$(MAKE) -C ipc

dmemory:    
	$(MAKE) -C memory 

dstdio:    
	$(MAKE) -C stdio

dsocket:    
	$(MAKE) -C socket 

dsystem:    
	$(MAKE) -C system 

dthread:    
	$(MAKE) -C thread

dposix:    
	$(MAKE) -C posix 
