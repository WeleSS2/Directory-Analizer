// Directory Analizer.cpp : Defines the entry point for the application.
//
#include "Directory Analizer.h"
#include "src/algorithms.h"
#include <thread>

using namespace std;

void start()
{
	auto mainWin = std::make_shared<Window_Init>("Directory Analizer");
	rerender render(std::move(mainWin));
	render.AppLoop();
}

int main(int argc, char* argv[])
{
	{
		Thread_Pool threads(std::thread::hardware_concurrency());
		threads.push_task(start);
	}
	return 0;
}