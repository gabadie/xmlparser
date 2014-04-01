#include "XmlCharacterData.hpp"
#include "../AppDebug.hpp"
#include "../Utils.hpp"

namespace Xml
{
    CharacterData::CharacterData(std::string const & content):
        Node(),
        mContent(content)
    {
        app_assert(content != "");
    }

    CharacterData::~CharacterData()
    {

    }

    ObjectLabel
    CharacterData::objectLabel() const
    {
        return ObjectLabel::CharacterData;
    }

    Node *
    CharacterData::clone()
    {
        return new CharacterData(this->mContent);
    }

    void
    CharacterData::exportToStream(std::ostream & stream, std::size_t level, std::string const & indent) const
    {
        stream << Utils::repeat(indent, level) << "<![CDATA[" << mContent << "]]>";
    }
}
