#ifndef _H_XSD_CHECKER
#define _H_XSD_CHECKER

#include <string>
#include <map>

#include "../Xml/XmlParser.hpp"
#include "XsdType.hpp"
#include "XsdAttribute.hpp"

namespace Xsd
{
    class Type;
    class Attribute;

    //Aliases
    using typesMap = std::map<std::string, Type const *>;
    using elementsTypesMap = std::map<std::string, std::string>;
    using attributesMap = std::map<std::string, Attribute const *>;
    using attributesTypesMap = std::map<std::string, std::string>;

    /**
     * Defines the schema of an XML Checker and allow its validation
     */
    class Checker
    {
    public :
        /**
         * Constructor
         */
        Checker(Xml::Document * xmlDoc);

        /**
         * Destructor
         */
        virtual ~Checker();

        /**
         * Returns true if the XML file respects the XML schema definition
         */
        bool
        isValid(Xml::Document * xmlDoc);

    protected:
        Xml::Document * mXsdDoc;
        Type * mRootType;

        typesMap mTypes;
        elementsTypesMap mElementsTypes;
        attributesMap mAttributes;
        attributesTypesMap mAttributesTypes;
    };
}

#endif //_H_XSD_CHECKER
