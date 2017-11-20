/**
 *  \file   CommandLine.cpp
 *  \author Jason Fernandez
 *  \date   11/18/2017
 *
 *  https://github.com/jfern2011/CommandLine
 */

#include "CommandLine.h"

/**
 * Constructor
 */
CommandLineOptions::CommandLineOptions()
{
}

/**
 * Destructor
 */
CommandLineOptions::~CommandLineOptions()
{
}

/**
 * Determine if a command line option exists, namely, if it has been
 * registered with \ref add()
 *
 * @param [in] _name The option to search for
 *
 * @return True if the option exists
 */
bool CommandLineOptions::exists(const std::string& _name) const
{
    std::string name = Util::trim(Util::to_lower(_name));

    return _options.find(name) !=
        _options.end();
}

/**
 * Print all command line options for the program to standard output
 *
 * @param[in] prog_name Usually the 1st command line argument, which
 *                      is the executable name
 */
void CommandLineOptions::print(const char* prog_name) const
{
    std::printf("usage: %s [options]\n", prog_name);
    std::printf("options:\n\n");

    for (auto iter = _options.begin(), end = _options.end();
         iter != end; ++iter)
    {
        iter->second->print();
    }

    std::fflush(stdout);
}

/**
 * Common code to create a new option
 *
 * @return True on success
 */
bool CommandLineOptions::_add_option(const std::string& name) const
{
    AbortIf ( name.empty(), false );

    auto iter = _options.find(name);

    if (iter != _options.end())
    {
        std::printf("duplicate option '%s'\n",
            name.c_str());
        Abort(false);
    }

    return true;
}

/**
 * Constructor
 *
 * @param[in] options A \ref CommandLineOptions object. All options
 *                    will be matched against the command line
 */
CommandLine::CommandLine(CommandLineOptions& options)
    : _options(options)
{
}

/**
 * Destructor
 */
CommandLine::~CommandLine()
{
}

/**
 * A static function that parses the command line into option, value
 * pairs
 *
 * @param[in] argc     Number of command line arguments
 * @param[in] argv     The arguments themselves
 * @param[out] opt_val A mapping from command line option to value.
 *                     If when parsing the command line the
 *                     value is not found, it is mapped to an empty
 *                     string
 *
 * @return True on success
 */
bool CommandLine::get_opt_val(int argc, char** argv,
            std::map<std::string,std::string>& opt_val)
{
    AbortIf(argc == 0, false);
    opt_val.clear();

    if (argc < 2) return true;

    Util::str_v tokens;

    for (int i = 1; i < argc; i++)
        tokens.push_back(Util::to_lower(Util::trim(argv[i])));

    AbortIf(tokens.size() == 0,
        false);

    /*
     * Make sure the first entry starts with "--":
     */
    AbortIf(tokens[0].size() <= 2 ||
            tokens[0][0] != '-' ||
            tokens[0][1] != '-', false);

    std::string cmdline =
        Util::build_string(tokens, " ");

    for (size_t option_ind = 0; option_ind < cmdline.size();
         option_ind += 2)
    {
        option_ind = cmdline.find("--", option_ind);
        if (option_ind == std::string::npos)
            break;

        /*
         * Make sure a stray '--' isn't found
         */
        AbortIf(option_ind+2 >= cmdline.size() ||
                cmdline[option_ind+2] == ' ',
            false);

        bool is_last_opt = false;

        size_t next_option = cmdline.find("--", option_ind+2);
        if (next_option == std::string::npos)
            is_last_opt = true;

        size_t value_ind = cmdline.find('=', option_ind);

        std::string name, value;

        if (value_ind < next_option)
        {
            /*
             * Make sure a stray '=' isn't found
             */
            const size_t eq_ind = value_ind;
            value_ind =
                cmdline.find_first_not_of( " =", value_ind );

            AbortIfNot(value_ind < next_option,
                false);

            size_t start = option_ind+2;

            name = cmdline.substr(start, eq_ind - start );
            start = eq_ind+1;

            if (is_last_opt)
                value = cmdline.substr(start);
            else
                value =
                    cmdline.substr(start, next_option-start);
        }
        else
        {
            size_t start = option_ind+2;

            if (is_last_opt)
                name = cmdline.substr(start);
            else
                name = cmdline.substr(start,
                    next_option-start);

            value = "";
        }

        opt_val[Util::trim(name)] =
            Util::trim(value);
    }

    return true;
}

/**
 *  Parse the command line, assigning a value to each command
 *  line option. The command line should have the form:
 *
 * <program_name> --option1[=value1] --option2[=value2] ...
 *
 * where items in [] are optional
 *
 * @param[in] argc The total number of command line arguments
 * @param[in] argv The arguments themselves
 *
 * @return True on success
 */
bool CommandLine::parse(int argc, char** argv)
{
    std::map<std::string,std::string>
        cmdline;
    AbortIfNot(get_opt_val( argc, argv, cmdline ), false);

    for (auto iter = cmdline.begin(), end = cmdline.end();
         iter != end; ++iter)
    {
        auto opt_iter =
            _options._options.find(iter->first);

        if (opt_iter == _options._options.end())
        {
            std::printf("unknown option '%s'\n",
                iter->first.c_str());
            Abort(false);
        }

        const std::string& type = opt_iter->second->get_type();
        const std::string& val  =
            Util::to_lower(iter->second);

        bool success = true;

        if (type == "bool")
        {
            bool value;

            /*
             * Note: If val == "", that means that --option was
             *       provided without a value. For the case
             *       of a boolean option, this is understood as
             *       setting its flag
             */
            if (val == "" || val == "true"  || val == "1")
                value = true;
            else if (val == "false" || val == "0")
                value = false;
            else
                success = false;

            if (success)
            {
                success =
                    _options.set<bool>(iter->first, value);
            }
        }
        else if (type == "char")
        {
            char value;
            if (val.size() == 1)
            {
                value = val[0];
                success =
                    _options.set<char>(iter->first, value);
            }
            else
                success = false;
        }
        else if (type == "int16")
        {
            int temp;
            success = Util::str_to_i32(val, 10, temp);

            if (success)
            {
                short value = temp;

                success =
                    _options.set<short>(iter->first, value);
            }
        }
        else if (type == "int32")
        {
            int value;
            success = Util::str_to_i32(val, 10, value);

            if (success)
            {
                success =
                    _options.set< int >(iter->first, value);
            }
        }
        else if (type == "uchar")
        {
            unsigned char value;

            if (val.size() == 1)
            {
                value = val[ 0 ];
                success =
                    _options.set<unsigned char>(iter->first, value);
            }
            else
                success = false;
        }
        else if (type == "uint16")
        {
            int temp;
            success = Util::str_to_i32(val, 10, temp);

            if (success)
            {
                unsigned short value = temp;

                success =
                    _options.set<unsigned short>(iter->first,value);
            }
        }
        else if (type == "uint32")
        {
            int temp;
            success = Util::str_to_i32(val, 10, temp);

            if (success)
            {
                unsigned int value = temp;

                success =
                    _options.set<unsigned int >(iter->first, value);
            }
        }
        else if (type == "float")
        {
            double temp;
            success = Util::str_to_f64(val, temp);

            if (success)
            {
                float value = temp;

                success =
                    _options.set<float >(iter->first, value);
            }
        }
        else if (type == "double")
        {
            double value;
            success = Util::str_to_f64(val, value);

            if (success)
            {
                success =
                    _options.set<double>(iter->first, value);
            }
        }
        else if (type == "string")
        {
            success = _options.set< std::string >
                (iter->first, val);
        }
        else
        {
            std::printf("unsupported type '%s'\n",
                type.c_str());
            Abort(false);
        }

        if (!success)
        {
            std::printf("unable to parse value '%s' for option "
                        "'%s'\n",
                iter->second.c_str(),
                iter->first.c_str());
            Abort(false);
        }
    }

    return true;
}
