#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stack>
#include "BST.hpp"
#include "../utils.hpp"

int main() {
	BST<int> tree;
	std::srand(std::time(0));
	for (int i = 0; i < 10000; ++i) {
		// tree.insert(std::rand());
		tree.insert(i);
	}
	for (int i = 500; i < 10000; ++i) {
		tree.remove(i);
	}
	for (int i = 0; i < 10; ++i) {
		tree.insert(10000);
	}
	for (int i = 0; i < 10; ++i) {
		tree.remove(10000);
	}

	display<int>(tree);
	std::cout << "is bst: " << verify_bst<int>(tree) << std::endl;
	std::cout << "height: " << tree.height() << std::endl;
}
