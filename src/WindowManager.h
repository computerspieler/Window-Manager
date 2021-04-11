#pragma once

#include <X11/Xlib.h> 

int WindowManager_onXError(Display* display, XErrorEvent* error);

int WindowManager_whenAnotherWMIsPresent(Display* display, XErrorEvent* error);
int WindowManager_checkForOtherWM();

int WindowManager_init(char* display);
int WindowManager_run();
int WindowManager_exit();
