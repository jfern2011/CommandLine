/**
 *  \file   CommandLine.cpp
 *  \author Jason Fernandez
 *  \date   11/18/2017
 *
 *  https://github.com/jfern2011/CommandLine
 */

#include "CommandLine.h"

/**
 * A specialization of \ref Option where the option has no value
 * associated with it
 */
template<>
struct CommandLineOptions::Option<void> :
    public CommandLineOptions::option_base
{
    /**
     * Constructor
     *
     * @param[in] _name The option name
     * @param[in] _desc A description for this option
     */
    Option( const std::string& _name, const std::string& _desc )
        : option_base(),
          description(_desc), name(_name)
    {
        type = "void";
    }

    /**
     * Destructor
     */
    ~Option()
    {
    }

    /**
     * Print the name, the value, and description of this option
     * to standard output
     */
    void print() const
    {
        std::printf("\t--%s\n", name.c_str());
        std::printf("\t\t%s\n",
                description.c_str());
    }

    /**
     * A description for this option
     */
    std::string description;

    /**
     * The name of this option
     */
    std::string name;
};

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
 * Add a new command line option which is settable via the command
 * line but has no value associated with it
 *
 * @param[in] _name The name of the option to add
 * @param[in] desc  A description for the option
 *
 * @return True on success
 */
bool CommandLineOptions::add_option(const std::string& _name,
    const std::string& desc)
{
    std::string name =
        Util::trim(Util::to_lower(_name));

    AbortIfNot(_add_option(name), false );

    _options[name].reset(new Option<void>(name, desc));

    return true;
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
        std::printf("duplicate option '%s' \n", name.c_str());
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
CommandLine::CommandLine(const CommandLineOptions& options)
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
 *                     If, when parsing the command line, the
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
 * Parse the command line, which should have the form
 *
 * <program_name> --option1[=value1] --option2[=value2] ...
 *
 * where items in [] are optional
 *
 * @param[in] argc Number of command line arguments
 * @param[in] argv The arguments themselves
 *
 * @return True on success
 */
bool CommandLine::parse(int argc, char** argv) const
{
    std::string cmdline;
    return true;
}
