#include "../AppDebug.hpp"
#include "XmlNode.hpp"

#include "../MemoryLeakTrackerOn.hpp"

namespace Xml
{
    Node::Node():
        Object(),
        mParent(nullptr)
    {

    }

    Node::~Node()
    {
        detach();
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

    void
    Node::detach()
    {
        if (mParent == nullptr)
        {
            return;
        }

#ifdef APP_DEBUG
        app_assert(mParent->remove(this) == true);
#else
        mParent->remove(this);
#endif
    }

    std::string const &
    Node::contentText() const
    {
        static std::string const dummy = "";
        return dummy;
    }
}

#include "../MemoryLeakTrackerOff.hpp"
