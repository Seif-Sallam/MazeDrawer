all:
	@cd MazeDrawer && $(MAKE) -s all

clean:
	@cd MazeDrawer && $(MAKE) -s clean

run:
	@$(MAKE) -s -j 100 all
	@cd MazeDrawer && ./Build/MazeDrawer.out