#pragma once
#include <iostream> 
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>
#include <filesystem>
#include <atomic>

class Algo
{
private:
	std::string folderpath = "";
	// Amount of parameters in folder and subfolders.
	std::atomic <unsigned long long> files_amount = 0, emptylines = 0, filledlines = 0, words_amount = 0, letters_amount = 0;
	// Supported extensions
	std::vector <std::string> ext{".pages", ".rtf", ".srt", ".log", ".msg", ".odt", ".lit", ".txt" , ".doc", ".docx"};
public:
	Algo()
	{
	};

	// Overloading constructor to create object with path
	Algo(std::string path)
		: folderpath(path)
	{
	};

	~Algo()
	{
	};

	void setPatch(std::string path)
	{
		folderpath = path;
	};

	// Check did file have suported extension
	bool checkExtension(std::filesystem::path(entry));

	// Iterate through directory to find directories and files inside
	template <class T>
	void mapFolder(T path);

	// If file is text file count parameters
	void isTextFile(std::filesystem::directory_entry dir);

	// Reset parameters if new folder is loaded
	void resetStats()
	{
		files_amount = 0;
		emptylines = 0;
		filledlines = 0;
		words_amount = 0;
		letters_amount = 0;
	};

	// Functions to get parameters
	unsigned long long getFilesAmount()
	{
		return files_amount;
	};
	unsigned long long getLinesAmount()
	{
		return filledlines;
	};
	unsigned long long getEmptyLinesAmount()
	{
		return emptylines;
	};
	unsigned long long getWordsAmount()
	{
		return words_amount;
	};
	unsigned long long getLettersAmount()
	{
		return letters_amount;
	};
};

class Thread_Pool
{
public:
	using Task = std::function<void()>;

	Thread_Pool(int Threads)
	{
		start(Threads);
	}
	~Thread_Pool()
	{
		stop();
	}

	// Push task into one of the threads
	void push_task(Task task)
	{
		{
			std::unique_lock<std::mutex> lock(mEventMutex);
			qTasks.emplace(std::move(task));
		}
		mEventVar.notify_one();
	}
private:
	std::vector<std::jthread> vec_mThreads;
	std::queue<Task> qTasks;


	std::condition_variable mEventVar;

	std::mutex mEventMutex;
	bool mStop = false;
	
	void start(int Threads)
	{
		// Constructor
		for (int i = 0; i < Threads; ++i)
		{
			vec_mThreads.emplace_back([=]
				{
					while (true)
					{
						Task task;
						
						{
							std::unique_lock<std::mutex> lock{ mEventMutex };

							mEventVar.wait(lock, [=] { 
								return mStop || !qTasks.empty();  
								});

							if (mStop && qTasks.empty())
							{
								break;
							};

							task = std::move(qTasks.front());
							qTasks.pop();
						}

						task();
 					}
				}
			);
		}
	}

	void stop() noexcept
	{
		// Destructor
		{
			std::unique_lock<std::mutex> lock{ mEventMutex };
			mStop = true;
		}
		mEventVar.notify_all();
		for (auto& thread : vec_mThreads)
		{
			thread.join();
		}
	}
};

Thread_Pool Threads(std::thread::hardware_concurrency());