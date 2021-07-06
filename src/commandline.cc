/**
 *  \file   commandline.cc
 *  \author Jason Fernandez
 *  \date   11/18/2017
 *
 *  https://github.com/jfern2011/CommandLine
 */

#include "commandline/commandline.h"

namespace jfern {
namespace internal {
constexpr char TypeToName<bool>::value[];
constexpr char TypeToName<std::int8_t>::value[];
constexpr char TypeToName<std::int16_t>::value[];
constexpr char TypeToName<std::int32_t>::value[];
constexpr char TypeToName<std::int64_t>::value[];
constexpr char TypeToName<std::uint8_t>::value[];
constexpr char TypeToName<std::uint16_t>::value[];
constexpr char TypeToName<std::uint32_t>::value[];
constexpr char TypeToName<std::uint64_t>::value[];
constexpr char TypeToName<float>::value[];
constexpr char TypeToName<double>::value[];
constexpr char TypeToName<std::string>::value[];

}  // namespace internal

/**
 * A static function that parses the command line into option, value pairs
 *
 * @param[in] argc     Number of command line arguments
 * @param[in] argv     The arguments themselves
 * @param[out] opt_val A mapping from command line option to value
 *
 * @return True on success
 */
bool CommandLine::GetOptVal(int argc, char** argv,
                            std::map<std::string, std::string>& opt_val) {
    if (argc <= 0) return false;
    opt_val.clear();

    if (argc <= 1) return true;

    std::vector<std::string> tokens;

    for (int i = 1; i < argc; i++)
        tokens.push_back(superstring(argv[i]).trim());

    /*
     * Make sure the first entry starts with "--":
     */
    if (tokens[0].size() <= 2 || tokens[0][0] != '-' || tokens[0][1] != '-') {
        return false;
    }

    const std::string cmdline =
        superstring::build(" ", tokens.begin(), tokens.end());

    std::size_t start = 0, equal;
    std::string subline = cmdline.substr(start, std::string::npos);
    while (NextPair(subline, &start, &equal)) {
        start += 2;
        const std::string name =
            subline.substr(start, equal-start);

        /*
         * Make sure the option name is not pure whitespace
         */
        if (superstring(name).trim().size() == 0)
            return false;

        subline = subline.substr(equal + 1, std::string::npos);

        const std::string value =
            NextPair(subline, &start, &equal) ? subline.substr(0, start) :
                                                subline;

        /*
         * Make sure the option value is not pure whitespace
         */
        if (superstring(value).trim().size() == 0)
            return false;

        opt_val[name] = value;
    }

    return true;
}
#if 0
/**
 *  Parse the command line, assigning a value to each command
 *  line option. The command line should have the form:
 *
 * @verbatim
   <program_name> --option1=value1 --option2=value2 ...
   @endverbatim 
 *
 * Note that for boolean options, it is sufficient to write
 * @verbatim --option @endverbatim
 * without a value. It is understood that value equals true
 * in this case
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

        AbortIf(opt_iter== _options._options.end(), false,
            "unknown option '%s'", iter->first.c_str());

        const std::string& type =
            opt_iter->second->get_type();
        const std::string& val  =
            Util::to_lower(iter->second);

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
            {
                Abort(false);
            }

            AbortIfNot(_options.set(iter->first, value),
                false);
        }
        else if (type == "char")
        {
            char value;
            if (val.size() == 1)
            {
                value = iter->second[0];
                AbortIfNot(_options.set(iter->first,
                    value), false);
            }
            else
            {
                Abort(false);
            }
        }
        else if (type == "int16")
        {
            std::int16_t value;
            AbortIfNot(Util::from_string(val, value), false);

            AbortIfNot(_options.set(iter->first, value),
                false);
        }
        else if (type == "int32")
        {
            std::int32_t value;
            AbortIfNot(Util::from_string(val, value), false);

            AbortIfNot(_options.set(iter->first, value),
                false);
        }
        else if (type == "uchar")
        {
            unsigned char value;

            if (val.size() == 1)
            {
                value = iter->second[ 0 ];
                AbortIfNot(_options.set(iter->first,value),
                    false);
            }
            else
            {
                Abort(false);
            }
        }
        else if (type == "uint16")
        {
            std::uint16_t value;
            AbortIfNot(Util::from_string(val, value), false);

            AbortIfNot(_options.set(iter->first, value),
                false);
        }
        else if (type == "uint32")
        {
            std::uint32_t value;
            AbortIfNot(Util::from_string(val, value), false);

            AbortIfNot(_options.set(iter->first, value),
                false);
        }
        else if (type == "float")
        {
            float value;
            AbortIfNot(Util::from_string(val, value), false);

            AbortIfNot(_options.set(iter->first, value),
                false);
        }
        else if (type == "double")
        {
            double value;
            AbortIfNot(Util::from_string(val, value), false);

            AbortIfNot(_options.set(iter->first, value),
                false);
        }
        else if (type == "string")
        {
            AbortIfNot(_options.set(iter->first,
                iter->second), false);
        }
        else
        {
            Abort(false, "unsupported type '%s'",
                type.c_str());
        }
    }

    return true;
}
#endif

/**
 * Helper method that searches for the next pair of "--" and "=" substrings
 * 
 * @param[in]  str     The search string
 * @param[out] p_start Index of the next "--"
 * @param[out] p_equal Index of the next "="
 * 
 * @return True if a pair was found, false otherwise
 */
bool CommandLine::NextPair(const std::string& str,
                           std::size_t* p_start,
                           std::size_t* p_equal) {
    std::size_t start = str.find("--");
    if (start == std::string::npos) return false;

    std::size_t equal = str.find("=", start);
    if (equal == std::string::npos) return false;

    std::size_t next = start;
    while (next < equal && next != std::string::npos) {
        start = next;
        next = str.find("--", start + 2);
    }

    *p_start = start;
    *p_equal = equal;

    return true;
}
}  // namespace jfern
