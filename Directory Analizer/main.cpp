// Directory Analizer.cpp : Defines the entry point for the application.
//
#pragma once
#include "main.h"
//#include "gtest/gtest.h"

int main(int argc, char* argv[])
{
	//::testing::InitGoogleTest(&argc, argv);

	// Creating window and renderer
	auto mainWin = std::make_shared<Window_Init>("Directory Analizer");
	
	// Moving renderer to main rendering class
	rerender render(std::move(mainWin));
	
	// Starting application
	render.AppLoop();


	//return RUN_ALL_TESTS();
	return 0;
}