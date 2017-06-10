#ifndef HEADER_BST
#define HEADER_BST

#include <tuple>
#include "BinTree.hpp"

template <class T>
class BST : public BinTree<T> {
protected:
	std::pair<
		BinTreeNode<T> **, // parent_ref of match
		BinTreeNode<T> *   // parent of match
	>
	_search(const T &key) {
		BinTreeNode<T> **parent_ref = &this->root(), *parent=nullptr;
		while (*parent_ref && (*parent_ref)->value() != key) {
			parent = *parent_ref;
			parent_ref = key < parent->value() ? &parent->left() : &parent->right();
		}
		return std::make_pair(parent_ref, parent);
	}

	std::pair<
		BinTreeNode<T> **, // parent_ref of leaf
		BinTreeNode<T> *   // parent of leaf
	>
	succ(BinTreeNode<T> *parent) {
		BinTreeNode<T> **parent_ref = &parent->right();
		while (*parent_ref) {
			parent = *parent_ref;
			parent_ref = &parent->left();
		}
		return std::make_pair(parent_ref, parent);
	}

public:
	BinTreeNode<T> *
	search(const T &key) {
		return *_search(key).first;
	}

	virtual
	BinTreeNode<T> *
	insert(const T &key) {
		BinTreeNode<T> **parent_ref, *parent;
		std::tie(parent_ref, parent) = _search(key);
		if (*parent_ref) {
			std::tie(parent_ref, parent) = succ(*parent_ref);
		}
		(new BinTreeNode<T>(key))->attach(parent, parent_ref);
		if (parent) {
			parent->update();
		}
		return *parent_ref;
	}

	virtual
	BinTreeNode<T> *
	remove(const T &key) {
		BinTreeNode<T> **parent_ref, *parent;
		std::tie(parent_ref, parent) = _search(key);
		if (*parent_ref) {
			if ((*parent_ref)->right()) {
				BinTreeNode<T> *non_leaf = succ(*parent_ref).second, *non_leaf_parent = non_leaf->parent();
				(*parent_ref)->value() = non_leaf->value();
				if (non_leaf->right()) {
					delete non_leaf->right()->replace(non_leaf);
				} else {
					delete non_leaf->unattach();
				}
				non_leaf_parent->update();
				return non_leaf_parent;
			} else if ((*parent_ref)->left()) {
				delete (*parent_ref)->left()->replace(*parent_ref);
				(*parent_ref)->update();
				return *parent_ref;
			} else {
				delete (*parent_ref)->unattach();
				parent->update();
				return parent;
			}
		}
		return nullptr;
	}
};

#endif
