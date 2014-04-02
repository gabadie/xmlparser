#ifndef _H_XSD_VALIDATE
#define _H_XSD_VALIDATE

#include "../Xml/XmlForward.hpp"


namespace Xsl
{

    bool
    validate(
        Xml::Document const * xmlDoc,
        Xml::Document const * xsdDoc,
        Xml::Log * log
    );

}

#endif //_H_XSD_VALIDATE
