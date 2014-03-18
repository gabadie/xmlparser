#include "XmlComment.hpp"
#include "Utils.hpp"

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
    Comment::exportToStream(std::ostream & stream, std::size_t level, std::string const & indent) const
    {
        stream << Utils::repeat(indent, level) << mContent;
    }

    std::string const &
    Comment::contentText() const
    {
        return mContent;
    }
}