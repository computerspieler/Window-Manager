#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h> 

#include "WindowManager.h"

GC default_gc;
Display* display;
Window root_window;

int WindowManager_onXError(Display* display, XErrorEvent* error)
{
	char text[2048 + 1];

	XGetErrorText(display, error->error_code, text, 2048);
	printf("[ERROR] %s\n", text);
	return 0;
}

int WindowManager_init(char* display_name)
{
	display = XOpenDisplay(display_name);
	if(!display)
		return -1;

	XSetErrorHandler(WindowManager_onXError);
	root_window = DefaultRootWindow(display);

	XWindowAttributes attributes;
	XGetWindowAttributes(display, root_window, &attributes);

	XGCValues default_gc_values;
	default_gc_values.foreground = 0xffffff;
	default_gc_values.background = 0x000000;

	default_gc = XCreateGC(display, root_window, GCForeground | GCBackground, &default_gc_values);

	Pixmap background = XCreatePixmap(display, root_window, attributes.width, attributes.height, attributes.depth);
	for(int y = 0; y < attributes.height; y++)
		for(int x = 0; x < attributes.width; x++)
			if((x + y) % 2 == 0)
				XDrawPoint(display, background, default_gc, x, y);

	XSetWindowBackgroundPixmap(display, root_window, background);	
	XClearWindow(display, root_window);

	XFreePixmap(display, background);

	return 0;
}

int WindowManager_run()
{
/*	while(1)
	{
	
	}
*/
	return 0;
}

int WindowManager_exit()
{
	XFreeGC(display, default_gc);
	XDestroyWindow(display, root_window);
	XCloseDisplay(display);
	return 0;
}

