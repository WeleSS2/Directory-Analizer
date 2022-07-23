#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include "algorithms.h"

template <class T>
void Algo::mapFolder(T path)
{
	// Directory analizer, iterating through directory and employing tasks into queue.
	int files = 0;
	namespace fs = std::filesystem;
	for (const auto& entry : fs::directory_iterator(path))
	{
		{
			++files;
		}
		// Directory checking, recursion
		if (entry.is_directory())
		{
			Threads.push_task([=] {
				mapFolder(fs::path{ entry });
				}
			);
		}
		// File checking
		else if (checkExtension(entry))
		{
			Threads.push_task([=] {
				isTextFile(entry);
				}
			);
		}
	}
	// Both directory and text files are counting as files in system (Directories are specific light-weighted files)
	files_amount += files;
};

void Algo::isTextFile(std::filesystem::directory_entry dir)
{
	// Text files analizer, checking for filled and empty lines in file, counting words (letters strings) and letters.
	int lines = 0, empty = 0, lett = 0, words = 0;
	namespace fs = std::filesystem;

	// Load file and start loop
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
					++empty;
				}
				else
				{
					// Checking letters and words
					++lines;
					int letters_in_row = 0;
					for (auto i : line)
					{
						if ((i >= 65 && i <= 90) || (i >= 97 && i <= 122))
						{
							++lett;
							++letters_in_row;
						}
						else if ((i == 32 || i == 33 || i == 63 || i == 46 || i == 44) && (letters_in_row > 0))
						{
							++words;
							letters_in_row = 0;
						}
					}
					// Checking did there was any letter before end of file. Checking EOF (ASCII 10 or 0x0a) doesn't work for some reasons.
					if (letters_in_row > 0)
					{
						++words;
					}
				}
			}
		}
	}
	// Adding into total count.
	emptylines += empty;
	filledlines += lines;
	letters_amount += lett;
	words_amount += words;
}

bool Algo::checkExtension(std::filesystem::path(entry))
{
	// Checking did program support extension of this file (top 10 popular text file extensions)
	bool ret = false;
	for (int i = 0; i < ext.size(); ++i)
	{
		if (std::filesystem::path(entry).extension() == ext[i])
		{
			ret = true;
			break;
		}
	}
	return ret;
}