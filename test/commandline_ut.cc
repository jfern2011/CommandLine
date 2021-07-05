/**
 *  \file   commandline_ut.cc
 *  \author Jason Fernandez
 *  \date   07/04/2021
 */

#include "gtest/gtest.h"
#include "commandline/commandline.h"

namespace {
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

}  // namespace
