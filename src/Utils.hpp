#ifndef _H_UTILS
#define _H_UTILS

#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>

namespace Utils
{
    inline std::string repeat(std::string const & word, std::size_t times)
    {
        std::string result;
        result.reserve(times * word.length());
        for(auto i = 0u; i < times; ++i)
        {
            result += word;
        }
        return result;
    }
}

#endif //_H_UTILS
