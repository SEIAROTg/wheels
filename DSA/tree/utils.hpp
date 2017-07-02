#include <stack>
#include <string>
#include <utility>
#include <tuple>
#include "BinTree/BinTree.hpp"
#include "AVL/AVL.hpp"

enum POS {
	ROOT,
	LEFT,
	RIGHT
};

const std::string get_indent(int indent, POS pos) {
	std::string ret = "";
	for (int i = 0; i < indent - 1; ++i) {
		ret += "|     ";
	}
	if (pos == LEFT) {
		ret += "|--->--";
	} else if (pos == RIGHT) {
		ret += "|---<--";
	}
	return ret;
}

template <class T>
void display(BinTree<T> &x) {
	std::stack<
		std::tuple<
			BinTreeNode<T> *,
			int,
			POS
		>
	> to_display;
	if (!x.root()) {
		std::cout << "EMPTY" << std::endl;
	} else {
		to_display.push(std::make_tuple(x.root(), 0, ROOT));
		while (to_display.size()) {
			auto top = to_display.top();
			to_display.pop();
			BinTreeNode<T> *node;
			int indent;
			POS pos;
			std::tie(node, indent, pos) = top;
			std::cout << get_indent(indent, pos) << node->value() << std::endl;
			if (node->right()) {
				to_display.push(std::make_tuple(node->right(), indent + 1, RIGHT));
			}
			if (node->left()) {
				to_display.push(std::make_tuple(node->left(), indent + 1, LEFT));
			}
		}
	}
}

template <class T, class Node>
bool verify_bst(BinTree<T, Node> &tree) {
	std::stack<Node*> to_verify;
	if (tree.root()) {
		to_verify.push(tree.root());
	}
	while (to_verify.size()) {
		Node* node = to_verify.top();
		to_verify.pop();
		if (node->left() && node->left()->value() > node->value()) return false;
		if (node->right() && node->right()->value() < node->value()) return false;
	}
	return true;
}

template <class T>
bool verify_avl(AVL<T> &tree) {
	std::stack<AVLNode<T>*> to_verify;
	if (tree.root()) {
		to_verify.push(tree.root());
	}
	while (to_verify.size()) {
		AVLNode<T>* node = to_verify.top();
		to_verify.pop();
		if (!node->balanced()) return false;
	}
	return true;
}
