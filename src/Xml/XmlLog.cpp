
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

    size_t
    Log::find(std::string const & subString) const
    {
        size_t lineId = 0;

        for (auto line : mLines)
        {
            lineId++;

            if (line.find(subString) != std::string::npos)
            {
                return lineId;
            }
        }

        return 0;
    }

}
