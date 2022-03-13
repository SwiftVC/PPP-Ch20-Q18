/*
The flow:
A change to the vectors allocated memory always goes though reserve.
resize and push_back both call reserve.
resize sets all elements to the default values.
*/

#ifndef cs_vector_
#define cs_vector_
#include "cs_allocator.h"
#include <iostream>
#include <exception>
template <typename T, typename A = cs_allocator<T>>
class cs_vector {
public:
	class iterator;

private:
	int sz;
	T* elem;
	int space;
	A alloc;

public:
	cs_vector() : sz{ 0 }, elem{ nullptr }, space{ 0 } {}
	explicit cs_vector(int s) : sz{ s }, elem{ new T[s] }, space{ s }{		// explicit prevents implicit conversion of int to vect in function arg
		for (int i = 0; i < sz; i++) elem[i] = 0; }
	cs_vector(const cs_vector& vect) : sz{ vect.size() }, elem{ new T[sz] }, space{ sz } {
		for (int i = 0; i < sz; i++) elem[i] = vect.elem[i];
	}

	~cs_vector() { alloc.deallocate(elem, space); }

	int capacity() const { return space; }
	int size() const { return sz; }
	void push_back(T d);
	cs_vector& operator=(cs_vector& vect);		// return type is convention so can chain assignment
	T& operator[](int i) { return elem[i]; }
	
	void reserve(int newalloc);
	void resize(int newsize, T val = T());

	iterator begin() { return iterator(elem, this); }
	iterator end() { return iterator(elem + sz, this); }

	class iterator {
	public:
		cs_vector* _vect;
		int _index;
		
		iterator(T* ptr, cs_vector* vect) : _vect{ vect } { _index = get_index(ptr); }
		iterator& operator++() {
			if (_index + 1  > _vect->size())
				throw std::exception("iterator exceeds allocated memory");
			_index++;
			return *this;
		}
		iterator& operator--() {
			if (_index - 1 <= -1)
				throw std::exception("iterator is below allocated memory");
			_index--;
			return *this;
		}
		iterator operator+(int i) const {
			if (_index + i > _vect->size())
				throw std::exception("iterator exceeds allocated memory");
			return iterator(_vect->elem + i, _vect);
		}
		iterator operator-(int i) const {
			if (_index - i < 0)
				throw std::exception("iterator exceeds allocated memory");
			return iterator(_vect->elem + i - i, _vect);
		}

		T& operator*() { if (_index > _vect->size()) throw std::exception("Cannot dereference end iterator or past the end"); return *(_vect->elem + _index); }

		bool operator!=(iterator _2nd) { if (_2nd._vect != _vect) throw std::exception("Different vector iterators compared"); return _2nd._index != _index; }

		int get_index(T* _ptr) {
			_index = 0;
			auto temp = _vect->elem;
			while (temp != _ptr)
			{
				++temp;
				++_index;
			}
			return _index;
		}

	};
};

template<typename T>
std::istream& operator>>(std::istream& in, cs_vector<T>& vect) {
	T temp;
	in >> temp;
	vect.push_back(temp);
	if (!in)
		throw std::exception("Bad input");

	return in;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, cs_vector<T>& vect) {
	if (vect.size() > 0)
		out << "[ ";
	for (int i = 0; i < vect.size(); i++)
	{
		out << vect[i];
		if (i + 1 < vect.size())
			out << ", ";
	}
	if (vect.size() > 0)
		out << " ]";

	return out;
}

template <typename T, typename A> void cs_vector<typename T, typename A>::reserve(int newalloc) {
	if (newalloc <= space) return;
	T* p = alloc.allocate(newalloc);
	for (int i = 0; i < sz; i++) alloc.construct(&p[i], elem[i]);
	for (int i = 0; i < sz; i++) alloc.destroy(&elem[i]);
	alloc.deallocate(elem, space);
	elem = p;
	space = newalloc;
}

template <typename T, typename A> void cs_vector<typename T, typename A>::resize(int newsize, T val) {
	reserve(newsize);
	for (int i = sz; i < space; i++) alloc.construct(&elem[i], val); // initialize new elements if sz<space
	for (int i = newsize; i < sz; i++) alloc.destroy(&elem[i]);		 // resets old
	sz = newsize;
}

template <typename T, typename A> void cs_vector<typename T, typename A>::push_back(T d) {
	if (space == 0)
		reserve(8);
	else if (sz == space)
		reserve(2 * space);
	alloc.construct(&elem[sz], d);
	++sz;
}

template <typename T, typename A>
cs_vector<typename T, typename A>& cs_vector<T,A>::operator=(cs_vector& vect) {
	// self-assignment protection
	if (this == &vect) return *this;

	// enough space - no changes to allocation
	if (space >= vect.capacity()) {
		for (int i = 0; i < vect.size(); i++)
			elem[i] = vect[i];
		sz = vect.size();
		return *this;
	}

	// else, increase allocation
	delete[] elem;
	sz = 0;
	space = 0;
	resize(vect.size());
	for (int i = 0; i < vect.size(); i++)
		elem[i] = vect[i];

	return *this;
}

#endif