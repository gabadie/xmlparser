#include "XmlComment.hpp"

namespace Xml
{
    Comment::Comment(std::string const & content, Node * parent):
        Node(parent),
        mContent(content)
    {

    }

    Comment::~Comment()
    {

    }

    void
    Comment::exportToStream(std::ostream & stream, std::string const & indent) const override
    {
        stream << indent << mContent;
    }

    std::string const &
    Comment::contentText() const
    {
        return mContent;
    }
}
