#include "XmlObject.hpp"

namespace Xml
{
    Object::~Object()
    {

    }

    Document *
    Object::document()
    {
        return const_cast<Document *>(static_cast<Object const *>(this)->document());
    }

    Object *
    Object::parent()
    {
        return const_cast<Object *>(static_cast<Object const *>(this)->parent());
    }

    bool
    Object::isElement() const
    {
        return false;
    }
}
