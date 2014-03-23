#include "XmlObject.hpp"

namespace Xml
{
    Object::~Object()
    {

    }

    Object *
    Object::parent()
    {
        return mParent;
    }

    Object const *
    Object::parent() const
    {
        return mParent;
    }

    Document *
    Object::document()
    {
        return const_cast<Document *>(static_cast<Object const *>(this)->document());
    }

    bool
    Object::isElement() const
    {
        return false;
    }
}
