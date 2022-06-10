all:
	@cd MazeDrawer && $(MAKE) -s all

clean:
	@cd MazeDrawer && $(MAKE) -s clean

cleanWin:
	@$(RM) MazeDrawer.sln
	@cd MazeDrawer && $(RM) MazeDrawer.vcxproj MazeDrawer.vcxproj.filters

run:
	@$(MAKE) -s -j 100 all
	@cd MazeDrawer && ./Build/MazeDrawer.out
