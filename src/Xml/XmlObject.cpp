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

    Element *
    Object::root()
    {
        return const_cast<Element *>(static_cast<Object const *>(this)->root());
    }

    bool
    Object::isElement() const
    {
        return false;
    }
}
