#include "XmlNode.hpp"

namespace Xml
{
    Node::Node(): mParent(nullptr)
    {

    }

    Node::~Node()
    {

    }

    Node *
    Node::parent()
    {
        return mParent;
    }

    Node const *
    Node::parent() const
    {
        return mParent;
    }

    Element const *
    Node::root() const
    {
        return mParent ? mParent->root() : nullptr;
    }

    std::string const &
    Node::contentText() const
    {
        static std::string const dummy = "";
        return dummy;
    }

    bool
    Node::isElement() const
    {
        return false;
    }
}
