#include "XsdChecker.hpp"
#include "XsdException.hpp"
#include "../Xml/XmlParser.hpp"

#ifdef APP_DEBUG
#include <cassert>
#endif

namespace Xsd
{
    //TODO: ctrl + f : "TODO"

    const std::string Checker::SCHEMA_ELT = "schema";
    const std::string Checker::ELEMENT_ELT = "element";
    const std::string Checker::ATTRIBUTE_ELT = "attribute";
    const std::string Checker::COMPLEX_TYP_ELT = "complexType";
    const std::string Checker::SEQUENCE_ELT = "sequence";
    const std::string Checker::CHOICE_ELT = "choice";

    const std::string Checker::NAME_ATTR = "name";
    const std::string Checker::TYPE_ATTR = "type";
    const std::string Checker::REF_ATTR = "ref";
    const std::string Checker::USE_ATTR = "use";
    const std::string Checker::MIXED_ATTR = "mixed";
    const std::string Checker::MIN_OCC_ATTR = "minOccurs";
    const std::string Checker::MAX_OCC_ATTR = "maxOccurs";

    const std::string Checker::ROOT = "root";

    const std::string Checker::STRING_TYPE = "string";
    const std::string Checker::DATE_TYPE = "date";
    const std::string Checker::USE_REQUIRED_VALUE = "required";

    const std::string Checker::OR_SEPARATOR = "|";
    const std::string Checker::AND_SEPARATOR = "";

    Checker * Checker::instance;

    Checker::Checker(const Xml::Document * const xsdDoc):
        mXsdDoc(xsdDoc),
        mTypes(),
        mElementsTypes(),
        // mAttributes(),
        mAttributesTypes()
    {
        const std::string dateRegex = "[0-9]{2}-[0-9]{2}-[0-9]{4}";
        const std::string stringRegex = "*";
        new Type(DATE_TYPE, dateRegex, std::list<Attribute *>());
        new Type(STRING_TYPE, stringRegex, std::list<Attribute *>());

        if(!(xsdDoc->root()->name().compare(SCHEMA_ELT) == 0))
        {
            throwInvalidElementException(xsdDoc->root()->name(), SCHEMA_ELT);
        }

        std::map<std::string, std::string> rootAttributesMap = xsdDoc->root()->namespaceAttributes("xmlns");
        auto iterAttr = rootAttributesMap.begin();
        while (iterAttr != rootAttributesMap.end())
        {
            if(iterAttr->second.compare("http://www.w3.org/2001/XMLSchema") == 0)
            {
                namespacePrefix = iterAttr->first;
                break;
            }
            iterAttr++;
        }
        if (iterAttr == rootAttributesMap.end())
        {
            namespacePrefix = "";
        }

        //Building intermediary structure from xmlDoc
        std::list<Attribute *> attributes;
        Xsd::Type::parseComplexType(xsdDoc->root(), OR_SEPARATOR, true, attributes, true);

        checkReferences();

    }

    Checker::~Checker()
    {
        for(auto iterator = mTypes.begin(); iterator != mTypes.end(); iterator++)
        {
            delete iterator->second;
        }
        mTypes.clear();

        mElementsTypes.clear();

        // for(auto iterator = mAttributes.begin(); iterator != mAttributes.end(); iterator++)
        // {
        //     delete iterator->second;
        // }
        // mAttributes.clear();

        mAttributesTypes.clear();
    }

/*
    using typesMap = std::map<std::string, Type const *>;
    using elementsTypesMap = std::map<std::string, std::string>;
    using attributesTypesMap = std::map<std::string, std::string>;
*/
    void
    Checker::checkReferences()
    {
        //Check types, attributes and elements
        //Check that every type from attributesTypesMap and elementsTypesMap are in typesMap
        //Check that every attribute in every type has its type in elementsTypesMap
        //Type const * type;

        /*
            pour tous les elts
                check le type de l elt

            pour tous les types
                pour tous les attrs
                    check le type des attrs
        */

        for (auto iterEltType = mElementsTypes.begin(); iterEltType != mElementsTypes.end(); ++iterEltType)
        {
            checkExistType(iterEltType->second);
        }

        for (auto iterType = mTypes.begin(); iterType != mTypes.end(); ++iterType)
        {
            std::list<Attribute *> attributes = iterType->second->attributes();
            for (auto iterAttr = attributes.begin(); iterAttr != attributes.end(); ++iterAttr)
            {
                checkExistType((*iterAttr)->name());
            }
        }
    }

    void
    Checker::checkExistType(const std::string & typeName)
    {
        if(!existType(typeName))
        {
            throw new XSDConstructionException("Error: Reference cannot be resolved: " + typeName);
        }
    }



    bool
    Checker::existType(const std::string & typeName)
    {
        if(getInstance()->mTypes.find(typeName) == getInstance()->mTypes.end())
        {
            return false;
        }
        return true;
    }

    bool
    Checker::isValid(const Xml::Document * const xsdDoc)
    {
        try
        {
            getElementType(ROOT)->checkValidity(xsdDoc->root());
            return true;
        }
        catch(const XSDValidationException & e)
        {
            std::cerr << e.what() << std::endl;
            return false;
        }
    }

    void
    Checker::addType(const std::string & typeName, Type * const type)
    {
        mTypes.insert(std::pair<std::string, Type * const>(typeName, type));
    }

    void
    Checker::addTypedElement(const std::string & elementName, const std::string & typeName)
    {
        mElementsTypes.insert(std::pair<std::string, std::string>(elementName, typeName));
    }

/*
    void
    Checker::addAttribute(const std::string Checker::& attributeName, const Attribute * const attribute)
    {
        attributesMap.insert(std::pair<std::string, Attribute const *>(attributeName, attribute));
    }
*/

    void
    Checker::addTypedAttribute(const std::string & attributeName, const std::string & typeName)
    {
        mAttributesTypes.insert(std::pair<std::string, std::string>(attributeName, typeName));
    }

    Type *
    Checker::getType(const std::string & typeName)
    {
        auto iterType = mTypes.find(typeName);
        if(iterType == mTypes.end())
        {
            return NULL;
        }
        return iterType->second;
    }

    std::string
    Checker::getDateType()
    {
        if (getInstance()->namespacePrefix != "")
        {
            return getInstance()->namespacePrefix + ':' + DATE_TYPE;
        }
        else
        {
            return DATE_TYPE;
        }
    }

    std::string
    Checker::getStringType()
    {
        if (getInstance()->namespacePrefix != "")
        {
            return getInstance()->namespacePrefix + ':' + STRING_TYPE;
        }
        else
        {
            return STRING_TYPE;
        }
    }

    Type *
    Checker::getElementType(const std::string & elementName)
    {
        auto iterType = mElementsTypes.find(elementName);
        if(iterType == mElementsTypes.end())
        {
            return NULL;
        }
        std::string typeName = iterType->second;
        return getType(typeName);
    }

/*
    Attribute * const
    Checker::getAttribute(const std::string Checker::& attributeName)
    {
        Attribute * att = *(attributesMap.find(attributeName)).second;
        if(att == attributesMap.end())
        {
            return NULL;
        }
        return att;
    }
*/

    Type *
    Checker::getAttributeType(const std::string & attributeName)
    {
        auto iterType = mAttributesTypes.find(attributeName);
        if(iterType == mAttributesTypes.end())
        {
            return NULL;
        }
        std::string typeName = iterType->second;
        return getType(typeName);
    }

    Checker *
    Checker::getInstance()
    {
        return instance;
    }

    bool
    Checker::parseXsd(const Xml::Document * const xsdDoc)
    {

        if(instance != NULL)
        {
            std::cerr << "Error: An XSD document has already been parsed" << std::endl;
            return false;
        }
        if(xsdDoc == NULL)
        {
            std::cerr << "Error: The document received has not been initialized" << std::endl;
            return false;
        }

        try
        {
            instance = new Checker(xsdDoc);
            return true;
        }
        catch(const XSDValidationException & e)
        {
            std::cerr << e.what() << std::endl;
            return false;
        }
    }

    void
    Checker::throwInvalidElementException(const std::string & received, const std::string & expected)
    {
        throw new XSDConstructionException("Error: Invalid XSD root element received :" + received + " (" + expected + " expected");
    }

    void
    Checker::throwMissingAttributeException(const std::string & element, const std::string & missingAttr)
    {
        throw new XSDConstructionException("Error: Missing attribute for " + element + " element: " + missingAttr);
    }

    void
    Checker::throwInvalidAttributeValueException(const std::string & element, const std::string & attr, const std::string & invalidValue)
    {
        throw new XSDConstructionException("Error: Invalid " + attr + " attribute value for " + element + " element: " + invalidValue);
    }
}
