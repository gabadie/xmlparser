#ifndef _H_XSD_TYPE
#define _H_XSD_TYPE

#include "../Xml/XmlElement.hpp"
#include <string>
#include <map>

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
        parseTypeFromComplexType(Xml::Element xmlElement);

        /**
         * Create the regex for an element
         */
        std::string
        getRegexFromElement(Xml::Element xmlElement);

    protected:
        std::string mRegex;

    };
 }

 #endif
