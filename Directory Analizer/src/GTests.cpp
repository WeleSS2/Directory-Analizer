#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>

#include <gtest/gtest.h>


class GTests {
public:
	void makeFolder(std::string path, int directories_amount);
	void makeFile(std::string path, int files_amount);
	void makeFoldersWithFiles(std::string path, int directories_amount, int files_amount_per_directory);
	void removeAll(std::string path);
	bool analizeTest(Algo& obj, unsigned long long files = 0, unsigned long long filledlines = 0, unsigned long long emptylines = 0,
		unsigned long long words = 0, unsigned long long letters = 0);
};

struct GTestsOverride : public ::testing::Test {
	virtual void SetUp() override
	{

	};
	virtual void TearDown() override
	{

	};
};

void GTests::makeFolder(std::string path, int directories_amount)
{
	for(int i = 0; i < directories_amount; ++i)
	{
		std::filesystem::create_directory(path + std::to_string(i));
	}
};

void GTests::makeFile(std::string path, int files_amount)
{
	for (int i = 0; i < files_amount; ++i)
	{
		std::ofstream file(path + "file" + std::to_string(i) + ".txt");
		for (int j = 0; j < 5; ++j)
		{
			file << "Content inside file " + std::to_string(i) << "   " << std::to_string(j) << "\n";
		}
	}
};
void GTests::makeFoldersWithFiles(std::string path, int directories_amount, int files_amount_per_directory)
{
	for (int i = 0; i < directories_amount; ++i)
	{
		std::filesystem::create_directory(path + std::to_string(i));
		for (int k = 0; k < files_amount_per_directory; ++k)
		{
			std::ofstream file(path + std::to_string(i) + "/file" + std::to_string(k) + ".txt");
			for (int j = 0; j < 5; ++j)
			{
				file << "Content inside file " + std::to_string(k) << "   " << std::to_string(j) << "\n";
			}
		}
	}
}

void GTests::removeAll(std::string path)
{
	std::filesystem::remove_all(path);
};

bool GTests::analizeTest(Algo& obj, unsigned long long files, unsigned long long filledlines, unsigned long long emptylines,
	unsigned long long words, unsigned long long letters)
{
	if (obj.getFilesAmount() != files || obj.getLinesAmount() != filledlines ||
		obj.getEmptyLinesAmount() != emptylines || obj.getWordsAmount() != words || obj.getLettersAmount() != letters)
	{
		std::cout << "Error, values:" << std::endl;
		std::cout << "Files: " << obj.getFilesAmount() << " should be " << std::to_string(files) << std::endl;
		std::cout << "Filled lines: " << obj.getLinesAmount() << " should be " << std::to_string(filledlines) << std::endl;
		std::cout << "Empty lines: " << obj.getEmptyLinesAmount() << " should be " << std::to_string(emptylines) << std::endl;
		std::cout << "Words: " << obj.getWordsAmount() << " should be " << std::to_string(words) << std::endl;
		std::cout << "Letters: " << obj.getLettersAmount() << " should be " << std::to_string(letters) << std::endl;
		return false;
	}
	else
	{
		return true;
	}
}

TEST(GTestsOverride, Zero)
{
	GTests objGTests;
	std::string path{ std::filesystem::temp_directory_path().string() };
	objGTests.makeFolder(path, 1);

	Algo objAlgo;
	objAlgo.mapFolder(path + "0");
	while (Threads.getFinishedStatus())
	{
		if (Threads.getFinishedStatus() == 0)
		{
			EXPECT_EQ(objGTests.analizeTest(objAlgo), TRUE);
			break;
		}
	}
	objGTests.removeAll(path + "0");
}

TEST(GTestsOverride, One)
{
	GTests objGTests;
	std::string path{ std::filesystem::temp_directory_path().string() };
	objGTests.makeFolder(path, 1);
	objGTests.makeFile(path + "0/", 1);
	objGTests.makeFoldersWithFiles(path + "0/", 1, 1);
			 
	Algo objAlgo;
	objAlgo.mapFolder(path + "0");
	while(Threads.getFinishedStatus())
	{
		if (Threads.getFinishedStatus() == 0)
		{
			EXPECT_EQ(objGTests.analizeTest(objAlgo, 3, 10, 0, 30, 170), TRUE);
			break;
		}
	}
	objGTests.removeAll(path + "0");
}

TEST(GTestsOverride, doubleCores)
{
	GTests objGTests;
	std::string path{ std::filesystem::temp_directory_path().string() };
	int maxthreads = (std::thread::hardware_concurrency() * 2);
	objGTests.makeFolder(path, 1);
	objGTests.makeFoldersWithFiles(path + "0/", maxthreads, maxthreads);

	Algo objAlgo;
	objAlgo.mapFolder(path + "0");
	while (Threads.getFinishedStatus())
	{
		if (Threads.getFinishedStatus() == 0)
		{
			EXPECT_EQ(objGTests.analizeTest(objAlgo, maxthreads * (maxthreads + 1), maxthreads * (maxthreads * 5), 0
			, maxthreads * (maxthreads * 15), maxthreads * (maxthreads * 85)), TRUE);
			break;
		}
	}
	objGTests.removeAll(path + "0");
}