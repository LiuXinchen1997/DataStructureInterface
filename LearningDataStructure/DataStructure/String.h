/* declaration of String */

#pragma once
#include <iostream>
#include "../General/Base.h"
#include "../DataStructure/Vector.h"

namespace lxc {
	template <class T> class Vector;

	class String
	{
	protected:
		SizeType _size;
		SizeType _capacity;
		char* _elements;

		void _copy_from(const char* cstr, SizeType low, SizeType high);
		void _copy_from(SizeType n, char c);
		void _expand();
		void _shrink();

	public:
		// static members
		static const SizeType DEFAULT_CAPACITY;
		static const SizeType NPOS;
		static const double SHRINK_RATIO;
		static bool show_full_data; // related to operator<<
		static SizeType _cstr_len(const char* cstr);
		static void _cstr_copy(char* dest, const char* source, SizeType low = 0, SizeType high = NPOS);
		static int _cstr_comp(const char* cstr1, const char* cstr2);
		static bool _cstr_equal(const char* cstr1, const char* cstr2);

		// type conversions
		static String to_String(int val);
		static String to_String(double val);
		// template <class T>	static String to_String(Vector<T> val);

		// constructor
		explicit String();
		String(const char* cstr, SizeType low = 0, SizeType high = NPOS);
		String(const String& str, SizeType low = 0, SizeType high = NPOS);
		String(SizeType n, char c);
		String& operator= (const String& str);
		String& operator= (const char* cstr);
		String& operator= (char c);

		// deconstructor
		~String() { delete[] this->_elements; }

		// getter
		const char* elements() const { return this->_elements; }
		SizeType size() const { return this->_size; }
		SizeType length() const { return this->size(); }
		SizeType capacity() const { return this->_capacity; }
		SizeType max_size() const { return this->NPOS; }
		const String to_str() const;
		const char* to_cstr() const { return this->to_str().elements(); }


		// capacity
		void resize(SizeType new_size, char c = char());
		void reserve(SizeType new_capacity);
		void clear();
		bool empty() const { return (this->_size == 0); };

		// element access
		char& operator[] (SizeType pos);
		const char at(SizeType pos) { return this->operator[](pos); };
		const char get(SizeType pos) { return this->at(pos); }

		// modifiers
		String& operator+= (const String& str);
		String& operator+= (const char* cstr);
		String& operator+= (char c);
		String& append(const char* cstr, SizeType low, SizeType high);
		String& append(const char* cstr, SizeType n);
		String& append(const char* cstr);
		String& append(const String& str, SizeType low, SizeType high);
		String& append(const String& str);
		String& append(SizeType n, char c);
		void push_back(char c);
		String& assign(const char* cstr, SizeType low, SizeType high);
		String& assign(const char* cstr, SizeType n);
		String& assign(const char* cstr);
		String& assign(SizeType n, char c);
		String& assign(const String& str, SizeType low, SizeType high);
		String& assign(const String& str);
		String& insert(SizeType pos, const char* cstr, SizeType low, SizeType high);
		String& insert(SizeType pos, const char* cstr);
		String& insert(SizeType pos, SizeType n, char c);
		String& insert(SizeType pos, const String& str);
		String& insert(SizeType pos, const String& str, SizeType low, SizeType high);
		String& erase(SizeType low = 0, SizeType high = NPOS);
		String& replace(SizeType low1, SizeType high1, const char* cstr, SizeType low2, SizeType high2);
		String& replace(SizeType low, SizeType high, const char* cstr);
		String& replace(SizeType low, SizeType high, const char* cstr, SizeType n);
		String& replace(SizeType low, SizeType high, SizeType n, char c);
		String& replace(SizeType low, SizeType high, const String& str);
		String& replace(SizeType low1, SizeType high1, const String& str, SizeType low2, SizeType high2);
		SizeType copy(char* cstr, SizeType low = 0, SizeType high = NPOS); // copy elements from this to cstr
		void swap(String& str);

		// operations
		bool operator<(const char* cstr) const { return _cstr_comp(_elements, cstr) < 0; }
		bool operator<(const String str) const { return this->operator<(str.elements()); }
		bool operator<=(const char* cstr) const { return _cstr_comp(_elements, cstr) <= 0; }
		bool operator<=(const String str) const { return this->operator<=(str.elements()); }
		bool operator>(const char* cstr) const { return _cstr_comp(_elements, cstr) > 0; }
		bool operator>(const String str) const { return this->operator>(str.elements()); }
		bool operator>=(const char* cstr) const { return _cstr_comp(_elements, cstr) >= 0; }
		bool operator>=(const String str) const { return this->operator>=(str.elements()); }
		bool operator==(const char* cstr) const { return this->equals(cstr); }
		bool operator==(const String str) const { return this->equals(str); }
		bool operator!=(const String str) const { return !this->equals(str); }
		bool operator!=(const char* cstr) const { return !this->equals(cstr); }
		String operator+(const String str) const;
		String operator+(const char* cstr) const;
		String operator+(const char c) const;

		bool equals(const char* cstr) const;
		bool equals(const String str) const;
		int compare(SizeType low1, SizeType high1, const String& str, SizeType low2, SizeType high2) const;
		int compare(SizeType low1, SizeType high1, const char* cstr, SizeType low2, SizeType high2) const;
		int compare(const String& str) const;
		int compare(const char* cstr) const;
		int compare(SizeType low, SizeType high, const String& str) const;
		int compare(SizeType low, SizeType high, const char* cstr) const;
		int compare(SizeType low, SizeType high, const char* cstr, SizeType n) const;
		SizeType find(const String& str, SizeType low = 0, SizeType high = NPOS) const;
		SizeType find(const char* cstr, SizeType low = 0, SizeType high = NPOS) const;
		SizeType find(char ch, SizeType low = 0, SizeType high = NPOS) const;
		SizeType rfind(const String& str, SizeType low = 0, SizeType high = NPOS) const;
		SizeType rfind(const char* cstr, SizeType low = 0, SizeType high = NPOS) const;
		SizeType rfind(char ch, SizeType low = 0, SizeType high = NPOS) const;
		SizeType find_first_of(const String& str, SizeType low = 0, SizeType high = NPOS) const;
		SizeType find_first_of(const char* cstr, SizeType low = 0, SizeType high = NPOS) const;
		SizeType find_first_of(char ch, SizeType low = 0, SizeType high = NPOS) const;
		SizeType find_last_of(const String& str, SizeType low = 0, SizeType high = NPOS) const;
		SizeType find_last_of(const char* cstr, SizeType low = 0, SizeType high = NPOS) const;
		SizeType find_last_of(char ch, SizeType low = 0, SizeType high = NPOS) const;
		SizeType find_first_not_of(const String& str, SizeType low = 0, SizeType high = NPOS) const;
		SizeType find_first_not_of(const char* cstr, SizeType low = 0, SizeType high = NPOS) const;
		SizeType find_first_not_of(char ch, SizeType low = 0, SizeType high = NPOS) const;
		SizeType find_last_not_of(const String& str, SizeType low = 0, SizeType high = NPOS) const;
		SizeType find_last_not_of(const char* cstr, SizeType low = 0, SizeType high = NPOS) const;
		SizeType find_last_not_of(char ch, SizeType low = 0, SizeType high = NPOS) const;
		String substr(SizeType low = 0, SizeType high = NPOS) const;
		Vector<String> split(const String& splitter) const;
		Vector<String> split(const char* splitter) const;
		Vector<String> split(const char splitter) const;
	};

	std::ostream& operator<< (std::ostream& os, const String& str);
	std::istream& operator>> (std::istream& is, String& str);
};
