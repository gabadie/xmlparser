#ifndef _H_XSD_TYPE
#define _H_XSD_TYPE

#include "../Xml/XmlElement.hpp"
#include <string>

namespace Xsd
{
    class Type
    {
    public:
         /**
         * Destructor
         */
        virtual
        ~Type();

    protected:
        /**
         * Constructor
         */
        Type();

        /**
         * Create the type regex
         */
        void
        ParseTypeFromComplexType(Xml::XmlElement xmlElement);

        /**
         * Create the regex for an element
         */
        void
        getRegexFromElement(Xml::XmlElement xmlElement);

    protected:
        std::string mRegex;

    };
 }