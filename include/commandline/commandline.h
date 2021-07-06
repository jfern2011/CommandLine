/**
 *  \file   commandline.h
 *  \author Jason Fernandez
 *  \date   11/18/2017
 *
 *  https://github.com/jfern2011/CommandLine
 */

#ifndef COMMAND_LINE_H_
#define COMMAND_LINE_H_

#include <algorithm>
#include <map>
#include <cstddef>
#include <cstdint>  //yes
#include <ostream> // yes
#include <string> // yes
#include <tuple> // yes
#include <type_traits> // yes
#include <utility> // yes
#include <vector> // yes

//#include "abort/abort.h"
//#include "util/str_util.h"
//#include "util/traits.h"

// ---

#include "superstring/superstring.h"

namespace jfern {
namespace internal {
/**
 * Conversions from a supported type to a human-readable type name
 * @{
 */
template <typename T>
struct TypeToName {
    constexpr static char value[] = "error";
};
template <>
struct TypeToName<bool> {
    constexpr static char value[] = "bool";
};
template <>
struct TypeToName<std::int8_t> {
    constexpr static char value[] = "int8";
};
template <>
struct TypeToName<std::int16_t> {
    constexpr static char value[] = "int16";
};
template <>
struct TypeToName<std::int32_t> {
    constexpr static char value[] = "int32";
};
template <>
struct TypeToName<std::int64_t> {
    constexpr static char value[] = "int64";
};
template <>
struct TypeToName<std::uint8_t> {
    constexpr static char value[] = "uint8";
};
template <>
struct TypeToName<std::uint16_t> {
    constexpr static char value[] = "uint16";
};
template <>
struct TypeToName<std::uint32_t> {
    constexpr static char value[] = "uint32";
};
template <>
struct TypeToName<std::uint64_t> {
    constexpr static char value[] = "uint64";
};
template <>
struct TypeToName<float> {
    constexpr static char value[] = "float";
};
template <>
struct TypeToName<double> {
    constexpr static char value[] = "double";
};
template <>
struct TypeToName<std::string> {
    constexpr static char value[] = "string";
};
/**
 * @}
 */

/**
 * Converts a value to its string representation. This simply wraps
 * std::to_string, with specializations for bool and string
 *
 * @{
 */
template <typename T>
std::string ToString(const T& value) {
    return std::to_string(value);
}
template <>
inline std::string ToString<bool>(const bool& value) {
    return value ? "true" : "false";
}
template <>
inline std::string ToString<std::string>(const std::string& value) {
    return value;
}
/**
 * @}
 */

}  // namespace internal

/**
 * Error codes returned by this library
 */
enum class CmdLineError {
    kDoesNotExist,    ///< No option exists with this name
    kDuplicate,       ///< Duplicate option name
    kEmptyName,       ///< Option name is an empty string
    kInvalidCmdLine,  ///< Command line is ill-formed
    kWrongType,       ///< Option has a different type
    kSuccess          ///< Completed successfully
};

/**
 * Class that builds a table of command line options
 */
template <typename... Ts>
class UserOptions final {
public:
    UserOptions() = default;

    UserOptions(const UserOptions& opts)            = default;
    UserOptions(UserOptions&& opts)                 = default;
    UserOptions& operator=(const UserOptions& opts) = default;
    UserOptions& operator=(UserOptions&& opts)      = default;

    ~UserOptions() = default;

    template <typename T>
    CmdLineError Add(const std::string& name,
                     const T& default_value,
                     const std::string& desc = "");

    template <typename T>
    CmdLineError Default(const std::string& name, T* value) const;

    CmdLineError Delete(const std::string& name);

    bool Exists(const std::string& name) const;

    template <typename T>
    CmdLineError Get(const std::string& name, T* value) const;

    template <typename T>
    CmdLineError Set(const std::string& name, const T& value);

    void Print(const char* prog_name, std::ostream& os) const;

    template <typename T>
    static constexpr bool IsSupported() noexcept;

private:
    template <typename U>
    CmdLineError Delete_(const std::string& name);

    template <typename U1, typename U2, typename... Us>
    CmdLineError Delete_(const std::string& name);

    template <typename U>
    bool Exists_(const std::string& name) const;

    template <typename U1, typename U2, typename... Us>
    bool Exists_(const std::string& name) const;

    template <typename T>
    static constexpr bool IsSupported_() noexcept;

    template <typename T, typename U, typename... Us>
    static constexpr bool IsSupported_() noexcept;

    /**
     * Base class for a generic command line option
     */
    class Option {
    public:
        Option(const std::string& name,
               const std::string& type,
               const std::string& description);

        Option(const Option& opt)            = default;
        Option(Option&& opt)                 = default;
        Option& operator=(const Option& opt) = default;
        Option& operator=(Option&& opt)      = default;

        virtual ~Option() = default;

        /**
         * Get the default value of this option
         *
         * @return The string representation of this option's default
         */
        virtual std::string Default() const = 0;

        /**
         * Get the current value of this option
         *
         * @return The string representation of this option's current
         *         value
         */
        virtual std::string Value() const = 0;

        std::string Description() const;

        std::string Name() const;

        void Print(std::ostream& os) const;

        std::string Type() const;

    protected:
        /**
         * A description for this option
         */
        std::string description_;

        /**
         * The name of this option
         */
        std::string name_;

        /**
         * The human-readable type of option
         */
        std::string type_;
    };

    /**
     * Represents a command line option
     *
     * @tparam T The type of this command line option
     */
    template <typename T>
    class TypedOption final : public Option {
    public:
        using ValueType = T;

        TypedOption(const std::string& name,
                    const std::string& description,
                    const ValueType& default_value);

        TypedOption(const TypedOption& opt)            = default;
        TypedOption(TypedOption&& opt)                 = default;
        TypedOption& operator=(const TypedOption& opt) = default;
        TypedOption& operator=(TypedOption&& opt)      = default;

        ~TypedOption() = default;

        void Assign(const ValueType& value) noexcept;

        ValueType CurrentValue() const noexcept;

        std::string Default() const override;

        ValueType DefaultValue() const noexcept;

        std::string Value() const override;

    private:
        /**
         * This option's default value
         */
        ValueType default_;

        /**
         * This option's current value
         */
        ValueType value_;
    };

    template <typename U>
    std::vector<const Option*> Accumulate_() const;

    template <typename U1, typename U2, typename... Us>
    std::vector<const Option*> Accumulate_() const;

    std::vector<const Option*> Accumulate () const;

    template <typename T>
    typename std::vector<TypedOption<T>>::const_iterator
    Find(const std::string& name, bool* valid) const;

    template <typename T>
    typename std::vector<TypedOption<T>>::iterator
    Load(const std::string& name, bool* valid);

    /**
     * Container for a set of options of the same type
     */
    template <typename T>
    using OptionSet = std::vector<TypedOption<T>>;

    /**
     * The complete set of available command line options
     */
    std::tuple<OptionSet<Ts>...>
        options_;
};

/**
 * Alias representing all types supported by this library
 */
using CommandLineOptions = UserOptions<bool,
                                       std::int8_t,
                                       std::int16_t,
                                       std::int32_t,
                                       std::int64_t,
                                       std::uint8_t,
                                       std::uint16_t,
                                       std::uint32_t,
                                       std::uint64_t,
                                       float,
                                       double,
                                       std::string>;

/**
 * A class that parses the command line and maintains a record of options. Only
 * supports long options
 */
class CommandLine {
public:

    explicit CommandLine(const CommandLineOptions& options);
    ~CommandLine();


#if 0
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
#endif
    bool parse(int argc, char** argv);

    CommandLine(const CommandLine& rhs) = delete;
    CommandLine&
      operator=(const CommandLine& rhs) = delete;

    static bool GetOptVal(int argc, char** argv,
                          std::map<std::string, std::string>& opt_val);

private:
    static bool NextPair(const std::string& str,
                         std::size_t* p_start,
                         std::size_t* p_equal);
    

    UserOptions<bool>&
        _options;
};

/**
 * Add a new command line option which is settable via the command line
 *
 * @tparam T The type of the option
 *
 * @param[in] name          The name of the option to add
 * @param[in] default_value The default value to assign the option
 * @param[in] desc          A description for the option
 *
 * @return A \ref CmdLineError return code
 */
template <typename... Ts>
template <typename T>
CmdLineError UserOptions<Ts...>::Add(const std::string& name,
                                     const T& default_value,
                                     const std::string& desc) {
    const std::string trimmed = superstring(name).trim();

    if (trimmed.empty()) return CmdLineError::kEmptyName;
    if (Exists(name)) return CmdLineError::kDuplicate;

    std::vector<TypedOption<T>>& options = std::get<OptionSet<T>>(options_);

    options.push_back(TypedOption<T>(name, desc, default_value));

    return CmdLineError::kSuccess;
}

/**
 * Get the default value of an option
 * 
 * @param[in]  name  The name of the option
 * @param[out] value The default value of this option
 * 
 * @return A \ref CmdLineError return code
 */
template <typename... Ts>
template <typename T>
CmdLineError
UserOptions<Ts...>::Default(const std::string& name, T* value) const {
    const std::string trimmed = superstring(name).trim();

    if (trimmed.empty())
        return CmdLineError::kEmptyName;

    if (!Exists(name))
        return CmdLineError::kDoesNotExist;

    bool valid = false;
    auto iter = Find<T>(name, &valid);

    if (!valid)
        return CmdLineError::kWrongType;

    *value = iter->DefaultValue();

    return CmdLineError::kSuccess;
}

/**
 * Compile-time recursive base case of this method
 */
template <typename... Ts>
template <typename U>
CmdLineError UserOptions<Ts...>::Delete_(const std::string& name) {
    auto& options = std::get<OptionSet<U>>(options_);

    auto iter = std::find_if(options.begin(), options.end(),
            [ &name ](const auto& option) { return option.Name() == name; });

    if (iter != options.end()) {
        options.erase(iter); return CmdLineError::kSuccess;
    }

    return CmdLineError::kDoesNotExist;
}

/**
 * Check for the existence of an option by name
 * 
 * @param[in] name The name of the option to search for
 * 
 * @return A \ref CmdLineError return code
 */
template <typename... Ts>
template <typename U1, typename U2, typename... Us>
CmdLineError UserOptions<Ts...>::Delete_(const std::string& name) {
    if (Delete_<U1>(name) == CmdLineError::kDoesNotExist)
        return Delete_<U2, Us...>(name);

    return CmdLineError::kSuccess;
}

/**
 * Delete an option
 * 
 * @param[in] name The name of the option to delete
 * 
 * @return A \ref CmdLineError return code
 */
template <typename... Ts>
CmdLineError UserOptions<Ts...>::Delete(const std::string& name) {
    return Delete_<Ts...>(name);
}

/**
 * Compile-time recursive base case of this method
 */
template <typename... Ts>
template <typename U>
bool UserOptions<Ts...>::Exists_(const std::string& name) const {
    const auto& options = std::get<OptionSet<U>>(options_);

    auto iter = std::find_if(options.begin(), options.end(),
            [ &name ](const auto& option) { return option.Name() == name; });

    if (iter != options.end()) return true;

    return false;
}

/**
 * Check for the existence of an option by name
 * 
 * @param[in] name The name of the option to search for
 * 
 * @return True if the option exists
 */
template <typename... Ts>
template <typename U1, typename U2, typename... Us>
bool UserOptions<Ts...>::Exists_(const std::string& name) const {
    if (!Exists_<U1>(name))
        return Exists_<U2, Us...>(name);

    return true;
}

/**
 * Check for the existence of an option by name
 * 
 * @param[in] name The name of the option to search for
 * 
 * @return True if the option exists
 */
template <typename... Ts>
bool UserOptions<Ts...>::Exists(const std::string& name) const {
    return Exists_<Ts...>(name);
}

/**
 * Get the current value of an option
 * 
 * @param[in]  name  The name of the option
 * @param[out] value The current value of this option
 * 
 * @return A \ref CmdLineError return code
 */
template <typename... Ts>
template <typename T>
CmdLineError UserOptions<Ts...>::Get(const std::string& name, T* value) const {
    const std::string trimmed = superstring(name).trim();

    if (trimmed.empty())
        return CmdLineError::kEmptyName;

    if (!Exists(name))
        return CmdLineError::kDoesNotExist;

    bool valid = false;
    auto iter = Find<T>(name, &valid);

    if (!valid)
        return CmdLineError::kWrongType;

    *value = iter->CurrentValue();

    return CmdLineError::kSuccess;
}

/**
 * Set the value of an option
 * 
 * @param[in]  name  The name of the option
 * @param[out] value The value to assign this option
 * 
 * @return A \ref CmdLineError return code
 */
template <typename... Ts>
template <typename T>
CmdLineError UserOptions<Ts...>::Set(const std::string& name, const T& value) {
    const std::string trimmed = superstring(name).trim();

    if (trimmed.empty())
        return CmdLineError::kEmptyName;

    if (!Exists(name))
        return CmdLineError::kDoesNotExist;

    bool valid = false;
    auto iter = Load<T>(name, &valid);

    if (!valid)
        return CmdLineError::kWrongType;

    iter->Assign(value);

    return CmdLineError::kSuccess;
}

/**
 * Print all command line options for the program
 *
 * @param[in] prog_name Usually the 1st command line argument, which
 *                      is the executable name
 * @param[in] os        The output stream object to write to
 */
template <typename... Ts>
void UserOptions<Ts...>::Print(const char* prog_name, std::ostream& os) const {
    std::vector<const Option*> options = Accumulate();

    auto compare = [](const Option* opt1, const Option* opt2) {
        return opt1->Name().compare(opt2->Name()) < 0;
    };

    std::sort(options.begin(), options.end(), compare);

    os << "usage: " << prog_name << " [options]\n";
    os << "options:\n\n";

    for (std::size_t i = 0; i < options.size(); i++)
        options[i]->Print();
}

/**
 * Compile-time recursive base case of this function
 * 
 * @return false
 */
template <typename... Ts>
template <typename T>
constexpr bool UserOptions<Ts...>::IsSupported_() noexcept {
    return false;
}

/**
 * Check if a type is supported by this library
 * 
 * @return True if supported
 */
template <typename... Ts>
template <typename T, typename U, typename... Us>
constexpr bool UserOptions<Ts...>::IsSupported_() noexcept {
    return std::is_same<T,U>::value || IsSupported_<T, Us...>();
}

/**
 * Check if the given type is supported by this library, i.e. it may be used
 * as a command line option
 * 
 * @tparam T The type to check
 * 
 * @return True if this type is supported
 */
template <typename... Ts>
template <typename T>
constexpr bool UserOptions<Ts...>::IsSupported() noexcept {
    return IsSupported_<T, Ts...>();
}

/**
 * Compile-time recursive base case of this method
 */
template <typename... Ts>
template <typename U>
auto UserOptions<Ts...>::Accumulate_() const -> std::vector<const Option*> {
    std::vector<const Option*> aggregate;

    const auto& options = std::get<OptionSet<U>>(options_);

    for (std::size_t i = 0; i < options.size(); i++) {
        aggregate.push_back(&options[i]);
    }

    return aggregate;
}

/**
 * Gather all options
 *
 * @return All options registered via \ref Add()
 */
template <typename... Ts>
template <typename U1, typename U2, typename... Us>
auto UserOptions<Ts...>::Accumulate_() const -> std::vector<const Option*> {
    std::vector<const Option*> aggregate = Accumulate_<U1>();

    std::vector<const Option*> partial = Accumulate_<U2, Us...>();

    aggregate.insert(aggregate.end(), partial.begin(), partial.end());

    return aggregate;
}

/**
 * Gather all options
 *
 * @return All options registered via \ref Add()
 */
template <typename... Ts>
auto UserOptions<Ts...>::Accumulate() const -> std::vector<const Option*> {
    return Accumulate_<Ts...>();
}

/**
 * Find an option
 * 
 * @param[in]  name  The option name
 * @oaram[out] valid True if the return value is a valid iterator
 * 
 * @return An iterator to the option
 */
template <typename... Ts>
template <typename T>
auto UserOptions<Ts...>::Find(const std::string& name, bool* valid) const
    -> typename std::vector<TypedOption<T>>::const_iterator {
    const std::vector<TypedOption<T>>& options =
        std::get<OptionSet<T>>(options_);

    auto iter = std::find_if(options.begin(), options.end(),
            [ &name ](const auto& option) { return option.Name() == name; });

    *valid = iter != options.end();
    return iter;
}

/**
 * Load an option. Similar to \ref Find(), but returns a non-const iterator
 * 
 * @param[in]  name  The option name
 * @oaram[out] valid True if the return value is a valid iterator
 * 
 * @return An iterator to the option
 */
template <typename... Ts>
template <typename T>
auto UserOptions<Ts...>::Load(const std::string& name, bool* valid)
    -> typename std::vector<TypedOption<T>>::iterator {
    std::vector<TypedOption<T>>& options = std::get<OptionSet<T>>(options_);

    auto iter = std::find_if(options.begin(), options.end(),
            [ &name ](const auto& option) { return option.Name() == name; });

    *valid = iter != options.end();
    return iter;
}

/**
 * Constructor
 * 
 * @param[in] name        The name of this option
 * @param[in] type        The type which holds the value of this option
 * @param[in] description A description for this option
 */
template <typename... Ts>
UserOptions<Ts...>::Option::Option(const std::string& name,
                                   const std::string& type,
                                   const std::string& description)
    : description_(description),
      name_(name),
      type_(type) {
}

/**
 * Get the description for this option
 *
 * @return The option's description
 */
template <typename... Ts>
std::string UserOptions<Ts...>::Option::Description() const {
    return description_;
}

/**
 * Get the name of this option
 *
 * @return The option's name
 */
template <typename... Ts>
std::string UserOptions<Ts...>::Option::Name() const {
    return name_;
}

/**
 * Format this option
 * 
 * @param[in] os The stream object to write to
 */
template <typename... Ts>
void UserOptions<Ts...>::Option::Print(std::ostream& os) const {
    std::string output =
        "\t--" + name_ + "=<" + type_ + "> [" + Default() + "]\n\t\t";

    output += description_;

    os << output << std::endl;
}

/**
 * Get the data type which holds this option's value
 *
 * @return The option's storage type
 */
template <typename... Ts>
std::string UserOptions<Ts...>::Option::Type() const {
    return type_;
}

/**
 * Constructor
 *
 * @param [in] name          The option name
 * @param [in] description   A description for this option
 * @param [in] default_value The option's default value
 */
template <typename... Ts>
template <typename T>
UserOptions<Ts...>::TypedOption<T>::TypedOption(const std::string& name,
                                                const std::string& description,
                                                const ValueType& default_value)
    : Option(name, internal::TypeToName<T>::value, description),
      default_(default_value), value_(default_value) {
    static_assert(UserOptions<Ts...>::IsSupported<T>(), "Non-supported type");
}

/**
 * Assign a value to this option
 * 
 * @param[in] value The value to give to this option
 */
template <typename... Ts>
template <typename T> void
UserOptions<Ts...>::TypedOption<T>::Assign(const ValueType& value) noexcept {
    value_ = value;
}

/**
 * Get the current value of this option
 *
 * @return The current value
 */
template <typename... Ts>
template <typename T>
auto UserOptions<Ts...>::TypedOption<T>::CurrentValue() const noexcept
    -> ValueType {
    return value_;
}

/**
 * @see Option::Default
 */
template <typename... Ts>
template <typename T>
std::string UserOptions<Ts...>::TypedOption<T>::Default() const {
    return internal::ToString(default_);
}

/**
 * Get the default value of this option
 *
 * @return The default value
 */
template <typename... Ts>
template <typename T>
auto UserOptions<Ts...>::TypedOption<T>::DefaultValue() const noexcept
    -> ValueType {
    return default_;
}

/**
 * @see Option::Value
 */
template <typename... Ts>
template <typename T>
std::string UserOptions<Ts...>::TypedOption<T>::Value() const {
    return internal::ToString(value_);
}

}  // namespace jfern

#endif  // COMMAND_LINE_H_
