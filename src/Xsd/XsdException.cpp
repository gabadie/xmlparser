/**
 * \file XsdException.cpp
 * \brief XSD Exception
 * \author rdomingues
 * \version 0.1
 * \date 18 mars 2014
 */

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
