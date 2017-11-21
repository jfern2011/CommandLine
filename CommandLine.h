/**
 *  \file   CommandLine.h
 *  \author Jason Fernandez
 *  \date   11/18/2017
 *
 *  https://github.com/jfern2011/CommandLine
 */

#ifndef __CMD_LINE_H__
#define __CMD_LINE_H__

#include <memory>
#include <map>
#include <string>

#include "util.h"

#ifndef DOXYGEN_SKIP

template <typename T>
struct is_bool
{
    static const bool value = false;
};

template<>
struct is_bool<bool>
{
    static const bool value = true;
};

template <typename T>
struct is_char
{
    static const bool value = false;
};

template <>
struct is_char<char>
{
    static const bool value = true;
};

template <typename T>
struct is_int16
{
    static const bool value = false;
};

template <>
struct is_int16<short>
{
    static const bool value = true;
};

template <typename T>
struct is_int32
{
    static const bool value = false;
};

template <>
struct is_int32<int>
{
    static const bool value = true;
};

template <typename T>
struct is_int64
{
    static const bool value = false;
};

template <>
struct is_int64<long long>
{
    static const bool value = true;
};

template <typename T>
struct is_uchar
{
    static const bool value = false;
};

template <>
struct is_uchar<unsigned char>
{
    static const bool value = true;
};

template <typename T>
struct is_uint16
{
    static const bool value = false;
};

template <>
struct is_uint16<unsigned short>
{
    static const bool value = true;
};

template <typename T>
struct is_uint32
{
    static const bool value = false;
};

template <>
struct is_uint32<unsigned int>
{
    static const bool value = true;
};

template <typename T>
struct is_uint64
{
    static const bool value = false;
};

template <>
struct is_uint64<unsigned long long>
{
    static const bool value = true;
};

template <typename T>
struct is_float
{
    static const bool value = false;
};

template<>
struct is_float<float>
{
    static const bool value = true;
};

template <typename T>
struct is_double
{
    static const bool value = false;
};

template<>
struct is_double<double>
{
    static const bool value = true;
};

template <typename T>
struct is_string
{
    static const bool value = false;
};

template<>
struct is_string<std::string>
{
    static const bool value = true;
};

#endif

/**
 * Class that builds a table of command line options
 */
class CommandLineOptions
{
    friend class CommandLine;

    /**
     * Base class that allows us to polymorphically get the value of
     * an option via the \a type field
     */
    struct option_base
    {
        virtual ~option_base() {}

        std::string get_type() const { return type; }
        virtual void print() const = 0;

    protected:

        std::string type;
    };

    /**
     * Represents a command line option
     *
     * @tparam T The type of this command line option
     */
    template <typename T>
    struct Option : public option_base
    {
        /**
         * Constructor
         *
         * @param [in] _default The default value to assign this option
         * @param [in] _name    The option name
         * @param [in] _desc    A description of this option
         */
        Option(const T& _default, const std::string& _name,
               const std::string& _desc)
            : option_base(),
              description(_desc), name(_name), value(_default)
        {
            if (is_bool<T>::value)
                type = "bool";
            else if (is_char<T>::value)
                type = "char";
            else if ( is_int16<T>::value)
                type = "int16";
            else if ( is_int32<T>::value)
                type = "int32";
            else if ( is_int64<T>::value)
                type = "int64";
            else if ( is_uchar<T>::value)
                type = "uchar";
            else if (is_uint16<T>::value)
                type = "uint16";
            else if (is_uint32<T>::value)
                type = "uint32";
            else if (is_uint64<T>::value)
                type = "uint64";
            else if ( is_float<T>::value)
                type = "float";
            else if (is_double<T>::value)
                type = "double";
            else if (is_string<T>::value)
                type = "string";
            else
                type = "";
        }

        /**
         * Destructor
         */
        ~Option()
        {
        }

        /**
         * Print the name, the value, and description of this
         * option to standard output
         */
        void print() const
        {
            if (type == "bool")
                std::printf("\t--%s=[%s]\n", name.c_str(), type.c_str());
            else
                std::printf("\t--%s=<%s>\n", name.c_str(), type.c_str());

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

        /**
         * The value of this option
         */
        T value;
    };

    typedef std::unique_ptr< option_base > option_ptr;

public:

    CommandLineOptions();
    ~CommandLineOptions();

    /**
     * Add a new command line option which is settable via the command
     * line
     *
     * @tparam T The type of the option
     *
     * @param[in] _name         The name of the option to add
     * @param[in] default_value The default value to assign the option
     * @param[in] desc          A description of the option
     *
     * @return True on success
     */
    template <typename T>
    bool add(const std::string& _name, const T& default_value,
             const std::string& desc="")
    {
        std::string name =
            Util::trim(Util::to_lower(_name));

        AbortIfNot(_add_option(name), false );

        _options[name].reset(
                    new Option<T>(default_value, name, desc));

        return true;
    }

    bool exists(const std::string& _name) const;

    void print(const char* prog_name) const;

    /**
     * Retrieve the value of a command line option. Either this is
     * its default or the command line value
     *
     * @tparam T The type of the option
     *
     * @param[in]  _name The name of the command line option. This
     *                   is case-insensitive
     * @param[out] value The value of this option
     *
     * @return True on success
     */
    template<typename T>
    bool get(const std::string& _name, T& value) const
    {
        std::string name = Util::trim(Util::to_lower(_name) );
        auto iter = _options.find(name);

        if (iter == _options.end())
        {
            std::printf(" no such command line option '%s'\n",
                name.c_str());
            Abort(false);
        }

        const option_ptr& option =
            iter->second;

        value = dynamic_cast<Option<T>*>
            (option.get())->value;

        return true;
    }

    /**
     *  Set the value of a command line option. This normally gets
     *  done via the command line
     *
     * @tparam T The type of the option
     *
     * @param[in]  _name The name of the command line option. This
     *                   is case-insensitive
     * @param[out] value The value of this option
     *
     * @return True on success
     */
    template <typename T>
    bool set(const std::string& _name, const T& value)
    {
        std::string name = Util::trim(Util::to_lower(_name) );
        auto iter = _options.find(name);

        if (iter == _options.end())
        {
            std::printf(" no such command line option '%s'\n",
                name.c_str());
            Abort(false);
        }

        option_ptr& option = iter->second;

        dynamic_cast<Option<T>*>
            (option.get())->value = value;

        return true;
    }

    CommandLineOptions(const CommandLineOptions& rhs) = delete;
    CommandLineOptions&
            operator= (const CommandLineOptions& rhs) = delete;

private:

    bool _add_option(const std::string& name) const;

    std::map<std::string, option_ptr>
        _options;
};

/**
 * A class that parses the command line and maintains a record of
 * options
 *
 * Note we could use <getopt.h>, but it isn't portable
 */
class CommandLine
{

public:

    CommandLine(CommandLineOptions& options);
    ~CommandLine();

    /**
     * Retrieve the value of a command line option. Either this is
     * its default or the command line value
     *
     * @tparam T The type of the option
     *
     * @param[in]  _name The name of the command line option. This
     *                   is case-insensitive
     * @param[out] value The value of this option
     *
     * @return True on success
     */
    template<typename T>
    bool get(const std::string& _name, T& value) const
    {
        AbortIfNot(_options.get<T>(_name, value),
            false);

        return true;
    }

    bool parse(int argc, char** argv);

    static bool get_opt_val(int argc, char** argv,
            std::map<std::string,std::string>& opt_val);

    CommandLine(const CommandLine& rhs) = delete;
    CommandLine&
      operator=(const CommandLine& rhs) = delete;

private:

    CommandLineOptions&
        _options;
};

#endif // __CMD_LINE_H__
