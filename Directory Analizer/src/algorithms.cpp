#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <cctype>
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
			threads.push_task([=] {
				mapFolder(fs::path{ entry });
				}
			);
			++files_amount;
		}
		else if (checkExtension(entry))
		{
			threads.push_task([=] {
				isTextFile(entry);
				}
			);
			++files_amount;
		}
		else
		{
			++files_amount;
		}
	}
};

void algo::isTextFile(std::filesystem::directory_entry dir)
{
	namespace fs = std::filesystem;
	std::ifstream file(fs::path{ dir }, std::ios::in);
	if (file.is_open())
	{
		while (!file.eof())
		{
			std::string line;
			while (std::getline(file, line))
			{
				if (line.empty())
				{
					++emptylines;
				}
				else
				{
					++filledlines;
					
					for (auto i : line)
					{
						if((i >= 65 && i <= 90) || (i >= 97 && i <= 122))
							++letters;
					}
				}
			}
		}
	}
}