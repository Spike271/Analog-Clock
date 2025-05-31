#include "../headers/Application.hpp"

#ifdef _MSC_VER
	#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif

int main()
{
	Clock::Application application;
	application.Run();
}
