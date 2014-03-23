#include "XsdAttribute.hpp"

#ifdef APP_DEBUG
#include <cassert>
#endif

namespace Xsd
{
    Attribute::Attribute(std::string const & name, bool required):
    mName(name),
    mRequired(required)
    {

    }

    Attribute::~Attribute()
    {

    }
}