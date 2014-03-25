#include "XsdChecker.hpp"
#include "../Xml/XmlParser.hpp"
#include "XsdException.hpp"

#ifdef APP_DEBUG
#include <cassert>
#endif

namespace Xsd
{

    const std::string SCHEMA_ELT = "schema";
    const std::string ELEMENT_ELT = "element";
    const std::string ATTRIBUTE_ELT = "attribute";
    const std::string COMPLEX_TYP_ELT = "complexType";
    const std::string SEQUENCE_ELT = "sequence";
    const std::string CHOICE_ELT = "choice";

    const std::string NAME_ATTR = "name";
    const std::string TYPE_ATTR = "type";
    const std::string REF_ATTR = "ref";
    const std::string USE_ATTR = "use";
    const std::string MIN_OCC_ATTR = "minOccurs";
    const std::string MAX_OCC_ATTR = "maxOccurs";

    const std::string ROOT = "root";

    const std::string STRING_TYPE_VALUE = "string";
    const std::string DATE_TYPE_VALUE = "date";
    const std::string USE_REQUIRED_VALUE = "required";

    Checker::Checker(const Xml::Document & xsdDoc):
        mXsdDoc(&xsdDoc),
        mTypes(),
        mElementsTypes(),
        mAttributes(),
        mAttributesTypes()
    {
        const std::string dateRegex = "[0-9]{2}-[0-9]{2}-[0-9]{4}";
        const std::string stringRegex = "*";
        new Type(DATE_TYPE_VALUE, dateRegex, std::NULL);
        new Type(STRING_TYPE_VALUE, stringRegex, std::NULL);

        if(!(xsdDoc.root()->attribute(NAME_ATTR).compare(SCHEMA_ELT) == 0))
        {
            throwInvalidElementException(xsdDoc.root()->attribute(NAME_ATTR), SCHEMA_ELT);
        }

        namespacePrefix = "TODO";
        stringTypeValue = namespacePrefix + ":" + STRING_TYPE_VALUE;
        dateTypeValue = namespacePrefix + ":" + DATE_TYPE_VALUE;

        //Check that the root element has xmlns:MYPREFIX="http://www.w3.org/2001/XMLSchema"
        //or noNamespaceSchemaLocation
        //Quand on parse un type complexe, ajouter ses attributs à la map !
        throw NotImplementedYet("TODO : gestion namespace !!!!!");

        //Building intermediary structure from xmlDoc
        Xsd::Type * rootType = new Xsd::Type(xsdDoc);

        //TODO
        //Check if every reference is linked to a type or an attribute

    }

    Checker::~Checker()
    {
        delete mXsdDoc;

        for(typesMap::iterator iterator = mTypes.begin(); iterator != mTypes.end(); iterator++)
        {
            delete iterator->second;
        }
        mTypes.clear();

        mElementsTypes.clear();

        for(attributesMap::iterator iterator = mAttributes.begin(); iterator != mAttributes.end(); iterator++)
        {
            delete iterator->second;
        }
        mAttributes.clear();

        mAttributesTypes.clear();
    }


    static Checker
    Checker::getInstance(const Xml::Document & xsdDoc)
    {
        return instance;
    }

    static void throwInvalidElementException(const std::string & received, const std::string & expected)
    {
        throw new XSDConstructionException("Error: Invalid XSD root element received :" + received + " (" + expected + " expected");
    }

    static void throwMissingAttributeException(const std::string & element, const std::string & missingAttr)
    {
        throw new XSDConstructionException("Error: Missing attribute for " + element + " element: " + missingAttr);
    }

    static void throwInvalidAttributeValueException(const std::string & element, const std::string & attr, const std::string & invalidValue)
    {
        throw new XSDConstructionException("Error: Invalid " + attr + " attribute value for " + element + " element: " + invalidValue);
    }

    static void
    Checker::initialize(const Xml::Document & xsdDoc)
    {
        if(instance == NULL)
        {
            intance = Checker(xmlDoc);
        }
    }

    bool
    Checker::isValid(Xml::Document & xmlDoc)
    {
        std::cerr << __func__ << " : not implemented yet" << std::endl;
        __builtin_trap();

        try
        {
            //TODO: Algo validation
            //Check with the namespace or noNamespaceSchemaLocation
            return true;
        }
        catch(XSDConstructionException e)
        {
            std::cerr << e.getMessage() << std:endl;
            return false;
        }
    }

    std::string
    Checker::getStringTypeValue()
    {
        return stringTypeValue;
    }

    std::string
    Checker::getDateTypeValue()
    {
        return dateTypeValue;
    }

    void
    Checker::addType(const std::string & typeName, const Type * const type)
    {
        typesMap.insert(std::pair<std::string, Type const *>(typeName, type));
    }

    void
    Checker::addTypedElement(const std::string & elementName, const std::string & typeName)
    {
        elementsTypesMap.insert(std::pair<std::string, std::string>(elementName, typeName));
    }

    void
    Checker::addAttribute(const std::string & attributeName, const Attribute * const attribute)
    {
        attributesMap.insert(std::pair<std::string, Attribute const *>(attributeName, attribute));
    }

    void
    Checker::addTypedAttribute(const std::string & attributeName, const std::string & typeName)
    {
        attributesTypesMap.insert(std::pair<std::string, std::string>(attributeName, typeName));
    }

    Type * const
    Checker::getType(const std::string & typeName)
    {
        Type * type = typesMap.find(typeName);
        if(type == typesMap::end)
        {
            return NULL;
        }
        return type;
    }

    Type * const
    Checker::getElementType(const std::string & elementName)
    {
        std::string typeName = elementsTypesMap.find(elementName);
        if(type == elementsTypesMap::end)
        {
            return NULL;
        }
        return getType(typeName);
    }

    Attribute * const
    Checker::getAttribute(const std::string & attributeName)
    {
        Type * type = attributesMap.find(attributeName);
        if(type == attributesMap::end)
        {
            return NULL;
        }
        return type;
    }

    Type * const
    Checker::getAttributeType(const std::string & attributeName)
    {
        std::string typeName = attributesTypesMap.find(attributeName);
        if(typeName == attributesTypesMap::end)
        {
            return NULL;
        }
        return getType(typeName);
    }
}
