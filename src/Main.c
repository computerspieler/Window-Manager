#include "WindowManager.h"

int main(int argc, char* argv[])
{
	/* This is hard coded for now, but in the near future, il will be automatically detected */
	if(WindowManager_init(":100"))
		return -1;
	WindowManager_run();
	return WindowManager_exit();
}
