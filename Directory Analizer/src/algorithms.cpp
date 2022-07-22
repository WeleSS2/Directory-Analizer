#pragma once
#include <iostream>
#include <filesystem>

#include "algorithms.h"

template <class T>
void algo::mapFolder(T path)
{
	namespace fs = std::filesystem;
	std::cout << path << "\n";
	for (const auto& entry : fs::directory_iterator(path))
	{
		if (entry.is_directory())
		{
			std::cout << "New thread called    " << fs::path{ entry } << "\n";
			threads.push_task([&] {
				mapFolder(fs::path{ entry });
				}
			);
		}
		else
		{
			std::cout << "Text file test" << "\n";
			isTextFile(entry);
		}
	}
};

void algo::isTextFile(std::filesystem::directory_entry dir)
{
	namespace fs = std::filesystem;
	std::cout << fs::path{ dir } << "\n";
}