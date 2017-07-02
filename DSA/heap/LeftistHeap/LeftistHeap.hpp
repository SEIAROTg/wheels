#ifndef HEADER_LEFTISTHEAP
#define HEADER_LEFTISTHEAP

#include <stack>
#include <iterator>
#include <utility>
#include <tuple>
#include "../Heap/Heap.hpp"
#include "../../tree/BinTree/BinTree.hpp"

template <class T>
class LeftistNode : public BinTreeNodeBase<T, LeftistNode<T>> {
private:
	int _npl = 0;
public:
	using BinTreeNodeBase<T, LeftistNode<T>>::BinTreeNodeBase;

	int &npl() {
		return _npl;
	}
};

template <class T>
class LeftistHeap : public Heap<T> {
private:
	LeftistNode<T> *data = nullptr;
	unsigned _size = 0;

	static LeftistNode<T> *merge(LeftistNode<T> *a, LeftistNode<T> *b) {
		if (!a) return b;
		if (!b) return a;
		if (a->value() < b->value()) std::swap(a, b);

		LeftistNode<T> *merged = merge(a->right(), b);
		if (!a->left() || a->left()->npl() < merged->npl()) {
			merged->attach(a, &a->left())->attach(a, &a->right());
		} else {
			merged->attach(a, &a->right());
		}
		a->npl() = a->right() ? a->right()->npl() + 1 : 0;
		return a;
	}
public:
	LeftistHeap() {}

	void merge(const LeftistHeap &another) {
		merge(data, another.data)->attach(nullptr, &data);
		_size += another._size;
	}

	void insert(const T &value) {
		merge(data, new LeftistNode<T>(value))->attach(nullptr, &data);
		++_size;
	}

	T getMax() {
		return data->value();
	}

	void delMax() {
		delete merge(data->left(), data->right())->attach(nullptr, &data);
		--_size;
	}

	unsigned size() {
		return _size;
	}

	void destroy() {
		std::stack<LeftistNode<T> *> stack;
		stack.push(data);
		while (stack.size()) {
			LeftistNode<T> *node = stack.top();
			stack.pop();
			if (node) {
				stack.push(node->right());
				stack.push(node->left());
				delete node;
			}
		}
		data = nullptr;
	}
};

#endif
