/**
 *  \file   Buffer.h
 *  \author Jason Fernandez
 *  \date   3/17/2018
 *
 *  https://github.com/jfern2011/util
 */

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <cstddef>
#include <cstring>

#include "abort.h"

/**
 **********************************************************************
 *
 * A wrapper for a C++ multi-dimensional array. This implements bounds
 * checking to make it easier to catch buffer overflows at runtime.
 * This class is kept as simple as possible to match the complexity of
 * accessing raw arrays
 *
 * @tparam T  The type of each element
 * @tparam N1 Number of elements along the 1st dimension
 * @tparam N2 Number of elements along higher dimensions
 *
 **********************************************************************
 */
template <typename T, size_t N1, size_t... N2>
class Buffer
{
	static_assert(N1 > 0, "Dimensions must be greater than zero.");

public:

	Buffer()
	{
	}

	~Buffer()
	{
	}

	/**
	 * Indexing operator. This call produces a Buffer whose number of
	 * dimensions is reduced by 1
	 *
	 * For example, if we have a Buffer<int,2,3>, then we'll get
	 * back a Buffer<int,3>
	 *
	 * @param [in] index The index to look up. An out of bounds value
	 *                   produces a warning message
	 *
	 * @return The element at \a index
	 */
	Buffer<T,N2...>& operator[](size_t index)
	{
		AbortIf(N1 <= index, data[index]);
		return data[index];
	}

	/**
	 * Indexing operator. This call produces a Buffer whose number of
	 * dimensions is reduced by 1
	 *
	 * For example, if we have a Buffer<int,2,3>, then we'll get
	 * back a Buffer<int,3>
	 *
	 * @param [in] index The index to look up. An out of bounds value
	 *                   produces a warning message
	 *
	 * @return A *const* reference to the element at \a index
	 */
	const Buffer<T,N2...>& operator[](size_t index) const
	{
		AbortIf(N1 <= index, data[index]);
		return data[index];
	}

private:

	Buffer<T,N2...> data[N1];
};

/**
 **********************************************************************
 *
 * A wrapper for a simple C++ array. Aside from behaving like a normal
 * array, this performs bounds checking to make it easier to catch
 * buffer overflows at runtime. The implementation is kept simple with
 * the goal of matching runtime performance with raw arrays
 *
 * @tparam T The type of each buffer element
 * @tparam N The number of elements
 *
 **********************************************************************
 */
template <typename T, size_t N>
class Buffer<T,N>
{
	static_assert(N > 0, "Buffer must contain at least 1 item.");

public:

	/**
	 * Constructor
	 */
	Buffer()
	{
	}

	/**
	 * Destructor
	 */
	~Buffer()
	{
	}

	/**
	 * Grab the element at the specified index. This is equivalent to
	 * the indexing operator []
	 *
	 * @param [in] index The index to look up. An out of bounds value
	 *                   produces a warning message
	 *
	 * @return The element at \a index
	 */
	T& at(size_t index)
	{
		AbortIf(N <= index, data[index]);
		return data[index];
	}

	/**
	 * Indexing operator. This will return a reference to the element
	 * at the specified index
	 *
	 * @param [in] index The index to look up. An out of bounds value
	 *                   produces a warning message
	 *
	 * @return The element at \a index
	 */
	T& operator[](size_t index)
	{
		AbortIf(N <= index, data[index]);
		return data[index];
	}

	/**
	 * Indexing operator. This will return a *const* reference to the
	 * element at the specified index
	 *
	 * @param [in] index The index to look up. An out of bounds value
	 *                   produces a warning message
	 *
	 * @return The element at \a index
	 */
	const T& operator[](size_t index) const
	{
		AbortIf(N <= index, data[index]);
		return data[index];
	}

	/**
	 * Deference operator
	 *
	 * @return The first element in this Buffer
	 */
	T& operator*()
	{
		return data[0];
	}

	/**
	 * Type conversion to a pointer. Allows passing a Buffer to stuff
	 * like std::memcpy()
	 *
	 * @return A pointer to the data
	 */
	operator T*()
	{
		return data;
	}

	/**
	 * Type conversion to a pointer. Allows passing a Buffer to stuff
	 * like std::memcpy()
	 *
	 * @return A const pointer to the data
	 */
	operator const T*() const
	{
		return data;
	}

	/**
	 * Pointer arithmetic (addition)
	 *
	 * @param[in] offset The offset to add to the start of the buffer
	 *
	 * @return  A pointer to the address at offset \a offset from the
	 *          buffer start, or nullptr on error
	 */
	T* operator+(size_t offset)
	{
		AbortIf(N <= offset, nullptr);
		return data + offset;
	}

	/**
	 * Sets all elements in the buffer to zero
	 */
	void zero()
	{
		std::memset( data, 0, N * sizeof(T) );
	}

private:

	T data[N];
};

#endif
