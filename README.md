# CommandLine

A generic command line interpreter

## CommandLineOptions

This class allows you to create command line options that are settable
via the command line. For example:

    CommandLineOptions options;
     
    /*
     * Add a boolean option that has a default value of false
     */
    options.add<bool>("some_flag", false);
     
    /*
     * Add an option that initializes a floating point value.
     * We also add an optional description
     */
    options.add<float>("float_option", 3.14159,
                       "A floating point option.");
     
    /*
     * Add an option that sets the default value of a std::string
     */
	options.add<std::string>("string_option", "hi there");

## CommandLine

This class parses the command line and matches it against a given
CommandLineOptions. Each option set in the command line will have its
default value overwritten. Continuing the above example,

    CommandLine cmd(options);
     
    /*
     * Forward the arguments of main(int argc, char** argv):
     */
    cmd.parse(argc, argv);
     
    /*
     * Let's say we have the following command line:
     *
     * program_name --some_flag      --float_option=2.71828
     *
     * which (by the way) is equivalent to 
     *
     * program_name --some_flag=true --float_option=2.71828
     *
     * which (by the way) is equivalent to
     *
     * program_name --some_flag=1    --float_option=2.71828
     *
     * we can now pull the values of these options:
     */
     bool b;
     float f;
     std::string s;
     
     cmd.get("some_flag", b);     // some_flag should now be true
     cmd.get("float_option", f);  // float_option should now be 2.71828
     cmd.get("string_option", s); // should still be "hi there"

## Contact

Feel free to email me with bug reports or suggestions:
jfernandez3@gatech.edu
