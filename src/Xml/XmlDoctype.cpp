#include "XmlDoctype.hpp"
#include "../AppDebug.hpp"
#include "../Utils.hpp"

#include "../MemoryLeakTrackerOn.hpp"

namespace Xml
{
    Doctype::Doctype(std::string const & content):
        DocumentNode(),
        mContent(content)
    {
        app_assert(content != "");
    }

    Doctype::~Doctype()
    {

    }

    Node *
    Doctype::clone() const
    {
        return new Doctype(this->mContent);
    }

    ObjectLabel
    Doctype::objectLabel() const
    {
        return ObjectLabel::Doctype;
    }

    void
    Doctype::exportToStream(std::ostream & stream, std::size_t level, std::string const & indent) const
    {
        stream << Utils::repeat(indent, level) << "<!DOCTYPE " << mContent << ">";
    }
}

#include "../MemoryLeakTrackerOff.hpp"
