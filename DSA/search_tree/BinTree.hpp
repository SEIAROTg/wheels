#ifndef HEADER_BINTREE
#define HEADER_BINTREE

#include <stack>
#include <algorithm>

template <class T, class Derived>
class BinTreeNodeBase {
public:
	BinTreeNodeBase(const T &value) : _value(value), _height(1) {}

	T &
	value() {
		return _value;
	}

	Derived *&
	parent() {
		return _parent;
	}

	Derived *&
	left() {
		return _left;
	}

	Derived *&
	right() {
		return _right;
	}

	Derived **&
	parent_ref() {
		return _parent_ref;
	}

	int
	height() {
		return _height;
	}

	void
	update() {
		Derived *node = reinterpret_cast<Derived *>(this);
		while (node) {
			node->_height = std::max(
				node->left() ? node->left()->height() : 0, node->right() ? node->right()->height() : 0
			) + 1;
			node = node->parent();
		}
	}

	static
	Derived *
	attach(Derived *node, Derived *parent, Derived **parent_ref) {
		Derived *old = *parent_ref;
		if (old) {
			old->unattach();
		}
		if (node) {
			if (node->parent_ref()) {
				*node->parent_ref() = nullptr;
			}
			node->parent() = parent;
			node->parent_ref() = parent_ref;
		}
		*parent_ref = node;
		return old;
	}

	static
	Derived *
	unattach(Derived *node) {
		if (node) {
			node->parent() = nullptr;
			*node->parent_ref() = nullptr;
			node->parent_ref() = nullptr;
		}
		return node;
	}

	static
	Derived *
	replace(Derived *repl, Derived *node) {
		return attach(repl, node->parent(), node->parent_ref());
	}

	Derived *
	attach(Derived *parent, Derived **parent_ref) {
		return attach(reinterpret_cast<Derived *>(this), parent, parent_ref);
	}

	Derived *
	unattach() {
		return unattach(reinterpret_cast<Derived *>(this));
	}

	Derived *
	replace(Derived *node) {
		return replace(reinterpret_cast<Derived *>(this), node);
	}
protected:
	T _value;
	Derived *_parent = nullptr, *_left = nullptr, *_right = nullptr, **_parent_ref = nullptr;
	int _height;
};

template <class T>
class BinTreeNode : public BinTreeNodeBase<T, BinTreeNode<T>> {
	using BinTreeNodeBase<T, BinTreeNode<T>>::BinTreeNodeBase;
};

template <class T, class Node = BinTreeNode<T>>
class BinTree {
public:
	virtual Node *search(const T &) = 0;
	virtual Node *insert(const T &) = 0;
	virtual Node *remove(const T &) = 0;
	Node *&root() { return _root; }
	int height() {
		return root() ? root()->height() : 0;
	}
private:
	Node *_root = nullptr;
};

#endif
