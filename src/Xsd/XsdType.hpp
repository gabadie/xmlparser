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
         * @param xmlElement XML ComplexType element
         */
        Type(const Xml::Element & xmlElement);

        Type(const Xml::Element & xmlElement, const std::string & name);

        /**
         * Create the type regex
         */
        static std::string
        parseComplexType(const Xml::Element & xmlElement) const;

        /**
         * Create the regex for an element
         */
        static std::string
        getRegexFromElement(const Xml::Element & xmlElement) const;

        std::list<Attribute *>
        attributes() const;

    protected:
        std::regex mRegex;
        std::string mName;
        std::list<Attribute *> mAttributes;
    };
 }

 #endif
