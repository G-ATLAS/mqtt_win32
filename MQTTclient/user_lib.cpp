
#include <windows.h>
#include <iostream>


#include "user_lib.h"










//debug
void user_debug(const char* format, ...)
{
	char outstr[512];
	int len = -1;
	va_list args;

	va_start(args, format);
	len = vsprintf_s(outstr, 512, format, args);
	va_end(args);

	if (len > 0)WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), outstr, len, nullptr, 0);
}
void user_debug_init(void)
{
#ifdef XDEBUG_ENABLE
	AllocConsole();
	//tips console online
	user_debug("---Console online---\n");
#endif 
}