/**
 *  \file   util.h
 *  \author Jason Fernandez
 *  \date   10/18/2017
 *
 *  https://github.com/jfern2011/util
 */

#ifndef __UTIL__
#define __UTIL__

#include <sys/stat.h>
#include <fstream>

#include "str_util.h"

namespace Util
{
	/**
     ******************************************************************
     *
     * Get the size of a regular file. See stat(2) man page for details
     *
     * @param[in] file The path to the file
     *
     * @return The size in bytes, or -1 on error (such as if \a file is
     *         a dirctory)
     *
     ******************************************************************
     */
	inline long long file_size(const std::string& file)
	{
		struct stat st;
		if (stat(file.c_str(), &st) == -1 || S_ISDIR(st.st_mode))
			return -1;
		else
			return st.st_size;
	}

	/**
     ******************************************************************
     *
     * Determine if the given name is a directory
     *
     * @param[in] name The name to check
     *
     * @return True if \a name is a directory
     *
     ******************************************************************
     */
	inline bool is_dir(const std::string& name)
	{
		struct stat st;
		if (stat(name.c_str(), &st) < 0)
			return false;
		else
			return S_ISDIR( st.st_mode );
	}

	/**
     ******************************************************************
     *
     * Determine if the given name is a regular file
     *
     * @param[in] name The name to check
     *
     * @return True if \a name is a regular file
     *
     ******************************************************************
     */
	inline bool is_file(const std::string& name)
	{
		struct stat st;
		if (stat(name.c_str(), &st) < 0)
			return false;
		else
			return S_ISREG( st.st_mode );
	}

	/**
	 ******************************************************************
	 *
	 * Read all lines in a file, storing them in a vector of strings.
	 * Blank (whitespace only) lines are discarded
	 *
	 * @param[in]  filename The file to read
	 * @param[out] lines    All (non-blank) lines within \a filename
	 *
	 * @return True on success
	 *
	 ******************************************************************
	 */
	inline bool readlines(const std::string& filename,
						  std::vector<std::string>& lines)
	{
		lines.clear();

		std::ifstream infile(filename);
		if (!infile.is_open()) return false;

		std::string line;
		while (std::getline(infile, line))
		{
			if (trim(line).size() > 0)
					lines.push_back( line );
		}

		return true;
	}
}

#endif
