#pragma once
#include <atomic>
#include <condition_variable>
#include <coroutine>
#include <functional>
#include <iostream>
#include <optional>
#include <queue>
#include <shared_mutex>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>

class algo
{
private:
	std::string folderpath = "";
public:
	algo(std::string path)
		: folderpath(path)
	{
	};

	~algo()
	{
	};

	void mapFolders();
};

class thread_pool
{
	unsigned int numThreads = std::thread::hardware_concurrency();
public:
	
};