workspace " MazeDrawer"  
    configurations { "Debug", "Release" } 

    architecture "x86_64"

    startproject "MazeDrawer"
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "MazeDrawer"
