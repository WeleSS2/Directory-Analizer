// Directory Analizer.cpp : Defines the entry point for the application.
//
#include "Directory Analizer.h"
#include "src/algorithms.h"
#include <thread>

using namespace std;

struct threads
{
	std::jthread obj;
	bool free = true;
};

int main(int argc, char* argv[])
{
	auto mainWin = std::make_shared<Window_Init>("Directory Analizer");
	
	//std::vector<threads> vec_threads(std::thread::hardware_concurrency());

	rerender render(std::move(mainWin));

	/*if (vec_threads[1].free)
	{
		std::jthread temp{ &rerender::AppLoop, std::ref(render) };
		vec_threads[1].obj.swap(temp);

		vec_threads[1].obj.join();
		vec_threads[1].free = false;
	}*/
	//render.AppLoop();
	std::jthread t1{&rerender::AppLoop, std::ref(render)};
	t1.join();
	/*pool.queue([&render] {
			render.AppLoop();
		}
	);*/
	return 0;
}
