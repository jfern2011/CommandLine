/**
 *  \file   abort.h
 *  \author Jason Fernandez
 *  \date   10/18/2017
 *
 *  https://github.com/jfern2011/abort
 */

#ifndef __ABORT_H__
#define __ABORT_H__

#include <cerrno>
#include <cstdio>

#ifndef DOXYGEN_SKIP

#define AbortIf_2(cond, ret)                           \
{                                                      \
	if (cond)                                          \
	{                                                  \
		char errMsg[256];                              \
                                                       \
		std::sprintf(errMsg, "[abort] %s in %s:%d",    \
			         __PRETTY_FUNCTION__,              \
			         __FILE__, __LINE__ );             \
                                                       \
		if (errno)                                     \
		{                                              \
			std::perror(errMsg); std::fflush(stderr);  \
			errno = 0;                                 \
		}                                              \
		else                                           \
		{                                              \
			std::printf("%s\n", errMsg);               \
			std::fflush(stdout);                       \
		}                                              \
		return (ret);                                  \
	}                                                  \
}

#define AbortIf_3(cond, ret, msg, ...)                         \
{                                                              \
	if (cond)                                                  \
	{                                                          \
		char errMsg[256], fmt_msg[256];                        \
		std::snprintf(fmt_msg, 256, msg, ##__VA_ARGS__);       \
                                                               \
		std::snprintf(errMsg, 256, "[abort] %s in %s:%d: %s",  \
			         __PRETTY_FUNCTION__,                      \
			         __FILE__, __LINE__,                       \
			         fmt_msg);                                 \
                                                               \
		                                                       \
		std::printf( "%s\n", errMsg ); std::fflush( stdout );  \
		return (ret);                                          \
	}                                                          \
}

#define AbortIfNot_2(cond, ret)                \
	AbortIf_2(!(cond), ret)

#define AbortIfNot_3(cond, ret, msg, ...)      \
	AbortIf_3(!(cond), ret, msg, ##__VA_ARGS__)


#define cat(a,b) a ## b
#define _select(name, nargin)  cat( name ## _, nargin )

#define get_nargin(_0 , _1,  _2 , _3 , _4 , _5 , _6 , _7 , \
				   _8 , _9,  _10, _11, _12, _13, _14, _15, \
				   _16, _17, _18, _19, _20, _21, _22, _23, \
				   _24, _25, _26, _27, _28, _29, _30, _31, \
				   _32, _33, _34, _35, _36, _37, _38, _39, \
				   _40, _41, _42, _43, _44, _45, _46, _47, \
				   _48, _49, _50, _51, _52, _53, _54, _55, \
				   _56, _57, _58, _59, _60, _61, _62, _63, \
				   nargin, ...) nargin

#define va_size(dummy, ...) get_nargin(0, ##__VA_ARGS__,    \
									3, 3, 3, 3, 3, 3, 3, 3, \
									3, 3, 3, 3, 3, 3, 3, 3, \
									3, 3, 3, 3, 3, 3, 3, 3, \
									3, 3, 3, 3, 3, 3, 3, 3, \
									3, 3, 3, 3, 3, 3, 3, 3, \
									3, 3, 3, 3, 3, 3, 3, 3, \
									3, 3, 3, 3, 3, 3, 3, 3, \
									3, 3, 3, 3, 3, 3, 3, 2)

#endif

/**
 * @def AbortIf(cond, ret, ...)
 *
 * Triggers an abort in the event that the specified condition \a cond
 * is true. This will cause the currently executing function to exit
 * with the return value \a ret. Any additional arguments will be used
 * to construct an error message
 */
#define AbortIf(cond, ret, ...) \
	_select(AbortIf, \
		va_size(0, ##__VA_ARGS__))(cond, ret, ##__VA_ARGS__)

/**
 * @def Abort(ret, ...)
 *
 * Triggers an unconditional abort.  This will cause the currently
 * executing function to exit with the return value \a ret. Any
 * additional arguments (optional) will be used to create an error
 * message
 */
#define Abort(ret, ...) \
	_select(AbortIf, \
		va_size(0, ##__VA_ARGS__))(true, ret, ##__VA_ARGS__)

/**
 * @def AbortIfNot(cond, ret, ...)
 *
 * Triggers an abort in the event that the specified condition \a cond
 * is false. This will cause the currently executing function to exit
 * with the return value \a ret. Any additional arguments will be used
 * to construct an error message
 */
#define AbortIfNot(cond, ret, ...) \
	_select(AbortIfNot, \
		va_size(0, ##__VA_ARGS__))(cond, ret, ##__VA_ARGS__)


#endif
