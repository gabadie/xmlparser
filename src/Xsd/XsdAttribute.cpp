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
    Attribute::Attribute(const std::string & name, bool required, const std::string & typeName, bool ref)
    {
        init(name, required, typeName, ref);
    }

    void
    Attribute::init(const std::string & name, bool required, const std::string & typeName, bool ref)
    {
        mName = name;
        mRequired = required;
        //Xsd::Checker::addAttribute(name, this);
        if(!ref)
        {
            Xsd::Checker::getInstance()->addTypedAttribute(name, typeName);
        }

    }

    Attribute *
    Attribute::parseAttribute(const Xml::Element * const xmlElement)
    {
        bool required = false, isRef = false;
        std::string notFound = "";
        std::string name = xmlElement->attribute(Checker::NAME_ATTR);
        std::string ref = xmlElement->attribute(Checker::REF_ATTR);
        std::string type = xmlElement->attribute(Checker::TYPE_ATTR);
        std::string use = xmlElement->attribute(Checker::USE_ATTR);

        if(!(name.compare(notFound) == 0))
        {
            // Add an attribute to the attribute type map
            if(!(type.compare(notFound) == 0) && Xsd::Type::isSimpleType(type))
            {
                    std::vector<std::string> tokens;
                    //throw new NotImplementedException("Not implemented yet");
                    //boost::algorithm::split(tokens, type, boost::algorithm::is_any_of(":"));
                    Checker::getInstance()->addTypedElement(name, tokens.back());
            }
            else
            {
                Checker::throwInvalidAttributeValueException(name, Checker::TYPE_ATTR, type);
            }
        }
        else if(ref.compare(notFound) != 0)
        {
            name = ref;
            isRef = true;
        }
        else
        {
            Checker::throwMissingAttributeException(name, Checker::TYPE_ATTR);
        }

        if(!(use.compare(notFound) == 0))
        {
            if(use.compare(Checker::USE_REQUIRED_VALUE) == 0)
            {
                required = true;
            }
            else
            {
                Checker::throwInvalidAttributeValueException(name, Checker::USE_ATTR, use);
            }
        }

        return new Attribute(name, required, type, isRef);
    }

    void
    Attribute::checkValidity(const std::string & value)
    {
        if(!Checker::getInstance()->getAttributeType(mName)->isValid(value))
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