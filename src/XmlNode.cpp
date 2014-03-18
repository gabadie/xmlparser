#include "XmlNode.hpp"

#ifdef APP_DEBUG
#include <cassert>
#endif

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

    void
    Node::setParent(Node * parent)
    {
        #ifdef APP_DEBUG
        assert(parent != nullptr);
        #endif

        mParent = parent;
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
