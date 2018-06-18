/**
 *  \file   bit_tools.h
 *  \author Jason Fernandez
 *  \date   3/11/2018
 *
 *  https://github.com/jfern2011/util
 */

#ifndef __BIT_TOOLS_H__
#define __BIT_TOOLS_H__

#include <vector>
#include <utility>

namespace Util
{
	/**
	 ******************************************************************
	 *
	 * Count the number of bits set in a word in O(n) time
	 *
	 * @param [in] word An n-bit word
	 *
	 * @return The number of bits set in the word
	 *
	 ******************************************************************
	 */
	template<typename T> inline int bit_count(T word)
	{
		int count = 0;

		for (count = 0; word; count++)
			word &= word - 1;

		return count;
	}

	/**
	 ******************************************************************
	 *
	 * Base case for the recursive build_word(index1,...) function
	 *
	 * @tparam T The C++ word type
	 *
	 * @return 0
	 *
	 ******************************************************************
	 */
	template <typename T> inline T build_word()
	{
		return 0;
	}

	/**
	 ******************************************************************
	 *
	 * Create a word by specifying which bit indexes you wish to set
	 *
	 * @tparam T  The C++ word type
	 *
	 * @param [in] index1  The first bit to set
	 * @param [in] indexes The indexes of any additional bits to set
	 *
	 * @return A word with bits set at specified indexes
	 *
	 ******************************************************************
	 */
	template <typename T, typename T1, typename... T2>
	inline T build_word(T1&& index1, T2&&... indexes)
	{
		T one = 1;
		return (one << index1) |
					build_word<T>(std::forward<T2>(indexes)...);
	}

	/**
	 ******************************************************************
	 *
	 * Clear the specified bit within a word
	 *
	 * @param [in]     bit  The bit to clear
	 * @param [in,out] word An n-bit word
	 *
	 ******************************************************************
	 */
	template<typename T> inline void clear_bit(int bit, T& word)
	{
		word ^= (word & ((T)1 << bit));
	}

	/**
	 ******************************************************************
	 *
	 * Clear the specified bits of a word
	 *
	 * @param [in]     mask Specifies the bits within \a word to clear
	 * @param [in,out] word An n-bit word
	 *
	 ******************************************************************
	 */
	template<typename T> inline void clear_bits(T mask, T& word)
	{
		word ^= (word & mask);
	}

	/**
	 ******************************************************************
	 *
	 * Retrieve a bitmask with only the specified bit set
	 *
	 * @param [in] bit The desired bit, indexed from 0
	 *
	 * @return A power of 2 whose base-2 logarithm = \a bit. If \a bit
	 *         exceeds the size of T (in bits), -1 is returned
	 *
	 ******************************************************************
	 */
	template<typename T> inline T get_bit(size_t bit)
	{
		if (!(bit < 8*sizeof(T))) return (T)~0;
		return ((T)1) << bit;
	}

	/**
	 ******************************************************************
	 *
	 * Get the index of the least significant bit set in O(n) time
	 *
	 * @param [in] word The word to scan
	 *
	 * @return The LSB, or -1 if no bits are set
	 *
	 ******************************************************************
	 */
	template<typename T> inline int get_lsb(T word)
	{
		int bit = 0; T mask = 1;

		while (mask)
		{
			if (mask & word) return bit;
			mask <<= 1; bit += 1;
		}

		return -1;
	}

	/**
	 ******************************************************************
	 *
	 * Get the index of the most significant bit set in O(n) time
	 *
	 * @param [in] word The word to scan
	 *
	 * @return The MSB, or -1 if no bits are set
	 *
	 ******************************************************************
	 */
	template<typename T> inline int get_msb(T word)
	{
		int bit = (8 * sizeof(T) - 1 );
		T mask = ((T)1) << bit;

		while (mask)
		{
			if (mask & word) return bit;
			mask >>= 1; bit -= 1;
		}

		return -1;
	}

	/**
	 ******************************************************************
	 *
	 * Returns the indexes of all bits set in a word
	 *
	 * @param [in]  word    The word to parse
	 * @param [out] indexes A list of bit indexes set
	 *
	 * @return True on success
	 *
	 ******************************************************************
	 */
	template<typename T>
	inline bool get_set_bits(T word, std::vector<int>& indexes)
	{
		indexes.clear();

		while (word)
		{
			int lsb= get_lsb(word);
			indexes.push_back(lsb);

			clear_bit(lsb, word);
		}

		return true;
	}

	/**
	 ******************************************************************
	 *
	 * Set the specified bit in a word
	 *
	 * @param [in]     bit  The desired bit, indexed from 0
	 * @param [in,out] word The word to modify
	 *
	 ******************************************************************
	 */
	template<typename T> inline void set_bit(int bit, T& word)
	{
		word |= ((T)1) << bit;
	}
}

#endif
