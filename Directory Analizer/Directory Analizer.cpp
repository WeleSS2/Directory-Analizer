// Directory Analizer.cpp : Defines the entry point for the application.
//
#include "Directory Analizer.h"

using namespace std;

int main(int argc, char* argv[])
{
	Window_Init mainWin("Directory Analizer");

	rerender render(&mainWin);
	render.AppLoop();

	return 0;
}
