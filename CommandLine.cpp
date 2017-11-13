#include "CommandLine.h"

template<>
struct CommandLineEntries::Entry<void> :
    public CommandLineEntries::entry_base
{
    Entry(const std::string& _name, const std::string& desc)
        : entry_base(),
          description(desc), name(_name)
    {
        type = "void";
    }

    ~Entry()
    {
    }

    std::string description;
    std::string name;
};

CommandLineEntries::CommandLineEntries()
{
}

CommandLineEntries::~CommandLineEntries()
{
}

bool CommandLineEntries::add_entry2(const std::string& _name,
    const std::string& desc)
{
    std::string name =
                    Util::trim(Util::to_lower(_name));

    AbortIfNot(_add_entry(name), false);

    _entries[name].reset(new Entry<void>(name, desc));

    return true;
}

bool CommandLineEntries::_add_entry(const std::string& name) const
{
    AbortIf ( name.empty(), false );

    auto iter = _entries.find(name);

    if (iter != _entries.end())
    {
        std::printf ( "duplicate entry '%s' \n", name.c_str() );
        Abort(false);
    }

    return true;
}

CommandLine::CommandLine(const CommandLineEntries& entries)
    : _entries(entries)
{
}

CommandLine::~CommandLine()
{
}

/**
 * The command line should have the form
 *
 * <program_name> --option1[=value1] --option2[=value2] ...
 *
 * where items in [] are optional
 */
bool CommandLine::parse(int argc, char** argv) const
{
    std::string cmdline;
    return true;
}

/**
 * Note we could use <getopt.h>, but it isn't portable
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
