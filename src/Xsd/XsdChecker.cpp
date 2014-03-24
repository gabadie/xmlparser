#include "XsdChecker.hpp"
#include "../Xml/XmlParser.hpp"
#include "XsdException.hpp"

#ifdef APP_DEBUG
#include <cassert>
#endif

namespace Xsd
{
    Checker::Checker(const Xml::Document & xsdDoc):
        mXsdDoc(xsdDoc),
        mTypes(),
        mElementsTypes(),
        mAttributes(),
        mAttributesTypes()
    {
        if(!xsdDoc.attribute(NAME_ATTR).equals(SCHEMA_ELT))
        {
            throwInvalidElementException(xsdDoc.attribute(NAME_ATTR), SCHEMA_ELT);
        }


        namespacePrefix = "TODO";
        stringTypeValue = "TODO";
        dateTypeValue = "TODO";

        //Check that the root element has xmlns:MYPREFIX="http://www.w3.org/2001/XMLSchema"
        //or noNamespaceSchemaLocation
        //Quand on parse un type complexe, ajouter ses attributs à la map !
        throw NotImplementedYet("TODO : gestion namespace !!!!!");

        //Building intermediary structure from xmlDoc
        Xsd::Type * rootType = new Xsd::Type(xsdDoc);

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
        throw new XSDConstructionException("Invalid XSD root element received :" + received + " (" + expected + " expected");
    }

    static void throwMissingAttributeException(const std::string & element, const std::string & missingAttr)
    {
        throw new XSDConstructionException("Missing attribute for " + element + " element: " + missingAttr);
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

    static void
    Checker::addType(const std::string & typeName, const Type * const type)
    {
        typesMap.insert(typeName, type);
    }

    static void
    Checker::addTypedElement(const std::string & elementName, const std::string & typeName)
    {
        elementsTypesMap.insert(elementName, typeName);
    }

    static void
    Checker::addAttribute(const std::string & attributeName, const Attribute * const attribute)
    {
        attributesMap.insert(attributeName, attribute);
    }

    static void
    Checker::addTypedAttribute(const std::string & attributeName, const std::string & typeName)
    {
        attributesTypesMap.insert(attributeName, typeName);
    }

    static Type * const
    Checker::getType(const std::string & typeName)
    {
        return typesMap[typeName];
    }

    static Type * const
    Checker::getElementType(const std::string & elementName)
    {
        return getType(elementsTypesMap[elementName]);
    }

    static Attribute * const
    Checker::getAttribute(const std::string & attributeName)
    {
        return attributesMap[attributeName];
    }

    static Type * const
    Checker::getAttributeType(const std::string & attributeName)
    {
        return getType(attributesTypesMap[attributeName]);
    }
}
