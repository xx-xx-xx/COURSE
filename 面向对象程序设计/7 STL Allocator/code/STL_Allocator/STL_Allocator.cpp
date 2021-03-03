#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <random>
#include <limits>
#include "MyAllocator.h"
template<class T>
using STDAllocator = std::allocator<T>;

template<class T>
using MyAllocator = MyAlloc<T>;

using Point2D = std::pair<int, int>;

const int TestSize = 18000;
const int PickSize = 1000;

int main()
{
	auto start = std::chrono::high_resolution_clock::now();

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, TestSize);

	// vector creation
	using IntVec = std::vector<int, MyAllocator <int>>;
	std::vector<IntVec, MyAllocator <IntVec>> vecints(TestSize);
	for (int i = 0; i < TestSize; i++)
		vecints[i].resize(dis(gen));

	using PointVec = std::vector<Point2D, MyAllocator <Point2D>>;
	std::vector<PointVec, MyAllocator <PointVec>> vecpts(TestSize);
	for (int i = 0; i < TestSize; i++)
		vecpts[i].resize(dis(gen));

	// vector resize
	for (int i = 0; i < PickSize; i++)
	{
		int idx = dis(gen) - 1;
		int size = dis(gen);
		vecints[idx].resize(size);
		vecpts[idx].resize(size);
	}

	// vector element assignment
	{
		int val = 10;
		int idx1 = dis(gen) - 1;
		int idx2 = vecints[idx1].size() / 2;
		vecints[idx1][idx2] = val;
		if (vecints[idx1][idx2] == val)
			std::cout << "correct assignment in vecints: " << idx1 << std::endl;
		else
			std::cout << "incorrect assignment in vecints: " << idx1 << std::endl;
	}
	{
		Point2D val(11, 15);
		int idx1 = dis(gen) - 1;
		int idx2 = vecpts[idx1].size() / 2;
		vecpts[idx1][idx2] = val;
		if (vecpts[idx1][idx2] == val)
			std::cout << "correct assignment in vecpts: " << idx1 << std::endl;
		else
			std::cout << "incorrect assignment in vecpts: " << idx1 << std::endl;
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "Time used: " << elapsed.count() << " seconds." << std::endl;

	return 0;
}
