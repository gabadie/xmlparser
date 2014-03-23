#include "XmlNode.hpp"

namespace Xml
{
    Node::Node(): Object()
    {

    }

    Node::~Node()
    {

    }

    Document const *
    Node::document() const
    {
        return mParent ? mParent->document() : nullptr;
    }

    std::string const &
    Node::contentText() const
    {
        static std::string const dummy = "";
        return dummy;
    }
}
