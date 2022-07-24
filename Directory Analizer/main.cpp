// Directory Analizer.cpp : Defines the entry point for the application.
//
#pragma once
#include "main.h"

#ifdef GTESTS__ENABLED
	#include "src/GTests.cpp"
#endif


int main(int argc, char* argv[])
{
#ifdef GTESTS__ENABLED
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else

	// Creating window and renderer
	auto mainWindow = std::make_shared<Window_Init>("Directory Analizer");
	
	// Moving renderer to main rendering class
	rerender render(std::move(mainWindow));
	
	// Starting application
	render.AppLoop();

	return 0;
#endif
}