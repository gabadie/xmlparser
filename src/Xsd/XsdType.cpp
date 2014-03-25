#include "XsdType.hpp"
#include "XsdChecker.hpp"

namespace Xsd
{
    Type::Type(const Xml::Element & xmlElement)
    {
        std::string name = xmlElement.attribute(Checker.NAME_ATTR);

        if(name == null)
        {
            name = xmlElement.parentElement->attribute(Checker.NAME_ATTR) + TYPE_SUFFIX;
        }

        mRegex = getRegexFromElement(xmlElement);
        Xsd::Checker.addType(name, &this);
    }

    Type::Type(const Xml::Element & xmlElement, const std::string & name):
    {
        mRegex = getRegexFromElement(xmlElement);
        Xsd::Checker.addType(name, &this);
    }


    Type::Type(const std::string & name, const std::string & const regex, std::list<Attribute *> attrs):
        mRegex(regex), mAttributes(attrs)
    {
        Xsd::Checker.addType(name, &this);
    }

    Type::~Type()
    {

    }

    static std::string
    Type::parseComplexType(const Xml::Element & xmlElement) const
    {
        bool eltParsed = false;
        std::string regex = "";

        for (std::list<Xml::Element>::const_iterator ci = xmlElement.elements().begin(); ci != xmlElement.elements().end(); ++ci)
        {
            if(ci->name().compare(Checker.SEQUENCE_ELT))
            {
                parseComplexType(*ci, "", true);
            }
            else if(ci->name().compare(Checker.CHOICE_ELT)
            {
                parseComplexType(*ci, "|", true);
            }
            else if(ci->name().compare(Checker.ELEMENT_ELT)
            {

            }
            else if(ci->name().compare(Checker.ATTRIBUTE_ELT)
            {
                mAttributes.push_back(new Xsd::Attribute(*ci));
            }
            else
            {
                Checker.throwInvalidElementException(Checker.COMPLEX_TYPE_ELT, ci->name());
            }
        }

        return regex;
    }

    static bool
    isSimpleType(const std::string & type)
    {
        return type.compare(Checker..getInstance().getStringTypeValue())
            || type.compare(Checker..getInstance().getDateTypeValue());
    }

    static std::string getOccursFromElement(const Xml::Element & xmlElement, const std::string & occursAttrName, const std::string & occursAttrValue)
    {
        if(occurs.compare(UNBOUNDED))
        {
            return UNBOUNDED_EXP_REG;
        }

        try
        {
            if(std::stoi(occursAttrValue) < 0)
            {
                throw new Exception();
            }
        }
        catch(const std::exception& e)
        {
            Checker.throwInvalidAttributeValueException(Checker.ELEMENT_ELT, occursAttrName, occursAttrValue);
        }

        return occursAttrValue;
    }

    static std::string
    Type::getRegexFromElement(const Xml::Element & xmlElement)
    {
        std::string notFound = "", regex;
        std::string name = xmlElement.attribute(Xsd::Checker.NAME_ATTR);
        std::string ref = xmlElement.attribute(Xsd::Checker.REF_ATTR);
        std::string minOccurs = xmlElement.attribute(Checker.MIN_OCC_ATTR);
        std::string supOccurs = xmlElement.attribute(Checker.MAX_OCC_ATTR);

        // Min and max occurs attributes
        if(!minOccurs.compare(notFound))
        {
            minOccurs = getOccursFromElement(xmlElement, Checker.MIN_OCC_ATTR, sMinOccurs);
        }
        else
        {
            minOccurs = "1";
        }

        if(!supOccurs.compare(notFound))
        {
            supOccurs = getOccursFromElement(xmlElement, Checker.MAX_OCC_ATTR, supOccurs);
        }
        else
        {
            supOccurs = "0";
        }

        if(minOccurs.compare(UNBOUNDED_EXP_REG) && !supOccurs.compare(UNBOUNDED_EXP_REG)
            || (!supOccurs.compare(UNBOUNDED_EXP_REG) && std::stoi(supOccurs) < std::stoi(minOccurs)))
        {
            throw new XSDConstructionException("Error: " Checker.MIN_OCC_ATTR + " attribute value is higher than " + Checker.MAX_OCC_ATTR + " value");
        }
        if(minOccurs.compare(UNBOUNDED_EXP_REG) && supOccurs.compare(UNBOUNDED_EXP_REG))
        {
            stringstream out;
            out << (std::stoi(supOccurs) - std::stoi(minOccurs);
            supOccurs = out.str();
        }

        // Name and ref attributes
        if(name.compare(notFound) && !ref.compare(notFound))
        {
            name = ref;
        }
        else
        {
            Checker.throwMissingAttributeException(Checker.ELEMENT_ELT, Checker.NAME_ATTR);
        }

        // Create the regex
        regex = "(<" + name + ">){" + minOccurs + "}";
        if(!supOccurs.compare("0"))
        {
            regex += "((<" + name + ">?){" + supOccurs + "})";
        }

        return regex;
    }

    std::list<Attribute *>
    attributes() const
    {
        return mAttributes;
    }
}
