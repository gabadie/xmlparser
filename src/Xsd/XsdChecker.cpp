/**
 * \file XsdChecker.cpp
 * \brief XSD Checker
 * \author rdomingues
 * \version 0.1
 * \date 18 mars 2014
 */

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


    Checker::Checker(const Xml::Document * const xsdDoc):
        mXsdDoc(xsdDoc),
        mTypes(),
        mElementsTypes(),
        // mAttributes(),
        mAttributesTypes()
    {
        const std::string dateRegex = "[0-9]{2}-[0-9]{2}-[0-9]{4}";
        const std::string stringRegex = ".*";

        if(xsdDoc->root()->name() != SCHEMA_ELT)
        {
            throwInvalidElementException(xsdDoc->root()->name(), SCHEMA_ELT);
        }

        std::map<std::string, std::string> rootAttributesMap = xsdDoc->root()->namespaceAttributes("xmlns");
        auto iterAttr = rootAttributesMap.begin();
        while (iterAttr != rootAttributesMap.end())
        {
            if(iterAttr->second == "http://www.w3.org/2001/XMLSchema")
            {
                mNamespacePrefix = iterAttr->first;
                break;
            }
            iterAttr++;
        }
        if (iterAttr == rootAttributesMap.end())
        {
            mNamespacePrefix = "";
        }

        initDateType();
        initStringType();

        Type * typeDate = new Type(dateRegex, NULL);
        addType(mDateType, typeDate);

        Type * typeString = new Type(stringRegex, NULL);
        addType(mStringType, typeString);
    }

    Checker::~Checker()
    {
        for(auto iterator : mTypes)
        {
            delete iterator.second;
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

    void
    Checker::checkReferences()
    {
        //Check types, attributes and elements
        //Check that every type from attributesTypesMap and elementsTypesMap are in typesMap
        //Check that every attribute in every type has its type in elementsTypesMap
        //Type const * type;


        for (auto iterEltType : mElementsTypes)
        {
            checkExistType(iterEltType.second);
        }

        for (auto iterType : mTypes)
        {
            std::map<std::string, Attribute *> * attributes = iterType.second->attributes();
            for (auto iterAttr : *attributes)
            {
                checkExistType(iterAttr.second->name());
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
        if(mTypes.find(typeName) == mTypes.end())
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
            Xsd::Type::checkRootValidity(xsdDoc->root(), this);
            return true;
        }
        catch(XSDValidationException * e)
        {
            std::cerr << e->what() << std::endl;
            delete e;
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

    void
    Checker::initDateType()
    {
        if (mNamespacePrefix != "")
        {
            mDateType = mNamespacePrefix + ':' + DATE_TYPE;
        }
        else
        {
            mDateType = DATE_TYPE;
        }
    }

    void
    Checker::initStringType()
    {
        if (mNamespacePrefix != "")
        {
            mStringType = mNamespacePrefix + ':' + STRING_TYPE;
        }
        else
        {
            mStringType = STRING_TYPE;
        }
    }


    std::string
    Checker::getDateType()
    {
        return mDateType;
    }

    std::string
    Checker::getStringType()
    {
        return mStringType;
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
    Checker::parseXsd(const Xml::Document * const xsdDoc)
    {
        if(xsdDoc == NULL)
        {
            std::cerr << "Error: The document received has not been initialized" << std::endl;
            return NULL;
        }
        try
        {
            Checker * checker = new Checker(xsdDoc);
            //Building intermediary structure from xmlDoc
            std::map<std::string, Attribute *> attributes;
            std::string rootRegex = Xsd::Type::parseComplexType(xsdDoc->root(), OR_SEPARATOR, true, &attributes, true, checker);
            checker->addType(ROOT + "Type", new Type(rootRegex, &attributes));
            checker->addTypedElement(ROOT, ROOT + "Type");

            checker->checkReferences();
            return checker;
        }
        catch(XSDConstructionException * e)
        {
            std::cerr << e->what() << std::endl;
            delete e;
            return NULL;
        }


    }

    void
    Checker::throwInvalidElementException(const std::string & received, const std::string & expected)
    {
        throw new XSDConstructionException("Error: Invalid XSD element received: " + received + " (" + expected + " expected)");
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
