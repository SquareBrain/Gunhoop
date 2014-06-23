all: tagbase tagcommon tagdata tagdfs tagmapreduce tagutils

clean:
	$(MAKE) -C gbase clean
	$(MAKE) -C gcommon clean
	$(MAKE) -C gdata clean
	$(MAKE) -C gdfs clean
	$(MAKE) -C gmapreduce clean
	$(MAKE) -C gutils clean

cleanall:
	$(MAKE) -C gbase cleanall
	$(MAKE) -C gcommon cleanall
	$(MAKE) -C gdata cleanall
	$(MAKE) -C gdfs cleanall
	$(MAKE) -C gmapreduce cleanall
	$(MAKE) -C gutils cleanall

tagbase:
	$(MAKE) -C gbase 

tagcommon:
	$(MAKE) -C gcommon

tagdata:    
	$(MAKE) -C gdata    

tagdfs:    
	$(MAKE) -C gdfs

tagmapreduce:    
	$(MAKE) -C gmapreduce

tagutils:    
	$(MAKE) -C gutils
