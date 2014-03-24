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
        static const std::string SCHEMA_ELT = "schema";
        static const std::string ELEMENT_ELT = "element";
        static const std::string ATTRIBUTE_ELT = "attribute";
        static const std::string COMPLEX_TYP_ELT = "complexType";
        static const std::string SEQUENCE_ELT = "sequence";
        static const std::string CHOICE_ELT = "choice";

        static const std::string NAME_ATTR = "name";
        static const std::string TYPE_ATTR = "type";
        static const std::string REF_ATTR = "ref";
        static const std::string USE_ATTR = "use";
        static const std::string MIN_OCC_ATTR = "minOccurs";
        static const std::string MAX_OCC_ATTR = "maxOccurs";

        static const std::string ROOT = "root";

        /**
         * Destructor
         */
        virtual ~Checker();

        /**
         * Returns true if the XML file respects the XML schema definition
         */
        bool
        isValid(Xml::Document * xmlDoc);

        std::string getStringTypeValue();

        std::string getDateTypeValue();

        static Checker
        getInstance();

        static void
        initialize(const Xml::Document & xmlDoc);

        static void
        addType(const std::string & typeName, const Type * const type);

        static void
        addTypedElement(const std::string & elementName, const std::string & typeName);

        static void
        addAttribute(const std::string & attributeName, const Attribute * const attribute);

        static void
        addTypedAttribute(const std::string & attributeName, const std::string & typeName);

        static Type * const
        getType(const std::string & typeName)

        static Type * const
        getElementType(const std::string & elementName);

        static Attribute * const
        getAttribute(const std::string & attributeName);

        static Type * const
        getAttributeType(const std::string & attributeName);

    protected:
        /**
         * Constructor
         */
        Checker(const Xml::Document & xmlDoc);

        /**
         * Unique instance
         */
        static Checker instance;

        static const std::string STRING_TYPE_VALUE = "string";
        static const std::string DATE_TYPE_VALUE = "date";

        std::string namespacePrefix;
        std::string stringTypeValue;
        std::string dateTypeValue;

        Xml::Document & mXsdDoc;

        typesMap mTypes;
        elementsTypesMap mElementsTypes;
        attributesMap mAttributes;
        attributesTypesMap mAttributesTypes;
    };
}

#endif //_H_XSD_CHECKER
