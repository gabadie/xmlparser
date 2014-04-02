/**
 * \file XsdAttribute.cpp
 * \brief XSD Attribute
 * \author jcourlet
 * \version 0.1
 * \date 18 mars 2014
 */

#include "XsdAttribute.hpp"
#include "XsdException.hpp"
#include "XsdChecker.hpp"

#ifdef APP_DEBUG
#include <cassert>
#endif

namespace Xsd
{
    Attribute::Attribute(const std::string & name, bool required, const std::string & typeName, bool ref, Checker * checker)
    {
        init(name, required, typeName, ref, checker);
    }

    void
    Attribute::init(const std::string & name, bool required, const std::string & typeName, bool ref, Checker * checker)
    {
        mName = name;
        mRequired = required;
        //Xsd::Checker::addAttribute(name, this);
        if(!ref)
        {
            checker->addTypedAttribute(name, typeName);
        }

    }

    Attribute *
    Attribute::parseAttribute(const Xml::Element * const xmlElement, Checker * checker)
    {
        bool required = false, isRef = false;
        std::string notFound = "";
        std::string name = xmlElement->attribute(checker->NAME_ATTR);
        std::string ref = xmlElement->attribute(checker->REF_ATTR);
        std::string type = xmlElement->attribute(checker->TYPE_ATTR);
        std::string use = xmlElement->attribute(checker->USE_ATTR);

        if(name != notFound)
        {
            // Add an attribute to the attribute type map
            if(type != notFound && Xsd::Type::isSimpleType(type, checker))
            {
                    checker->addTypedAttribute(name, type);
            }
            else
            {
                checker->throwInvalidAttributeValueException(name, checker->TYPE_ATTR, type);
            }
        }
        else if(ref != notFound)
        {
            name = ref;
            isRef = true;
        }
        else
        {
            checker->throwMissingAttributeException(name, checker->TYPE_ATTR);
        }

        if(use != notFound)
        {
            if(use == checker->USE_REQUIRED_VALUE)
            {
                required = true;
            }
            else
            {
                checker->throwInvalidAttributeValueException(name, checker->USE_ATTR, use);
            }
        }

        return new Attribute(name, required, type, isRef, checker);
    }

    void
    Attribute::checkValidity(const std::string & value, Checker * checker)
    {
        if(!checker->getAttributeType(mName)->isValid(value))
        {
            throw new XSDValidationException("Error: Invalid attribute: " + mName);
        }
    }

    std::string
    Attribute::name()
    {
        return mName;
    }

    bool
    Attribute::isRequired()
    {
        return mRequired;
    }
}
