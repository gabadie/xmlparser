#ifndef _H_XSD_VALIDATE
#define _H_XSD_VALIDATE

#include "../Xml/XmlForward.hpp"

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

}

#endif //_H_XSD_VALIDATE
