#include "XmlText.hpp"

namespace Xml
{
    Text::Text(std::string const & content, Node * parent):
        Node(parent),
        mText(content)
    {

    }

    Text::~Text()
    {

    }

    void
    Text::exportToStream(std::ostream & stream, std::string const & indent) const
    {
        stream << indent << mText;
    }

    std::string const &
    Text::contentText() const
    {
        return mText;
    }
}
