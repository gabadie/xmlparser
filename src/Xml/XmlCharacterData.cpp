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

    Node *
    CharacterData::clone() const
    {
        return new CharacterData(this->mContent);
    }

    void
    CharacterData::exportToStream(std::ostream & stream, std::size_t level, std::string const & indent) const
    {
        stream << Utils::repeat(indent, level) << "<![CDATA[" << mContent << "]]>";
    }
}