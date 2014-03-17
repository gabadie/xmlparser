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
    Text::setText(std::string const & text)
    {
        mText = text;
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
