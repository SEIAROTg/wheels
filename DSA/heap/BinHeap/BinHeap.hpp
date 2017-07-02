#ifndef HEADER_BINHEAP
#define HEADER_BINHEAP

#include <vector>
#include <iterator>
#include "../Heap/Heap.hpp"

template <class T>
class BinHeap : public Heap<T> {
private:
	std::vector<T> data;

	void percolate_up(unsigned i) {
		const T &t = data[i];
		while (i && data[i] <= t) {
			unsigned parent = (i - 1) >> 1;
			data[i] = data[parent];
			i = parent;
		}
		data[i] = t;
	}

	void percolate_down(unsigned i) {
		const T &t = data[i];
		unsigned left = (i << 1) + 1;
		unsigned right = (i << 1) + 2;
		while (left < data.size()) {
			if (right < data.size() && data[right] > t && data[right] > data[left]) {
				data[i] = data[right];
				i = right;
			} else if (data[left] > t) {
				data[i] = data[left];
				i = left;
			} else {
				break;
			}
			left = (i << 1) + 1;
			right = (i << 1) + 2;
		}
		data[i] = t;
	}

public:
	BinHeap() {}

	template <class InputIt>
	BinHeap(InputIt first, InputIt last) {
		for (InputIt it = first; it < last; ++it) {
			data.push_back(*it);
		}
		for (int i = data.size() - 1; i >= 0; --i) {
			percolate_down(i);
		}
	}

	void insert(const T &value) {
		data.push_back(value);
		percolate_up(data.size() - 1);
	}

	T getMax() {
		return data.at(0);
	}

	void delMax() {
		data[0] = *data.rbegin();
		data.pop_back();
		if (data.size()) {
			percolate_down(0);
		}
	}

	unsigned size() {
		return data.size();
	}
};

#endif
