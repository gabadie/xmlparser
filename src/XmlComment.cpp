#include "XmlComment.hpp"

namespace Xml
{
    Comment::Comment(std::string const & content, Node * parent):
        MiscNode(parent),
        mContent(content)
    {

    }

    Comment::~Comment()
    {

    }

    void
    Comment::exportToStream(std::ostream & stream, std::string const & indent) const
    {
        stream << indent << mContent;
    }

    std::string const &
    Comment::contentText() const
    {
        return mContent;
    }
}
