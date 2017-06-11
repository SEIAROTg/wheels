#ifndef HEADER_BST
#define HEADER_BST

#include <tuple>
#include "BinTree.hpp"

template <class T, class Node = BinTreeNode<T>>
class BST : public BinTree<T, Node> {
protected:
	std::pair<
		Node **, // parent_ref of match
		Node *   // parent of match
	>
	_search(const T &key) {
		Node **parent_ref = &this->root(), *parent=nullptr;
		while (*parent_ref && (*parent_ref)->value() != key) {
			parent = *parent_ref;
			parent_ref = key < parent->value() ? &parent->left() : &parent->right();
		}
		return std::make_pair(parent_ref, parent);
	}

	std::pair<
		Node **, // parent_ref of leaf
		Node *   // parent of leaf
	>
	succ(Node *parent) {
		Node **parent_ref = &parent->right();
		while (*parent_ref) {
			parent = *parent_ref;
			parent_ref = &parent->left();
		}
		return std::make_pair(parent_ref, parent);
	}

public:
	Node *
	search(const T &key) {
		return *_search(key).first;
	}

	virtual
	Node *
	insert(const T &key) {
		Node **parent_ref, *parent;
		std::tie(parent_ref, parent) = _search(key);
		if (*parent_ref) {
			std::tie(parent_ref, parent) = succ(*parent_ref);
		}
		(new Node(key))->attach(parent, parent_ref);
		if (parent) {
			parent->update();
		}
		return *parent_ref;
	}

	virtual
	Node *
	remove(const T &key) {
		Node **parent_ref, *parent;
		std::tie(parent_ref, parent) = _search(key);
		if (*parent_ref) {
			if ((*parent_ref)->right()) {
				Node *non_leaf = succ(*parent_ref).second, *non_leaf_parent = non_leaf->parent();
				(*parent_ref)->value() = non_leaf->value();
				delete Node::replace(non_leaf->right(), non_leaf);
				non_leaf_parent->update();
				return non_leaf_parent;
			} else {
				delete Node::replace((*parent_ref)->left(), *parent_ref);
				return parent;
			}
		}
		return nullptr;
	}
};

#endif
