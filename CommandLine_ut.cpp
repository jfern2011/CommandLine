#include <cstdlib>

#include "CommandLine.h"

class Test
{

public:

    bool test_entries()
    {
        CommandLineEntries entries;
        AbortIfNot(entries.add_entry<char>("char_entry", -'a'), false);
        AbortIfNot(entries.add_entry<short>("int16_entry", -16, ""), false);
        AbortIfNot(entries.add_entry<int>("int32_entry", -32, ""), false);
        AbortIfNot(entries.add_entry<long long>("int64_entry", -64, ""), false);

        AbortIfNot(entries.add_entry<unsigned char>("uchar_entry", 'a', ""), false);
        AbortIfNot(entries.add_entry<unsigned short>("uint16_entry", 16, ""), false);
        AbortIfNot(entries.add_entry<unsigned int>("uint32_entry", 32, ""), false);
        AbortIfNot(entries.add_entry<unsigned long long>("uint64_entry", 64, ""), false);

        AbortIfNot(entries.add_entry<float>("float_entry",  3.14, "" ), false);
        AbortIfNot(entries.add_entry<double>("double_entry", 2.71, "" ), false);
        AbortIfNot(entries.add_entry<std::string>("string_entry", "hey", ""), false);

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

        AbortIfNot(entries.get("char_entry",  c  ), false);
        AbortIfNot(entries.get("int16_entry", i16), false);
        AbortIfNot(entries.get("int32_entry", i32), false);
        AbortIfNot(entries.get("int64_entry", i64), false);

        AbortIfNot(entries.get("uchar_entry",  uc ), false);
        AbortIfNot(entries.get("uint16_entry", u16), false);
        AbortIfNot(entries.get("uint32_entry", u32), false);
        AbortIfNot(entries.get("uint64_entry", u64), false);

        AbortIfNot(entries.get("float_entry",  f), false);
        AbortIfNot(entries.get("double_entry", d), false);
        AbortIfNot(entries.get("string_entry", s), false);

        std::printf("char_entry   = %c\n",  c  );
        std::printf("int16_entry  = %d\n",  i16);
        std::printf("int32_entry  = %d\n",  i32);
        std::printf("int64_entry  = %ld\n", i64);

        std::printf("uchar_entry  = %c\n",  uc );
        std::printf("uint16_entry = %u\n",  u16);
        std::printf("uint32_entry = %u\n",  u32);
        std::printf("uint64_entry = %lu\n", u64);

        std::printf("float_entry  = %f\n", f);
        std::printf("double_entry = %f\n", d);
        std::printf("string_entry = %s\n", s.c_str());

        std::fflush(stdout);

        AbortIfNot(entries.add_entry2("void_entry", ""),
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

struct base
{
    virtual ~base() {};
};

template <class T>
struct S1 : public base
{
    S1(T _value) : value(_value) {}

    T get() {return value;}

    T value;
};

struct S2
{
    S2() : ptr(NULL) {}
    ~S2() {if (ptr) delete ptr;}

    template <typename T>
    void func(T value)
    {
        ptr = new S1<T>(value);
    }

    base* ptr;
};

int main(int argc, char** argv)
{
    S2 s2;
    s2.func<char>(-'a');

    Test test;
    AbortIfNot(test.test_entries(), EXIT_FAILURE);
    AbortIfNot(test.test_parser(argc, argv), EXIT_FAILURE);
    return EXIT_SUCCESS;
}
