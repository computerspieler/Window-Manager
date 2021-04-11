#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h> 

#include "WindowManager.h"

GC default_gc;
Display* display;
Window root_window;

int other_wm_detected;

int WindowManager_onXError(Display* display, XErrorEvent* error)
{
	char text[2048 + 1];

	XGetErrorText(display, error->error_code, text, 2048);
	printf("[ERROR] %s\n", text);
	return 0;
}

int WindowManager_whenAnotherWMIsPresent(Display* display, XErrorEvent* error)
{
	other_wm_detected = 1;
}

int WindowManager_checkForOtherWM()
{
	other_wm_detected = 0;
	XSelectInput(display, root_window, SubstructureRedirectMask);
	XSetErrorHandler(WindowManager_whenAnotherWMIsPresent);
	XSync(display, 0);
}

int WindowManager_init(char* display_name)
{
	int x;
	int y;
	Pixmap background;
	XGCValues default_gc_values;
	XWindowAttributes attributes;

	display = XOpenDisplay(display_name);
	if(!display)
	{
		printf("[ERROR] Can't open the display\n");
		return -1;
	}

	root_window = DefaultRootWindow(display);

	WindowManager_checkForOtherWM();
	if(other_wm_detected)
	{
		printf("[ERROR] Can't launch next to an another window manager\n");
		XCloseDisplay(display);
		return -1;
	}

	XSetErrorHandler(WindowManager_onXError);
	XGetWindowAttributes(display, root_window, &attributes);

	default_gc_values.foreground = 0xffffff;
	default_gc_values.background = 0x000000;

	default_gc = XCreateGC(display, root_window, GCForeground | GCBackground, &default_gc_values);
	
	background = XCreatePixmap(display, root_window, 2, 2, attributes.depth);
	XDrawPoint(display, background, default_gc, 0, 0);
	XDrawPoint(display, background, default_gc, 1, 1);

	XSetWindowBackgroundPixmap(display, root_window, background);	
	XClearWindow(display, root_window);

	XFreePixmap(display, background);
	XSync(display, 0);

	return 0;
}

int WindowManager_run()
{
	XEvent event;

	while(1)
	{
		XNextEvent(display, &event);
		switch(event.type)
		{
		case MapRequest:
			XMapWindow(display, event.xmaprequest.window);
			break;
		default:
			printf("Unknown event no %i\n", event.type);
		}
	}

	return 0;
}

int WindowManager_exit()
{
	XFreeGC(display, default_gc);
	XDestroyWindow(display, root_window);
	XCloseDisplay(display);
	return 0;
}

