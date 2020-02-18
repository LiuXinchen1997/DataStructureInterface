/* implementation of Vector */
#pragma once
#include <iostream>

#include "Vector.h"


template <class T>
std::ostream& operator<< (std::ostream& os, const lxc::Vector<T>& v)
{
	os << "Vector {";
	os << "capacity: " <<  v.capacity() << ", ";
	os << "size: " << v.size() << ", ";
	os << "elements: [";
	for (int i = 0; i < v.size(); i++) {
		if (i) { os << ", "; }
		os << v[i];
	}
	os << "]}\n";

	return os;
}


// auxiliary
template <class T>
void lxc::Vector<T>::_copy_from(const T* arr, SizeType low, SizeType high)
{
	// copy from [low, high)
	this->_capacity = (high - low) * 2;
	this->_elements = new T[this->_capacity];
	this->_size = 0;
	while (low < high) {
		this->_elements[_size++] = arr[low++];
	}
}

template <class T>
void lxc::Vector<T>::_expand()
{
	T* new_elements = new T[(this->_capacity <<= 1)];
	for (int i = 0; i < this->_size; i++) { new_elements[i] = this->_elements[i]; }
	delete[] this->_elements;
	this->_elements = new_elements;
}

template <class T>
void lxc::Vector<T>::_shrink()
{
	if (this->_capacity <= DEFAULT_CAPACITY) { return; }

	T* new_elements = new T[(this->_capacity >>= 1)];
	for (int i = 0; i < min_of_2(this->_size, this->_capacity); i++) {
		new_elements[i] = this->_elements[i];
	}
	delete[] this->_elements;
	this->_elements = new_elements;
}


// constructor
template <class T>
lxc::Vector<T>::Vector(SizeType capacity, SizeType s, T ele)
{
	this->_capacity = capacity;
	this->_size = s;
	this->_elements = new T[s];
	for (int i = 0; i < s; i++) {
		this->_elements[i] = ele;
	}
}

template <class T>
lxc::Vector<T>::Vector(const T* arr, SizeType n)
{
	this->_copy_from(arr, 0, n);
}

template <class T>
lxc::Vector<T>::Vector(const T* arr, SizeType low, SizeType high)
{
	this->_copy_from(arr, low, high);
}

template <class T>
lxc::Vector<T>::Vector(const lxc::Vector<T>& v)
{
	this->_copy_from(v._elements, 0, v._size);
}

template <class T>
lxc::Vector<T>::Vector(const lxc::Vector<T>& v, SizeType low, SizeType high)
{
	this->_copy_from(v._elements, low, high);
}


// deconstructor
template <class T>
lxc::Vector<T>::~Vector() { delete[] this->_elements; }


// capacity
template <class T>
SizeType lxc::Vector<T>::size() const { return this->_size; }

template <class T>
SizeType lxc::Vector<T>::capacity() const { return this->_capacity; }

template <class T>
bool lxc::Vector<T>::empty() const { return this->_size == 0; }

template <class T>
void lxc::Vector<T>::resize(SizeType new_size, T ele)
{
	if (new_size <= this->_size) { this->_size = new_size; return; }
	if (new_size <= this->_capacity) {
		for (; this->_size < new_size; ) { this->_elements[this->_size++] = ele; }
		return;
	}

	while (new_size > this->_capacity) { this->_expand(); }
	for (; this->_size < new_size; ) { this->_elements[this->_size++] = ele; }
}

template <class T>
void lxc::Vector<T>::reserve(SizeType new_size)
{
	if (new_size <= this->_size) { return; }
	T* new_elements = new T[new_size];
	for (int i = 0; i < this->_size; i++) {
		new_elements[i] = this->_elements[i];
	}

	delete[] this->_elements;
	this->_elements = new_elements;
	this->_capacity = new_size;
}


// access elements(read only)
template <class T>
bool lxc::Vector<T>::is_ordered() const
{
	for (int i = 0; i < this->_size - 1; i++) {
		if (this->_elements[i] > this->_elements[i + 1]) { return false; }
	}
	return true;
}

template <class T>
T lxc::Vector<T>::get(SizeType r) const { return this->_elements[r]; }

template <class T>
SizeType lxc::Vector<T>::find(const T& ele) const { return this->find(ele, 0, this->_size); }

template <class T>
SizeType lxc::Vector<T>::find(const T& ele, SizeType low, SizeType high) const
{
	for (int i = low; i < high; i++) {
		if (this->get(i) == ele) { return i; }
	}
	return -1; // don't find it.
}

template <class T>
SizeType lxc::Vector<T>::search(const T& ele) const
{
	return this->search(ele, 0, this->_size);
}

template <class T>
SizeType lxc::Vector<T>::search(const T& ele, SizeType low, SizeType high) const
{
	if (!this->is_ordered()) { throw "lxc::Vector is not ordered."; }
	while (low < high) {
		SizeType mid = (low + high) / 2;
		if (this->get(mid) == ele) { return mid; }
		else if (this->get(mid) < ele) { low = mid + 1; }
		else { high = mid; }
	}

	return -1;
}

template <class T>
bool lxc::Vector<T>::operator==(const lxc::Vector<T>& v) const
{
	if (this->_size != v._size) { return false; }
	for (int i = 0; i < this->_size; i++) {
		if (this->get(i) != v.get(i)) { return false; }
	}

	return true;
}


// writable interface and modifier
template <class T>
T& lxc::Vector<T>::operator[] (SizeType pos) const { return this->_elements[pos]; }

template <class T>
lxc::Vector<T>& lxc::Vector<T>::operator=(const lxc::Vector<T>& v)
{
	if (this->_elements) delete[] this->_elements;
	this->_copy_from(v._elements, 0, v._size);
	return *this;
}

template <class T>
T lxc::Vector<T>::remove(SizeType pos)
{
	T ele = this->get(pos);
	this->remove(pos, pos + 1);

	return ele;
}

template <class T>
SizeType lxc::Vector<T>::remove(SizeType low, SizeType high)
{
	T* new_elements = new T[this->_capacity];
	for (int i = 0, j = 0; i < this->_size; i++) {
		if (low <= i && i < high) { continue; }
		new_elements[j] = this->_elements[i];
		j++;
	}

	delete[] this->_elements;
	this->_elements = new_elements;
	this->_size -= (high - low);

	if (double(this->_size) / this->_capacity < 0.25) { this->_shrink(); }
	return (high - low);
}

template <class T>
T lxc::Vector<T>::remove_back()
{
	T ele = this->_elements[this->_size - 1];
	this->_size--;

	if (double(this->_size) / this->_capacity < 0.25) { this->_shrink(); }
	return ele;
}

template <class T>
T lxc::Vector<T>::remove_front()
{
	T ele = this->_elements[0];
	T* new_elements = new T[this->_capacity];
	for (int i = 1, j = 0; i < this->_size; i++, j++) {
		new_elements[j] = this->_elements[i];
	}
	delete[] this->_elements;
	this->_elements = new_elements;
	this->_size--;

	if (double(this->_size) / this->_capacity < 0.25) { this->_shrink(); }
	return ele;
}

template <class T>
void lxc::Vector<T>::insert(SizeType pos, const T& ele)
{
	if (this->_size == this->_capacity) { this->_expand(); }

	T* new_elements = new T[this->_capacity];
	for (int i = 0, j = 0; i < this->_size;) {
		if (j == pos) {
			new_elements[j] = ele;
			j++;
			continue;
		}

		new_elements[j] = this->_elements[i];
		i++; j++;
	}

	delete[] this->_elements;
	this->_elements = new_elements;
	this->_size++;
}

template <class T>
void lxc::Vector<T>::insert(SizeType pos, const T* arr, SizeType low, SizeType high)
{
	int prev_size = this->_size;
	while (this->_capacity < this->_size + (high - low)) { _expand(); }

	T* new_elements = new T[this->_capacity];
	for (int i = 0, j = 0; i < prev_size;) {
		if (pos <= j && j < pos + high - low) {
			new_elements[j] = arr[low + j - pos];
			j++;
			continue;
		}
		new_elements[j] = this->_elements[i];
		i++; j++;
	}

	delete[] this->_elements;
	this->_elements = new_elements;
	this->_size += (high - low);
}

template <class T>
void lxc::Vector<T>::insert(SizeType pos, const lxc::Vector<T>& v, SizeType low, SizeType high)
{
	this->insert(pos, v._elements, low, high);
}

template <class T>
void lxc::Vector<T>::insert(SizeType pos, const lxc::Vector<T>& v, SizeType n)
{
	this->insert(pos, v, 0, n);
}

template <class T>
void lxc::Vector<T>::insert(SizeType pos, const lxc::Vector<T>& v)
{
	this->insert(pos, v, 0, v._size);
}

template <class T>
void lxc::Vector<T>::insert_back(const T& ele)
{
	if (this->_size == this->_capacity) { this->_expand(); }

	this->_elements[this->_size] = ele;
	this->_size++;
}

template <class T>
void lxc::Vector<T>::insert_front(const T& ele)
{
	if (this->_size == this->_capacity) { this->_expand(); }

	T prev = this->_elements[0];
	this->_elements[0] = ele;
	for (int i = 1; i <= this->_size; i++) {
		T tmp = prev; prev = this->_elements[i]; this->_elements[i] = tmp;
	}

	this->_size++;
}


// sort/unsort/uniquify
template <class T>
void lxc::Vector<T>::_swap(T& ele1, T& ele2) { T tmp = ele1; ele1 = ele2; ele2 = tmp; }

template <class T>
void lxc::Vector<T>::_bubble_sort(SizeType low, SizeType high, bool(*comp)(T&, T&))
{
	for (int i = 0; i < high - low; i++) {
		for (int j = low; j < high - i - 1; j++) {
			if (!comp(this->_elements[j], this->_elements[j + 1])) {
				T tmp = this->_elements[j];
				this->_elements[j] = this->_elements[j + 1];
				this->_elements[j + 1] = tmp;
			}
		}
	}
}

template <class T>
void lxc::Vector<T>::_insert_sort(SizeType low, SizeType high, bool(*comp)(T&, T&))
{
	for (int i = low; i < high; i++) {
		T tmp = this->_elements[i];
		int j = i - 1;
		while (j >= low) {
			if (comp(tmp, this->_elements[j])) {
				this->_elements[j + 1] = this->_elements[j];
				j--;
			}
			else {
				break;
			}
		}
		this->_elements[j + 1] = tmp;
	}
}

template <class T>
void lxc::Vector<T>::_select_sort(SizeType low, SizeType high, bool(*comp)(T&, T&))
{
	for (int i = high - 1; i > low; i--) {
		T _max = this->_elements[low];
		SizeType _max_pos = low;
		for (int j = low + 1; j <= i; j++) {
			if (comp(_max, this->_elements[j])) {
				_max = this->_elements[j];
				_max_pos = j;
			}
		}
		T tmp = this->_elements[_max_pos];
		this->_elements[_max_pos] = this->_elements[i];
		this->_elements[i] = tmp;
	}
}

template <class T>
void lxc::Vector<T>::_max_heapify(SizeType low, SizeType high, SizeType i, bool(*comp)(T&, T&))
{
	SizeType lc = (i - low) * 2 + 1 + low;
	SizeType rc = (i - low) * 2 + 2 + low;
	SizeType _max_pos = i;
	if (lc < high && comp(this->_elements[_max_pos], this->_elements[lc])) {
		_max_pos = lc;
	}
	if (rc < high && comp(this->_elements[_max_pos], this->_elements[rc])) {
		_max_pos = rc;
	}

	if (_max_pos != i) {
		this->_swap(this->_elements[_max_pos], this->_elements[i]);
		this->_max_heapify(low, high, _max_pos, comp);
	}
}

template <class T>
void lxc::Vector<T>::_build_max_heap(SizeType low, SizeType high, bool(*comp)(T&, T&))
{
	for (int i = low + (low + high) / 2 + 1; i >= low; i--) {
		this->_max_heapify(low, high, i, comp);
	}
}

template <class T>
void lxc::Vector<T>::_heap_sort(SizeType low, SizeType high, bool(*comp)(T&, T&))
{
	this->_build_max_heap(low, high, comp);
	for (int i = high - 1; i > low; i--) {
		this->_swap(this->_elements[low], this->_elements[i]);
		this->_max_heapify(low, i, low, comp);
	}
}

//void _merge_sort(Rank low, Rank high, bool(*comp)(T&, T&) = common_comp);
//void _quick_sort(Rank low, Rank high, bool(*comp)(T&, T&) = common_comp);

template <class T>
void lxc::Vector<T>::sort(SizeType low, SizeType high, const char* type, bool(*comp)(T&, T&))
{
	if ('b' == type[0]) {
		this->_bubble_sort(low, high, comp);
	}
	else if ('i' == type[0]) {
		this->_insert_sort(low, high, comp);
	}
	else if ('s' == type[0]) {
		this->_select_sort(low, high, comp);
	}
	else if ('h' == type[0]) {
		this->_heap_sort(low, high, comp);
	}
}


// traverse
template <class T>
void lxc::Vector<T>::traverse(void(*visit)(T&))
{
	for (int i = 0; i < this->_size; i++) { visit(this->_elements[i]); }
}

template <class T> template <class VST>
void lxc::Vector<T>::traverse(VST& visit)
{
	for (int i = 0; i < this->_size; i++) { visit(this->_elements[i]); }
}