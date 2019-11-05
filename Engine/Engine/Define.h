#ifndef DEFINE_H
#define DEFINE_H

template<class Type>
struct ArrayDeleter {
	void operator() (T *ptr) {
		delete[] ptr;
	}
};
#endif