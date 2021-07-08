#pragma once
#include <array>

template<typename T, size_t size>
class StaticRingBuffer
{
public:
	StaticRingBuffer();
	~StaticRingBuffer();

	size_t get_size() const { return m_data.size(); }

	void add(T value) 
	{ 
		m_data[m_head] = value; 
		m_head = (m_head + 1) % size;
		m_tail = (m_tail + 1) % size;
	}

	T& operator[](size_t index) { return m_data[index]; }

	size_t get_head_index() const { return m_head; }
	size_t get_tail_index() const { return m_tail; }
private:
	size_t m_head;
	size_t m_tail;
	std::array<T, size> m_data;
};

template<typename T, size_t size>
inline StaticRingBuffer<T, size>::StaticRingBuffer():m_head(0), m_tail(size - 1), m_data()
{
}

template<typename T, size_t size>
inline StaticRingBuffer<T, size>::~StaticRingBuffer()
{
}
