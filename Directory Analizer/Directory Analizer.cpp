// Directory Analizer.cpp : Defines the entry point for the application.
//
#pragma once
#include "Directory Analizer.h"
#include "src/algorithms.cpp"
#include "src/window_mng.cpp"
#include "src/graphics_engine.cpp"
#include "src/graphics_engine_rerender.cpp"

int main(int argc, char* argv[])
{
	auto mainWin = std::make_shared<Window_Init>("Directory Analizer");
	rerender render(std::move(mainWin));
	render.AppLoop();
	/*threads.push_task([] {
		auto mainWin = std::make_shared<Window_Init>("Directory Analizer");
		rerender render(std::move(mainWin));
		render.AppLoop();
		}
	);*/
	return 0;
}