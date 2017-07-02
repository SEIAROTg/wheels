#include "Heap/Heap.hpp"

template <class T>
bool check_ordered(Heap<T> &heap) {
	if (!heap.size()) {
		return true;
	}
	T t = heap.getMax();
	heap.delMax();
	while (heap.size()) {
		T tt = heap.getMax();
		if (tt > t) {
			return false;
		}
		t = tt;
		heap.delMax();
	}
	return true;
}

