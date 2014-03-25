
#include <iostream>
#include <string>

#include <re2/re2.h>

int
main()
{
    // Simple regular expression matching
    // documentation: https://code.google.com/p/re2/wiki/CplusplusAPI
    std::string fnames[] = {"foo.txt", "bar.txt", "baz.dat", "zoidberg"};
    std::string txt_regex = "[a-z]+\\.txt";

    for (const auto &fname : fnames)
    {
        std::cout << fname << ": " << RE2::FullMatch(fname, txt_regex) << '\n';
    }

    return 0;
}
