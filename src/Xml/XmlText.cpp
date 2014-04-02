#include "XmlText.hpp"
#include "../AppDebug.hpp"
#include "../Utils.hpp"

#include "../MemoryLeakTrackerOn.hpp"

namespace Xml
{
    Text::Text(std::string const & content):
        Node(),
        mText(content)
    {
        app_assert(content != "");
    }

    Text::~Text()
    {

    }

    ObjectLabel
    Text::objectLabel() const
    {
        return ObjectLabel::Text;
    }

    Node *
    Text::clone() const
    {
        return new Text(this->mText);
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

#include "../MemoryLeakTrackerOff.hpp"
