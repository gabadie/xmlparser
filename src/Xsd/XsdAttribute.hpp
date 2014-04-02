#ifndef _H_XSD_ATTRIBUTE
#define _H_XSD_ATTRIBUTE

#include "XsdInstruction.hpp"


namespace Xsd
{

    class Attribute final: public Instruction
    {
    public:

        bool
        validate(
            Xml::Element const * xmlElement,
            Xml::Document const * xsdDocument,
            Xml::Element const * xsdElement
        ) const override;

        bool
        regex(
            std::string & regexOut,
            Xml::Element const * xmlElement,
            Xml::Document const * xsdDocument,
            Xml::Element const * xsdElement
        ) const override;

    };

}

#endif //_H_XSD_ATTRIBUTE
