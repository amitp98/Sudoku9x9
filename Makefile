project: gui.c gui.h solver.c generate.c
	gcc gui.c `pkg-config --libs --cflags gtk+-3.0` -rdynamic -o project
