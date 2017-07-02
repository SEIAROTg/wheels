#ifndef HEADER_AVL
#define HEADER_AVL

#include "../BST/BST.hpp"

template <class T>
class AVLNode : public BinTreeNodeBase<T, AVLNode<T>> {
public:
	using BinTreeNodeBase<T, AVLNode<T>>::BinTreeNodeBase;
	bool balanced() {
		int balance = (this->left() ? this->left()->height() : 0) - (this->right() ? this->right()->height() : 0);
		return balance >= -1 && balance <= 1;
	}
	AVLNode<T> *taller() {
		if (!this->left()) {
			return this->right();
		} else if (!this->right()) {
			return this->left();
		} else if (this->left()->height() > this->right()->height()) {
			return this->left();
		} else {
			return this->right();
		}
	}
};

template <class T>
class AVL : public BST<T, AVLNode<T>> {
private:
	static
	AVLNode<T> *
	connect34(
		AVLNode<T> *root,
		AVLNode<T> *x, AVLNode<T> *y, AVLNode<T> *z,
		AVLNode<T> *a, AVLNode<T> *b, AVLNode<T> *c, AVLNode<T> *d
	) {
		y->replace(root);
		AVLNode<T>::attach(a, x, &x->left());
		AVLNode<T>::attach(b, x, &x->right());
		AVLNode<T>::attach(c, z, &z->left());
		AVLNode<T>::attach(d, z, &z->right());
		x->attach(y, &y->left());
		z->attach(y, &y->right());
		x->update();
		z->update();
		return y;
	}

	static
	AVLNode<T> *
	rotate(AVLNode<T> *node) {
		AVLNode<T> *child = node->taller(), *grandchild = child->taller();
		if (child == node->left()) {
			if (grandchild == child->left()) {
				return connect34(node, grandchild, child, node, grandchild->left(), grandchild->right(), child->right(), node->right());
			} else {
				return connect34(node, child, grandchild, node, child->left(), grandchild->left(), grandchild->right(), node->right());
			}
		} else {
			if (grandchild == child->left()) {
				return connect34(node, node, grandchild, child, node->left(), grandchild->left(), grandchild->right(), child->right());
			} else {
				return connect34(node, node, child, grandchild, node->left(), child->left(), grandchild->left(), grandchild->right());
			}
		}
	}

public:
	AVLNode<T> *
	insert(const T &key) {
		AVLNode<T> *inserted = this->BST<T, AVLNode<T>>::insert(key), *node = inserted;
		while (node && node->balanced()) {
			node = node->parent();
		}
		if (node) {
			rotate(node);
		}
		return inserted;
	}

	AVLNode<T> *
	remove(const T &key) {
		AVLNode<T> *affected = this->BST<T, AVLNode<T>>::remove(key), *node = affected;
		while (node) {
			if (!node->balanced()) {
				rotate(node);
			}
			node = node->parent();
		}
		return affected;
	}

	operator BST<T>&() { return *reinterpret_cast<BST<T>*>(this); }
};

#endif
