#include "XmlObject.hpp"

namespace Xml
{
    Object::~Object()
    {

    }

    Element *
    Object::root()
    {
        return const_cast<Element *>(static_cast<Object const *>(this)->root());
    }
}
