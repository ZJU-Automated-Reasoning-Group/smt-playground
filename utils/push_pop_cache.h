#pragma once

#include <cassert>
#include <cstddef>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template<typename T> class push_pop_vector {
protected:
	std::vector<size_t> cache_stack;
	std::vector<T> cache_vector;

	size_t last_sz = 0;

public:
	virtual ~push_pop_vector() {
	}

	virtual void add(T N) {
		cache_vector.push_back(N);
	}

	void push_back(T N) {
		add(N);
	}

	template<typename VecTy> void add_all(VecTy &Vec) {
		for (unsigned I = 0; I < Vec.size(); I++) {
			push_back(Vec[I]);
		}
	}

	T& operator[](size_t Index) {
		assert(Index < size());
		return cache_vector[Index];
	}

	virtual void push() {
		cache_stack.push_back(cache_vector.size());
	}

	virtual void pop(unsigned N = 1) {
		size_t orig_sz = size();
		size_t target_sz = orig_sz;
		while (N-- > 0) {
			assert(!cache_stack.empty());
			target_sz = cache_stack.back();
			cache_stack.pop_back();
		}

		if (target_sz != orig_sz) {
			assert(target_sz < orig_sz);
			cache_vector.erase(cache_vector.begin() + target_sz,
					cache_vector.end());
		}

		if (last_sz > cache_vector.size()) {
			last_sz = cache_vector.size();
		}
	}

	virtual void reset() {
		cache_stack.clear();
		cache_vector.clear();
		last_sz = 0;
	}

	size_t size() const {
		return cache_vector.size();
	}

	bool empty() const {
		return !size();
	}

	const std::vector<T>& getcache_vector() const {
		return cache_vector;
	}

	/// This function only gets the elements you have
	/// not got using this function.
	///
	/// The first call of this function returns the all
	/// the elements. A second call of the function
	/// with an argument \c false will return elements
	/// newly added. A second call of the function with
	/// an argument \c true will return all the elements.
	std::pair<typename std::vector<T>::iterator,
			typename std::vector<T>::iterator> getcache_vector(bool restart) {
		if (restart) {
			last_sz = 0;
		}

		size_t Start = last_sz;
		size_t End = cache_vector.size();

		last_sz = End;
		return std::make_pair(cache_vector.begin() + Start, cache_vector.end());
	}
};

template<typename T> class push_pop_cache: public push_pop_vector<T> {
protected:
	std::unordered_set<T> cache_set;

public:
	virtual ~push_pop_cache() {
	}

	bool contains(T N) const {
		return cache_set.count(N);
	}

	virtual void add(T N) {
		if (!contains(N)) {
			cache_set.insert(N);
			push_pop_vector<T>::add(N);
		}

		// since we always test inCache(N) before calling this method
		// it guarantees cache_vector.size() == cache_set.size(),
		// which means that element in the vector is unique.
		assert(push_pop_vector<T>::cache_vector.size() == cache_set.size());
	}

	virtual void pop(unsigned N = 1) {
		unsigned M = 0;
		while (M < N) {
			size_t P = push_pop_vector<T>::cache_stack.back();
			push_pop_vector<T>::cache_stack.pop_back();
			while (push_pop_vector<T>::cache_vector.size() > P) {
				auto Nd = push_pop_vector<T>::cache_vector.back();
				push_pop_vector<T>::cache_vector.pop_back();
				cache_set.erase(Nd);
			}

			M++;
		}

		if (push_pop_vector<T>::last_sz > cache_set.size()) {
			push_pop_vector<T>::last_sz = cache_set.size();
		}
	}

	virtual void reset() {
		push_pop_vector<T>::reset();
		cache_set.clear();
	}

	const std::unordered_set<T>& getcache_set() {
		return cache_set;
	}
};
