#include "XmlNode.hpp"

namespace Xml
{
    Node::Node(Node * parent): mParent(parent)
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
}
