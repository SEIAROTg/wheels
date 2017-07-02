#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stack>
#define private public
#include "LeftistHeap.hpp"
#undef private
#include "../utils.hpp"
#include "../../tree/utils.hpp"

template <class T>
bool check_npl(LeftistHeap<T> &heap) {
	LeftistNode<T> *root = heap.data;
	std::stack<LeftistNode<T> *> stack;
	stack.push(root);
	while (stack.size()) {
		LeftistNode<T> *node = stack.top();
		stack.pop();
		if (node) {
			stack.push(node->right());
			stack.push(node->left());
			if (!node->left() && node->npl()) {
				return false;
			}
			if (node->right() && node->npl() != node->right()->npl() + 1) {
				return false;
			}
			if (node->right() && node->left()->npl() < node->right()->npl()) {
				return false;
			}
		}
	}
	return true;
}

int main() {
	std::srand(std::time(0));

	LeftistHeap<int> heap1;
	for (int i = 0; i < 10000000; ++i) {
		heap1.insert(std::rand());
	}
	std::cout << "size: " << heap1.size() << std::endl;
	// std::cout << "npl: " << check_npl(heap1) << std::endl;
	// std::cout << "ordered: " << check_ordered(heap1) << std::endl;


	// LeftistHeap<int> heap2;
	// for (int i = 0; i < 10000; ++i) {
	// 	heap1.insert(std::rand());
	// }
	// for (int i = 0; i < 10000; ++i) {
	// 	heap2.insert(std::rand());
	// }
	// heap1.merge(heap2);

	// std::cout << "size: " << heap1.size() << std::endl;
	// std::cout << "npl: " << check_npl(heap1) << std::endl;
	// std::cout << "ordered: " << check_ordered(heap1) << std::endl;

}
