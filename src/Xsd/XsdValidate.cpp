#include <map>
#include <string>

#include "../AppDebug.hpp"

#include "XsdElement.hpp"
#include "XsdInstruction.hpp"
#include "XsdValidate.hpp"

#include "XsdForward.hpp"

// TODO
//#include "XsdChoice.hpp"
//#include "XsdAttribute.hpp"

namespace
{
    std::map<std::string, Xsd::Instruction const *> xsdInstructions =
    {
        {"element", new Xsd::Element},
        //{"choice", new Xsd::Choice},
        //{"attribute", new Xsd::Attribute}
    };
}

namespace Xsd
{

    bool
    validate(
        Xml::Document const * xmlDoc,
        Xml::Document const * xsdDoc,
        Xml::Log * log
    )
    {
        app_assert(xmlDoc != nullptr);
        app_assert(xsdDoc != nullptr);
        app_assert(log != nullptr);

        return false;
    }

    Xsd::Instruction const *
    instruction(std::string const & key)
    {
        auto const it = xsdInstructions.find(key);
        return it != std::end(xsdInstructions) ? it->second : nullptr;
    }
}
