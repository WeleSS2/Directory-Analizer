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

	explicit Thread_Pool(std::size_t Threads)
	{
		start(Threads);
	}
	~Thread_Pool()
	{
		stop();
	}

	/*void queueold(Task task)
	{
		{
			std::unique_lock<std::mutex> lock(mEventMutex);
			qTasks.emplace(std::move(task));
		}

		mEventVar.notify_one();
	}*/

	template <class T>
	auto queue(T task)->std::future<decltype(task())>
	{
		std::cout << "Queue in" << "\n";
		auto wrapper = std::make_shared<std::packaged_task<decltype(task()) ()>>(std::move(task));
		{
			std::cout << "Queue in 2" << "\n";
			std::unique_lock<std::mutex> lock(mEventMutex);
			std::cout << "Preemplace" << "\n";
			qTasks.emplace([=]
				{
					(*wrapper)();
				});
			std::cout << "Post emplace" << "\n";
		}

		mEventVar.notify_one();
		return wrapper->get_future();
		std::cout << "Queue out" << "\n";
	}
private:
	std::vector<std::thread> vec_mThreads;
	std::queue<Task> qTasks;


	std::condition_variable mEventVar;

	std::mutex mEventMutex;
	bool mStop = false;

	void start(std::size_t Threads)
	{
		std::cout << "Start in" << "\n";
		for (int i = 0; i < Threads; ++i)
		{
			vec_mThreads.emplace_back([=]
				{
					while (true)
					{
						Task task;
						
						{
							std::unique_lock<std::mutex> lock{ mEventMutex };

							mEventVar.wait(lock, [=] {return mStop || qTasks.empty();  });

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
		std::cout << "Start out" << "\n";
	}

	void stop() noexcept
	{
		std::cout << "Stop in" << "\n";
		{
			std::unique_lock<std::mutex> lock{ mEventMutex };
			mStop = true;
		}
		mEventVar.notify_all();
		for (auto& thread : vec_mThreads)
		{
			thread.join();
		}
		std::cout << "Stop out" << "\n";
	}
};