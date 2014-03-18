
#include "XmlLog.hpp"


namespace Xml
{

    Log::Log() = default;
    Log::~Log() = default;

    std::ostream &
    Log::operator >> (std::ostream & stream) const
    {
        for (std::string const & l : mLines)
        {
            stream << l << std::endl;
        }

        return stream;
    }

    void
    Log::append(std::string const & line)
    {
        mLines.push_back(line);
    }

}
