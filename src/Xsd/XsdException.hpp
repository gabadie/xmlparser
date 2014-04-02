#ifndef _H_XSD_EXCEPTION
#define _H_XSD_EXCEPTION

#include <stdexcept>
#include <string>

namespace Xsd
{
    class XSDConstructionException : public std::runtime_error
    {
    public:
        XSDConstructionException(const std::string & msg);
    };

    class XSDValidationException : public std::runtime_error
    {
    public:
        XSDValidationException(const std::string & msg);
    };
}

#endif //_H_XSD_EXCEPTION
