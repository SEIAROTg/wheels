#ifndef HEADER_BINTREE
#define HEADER_BINTREE

#include <stack>
#include <algorithm>

template <class T>
class BinTreeNode {
public:
	BinTreeNode(const T &value) : BinTreeNode(value, nullptr) {}

	BinTreeNode(const T &value, BinTreeNode<T> *parent) :
		_value(value),
		_parent(parent),
		_left(nullptr),
		_right(nullptr),
		_height(1)
		{}

	~BinTreeNode() {
		// std::stack<BinTreeNode<T>*> to_delete;
		// to_delete.push(this);
		// while (to_delete.size()) {
		// 	BinTreeNode<T> *top = to_delete.top();
		// 	to_delete.pop();
		// 	if (top->left()) {
		// 		to_delete.push(top->left());
		// 	}
		// 	if (top->right()) {
		// 		to_delete.push(top->right());
		// 	}
		// 	delete top;
		// }
	};

	const T &
	value() {
		return _value;
	}

	BinTreeNode<T> *&
	parent() {
		return _parent;
	}

	BinTreeNode<T> *&
	left() {
		return _left;
	}

	BinTreeNode<T> *&
	right() {
		return _right;
	}

	int
	height() {
		return _height;
	}

	void
	update() {
		BinTreeNode<T> *node = this;
		while (node) {
			node->_height = std::max(
				node->left() ? node->left()->height() : 0, node->right() ? node->right()->height() : 0
			) + 1;
			node = node->parent();
		}
	}
protected:
	T _value;
	BinTreeNode<T> *_parent, *_left, *_right;
	int _height;
};

template <class T>
class BinTree {
public:
	virtual BinTreeNode<T> *search(const T &) = 0;
	virtual BinTreeNode<T> *insert(const T &) = 0;
	virtual BinTreeNode<T> *remove(const T &) = 0;
	BinTreeNode<T> *&root() { return _root; }
	int height() {
		return root() ? root()->height() : 0;
	}
private:
	BinTreeNode<T> *_root = nullptr;
};

#endif
