#ifndef _H_XSD_VALIDATE
#define _H_XSD_VALIDATE

#include "../Xml/XmlForward.hpp"
#include "XsdInstruction.hpp"

namespace Xsd
{
    bool
    validate(
        Xml::Document const * xmlDoc,
        Xml::Document const * xsdDoc,
        Xml::Log * log
    );

    Xsd::Instruction const *
    instruction(std::string const & key);

    bool
    isDate(std::string const & str);
}

#endif //_H_XSD_VALIDATE
