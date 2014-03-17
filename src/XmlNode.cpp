#include "XmlNode.hpp"

namespace Xml
{
    Node::Node(Node * parent): mParent(parent)
    {

    }

    Node::~Node() = default;

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
