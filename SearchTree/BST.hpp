#ifndef HEADER_BST
#define HEADER_BST

#include <tuple>
#include "BinTree.hpp"

template <class T>
class BST : public BinTree<T> {
protected:
	std::pair<
		BinTreeNode<T> **, // node
		BinTreeNode<T> **  // parent of match
	>
	_search(const T &key) {
		BinTreeNode<T> **node = &this->root(), **parent=node;
		while (*node && (*node)->value() != key) {
			parent = node;
			node = key < (*node)->value() ? &(*node)->left() : &(*node)->right();
		}
		return std::make_pair(node, parent);
	}

	std::pair<
		BinTreeNode<T> **, // node
		BinTreeNode<T> **  // parent of match
	>
	succ(BinTreeNode<T> **node) {
		BinTreeNode<T> **cursor = &(*node)->right();
		if (!*cursor) {
			return std::make_pair(cursor, node);
		} else {
			while (*cursor) {
				node = cursor;
				cursor = &(*cursor)->left();
			}
			return std::make_pair(cursor, node);
		}
	}


	// BinTreeNode<T> *&
	// move_to_leaf(BinTreeNode<T> *const &node, BinTreeNode<T> *&parent) {
	// 	if (!node) {
	// 		return node;
	// 	}
	// 	BinTreeNode<T> **cursor = &node->right();
	// 	while (*cursor) {
	// 		parent = *cursor;
	// 		cursor = &(*cursor)->left();
	// 	}
	// 	return *cursor;
	// }


	// std::pair<
	// 	std::reference_wrapper<BinTreeNode<T>*>, // succ leaf
	// 	std::reference_wrapper<BinTreeNode<T>*>  // parent of succ leaf
	// >
	// succ(BinTreeNode<T> *const &node, BinTreeNode<T> *const &parent) {
	// 	BinTreeNode<T> **cursor = &node->right();
	// 	if (!*cursor) {
	// 		return make_pair(node->right(), parent);
	// 	}
	// }

	// BinTreeNode<T> *&
	// succ_node(BinTreeNode<T> *const &node) {
	// 	BinTreeNode<T> **cursor = &node->right();
	// 	if (!*cursor) {
	// 		return node->parent();
	// 	} else {
	// 		while ((*cursor)->left()) {
	// 			cursor = &(*cursor)->left();
	// 		}
	// 		return *cursor;
	// 	}
	// }

	// BinTreeNode<T> *&
	// succ_leaf(BinTreeNode<T> *const &node) {
	// 	BinTreeNode<T> **cursor = &node->right();
	// 	if (!*cursor) {
	// 		return *cursor;
	// 	} else {
	// 		while (*cursor) {
	// 			cursor = &(*cursor)->left();
	// 		}
	// 		return *cursor;
	// 	}
	// }

public:
	BinTreeNode<T> *
	search(const T &key) {
		return *_search(key).first;
	}

	virtual
	BinTreeNode<T> *
	insert(const T &key) {
		BinTreeNode<T> **node, **parent;
		std::tie(node, parent) = _search(key);
		if (*node) {
			std::tie(node, parent) = succ(node);
		}
		*node = new BinTreeNode<T>(key, *parent);
		if (*parent) {
			(*parent)->update();
		}
		return *node;
	}

	virtual
	BinTreeNode<T> *
	remove(const T &key) {
		BinTreeNode<T> **node, **parent;
		std::tie(node, parent) = _search(key);
		if (*node) {
			if ((*node)->right()) {
				parent = succ(node).second;
				(*node)->value((*parent)->value());
				if ((*parent)->right()) {
					(*parent)->right()->parent() = (*parent)->parent();
				}
				*parent = (*parent)->right();
				(*parent)->update();
				return *parent;
			} else {
				BinTreeNode<T> *left = (*node)->left();
				delete *node;
				*node = left;
				if (*parent) {
					(*parent)->update();
					return *parent;
				}
			}
		}
		return nullptr;
	}
};

#endif
