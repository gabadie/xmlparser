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

    /*
     * Get an instruction by name
     *
     * @param name Name of the instruction
     *
     * @return The instruction if found, nullptr otherwise
     */
    Xsd::Instruction const *
    instruction(std::string const & name);

    /*
     * Tells whether or not the given string in a xsd::date
     * (YYYY-MM-DD)
     *
     * @param str String to test
     *
     * @return true if is the string is a xsd::date, false otherwise.
     */
    bool
    isDate(std::string const & str);

    /*
     * Check the presence of a required xsd token's attribute
     *
     * @param xsdDoc Xsd document to check
     * @param name Name of the token
     * @param attrValue Value of the attribute
     *
     * @return true if found, false otherwise
     */
    bool
    hasRequired(Xml::Document const * xsdDoc,
        std::string const & name, std::string const & attrValue);
}

#endif //_H_XSD_VALIDATE
