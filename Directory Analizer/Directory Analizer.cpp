// Directory Analizer.cpp : Defines the entry point for the application.
//
#include "Directory Analizer.h"

using namespace std;

int main(int argc, char* argv[])
{
	auto mainWin = std::make_shared<Window_Init>("Directory Analizer");
	
	rerender render(std::move(mainWin));
	render.AppLoop();

	return 0;
}
