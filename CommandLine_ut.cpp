#include <cstdlib>

#include "CommandLine.h"

class Test
{

public:

    bool test_options()
    {
        CommandLineOptions options;

        AbortIfNot(options.add_option<bool>("bool_entry", true, "boolean option."),
            false);
        AbortIfNot(options.add_option<char>("char_entry", -'a', "character option."),
            false);
        AbortIfNot(options.add_option<short>("int16_entry", -16, "int16 option."),
            false);
        AbortIfNot(options.add_option<int>("int32_entry", -32, "int32 option."),
            false);
        AbortIfNot(options.add_option<long long>("int64_entry", -64, "int64 option."),
            false);

        AbortIfNot(options.add_option<unsigned char>("uchar_entry", 'a', "unsigned char option."),
            false);
        AbortIfNot(options.add_option<unsigned short>("uint16_entry", 16, "unsigned int16 option."),
            false);
        AbortIfNot(options.add_option<unsigned int>("uint32_entry", 32, "unsigned int32 option."),
            false);
        AbortIfNot(options.add_option<unsigned long long>("uint64_entry", 64, "unsigned int64 option."),
            false);

        AbortIfNot(options.add_option<float>("float_entry",  3.14, "float option." ),
            false);
        AbortIfNot(options.add_option<double>("double_entry", 2.71, "double option." ),
            false);
        AbortIfNot(options.add_option<std::string>("string_entry", "hey", "std::string option."),
            false);

        bool               b;
        char               c;
        short              i16;
        int                i32;
        long long          i64;
        unsigned char      uc;
        unsigned short     u16;
        unsigned int       u32;
        unsigned long long u64;
        float              f;
        double             d;
        std::string        s;

        AbortIfNot(options.get("bool_entry",  b  ), false);
        AbortIfNot(options.get("char_entry",  c  ), false);
        AbortIfNot(options.get("int16_entry", i16), false);
        AbortIfNot(options.get("int32_entry", i32), false);
        AbortIfNot(options.get("int64_entry", i64), false);

        AbortIfNot(options.get("uchar_entry",  uc ), false);
        AbortIfNot(options.get("uint16_entry", u16), false);
        AbortIfNot(options.get("uint32_entry", u32), false);
        AbortIfNot(options.get("uint64_entry", u64), false);

        AbortIfNot(options.get("float_entry",  f), false);
        AbortIfNot(options.get("double_entry", d), false);
        AbortIfNot(options.get("string_entry", s), false);

        std::printf("bool_entry   = %s\n",  b ? "true":"false");
        std::printf("char_entry   = %c\n",  c  );
        std::printf("int16_entry  = %d\n",  i16);
        std::printf("int32_entry  = %d\n",  i32);
        std::printf("int64_entry  = %lld\n", i64);

        std::printf("uchar_entry  = %c\n",  uc );
        std::printf("uint16_entry = %u\n",  u16);
        std::printf("uint32_entry = %u\n",  u32);
        std::printf("uint64_entry = %llu\n", u64);

        std::printf("float_entry  = %f\n", f);
        std::printf("double_entry = %f\n", d);
        std::printf("string_entry = %s\n", s.c_str());

        std::fflush(stdout);

        AbortIfNot(options.add_option("void_entry", "void option."),
            false);

        options.print("test");
        return true;
    }

    bool test_parser(int argc, char** argv)
    {
        std::map<std::string,std::string> opt_val;
        AbortIfNot(CommandLine::get_opt_val( argc, argv, opt_val ),
            false);

        std::printf("\ncommand line: \n");
        for (auto iter = opt_val.begin(), end = opt_val.end();
             iter != end; ++iter)
        {
            std::printf("'%s' -> '%s'\n",
                        iter->first.c_str(), iter->second.c_str());
        }

        std::fflush(stdout);
        return true;
    }
};

int main(int argc, char** argv)
{
    Test test;
    AbortIfNot(test.test_options(), EXIT_FAILURE);
    AbortIfNot(test.test_parser(argc, argv), EXIT_FAILURE);
    return EXIT_SUCCESS;
}
