#include "XsdException.hpp"

#ifdef APP_DEBUG
#include <cassert>
#endif

namespace Xsd
{
    XSDConstructionException::XSDConstructionException(const std::string & msg):
        std::runtime_error(msg)
    {
    };

    XSDValidationException::XSDValidationException(const std::string & msg):
        std::runtime_error(msg)
    {
    };
}
