#include <cstdlib>
#include <cstring>

#include "CommandLine.h"
#include "types.h"

class Test
{

public:

    bool test_options()
    {
        CommandLineOptions options;

        AbortIfNot(options.add<bool>("bool_entry", false, "boolean option."),
            false);
        AbortIfNot(options.add<char>("char_entry", -'a', "character option."),
            false);
        AbortIfNot(options.add<short>("int16_entry", -16, "int16 option."),
            false);
        AbortIfNot(options.add<int>("int32_entry", -32, "int32 option."),
            false);
        AbortIfNot(options.add<long long>("int64_entry", -64, "int64 option."),
            false);

        AbortIfNot(options.add<unsigned char>("uchar_entry", 'a', "unsigned char option."),
            false);
        AbortIfNot(options.add<unsigned short>("uint16_entry", 16, "unsigned int16 option."),
            false);
        AbortIfNot(options.add<unsigned int>("uint32_entry", 32, "unsigned int32 option."),
            false);
        AbortIfNot(options.add<unsigned long long>("uint64_entry", 64, "unsigned int64 option."),
            false);

        AbortIfNot(options.add<float>("float_entry",  3.14, "float option." ),
            false);
        AbortIfNot(options.add<double>("double_entry", 2.71, "double option." ),
            false);
        AbortIfNot(options.add<std::string>("string_entry", "Hey", "std::string option."),
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
        std::printf("string_entry = '%s'\n", s.c_str());

        std::fflush(stdout);

        options.print("test");

        const char cmdline1[] =
            "program --bool_entry --char_entry=p --int16_entry=-32768 "
            "--int32_entry=-2147483648 --uchar_entry=z --uint16_entry=65535 "
            "--uint32_entry=4294967295 --float_entry=3.1234567 "
            "--double_entry=4.123456789101112 --string_entry=what's up dude?!";

        types::str_v tokens;
        Util::split(cmdline1, tokens);

        char* cmdline_raw[32];
        for (size_t i = 0; i < tokens.size(); i++)
        {
            cmdline_raw[i] = static_cast<char*>(std::malloc(256));
            std::strcpy(cmdline_raw[i], tokens[i].c_str());
        }

        CommandLine cmd(options);
        AbortIfNot(cmd.parse( tokens.size(), cmdline_raw ),
            false);

        for (size_t i = 0; i < tokens.size(); i++)
            std::free(cmdline_raw[i]);

        /*
         * Now make sure all the right options were updated:
         */
        std::printf("\n");
        AbortIfNot(cmd.get("bool_entry",  b  ), false);
        AbortIfNot(cmd.get("char_entry",  c  ), false);
        AbortIfNot(cmd.get("int16_entry", i16), false);
        AbortIfNot(cmd.get("int32_entry", i32), false);
        AbortIfNot(cmd.get("int64_entry", i64), false);

        AbortIfNot(cmd.get("uchar_entry",  uc ), false);
        AbortIfNot(cmd.get("uint16_entry", u16), false);
        AbortIfNot(cmd.get("uint32_entry", u32), false);
        AbortIfNot(cmd.get("uint64_entry", u64), false);

        AbortIfNot(cmd.get("float_entry",  f), false);
        AbortIfNot(cmd.get("double_entry", d), false);
        AbortIfNot(cmd.get("string_entry", s), false);

        std::printf("bool_entry   = %s\n",  b ? "true":"false");
        std::printf("char_entry   = %c\n",  c  );
        std::printf("int16_entry  = %d\n",  i16);
        std::printf("int32_entry  = %d\n",  i32);
        std::printf("int64_entry  = %lld\n", i64);

        std::printf("uchar_entry  = %c\n",  uc );
        std::printf("uint16_entry = %u\n",  u16);
        std::printf("uint32_entry = %u\n",  u32);
        std::printf("uint64_entry = %llu\n", u64);

        std::printf("float_entry  = %0.7f\n", f);
        std::printf("double_entry = %0.15f\n", d);
        std::printf("string_entry = '%s'\n", s.c_str());

        std::fflush(stdout);

        char buf1[] = "program";
        char buf2[] = "--bool_entry=false";

        char* cmdline2[2] = {buf1, buf2};

        AbortIfNot(cmd.parse( 2, cmdline2 ),
            false);

        /*
         * Make sure only the boolean entry updated:
         */
        std::printf("\n");
        AbortIfNot(cmd.get("bool_entry",  b  ), false);
        AbortIfNot(cmd.get("char_entry",  c  ), false);
        AbortIfNot(cmd.get("int16_entry", i16), false);
        AbortIfNot(cmd.get("int32_entry", i32), false);
        AbortIfNot(cmd.get("int64_entry", i64), false);

        AbortIfNot(cmd.get("uchar_entry",  uc ), false);
        AbortIfNot(cmd.get("uint16_entry", u16), false);
        AbortIfNot(cmd.get("uint32_entry", u32), false);
        AbortIfNot(cmd.get("uint64_entry", u64), false);

        AbortIfNot(cmd.get("float_entry",  f), false);
        AbortIfNot(cmd.get("double_entry", d), false);
        AbortIfNot(cmd.get("string_entry", s), false);

        std::printf("bool_entry   = %s\n",  b ? "true":"false");
        std::printf("char_entry   = %c\n",  c  );
        std::printf("int16_entry  = %d\n",  i16);
        std::printf("int32_entry  = %d\n",  i32);
        std::printf("int64_entry  = %lld\n", i64);

        std::printf("uchar_entry  = %c\n",  uc );
        std::printf("uint16_entry = %u\n",  u16);
        std::printf("uint32_entry = %u\n",  u32);
        std::printf("uint64_entry = %llu\n", u64);

        std::printf("float_entry  = %0.7f\n", f);
        std::printf("double_entry = %0.15f\n", d);
        std::printf("string_entry = '%s'\n", s.c_str());

        std::fflush(stdout);

        char buf3[] = "program";
        char buf4[] = "--bool_entry=1";

        char* cmdline3[2] = {buf3, buf4};

        AbortIfNot(cmd.parse( 2, cmdline3 ),
            false);

        /*
         * Make sure only the boolean entry updated:
         */
        std::printf("\n");
        AbortIfNot(cmd.get("bool_entry",  b  ), false);
        AbortIfNot(cmd.get("char_entry",  c  ), false);
        AbortIfNot(cmd.get("int16_entry", i16), false);
        AbortIfNot(cmd.get("int32_entry", i32), false);
        AbortIfNot(cmd.get("int64_entry", i64), false);

        AbortIfNot(cmd.get("uchar_entry",  uc ), false);
        AbortIfNot(cmd.get("uint16_entry", u16), false);
        AbortIfNot(cmd.get("uint32_entry", u32), false);
        AbortIfNot(cmd.get("uint64_entry", u64), false);

        AbortIfNot(cmd.get("float_entry",  f), false);
        AbortIfNot(cmd.get("double_entry", d), false);
        AbortIfNot(cmd.get("string_entry", s), false);

        std::printf("bool_entry   = %s\n",  b ? "true":"false");
        std::printf("char_entry   = %c\n",  c  );
        std::printf("int16_entry  = %d\n",  i16);
        std::printf("int32_entry  = %d\n",  i32);
        std::printf("int64_entry  = %lld\n", i64);

        std::printf("uchar_entry  = %c\n",  uc );
        std::printf("uint16_entry = %u\n",  u16);
        std::printf("uint32_entry = %u\n",  u32);
        std::printf("uint64_entry = %llu\n", u64);

        std::printf("float_entry  = %0.7f\n", f);
        std::printf("double_entry = %0.15f\n", d);
        std::printf("string_entry = '%s'\n", s.c_str());

        std::fflush(stdout);

        /*
         * Check invalid argument:
         */
        char buf5[] = "program";
        char buf6[] = "--blah=1";

        char* cmdline4[2] = {buf5, buf6};

        AbortIf(cmd.parse( 2, cmdline4 ),
            false);

        /*
         * The following should fail:
         */
        char buf7[] = "program";
        char buf8[] = "--bool_entry=bluh";

        char* cmdline5[2] = {buf7, buf8};

        AbortIf(cmd.parse( 2, cmdline5 ),
            false);

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
