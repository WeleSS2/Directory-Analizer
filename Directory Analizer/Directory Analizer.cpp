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
	// Creating window and renderer
	auto mainWin = std::make_shared<Window_Init>("Directory Analizer");
	
	// Moving renderer to main rendering class
	rerender render(std::move(mainWin));
	
	// Starting application
	render.AppLoop();


	return 0;
}