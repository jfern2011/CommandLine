# abort
Macros for tracing run-time errors

Abort, AbortIf, and AbortIfNot can be used to print a stack trace
leading up to the point at which the last error occured in your
program. AbortIf and AbortIfNot each have two versions, one that takes
two inputs and one that takes three or more. Here's a contrived
example:

	#include <unistd.h>
     
	#include "abort.h"
     
	int func1()
	{
		int fd = -1;
        
		/*
		 * Some code that tries to close() a file descriptor:
		 */
		int ret = ::close(fd);
		
		/*
		 * This says that if close() returned with an error, then
		 * return -1. The first argument is a logical expression and
		 * the 2nd is the return value. Note that this also calls
		 * perror() if errno is nonzero
		 */
		AbortIf(ret != 0, -1);
        
		return 0;
	}
     
	bool func2()
	{
		/*
		 * This says that if func1() did not return 0 (to indicate
		 * success), then return false. Here we use an optional 3rd
		 * argument to print a custom message
		 */
		AbortIf(func1() != 0, false, "func1() failed :(");
        
		return true;
	}
     
	bool func3()
	{
		/*
		 * This says that if func2() returns false (an error), then
		 * return false. Note that we can also format our error
		 * message just like printf():
		 */
		AbortIfNot(func2(), false, "func2() failed. %s", "[more details]");
        
		return true;
	}
     
	int main()
	{
		AbortIfNot(func3(), -1);
        
		return 0;
	}

Abort is like AbortIf and AbortIfNot except that it does not take a
logical expression but rather always returns from the calling function.
Its behavior is identical to AbortIf and AbortIfNot where the logical
is always true and false, respectively.

## Usage

Simply include abort.h in your project.

## Contact

Feel free to email me with bug reports or suggestions:
jfernandez3@gatech.edu