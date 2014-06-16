all : tag_master tag_slave

clean:
	@(MAKE) -C Master clean
	@(MAKE) -C Slave clean

cleanall:
	@(MAKE) -C Master clean
	@(MAKE) -C Master clean

tag_master:
	@(MAKE) -C Master
	@(MAKE) -C Slaver
