#pragma once

#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

// Basic Types
typedef uint8_t  u8;
typedef int8_t   s8;
typedef uint16_t u16;
typedef int16_t  s16;
typedef uint32_t u32;
typedef int32_t  s32;
typedef uint64_t u64;
typedef int64_t  s64;
typedef float    f32;
typedef double   f64;
typedef u32      rune;

#define KB(x) (u64) ((x) << 10)
#define MB(x) (u64) ((x) << 20)
#define GB(x) (u64) ((x) << 30)

#define Align_Up_Power_2(val, alignment) (((val) + (alignment) - 1) & ~((alignment) - 1))

// Inline placement new to avoid including <new>
inline void *operator new(size_t, void *ptr) noexcept { return ptr; }

//
// Data Structures
//

template <typename T>
struct vector {
	T *m_data;
	size_t m_size;
	size_t m_capacity;

	vector() : m_data(nullptr), m_size(0), m_capacity(0) {}

	~vector() {
		delete[] m_data;
	}

	vector(const vector& other) : m_data(nullptr), m_size(0), m_capacity(0) {
		if (other.m_capacity > 0) {
			m_data = new T[other.m_capacity];
			m_capacity = other.m_capacity;
			m_size = other.m_size;
			for (size_t i = 0; i < m_size; ++i) {
				m_data[i] = other.m_data[i];
			}
		}
	}

	vector(size_t sz, const T& val = T()) : m_data(nullptr), m_size(sz), m_capacity(sz) {
		m_data = new T[sz];
		for (size_t i = 0; i < sz; ++i) {
			m_data[i] = val;
		}
	}

	vector& operator=(const vector& other) {
		if (this != &other) {
			delete[] m_data;
			m_data = nullptr;
			m_size = 0;
			m_capacity = 0;
			if (other.m_capacity > 0) {
				m_data = new T[other.m_capacity];
				m_capacity = other.m_capacity;
				m_size = other.m_size;
				for (size_t i = 0; i < m_size; ++i) {
					m_data[i] = other.m_data[i];
				}
			}
		}
		return *this;
	}

	vector(vector&& other) noexcept : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity) {
		other.m_data = nullptr;
		other.m_size = 0;
		other.m_capacity = 0;
	}

	vector& operator=(vector&& other) noexcept {
		if (this != &other) {
			delete[] m_data;
			m_data = other.m_data;
			m_size = other.m_size;
			m_capacity = other.m_capacity;
			other.m_data = nullptr;
			other.m_size = 0;
			other.m_capacity = 0;
		}
		return *this;
	}

	void reserve(size_t new_cap) {
		if (new_cap <= m_capacity) return;
		T *new_data = new T[new_cap];
		for (size_t i = 0; i < m_size; ++i) {
			new_data[i] = static_cast<T&&>(m_data[i]);
		}
		delete[] m_data;
		m_data = new_data;
		m_capacity = new_cap;
	}

	void push_back(const T& val) {
		if (m_size >= m_capacity) {
			reserve(m_capacity == 0 ? 4 : m_capacity * 2);
		}
		m_data[m_size++] = val;
	}

	void push_back(T&& val) {
		if (m_size >= m_capacity) {
			reserve(m_capacity == 0 ? 4 : m_capacity * 2);
		}
		m_data[m_size++] = static_cast<T&&>(val);
	}

	void pop_back() {
		if (m_size > 0) m_size--;
	}

	void clear() { m_size = 0; }

	size_t size() const { return m_size; }
	bool empty() const { return m_size == 0; }

	T& operator[](size_t index) { return m_data[index]; }
	const T& operator[](size_t index) const { return m_data[index]; }

	T *begin() { return m_data; }
	T *end() { return m_data + m_size; }
	const T *begin() const { return m_data; }
	const T *end() const { return m_data + m_size; }
};

struct string {
	char *m_data;
	size_t m_size;
	size_t m_capacity;

	string() : m_data(nullptr), m_size(0), m_capacity(0) {}

	string(const char *str) {
		m_size = str ? strlen(str) : 0;
		m_capacity = m_size;
		m_data = (char *)malloc(m_size + 1);
		if (str) {
			memcpy(m_data, str, m_size + 1);
		} else {
			if (m_data)
				m_data[0] = '\0';
		}
	}

	string(const char *str, size_t len) {
		m_size = len;
		m_capacity = len;
		m_data = (char *)malloc(len + 1);
		if (str && len > 0) {
			memcpy(m_data, str, len);
		}
		if (m_data)
			m_data[len] = '\0';
	}

	~string() {
		free(m_data);
	}

	string(const string& other) {
		m_size = other.m_size;
		m_capacity = other.m_capacity;
		if (other.m_data) {
			m_data = (char *)malloc(m_capacity + 1);
			if (m_data) {
				memcpy(m_data, other.m_data, m_size + 1);
			}
		} else {
			m_data = nullptr;
		}
	}

	string& operator=(const string& other) {
		if (this != &other) {
			char *new_data = nullptr;
			if (other.m_data) {
				new_data = (char *)malloc(other.m_capacity + 1);
				if (new_data) {
					memcpy(new_data, other.m_data, other.m_size + 1);
				}
			}
			free(m_data);
			m_data = new_data;
			m_size = other.m_size;
			m_capacity = other.m_capacity;
		}
		return *this;
	}

	string(string&& other) noexcept : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity) {
		other.m_data = nullptr;
		other.m_size = 0;
		other.m_capacity = 0;
	}

	string& operator=(string&& other) noexcept {
		if (this != &other) {
			free(m_data);
			m_data = other.m_data;
			m_size = other.m_size;
			m_capacity = other.m_capacity;
			other.m_data = nullptr;
			other.m_size = 0;
			other.m_capacity = 0;
		}
		return *this;
	}

	char *data() const { return m_data; }

	size_t length() const { return m_size; }

	size_t size() const { return m_size; }

	bool is_empty() const { return m_size == 0; }

	void reserve(size_t new_cap) {
		if (new_cap <= m_capacity)
			return;
		char *new_data = (char *)realloc(m_data, new_cap + 1);
		if (new_data) {
			m_data = new_data;
			m_capacity = new_cap;
		}
	}

	void append(const char *str, size_t len) {
		if (!str || len == 0)
			return;
		if (m_size + len > m_capacity) {
			reserve((m_size + len) * 2);
		}
		memcpy(m_data + m_size, str, len);
		m_size += len;
		m_data[m_size] = '\0';
	}

	vector<string> split(const char delimiter) const {
		vector<string> tokens;
		if (!m_data || m_size == 0) {
			return tokens;
		}
		size_t start = 0;
		for (size_t i = 0; i <= m_size; ++i) {
			if (i == m_size || m_data[i] == delimiter) {
				tokens.push_back(string(m_data + start, i - start));
				start = i + 1;
			}
		}
		return tokens;
	}

	char& operator[](std::size_t index) {
			return m_data[index];
	}

	bool operator==(const string& other) const {
		const char *lhs = m_data ? m_data : "";
		const char *rhs = other.m_data ? other.m_data : "";
		return strcmp(lhs, rhs) == 0;
	}

	bool operator!=(const string& other) const {
		const char *lhs = m_data ? m_data : "";
		const char *rhs = other.m_data ? other.m_data : "";
		return strcmp(lhs, rhs) != 0;
	}

	friend string operator+(const string& lhs, const string& rhs) {
		string result;
		size_t total_len = lhs.length() + rhs.length();
		result.reserve(total_len);
		if (lhs.data()) {
			result.append(lhs.data(), lhs.length());
		}
		if (rhs.data()) {
			result.append(rhs.data(), rhs.length());
		}
		return result;
	}
};
