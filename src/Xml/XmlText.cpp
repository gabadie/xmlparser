#include "XmlText.hpp"
#include "../Utils.hpp"

namespace Xml
{
    Text::Text(std::string const & content):
        Node(),
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
    Text::exportToStream(std::ostream & stream, std::size_t level,
        std::string const & indent) const
    {
        stream << Utils::repeat(indent, level) << mText;
    }

    std::string const &
    Text::contentText() const
    {
        return mText;
    }
}
