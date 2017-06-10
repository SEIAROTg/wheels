#ifndef HEADER_BINTREE
#define HEADER_BINTREE

#include <stack>
#include <algorithm>

template <class T>
class BinTreeNode {
public:
	BinTreeNode(const T &value) : _value(value), _height(1) {}

	T &
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

	BinTreeNode<T> **&
	parent_ref() {
		return _parent_ref;
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

	BinTreeNode<T> *
	attach(BinTreeNode<T> *parent, BinTreeNode<T> **parent_ref) {
		BinTreeNode<T> *old = *parent_ref;
		if (old) {
			old->unattach();
		}
		if (this->parent_ref()) {
			*this->parent_ref() = nullptr;
		}
		this->parent() = parent;
		this->parent_ref() = parent_ref;
		*this->parent_ref() = this;
		return old;
	}

	BinTreeNode<T> *
	unattach() {
		this->parent() = nullptr;
		*this->parent_ref() = nullptr;
		this->parent_ref() = nullptr;
		return this;
	}

	BinTreeNode<T> *
	replace(BinTreeNode<T> *node) {
		return attach(node->parent(), node->parent_ref());
	}
protected:
	T _value;
	BinTreeNode<T> *_parent = nullptr, *_left = nullptr, *_right = nullptr, **_parent_ref = nullptr;
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
