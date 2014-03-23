#include "XmlNode.hpp"

namespace Xml
{
    Node::Node(): Object()
    {

    }

    Node::~Node()
    {

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
}
