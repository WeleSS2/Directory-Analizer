#pragma once
#include <iostream> 
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>
#include <filesystem>

class algo
{
private:
	std::string folderpath = "";
	unsigned long long files_amount = 0, emptylines = 0, filledlines = 0, words = 0, letters = 0;
	std::vector <std::string> ext{".pages", ".rtf", ".srt", ".log", ".msg", ".odt", ".lit", ".txt" , ".doc", ".docx"};
public:
	algo()
	{
	};
	algo(std::string path)
		: folderpath(path)
	{
	};

	void setPatch(std::string path)
	{
		folderpath = path;
	};

	~algo()
	{
	};

	bool checkExtension(std::filesystem::path(entry))
	{
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
	template <class T>
	void mapFolder(T path);
	void analizeFile();
	void isTextFile(std::filesystem::directory_entry dir);

	void resetStats()
	{
		files_amount = 0;
		emptylines = 0;
		filledlines = 0;
		words = 0;
		letters = 0;
	};

	unsigned long long GetFilesAmount()
	{
		return files_amount;
	};
	unsigned long long GetLinesAmount()
	{
		return filledlines;
	};
	unsigned long long GetEmptyLinesAmount()
	{
		return emptylines;
	};
	unsigned long long GetWordsAmount()
	{
		return words;
	};
	unsigned long long GetLettersAmount()
	{
		return letters;
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

Thread_Pool threads(std::thread::hardware_concurrency());