#include "XmlComment.hpp"
#include "../AppDebug.hpp"
#include "../Utils.hpp"

namespace Xml
{
    Comment::Comment(std::string const & content):
        DocumentNode(),
        mContent(content)
    {
        app_assert(content != "");
    }

    Comment::~Comment()
    {

    }

    Node *
    Comment::clone()
    {
        return new Comment(this->mContent);
    }

    void
    Comment::exportToStream(std::ostream & stream, std::size_t level, std::string const & indent) const
    {
        stream << Utils::repeat(indent, level) << "<!--" << mContent << "-->";
    }
}
