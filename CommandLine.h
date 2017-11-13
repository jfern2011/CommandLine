#ifndef __CMD_LINE_H__
#define __CMD_LINE_H__

#include <cstdio>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "abort.h"
#include "util.h"

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

/**
 * Class that builds a table of command line entries
 */
class CommandLineEntries
{
    typedef enum
    {
        CHAR,
        INT16,
        INT32,
        INT64,
        UCHAR,
        UINT16,
        UINT32,
        UINT64,
        FLOAT,
        DOUBLE,
        STRING

    } type_id;

    /**
     * Base class that allows us to polymorphically get the value of
     * an entry via the \a type field
     */
    struct entry_base
    {
        virtual ~entry_base() {}

    protected:

        std::string type;
    };

    /**
     * Represents a command line entry
     *
     * @tparam T The type of this command line entry
     */
    template <typename T>
    struct Entry : public entry_base
    {
        /**
         * Constructor
         *
         * @param[in] _default The default value to assign this entry
         */
        Entry(const T& _default, const std::string& _name,
              const std::string& desc)
            : entry_base(),
              description(desc), name(_name), value(_default)
        {
            if (is_char<T>::value)
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

        ~Entry()
        {
        }

        T get() const
        {
            return value;
        }

        std::string description;
        std::string name;
        T value;
    };

    typedef std::unique_ptr<entry_base> entry_ptr;

public:

    CommandLineEntries();
    ~CommandLineEntries();

    template <typename T>
    bool add_entry(const std::string& _name, const T& default_value,
                   const std::string& desc="")
    {
        std::string name = Util::trim(Util::to_lower(_name));

        AbortIfNot(_add_entry(name), false);

        _entries[name].reset(
            new Entry<T>(default_value, name, desc));

        return true;
    }

    bool add_entry2(const std::string& _name,
        const std::string& desc);

    template<typename T>
    bool get(const std::string& _name, T& value) const
    {
        std::string name =
            Util::trim(Util::to_lower(_name));

        auto iter = _entries.find(name);
        if (iter == _entries.end())
        {
            std::printf(" no such command line argument '%s' \n ",
                name.c_str());
            Abort(false);
        }

        const entry_ptr& entry = iter->second;

        value =
            dynamic_cast<Entry<T>*>(entry.get())->get();

        return true;
    }

    CommandLineEntries(const CommandLineEntries& rhs) = delete;
    CommandLineEntries&
            operator= (const CommandLineEntries& rhs) = delete;

private:

    bool _add_entry(const std::string& name) const;

    std::map<std::string, entry_ptr>
        _entries;
};

class CommandLine
{

public:

    CommandLine(const CommandLineEntries& entries);
    ~CommandLine();

    static bool get_opt_val(int argc, char** argv,
        std::map<std::string,std::string>& opt_val);

    bool parse(int argc, char** argv) const;

    CommandLine(const CommandLine& rhs) = delete;
    CommandLine&
      operator=(const CommandLine& rhs) = delete;

private:

    const CommandLineEntries&
        _entries;

    std::map<std::string,std::string>
        _opt_to_value;
};

#endif // __CMD_LINE_H__
