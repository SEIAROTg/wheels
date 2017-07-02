#include <iostream>
#include <cstdlib>
#include <ctime>
#include "BinHeap.hpp"
#include "../utils.hpp"

int main() {
	std::srand(std::time(0));

	BinHeap<int> heap1;
	for (int i = 0; i < 10000; ++i) {
		heap1.insert(std::rand());
	}

	std::cout << "size: " << heap1.size() << std::endl;
	std::cout << "ordered: " << check_ordered(heap1) << std::endl;

	std::vector<int> data;
	for (int i = 0; i < 10000; ++i) {
		data.push_back(std::rand());
	}
	BinHeap<int> heap2(data.begin(), data.end());

	std::cout << "size: " << heap2.size() << std::endl;
	std::cout << "ordered: " << check_ordered(heap2) << std::endl;

}
