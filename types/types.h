/**
 *  \file   types.h
 *  \author Jason Fernandez
 *  \date   10/18/2017
 *
 *  https://github.com/jfern2011/types
 */

#ifndef __TYPES__
#define __TYPES__

#include <cstdint>
#include <string>
#include <vector>

namespace types
{
	/** Unsigned 64-bit integer */
	using uint64 = std::uint64_t;

	/** Signed 64-bit integer */
	using int64  = std::int64_t;

	/** Unsigned 32-bit integer */
	using uint32 = std::uint32_t;

	/** Signed 32-bit integer */
	using int32  = std::int32_t;

	/** Unsigned 16-bit integer */
	using uint16 = std::uint16_t;

	/** Signed 16-bit integer */
	using int16  = std::int16_t;

	/** Unsigned 8-bit integer  */
	using uint8  = std::uint8_t;

	/** Signed 8-bit integer */
	using int8   = std::int8_t;

	/** std::vector of unsigned 64-bit integers */
	using uint64_v = std::vector<uint64>;

	/** std::vector of signed 64-bit integers */
	using int64_v  = std::vector<int64>;

	/** std::vector of unsigned 32-bit integers */
	using uint32_v = std::vector<uint32>;

	/** std::vector of signed 32-bit integers */
	using int32_v  = std::vector<int32>;

	/** std::vector of unsigned 16-bit integers */
	using uint16_v = std::vector<uint16>;

	/** std::vector of signed 16-bit integers */
	using int16_v  = std::vector<int16>;

	/** std::vector of unsigned 8-bit integers  */
	using uint8_v  = std::vector<uint8>;

	/** std::vector of signed 8-bit integers */
	using int8_v   = std::vector<int8>;

	/** std::vector of std::strings */
	using str_v = std::vector<std::string>;

	/*
	 * Add more as needed
	 */
}

#endif
