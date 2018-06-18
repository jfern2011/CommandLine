/**
 *  \file   traits.h
 *  \author Jason Fernandez
 *  \date   3/11/2018
 *
 *  https://github.com/jfern2011/util
 */

#ifndef __TRAITS_UTIL_H__
#define __TRAITS_UTIL_H__

#include <string>

namespace Util
{
	/**
	 ******************************************************************
	 *
	 * A traits class that determines whether the given type is a C++
	 * boolean
	 *
	 * @tparam The data type
	 *
	 ******************************************************************
	 */
	template <typename T>
	struct is_bool
	{
		/**
		 * Indicates whether or not the data type is a boolean
		 */
	    static const bool value = false;
	};

	/**
	 ******************************************************************
	 *
	 * Boolean specialization of \ref is_bool
	 *
	 ******************************************************************
	 */
	template<>
	struct is_bool<bool>
	{
		/**
		 * Indicates whether or not the data type is a boolean
		 */
	    static const bool value =  true;
	};

	/**
	 ******************************************************************
	 *
	 * A traits class that determines whether the given type is a C++
	 * char
	 *
	 * @tparam The data type
	 *
	 ******************************************************************
	 */
	template <typename T>
	struct is_char
	{
		/**
		 * Indicates whether or not the data type is a char
		 */
	    static const bool value = false;
	};

	/**
	 ******************************************************************
	 *
	 * C++ char specialization of \ref is_char
	 *
	 ******************************************************************
	 */
	template <>
	struct is_char<char>
	{
		/**
		 * Indicates whether or not the data type is a char
		 */
	    static const bool value =  true;
	};

	/**
	 ******************************************************************
	 *
	 * A traits class that determines whether the given type is a
	 * signed 16-bit integer
	 *
	 * @tparam The data type
	 *
	 ******************************************************************
	 */
	template <typename T>
	struct is_int16
	{
		/**
		 * Indicates whether or not the data type is a C++ short
		 */
	    static const bool value = false;
	};

	/**
	 ******************************************************************
	 *
	 * C++ short specialization of \ref is_int16
	 *
	 ******************************************************************
	 */
	template <>
	struct is_int16<short>
	{
		/**
		 * Indicates whether or not the data type is a C++ short
		 */
	    static const bool value =  true;
	};

	/**
	 ******************************************************************
	 *
	 * A traits class that determines whether the given type is a
	 * signed 32-bit integer
	 *
	 * @tparam The data type
	 *
	 ******************************************************************
	 */
	template <typename T>
	struct is_int32
	{
		/**
		 * Indicates whether or not the data type is a C++ int
		 */
	    static const bool value = false;
	};

	/**
	 ******************************************************************
	 *
	 * C++ int specialization of \ref is_int32
	 *
	 ******************************************************************
	 */
	template <>
	struct is_int32<int>
	{
		/**
		 * Indicates whether or not the data type is a C++ int
		 */
	    static const bool value =  true;
	};

	/**
	 ******************************************************************
	 *
	 * A traits class that determines whether the given type is a
	 * signed 64-bit integer
	 *
	 * @tparam The data type
	 *
	 ******************************************************************
	 */
	template <typename T>
	struct is_int64
	{
		/**
		 * Indicates whether or not the data type is a C++ long long
		 */
	    static const bool value = false;
	};

	/**
	 ******************************************************************
	 *
	 * C++ long long specialization of \ref is_int64
	 *
	 ******************************************************************
	 */
	template <>
	struct is_int64<long long>
	{
		/**
		 * Indicates whether or not the data type is a C++ long long
		 */
	    static const bool value =  true;
	};

	/**
	 ******************************************************************
	 *
	 * A traits class that determines whether the given type is a C++
	 * unsigned char
	 *
	 * @tparam The data type
	 *
	 ******************************************************************
	 */
	template <typename T>
	struct is_uchar
	{
		/**
		 * Indicates whether or not the data type is an unsigned char
		 */
	    static const bool value = false;
	};

	/**
	 ******************************************************************
	 *
	 * C++ unsigned char specialization of \ref is_uchar
	 *
	 ******************************************************************
	 */
	template <>
	struct is_uchar<unsigned char>
	{
		/**
		 * Indicates whether or not the data type is an unsigned char
		 */
	    static const bool value =  true;
	};

	/**
	 ******************************************************************
	 *
	 * A traits class that determines whether the given type is an
	 * unsigned 16-bit integer
	 *
	 * @tparam The data type
	 *
	 ******************************************************************
	 */
	template <typename T>
	struct is_uint16
	{
		/**
		 * Indicates whether or not the data type is a C++ unsigned
		 * short
		 */
	    static const bool value = false;
	};

	/**
	 ******************************************************************
	 *
	 * C++ unsigned short specialization of \ref is_uint16
	 *
	 ******************************************************************
	 */
	template <>
	struct is_uint16<unsigned short>
	{
		/**
		 * Indicates whether or not the data type is a C++ unsigned
		 * short
		 */
	    static const bool value =  true;
	};

	/**
	 ******************************************************************
	 *
	 * A traits class that determines whether the given type is an
	 * unsigned 32-bit integer
	 *
	 * @tparam The data type
	 *
	 ******************************************************************
	 */
	template <typename T>
	struct is_uint32
	{
		/**
		 * Indicates whether or not the data type is a C++ unsigned int
		 */
	    static const bool value = false;
	};

	/**
	 ******************************************************************
	 *
	 * C++ unsigned int specialization of \ref is_uint32
	 *
	 ******************************************************************
	 */
	template <>
	struct is_uint32<unsigned int>
	{
		/**
		 * Indicates whether or not the data type is a C++ unsigned int
		 */
	    static const bool value =  true;
	};

	/**
	 ******************************************************************
	 *
	 * A traits class that determines whether the given type is an
	 * unsigned 64-bit integer
	 *
	 * @tparam The data type
	 *
	 ******************************************************************
	 */
	template <typename T>
	struct is_uint64
	{
		/**
		 * Indicates whether or not the data type is a C++ unsigned
		 * long long
		 */
	    static const bool value = false;
	};

	/**
	 ******************************************************************
	 *
	 * C++ unsigned long long specialization of \ref is_uint64
	 *
	 ******************************************************************
	 */
	template <>
	struct is_uint64<unsigned long long>
	{
		/**
		 * Indicates whether or not the data type is a C++ unsigned
		 * long long
		 */
	    static const bool value =  true;
	};

	/**
	 ******************************************************************
	 *
	 * A traits class that determines whether the given type is a C++
	 * float
	 *
	 * @tparam The data type
	 *
	 ******************************************************************
	 */
	template <typename T>
	struct is_float
	{
		/**
		 * Indicates whether or not the data type is a float
		 */
	    static const bool value = false;
	};

	/**
	 ******************************************************************
	 *
	 * C++ float specialization of \ref is_float
	 *
	 ******************************************************************
	 */
	template<>
	struct is_float<float>
	{
		/**
		 * Indicates whether or not the data type is a float
		 */
	    static const bool value =  true;
	};

	/**
	 ******************************************************************
	 *
	 * A traits class that determines whether the given type is a C++
	 * double
	 *
	 * @tparam The data type
	 *
	 ******************************************************************
	 */
	template <typename T>
	struct is_double
	{
		/**
		 * Indicates whether or not the data type is a double
		 */
	    static const bool value = false;
	};

	/**
	 ******************************************************************
	 *
	 * C++ double specialization of \ref is_double
	 *
	 ******************************************************************
	 */
	template<>
	struct is_double<double>
	{
		/**
		 * Indicates whether or not the data type is a double
		 */
	    static const bool value =  true;
	};

	/**
	 ******************************************************************
	 *
	 * A traits class that determines whether the given type is a
	 * std::string
	 *
	 * @tparam The data type
	 *
	 ******************************************************************
	 */
	template <typename T>
	struct is_string
	{
		/**
		 * Indicates whether or not the data type is a std::string
		 */
	    static const bool value = false;
	};

	/**
	 ******************************************************************
	 *
	 * std::string specialization of \ref is_string
	 *
	 ******************************************************************
	 */
	template<>
	struct is_string<std::string>
	{
		/**
		 * Indicates whether or not the data type is a std::string
		 */
	    static const bool value =  true;
	};
}

#endif
