
#include "../AppDebug.hpp"

#include "XsdValidate.hpp"


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

}
