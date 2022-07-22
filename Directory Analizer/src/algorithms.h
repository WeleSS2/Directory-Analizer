#pragma once
#include <iostream> 
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>

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

class Thread_Pool
{
public:
	using Task = std::function<void()>;

	explicit Thread_Pool(int Threads)
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
								return mStop || !qTasks.empty();  });

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