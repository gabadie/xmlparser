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
    using typesMap = std::map<std::string, Type * const>;
    using elementsTypesMap = std::map<std::string, std::string>;
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
        static const std::string USE_ATTR;
        static const std::string MIXED_ATTR;
        static const std::string MIN_OCC_ATTR;
        static const std::string MAX_OCC_ATTR;

        static const std::string ROOT;

        static const std::string STRING_TYPE;
        static const std::string DATE_TYPE;
        static const std::string USE_REQUIRED_VALUE;

        static const std::string OR_SEPARATOR;
        static const std::string AND_SEPARATOR;

        /**
         * Lauches the parsing of an xsd document
         *
         * @param The xsd document
         *
         * @return True if the validation is of, false otherwise
         */
        static Checker *
        parseXsd(const Xml::Document * const xsdDoc);

        /**
         * Destructor
         */
        virtual ~Checker();

        /**
         * Checks if all the reference are included in the associated map
         */
        void
        checkReferences();

        /**
         * Check if an xsd document is valid
         *
         * @param xsdDoc The xsd document
         *
         * @return True if the XML file respects the XML schema definition
         */
        bool
        isValid(const Xml::Document * const xsdDoc);

        /**
         * Add the type to the type map
         *
         * @param typeName The type name
         * @param type The type
         */
        void
        addType(const std::string & typeName, Type * const type);

        /**
         * Add the element to the element map
         *
         * @param elementName The element name
         * @param typeName The type name
         */
        void
        addTypedElement(const std::string & elementName, const std::string & typeName);

        /**
         * Add the attribute to the attribute map
         *
         * @param attributeName The attribute name
         * @param typeName The type name
         */
        void
        addTypedAttribute(const std::string & attributeName, const std::string & typeName);

        /**
         * Checks if the type exists in the type map
         *
         * @param typeName The type name
         */
        void
        checkExistType(const std::string & typeName);

        /**
         * Checks if the type exists in the type map
         *
         * @param typeName The type name
         *
         * @return True if the type exists, false otherwise
         */
        bool
        existType(const std::string & typeName);

        /**
         * Gets the type associated to the type name
         *
         * @param typeName The type name
         *
         * @return The type
         */
        Type *
        getType(const std::string & typeName);

        /**
         * Initialize the date type value for the type attribute
         */
        void
        initDateType();

        /**
         * Initialize the string type value for the type attribute
         */
        void
        initStringType();

        /**
         * Get the date type
         *
         * @return the namespace concatenated with the date string
         */
        std::string
        getDateType();

        /**
         * Get the string type
         *
         * @return the namespace concatenated with the "string" string
         */
        std::string
        getStringType();

        /**
         * Get the type associated to an element
         *
         * @param elementName The element Name
         *
         * @return The type
         */
        Type *
        getElementType(const std::string & elementName);

        /**
         * Get the type associated to an attribute
         *
         * @param attributreName The attribute Name
         *
         * @return The type
         */
        Type *
        getAttributeType(const std::string & attributeName);

        /**
         * Throw an invalid element exception
         *
         * @param received The string received
         * @param expected The string expected
         */
        static void
        throwInvalidElementException(const std::string & received, const std::string & expected);

        /**
         * Throw a missing attribute exception
         *
         * @param element The element name
         * @param missingAttr The missing attribute
         */
        static void
        throwMissingAttributeException(const std::string & element, const std::string & missingAttr);

        /**
         * Throw an invalid attribute value exception
         *
         * @param element The element name
         * @param attr The invalid name of the attribute
         * @param invalidValue The invalid value of the attribute
         */
        static void
        throwInvalidAttributeValueException(const std::string & element, const std::string & attr, const std::string & invalidValue);


    protected:
        /**
         * Constructor
         */
        Checker(const Xml::Document * const xmlDoc);

        std::string mNamespacePrefix;           /// the namespace of the xsd document
        std::string mDateType;                  /// the string associated to the date type
        std::string mStringType;                /// the string associated to the string type

        const Xml::Document * const mXsdDoc;    /// the xsd document

        typesMap mTypes;                        /// the type map
        elementsTypesMap mElementsTypes;        /// the element/type map
        attributesTypesMap mAttributesTypes;    /// the attribute/type map
    };
}

#endif //_H_XSD_CHECKER
