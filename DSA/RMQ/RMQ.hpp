#ifndef HEADER_RMQ
#define HEADER_RMQ

#include <vector>

template <class T>
class RMQ {
private:
	std::vector<std::vector<T>> f;
public:
	RMQ(const std::vector<T> &a) {
		f.empty();
		std::vector<T> l;
		for (const T &ai : a) {
			l.push_back(ai);
		}
		f.push_back(std::move(l));
		for (int j = 1, j_pow_last = 1; j < a.size(); j += 1, j_pow_last <<= 1) {
			std::vector<T> l;
			for (int i = 0; i < a.size(); ++i) {
				if (i + j_pow_last >= a.size()) {
					l.push_back(f[j-1][i]);
				} else {
					l.push_back(std::max(f[j-1][i], f[j-1][i + j_pow_last]));
				}
			}
			f.push_back(std::move(l));
		}
	}

	const T& query(int i, int j) const {
		int interval = j - i + 1;
		int k = 0, k_pow = 1;
		while (interval >> 1) {
			interval >>= 1;
			k += 1;
			k_pow <<= 1;
		}
		return std::max(f[k][i], f[k][j - k_pow + 1]);
	}
};

#endif