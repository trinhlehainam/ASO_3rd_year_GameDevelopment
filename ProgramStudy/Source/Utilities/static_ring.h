#pragma once
#include <array>
#include <cassert>

// use for ring's range-based loop
template<typename T, size_t size>
class ring_iterator
{
public:
	using value_type = T;
	using pointer = T*;
	using reference = T&;

	ring_iterator() :m_ptr(), m_idx(0), m_loopIdx(0), m_isReversed(false) {}
	explicit ring_iterator(pointer ptr, size_t index) :
		m_ptr(ptr), m_idx(index), m_loopIdx(0), m_isReversed(false) 
	{ assert(is_index_valid()); }
	explicit ring_iterator(pointer ptr, size_t index, bool isReversed) :
		m_ptr(ptr), m_idx(index), m_loopIdx(0), m_isReversed(isReversed)
	{ assert(is_index_valid()); }

	void set_index(size_t index) {
		assert(is_index_valid());
		m_idx = index; 
	}

	void enable_reverse() { m_isReversed = true; }
	void disable_reverse() { m_isReversed = false; }

	pointer operator->() const noexcept {
		return m_ptr + m_idx;
	}
	reference operator*() noexcept {
		return *(m_ptr + m_idx);
	}

	// prefix
	ring_iterator& operator++() noexcept {
		if (m_isReversed)
			m_idx = (m_idx + size - 1) % size;
		else
			m_idx = (m_idx + 1) % size;
		++m_loopIdx;
		return *this;
	}

	// postfix
	ring_iterator operator++(int) noexcept {
		ring_iterator tmp = *this;
		++(*this);
		return tmp;
	}

	// is range-based loop condition
	bool operator!=(const ring_iterator& right) const noexcept {
		return is_loop_index_valid();
	}

private:
	bool is_index_valid() { return m_idx < size; }
	bool is_loop_index_valid() const { return m_loopIdx < size; }

private:
	pointer m_ptr;
	size_t m_idx;
	size_t m_loopIdx;
	bool m_isReversed;
};

template<typename T, size_t size>
class static_ring
{
public:
	using iterator = ring_iterator<T, size>;

	static_ring() :m_head(size - 1), m_tail((m_head + 1) % size), m_data{}, m_isReversedLoop(false) {}
	~static_ring() {}

	void insert(const T& value)
	{
		m_head = (m_head + 1) % size;
		m_tail = (m_tail + 1) % size;
		m_data[m_head] = value;
	}

	void set_head_to_tail_loop() { m_isReversedLoop = true; }
	void set_tail_to_head_loop() { m_isReversedLoop = false; }

#pragma region get methods
	T& operator[](size_t index) { return m_data[index]; }

	size_t get_head_index() const { return m_head; }
	size_t get_tail_index() const { return m_tail; }

	size_t get_size() const { return m_data.size(); }

	T* data() { return m_data.data(); }
#pragma endregion

	iterator begin() { 
		return iterator(m_data.data(), m_isReversedLoop ? m_head : m_tail, m_isReversedLoop); 
	}
	iterator end() { return iterator(); }
private:
	size_t m_head;
	size_t m_tail;
	std::array<T, size> m_data;
	bool m_isReversedLoop;
};
