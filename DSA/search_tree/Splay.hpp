#ifndef HEADER_SPLAY
#define HEADER_SPLAY

#include "BST.hpp"

template <class T, class Node = BinTreeNode<T>>
class Splay : public BST<T, Node> {
private:
	Node *splay(Node *node) {
		Node *parent = nullptr, *grandparent;
		while (node && (parent = node->parent()) && (grandparent=parent->parent())) {
			bool node_left = node == parent->left();
			bool parent_left = parent == grandparent->left();
			node->replace(grandparent);
			if (node_left) {
				parent->attach(node, &node->right())->attach(parent, &parent->left());
				if (parent_left) {
					grandparent->attach(parent, &parent->right())->attach(grandparent, &grandparent->left());
				} else {
					grandparent->attach(node, &node->left())->attach(grandparent, &grandparent->right());
				}
			} else {
				parent->attach(node, &node->left())->attach(parent, &parent->right());
				if (parent_left) {
					grandparent->attach(node, &node->right())->attach(grandparent, &grandparent->left());
				} else {
					grandparent->attach(parent, &parent->left())->attach(grandparent, &grandparent->right());
				}
			}
			parent->update();
			grandparent->update();
		}
		if (parent) {
			bool node_left = node == parent->left();
			node->replace(parent);
			if (node_left) {
				parent->attach(node, &node->right())->attach(parent, &parent->left());
			} else {
				parent->attach(node, &node->left())->attach(parent, &parent->right());
			}
			parent->update();
		}
		return node;
	}
public:
	virtual
	Node *
	search(const T &key) {
		Node **parent_ref, *parent;
		std::tie(parent_ref, parent) = this->_search(key);
		splay(*parent_ref ? *parent_ref : parent);
		return *parent_ref;
	}

	virtual
	Node *
	insert(const T &key) {
		search(key);
		return this->BST<T, Node>::insert(key);
	}

	virtual
	Node *
	remove(const T &key) {
		search(key);
		return this->BST<T, Node>::remove(key);
	}
};

#endif
