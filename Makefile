all:
	@cd MazeDrawer && $(MAKE) -s all

clean:
	@cd MazeDrawer && $(MAKE) -s clean

run:
	@cd MazeDrawer && ./Build/MazeDrawer.out