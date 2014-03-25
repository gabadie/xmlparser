#include "XmlNode.hpp"

namespace Xml
{
    Node::Node():
        Object(),
        mParent(nullptr)
    {

    }

    Node::~Node()
    {
        if (mParent != nullptr)
        {
            mParent->remove(this);
        }
    }

    Document const *
    Node::document() const
    {
        return mParent ? mParent->document() : nullptr;
    }

    Object const *
    Node::parent() const
    {
        return mParent;
    }

    std::string const &
    Node::contentText() const
    {
        static std::string const dummy = "";
        return dummy;
    }
}
