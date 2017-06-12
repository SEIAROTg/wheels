#include <vector>
#include <iostream>
#include "RMQ.hpp"

int main() {
	const RMQ<int> a({1,2,3,4,5,4,3,2,1});

	int i, j;
	while (std::cin >> i >> j) {
		std::cout << a.query(i, j) << std::endl;
	}
	return 0;
}
