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
    //using attributesMap = std::map<std::string, Attribute const *>;
    using attributesTypesMap = std::map<std::string, std::string>;

    /**
     * Defines the schema of an XML Checker and allow its validation
     */
    class Checker
    {
    public:
        static const std::string SCHEMA_ELT;
        static const std::string ELEMENT_ELT;
        static const std::string ATTRIBUTE_ELT;
        static const std::string COMPLEX_TYP_ELT;
        static const std::string SEQUENCE_ELT;
        static const std::string CHOICE_ELT;

        static const std::string NAME_ATTR;
        static const std::string TYPE_ATTR;
        static const std::string REF_ATTR;
        static const std::string MIXED_ATTR;
        static const std::string MIN_OCC_ATTR;
        static const std::string MAX_OCC_ATTR;

        static const std::string ROOT;

        static const std::string STRING_TYPE_VALUE;
        static const std::string DATE_TYPE_VALUE;
        static const std::string USE_REQUIRED_VALUE;

        static Checker
        getInstance();

        static void
        initialize(const Xml::Document & xmlDoc);

        /**
         * Destructor
         */
        virtual ~Checker();

        void
        checkReferences();

        /**
         * Returns true if the XML file respects the XML schema definition
         */
        bool
        isValid(Xml::Document xmlDoc);

        std::string getStringTypeValue();

        std::string getDateTypeValue();

        void
        addType(const std::string & typeName, const Type * const type);

        void
        addTypedElement(const std::string & elementName, const std::string & typeName);
/*
        void
        addAttribute(const std::string & attributeName, const Attribute * const attribute);
*/
        void
        addTypedAttribute(const std::string & attributeName, const std::string & typeName);

        static void
        checkExistType(const std::string & typeName);

        static bool
        existType(const std::string & typeName);

        Type * const
        getType(const std::string & typeName);

        Type * const
        getElementType(const std::string & elementName);
/*
        Attribute * const
        getAttribute(const std::string & attributeName);
*/
        Type * const
        getAttributeType(const std::string & attributeName);

        static void
        throwInvalidElementException(const std::string & received, const std::string & expected);

        static void
        throwMissingAttributeException(const std::string & element, const std::string & missingAttr);

        static void
        throwInvalidAttributeValueException(const std::string & element, const std::string & attr, const std::string & invalidValue);



    protected:
        /**
         * Constructor
         */
        Checker(const Xml::Document & xmlDoc);

        /**
         * Unique private instance
         */
        static Checker instance;

        std::string namespacePrefix;
        std::string stringTypeValue;
        std::string dateTypeValue;

        const Xml::Document * const mXsdDoc;

        typesMap mTypes;
        elementsTypesMap mElementsTypes;
        //attributesMap mAttributes;
        attributesTypesMap mAttributesTypes;
    };
}

#endif //_H_XSD_CHECKER
