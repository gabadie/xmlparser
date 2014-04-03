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
    Attribute::Attribute(std::string const & name, bool required):
        mName(name),
        mRequired(required)
    {
    }


    Attribute::~Attribute()
    {
    }

    Attribute *
    Attribute::parseAttribute(Xml::Element const * const xmlElement, Checker * checker)
    {
        bool required = false;
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

        return new Attribute(name, required);
    }

    void
    Attribute::checkValidity(std::string const & value, Checker * checker)
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
