/**
 *  \file   commandline_ut.cc
 *  \author Jason Fernandez
 *  \date   07/04/2021
 */

#include <array>
#include <cstring>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "superstring/superstring.h"

#include "commandline/commandline.h"

namespace {
class CommandLineTest : public ::testing::Test {
public:
    CommandLineTest() : m_argCount(0) {
    }

    ~CommandLineTest() {
        reset();
    }

    char** CmdlineToArgv(const std::string& cmdline, int* argc) {
        reset();

        std::vector<std::string> args = jfern::superstring(cmdline).split();

        for (std::size_t i = 0; i < args.size(); i++) {
            const std::size_t len = args[i].size();
            m_args[i] = new char[len+1];

            std::memcpy(m_args[i], args[i].c_str(), len);
            m_args[i][len] = '\0';
        }

        m_argCount = args.size();

        *argc = static_cast<int>(m_argCount);

        return m_args;
    }

    void reset() {
        for (std::size_t i = 0; i < m_argCount; i++)
            delete[] m_args[i];

        m_argCount = 0;
    }

protected:
    std::size_t m_argCount;

    char* m_args[64];
};

template <typename T>
class UserOptionsTest : public ::testing::Test {
 public:
};

template <typename T>
T DefaultValue() {
    static_assert(jfern::CommandLineOptions::IsSupported<T>(),
                  "Non-supported type");
    return T();
}

template <>
bool DefaultValue<bool>() {
    return true;
}
template <>
std::int8_t DefaultValue<std::int8_t>() {
    return 0xab;
}
template <>
std::int16_t DefaultValue<std::int16_t>() {
    return 0xbeef;
}
template <>
std::int32_t DefaultValue<std::int32_t>() {
    return 0xbeefcafe;
}
template <>
std::int64_t DefaultValue<std::int64_t>() {
    return 0xfeedbabecafebeef;
}
template <>
std::uint8_t DefaultValue<std::uint8_t>() {
    return 0xcd;
}
template <>
std::uint16_t DefaultValue<std::uint16_t>() {
    return 0xcafe;
}
template <>
std::uint32_t DefaultValue<std::uint32_t>() {
    return 0xcafebeef;
}
template <>
std::uint64_t DefaultValue<std::uint64_t>() {
    return 0xfeeddeafbabebeef;
}
template <>
float DefaultValue<float>() {
    return 3.14159;
}
template <>
double DefaultValue<double>() {
    return 2.71828182846;
}
template <>
std::string DefaultValue<std::string>() {
    return "hey man what's up";
}

template <typename T>
T AssignedValue() {
    static_assert(jfern::CommandLineOptions::IsSupported<T>(),
                  "Non-supported type");
    return T();
}

template <>
bool AssignedValue<bool>() {
    return false;
}
template <>
std::int8_t AssignedValue<std::int8_t>() {
    return 0xba;
}
template <>
std::int16_t AssignedValue<std::int16_t>() {
    return 0xfeeb;
}
template <>
std::int32_t AssignedValue<std::int32_t>() {
    return 0xcafebeef;
}
template <>
std::int64_t AssignedValue<std::int64_t>() {
    return 0xbeefcafebabefeed;
}
template <>
std::uint8_t AssignedValue<std::uint8_t>() {
    return 0xdc;
}
template <>
std::uint16_t AssignedValue<std::uint16_t>() {
    return 0xface;
}
template <>
std::uint32_t AssignedValue<std::uint32_t>() {
    return 0xbeefcafe;
}
template <>
std::uint64_t AssignedValue<std::uint64_t>() {
    return 0xbeefbabedeaffeed;
}
template <>
float AssignedValue<float>() {
    return 1.41421;
}
template <>
double AssignedValue<double>() {
    return 1.61803398875;
}
template <>
std::string AssignedValue<std::string>() {
    return "not much bro";
}

using TestTypes = ::testing::Types<bool,
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

TYPED_TEST_SUITE(UserOptionsTest, TestTypes);

TYPED_TEST(UserOptionsTest, Add) {
    const std::string option_name = "hello";
    jfern::CommandLineOptions options;
    EXPECT_EQ(jfern::CmdLineError::kSuccess,
              options.Add<TypeParam>(option_name,
                                     DefaultValue<TypeParam>(),
                                     "some option"));

    EXPECT_TRUE(options.Exists(option_name));

    TypeParam value;
    EXPECT_EQ(jfern::CmdLineError::kSuccess,
              options.Default<TypeParam>(option_name, &value));

    EXPECT_EQ(value, DefaultValue<TypeParam>());

    value = TypeParam();

    EXPECT_EQ(jfern::CmdLineError::kSuccess,
              options.Get<TypeParam>(option_name, &value));

    EXPECT_EQ(value, DefaultValue<TypeParam>());

    // Error cases

    EXPECT_EQ(jfern::CmdLineError::kEmptyName,
              options.Add<TypeParam>(" ",
                                     DefaultValue<TypeParam>(),
                                     "some option"));

    EXPECT_FALSE(options.Exists(" "));

    EXPECT_EQ(jfern::CmdLineError::kEmptyName,
              options.Add<TypeParam>("",
                                     DefaultValue<TypeParam>(),
                                     "some option"));

    EXPECT_FALSE(options.Exists(""));

    EXPECT_EQ(jfern::CmdLineError::kDuplicate,
              options.Add<TypeParam>(option_name,
                                     AssignedValue<TypeParam>(),
                                     "some option"));

    // Make sure we didn't modify the existing entry

    value = TypeParam();

    EXPECT_EQ(jfern::CmdLineError::kSuccess,
              options.Default<TypeParam>(option_name, &value));

    EXPECT_EQ(value, DefaultValue<TypeParam>());

    value = TypeParam();

    EXPECT_EQ(jfern::CmdLineError::kSuccess,
              options.Get<TypeParam>(option_name, &value));

    EXPECT_EQ(value, DefaultValue<TypeParam>());
}

TYPED_TEST(UserOptionsTest, Get) {
    const std::string option_name = "hello";
    jfern::CommandLineOptions options;
    ASSERT_EQ(jfern::CmdLineError::kSuccess,
              options.Add<TypeParam>(option_name,
                                     DefaultValue<TypeParam>(),
                                     "some option"));

    EXPECT_TRUE(options.Exists(option_name));

    TypeParam value;
    EXPECT_EQ(jfern::CmdLineError::kSuccess,
              options.Get(option_name, &value));

    EXPECT_EQ(value, DefaultValue<TypeParam>());

    EXPECT_EQ(jfern::CmdLineError::kDoesNotExist,
              options.Get("howdy", &value));

    EXPECT_EQ(jfern::CmdLineError::kEmptyName,
              options.Get("", &value));

    EXPECT_EQ(jfern::CmdLineError::kEmptyName,
              options.Get(" ", &value));

    if (std::is_same<std::uint32_t, TypeParam>::value) {
        std::uint64_t value;
        EXPECT_EQ(jfern::CmdLineError::kWrongType,
                  options.Get<std::uint64_t>(option_name, &value));
    } else {
        std::uint32_t value;
        EXPECT_EQ(jfern::CmdLineError::kWrongType,
                  options.Get<std::uint32_t>(option_name, &value));
    }
}

TYPED_TEST(UserOptionsTest, Set) {
    const std::string option_name = "hello";
    jfern::CommandLineOptions options;
    ASSERT_EQ(jfern::CmdLineError::kSuccess,
              options.Add<TypeParam>(option_name,
                                     DefaultValue<TypeParam>(),
                                     "some option"));

    EXPECT_EQ(jfern::CmdLineError::kSuccess,
              options.Set<TypeParam>(option_name, AssignedValue<TypeParam>()));

    auto value = TypeParam();

    EXPECT_EQ(jfern::CmdLineError::kSuccess,
              options.Default<TypeParam>(option_name, &value));

    EXPECT_EQ(value, DefaultValue<TypeParam>());

    value = TypeParam();

    EXPECT_EQ(jfern::CmdLineError::kSuccess,
              options.Get<TypeParam>(option_name, &value));

    EXPECT_EQ(value, AssignedValue<TypeParam>());

    // Error cases

    EXPECT_EQ(jfern::CmdLineError::kDoesNotExist,
              options.Set<TypeParam>("howdy", AssignedValue<TypeParam>()));

    EXPECT_EQ(jfern::CmdLineError::kEmptyName,
              options.Set<TypeParam>("", AssignedValue<TypeParam>()));

    EXPECT_EQ(jfern::CmdLineError::kEmptyName,
              options.Set<TypeParam>(" ", AssignedValue<TypeParam>()));

    if (std::is_same<std::uint32_t, TypeParam>::value) {
        EXPECT_EQ(jfern::CmdLineError::kWrongType,
                  options.Set<std::uint64_t>(option_name,
                                             AssignedValue<std::uint64_t>()));
    } else {
        EXPECT_EQ(jfern::CmdLineError::kWrongType,
                  options.Set<std::uint32_t>(option_name,
                                             AssignedValue<std::uint32_t>()));
    }

    // Make sure original data is unmodified

    value = TypeParam();

    EXPECT_EQ(jfern::CmdLineError::kSuccess,
              options.Default<TypeParam>(option_name, &value));

    EXPECT_EQ(value, DefaultValue<TypeParam>());

    value = TypeParam();

    EXPECT_EQ(jfern::CmdLineError::kSuccess,
              options.Get<TypeParam>(option_name, &value));

    EXPECT_EQ(value, AssignedValue<TypeParam>());
}

TYPED_TEST(UserOptionsTest, Default) {
    const std::string option_name = "hello";
    jfern::CommandLineOptions options;
    ASSERT_EQ(jfern::CmdLineError::kSuccess,
              options.Add<TypeParam>(option_name,
                                     DefaultValue<TypeParam>(),
                                     "some option"));

    EXPECT_TRUE(options.Exists(option_name));

    TypeParam value;
    EXPECT_EQ(jfern::CmdLineError::kSuccess,
              options.Default(option_name, &value));

    EXPECT_EQ(value, DefaultValue<TypeParam>());

    EXPECT_EQ(jfern::CmdLineError::kDoesNotExist,
              options.Default("howdy", &value));

    EXPECT_EQ(jfern::CmdLineError::kEmptyName,
              options.Default("", &value));

    EXPECT_EQ(jfern::CmdLineError::kEmptyName,
              options.Default(" ", &value));

    if (std::is_same<std::uint32_t, TypeParam>::value) {
        std::uint64_t value;
        EXPECT_EQ(jfern::CmdLineError::kWrongType,
                  options.Default<std::uint64_t>(option_name, &value));
    } else {
        std::uint32_t value;
        EXPECT_EQ(jfern::CmdLineError::kWrongType,
                  options.Default<std::uint32_t>(option_name, &value));
    }
}

TYPED_TEST(UserOptionsTest, Delete) {
    const std::string option_name = "hello";
    jfern::CommandLineOptions options;
    ASSERT_EQ(jfern::CmdLineError::kSuccess,
              options.Add<TypeParam>(option_name,
                                     DefaultValue<TypeParam>(),
                                     "some option"));

    EXPECT_TRUE(options.Exists(option_name));

    EXPECT_EQ(jfern::CmdLineError::kSuccess, options.Delete(option_name));

    EXPECT_FALSE(options.Exists(option_name));

    EXPECT_EQ(jfern::CmdLineError::kDoesNotExist, options.Delete(option_name));

    EXPECT_EQ(jfern::CmdLineError::kDoesNotExist, options.Delete("" ));
    EXPECT_EQ(jfern::CmdLineError::kDoesNotExist, options.Delete(" "));
}

TEST_F(CommandLineTest, GetOptVal) {
    std::string cmdline = "program_name"
                          " --bool_opt=true"
                          " --i8_opt=i8_value"
                          " --i16_opt=i16_value"
                          " --i32_opt=i32_value"
                          " --i64_opt=i64_value"
                          " --u8_opt=u8_value"
                          " --u16_opt=u16_value"
                          " --u32_opt=u32_value"
                          " --u64_opt=u64_value"
                          " --float_opt=float_value"
                          " --double_opt=double_value"
                          " --string_opt=string_value";

    int argc;
    char** argv = CmdlineToArgv(cmdline, &argc);

    std::map<std::string, std::string> opt2val;
    ASSERT_TRUE(jfern::CommandLine::GetOptVal(argc, argv, opt2val));

    EXPECT_EQ(opt2val.size(), 12u);
    EXPECT_EQ(jfern::superstring(opt2val["bool_opt"]).trim(), "true");
    EXPECT_EQ(jfern::superstring(opt2val["i8_opt"]).trim(), "i8_value");
    EXPECT_EQ(jfern::superstring(opt2val["i16_opt"]).trim(), "i16_value");
    EXPECT_EQ(jfern::superstring(opt2val["i32_opt"]).trim(), "i32_value");
    EXPECT_EQ(jfern::superstring(opt2val["i64_opt"]).trim(), "i64_value");
    EXPECT_EQ(jfern::superstring(opt2val["u8_opt"]).trim(), "u8_value");
    EXPECT_EQ(jfern::superstring(opt2val["u16_opt"]).trim(), "u16_value");
    EXPECT_EQ(jfern::superstring(opt2val["u32_opt"]).trim(), "u32_value");
    EXPECT_EQ(jfern::superstring(opt2val["u64_opt"]).trim(), "u64_value");
    EXPECT_EQ(jfern::superstring(opt2val["float_opt"]).trim(), "float_value");
    EXPECT_EQ(jfern::superstring(opt2val["double_opt"]).trim(), "double_value");
    EXPECT_EQ(jfern::superstring(opt2val["string_opt"]).trim(), "string_value");
}

}  // namespace
