#include <map>
#include <string>

#include <re2/re2.h>

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
        {"element", new Xsd::Element}
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

    bool
    isDate(std::string const & str)
    {
        std::string date = "^[0-9]{4}-(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])$";

        return RE2::FullMatch(str, date);
    }
}
