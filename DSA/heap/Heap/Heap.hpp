#ifndef HEADER_HEAP
#define HEADER_HEAP

#include <iterator>

template <class T>
class Heap {
public:
	virtual void insert(const T &value) = 0;
	virtual T getMax() = 0;
	virtual void delMax() = 0;
	virtual unsigned size() = 0;
};

#endif
