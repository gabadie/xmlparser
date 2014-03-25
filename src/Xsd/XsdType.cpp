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
    Type::parseComplexType(const Xml::Element & xmlElement, const std::string & separator, bool eltSeqChoice)
    {
        bool eltParsed = false;
        std::string regex = "";

        for (std::list<Xml::Element>::const_iterator ci = xmlElement.elements().begin(); ci != xmlElement.elements().end(); ++ci)
        {
            if(ci->name().equals(Checker.SEQUENCE_ELT))
            {
                parseComplexType(*ci, "", true);
            }
            else if(ci->name().equals(Checker.CHOICE_ELT))
            {
                parseComplexType(*ci, "|", true);
            }
            else if(ci->name().equals(Checker.ELEMENT_ELT)
            {

            }
            else if(ci->name().equals(Checker.ATTRIBUTE_ELT)
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
        return type.equals(checker..getInstance().getStringTypeValue())
            || type.equals(checker..getInstance().getDateTypeValue());
    }

    static std::string getOccursFromElement(const Xml::Element & xmlElement, const std::string & occursAttrName, const std::string & occursAttrValue)
    {
        if(occurs.equals(UNBOUNDED))
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
        catch(Exception e)
        {
            Checker.throwInvalidAttributeValueException(Checker.ELEMENT_ELT, occursAttrName, occursAttrValue);
        }

        return occursAttrValue;
    }

    static std::string
    Type::getRegexFromElement(const Xml::Element & xmlElement)
    {
        std::string notFound = "", regex;
        std::string name = xmlElement.attribute(Checker.NAME_ATTR);
        std::string ref = xmlElement.attribute(Checker.REF_ATTR);
        std::string minOccurs = xmlElement.attribute(Checker.MIN_OCC_ATTR);
        std::string maxOccurs = xmlElement.attribute(Checker.MAX_OCC_ATTR);

        // Min and max occurs attributes
        if(!minOccurs.equals(notFound))
        {
            minOccurs = getOccursFromElement(xmlElement, Checker.MIN_OCC_ATTR, sMinOccurs);
        }
        else
        {
            minOccurs = "1";
        }

        if(!supOccurs.equals(notFound))
        {
            supOccurs = getOccursFromElement(xmlElement, Checker.MAX_OCC_ATTR, supOccurs);
        }
        else
        {
            supOccurs = "0";
        }

        if(minOccurs.equals(UNBOUNDED_EXP_REG) && !supOccurs.equals(UNBOUNDED_EXP_REG)
            || (!supOccurs.equals(UNBOUNDED_EXP_REG) && std::stoi(supOccurs) < std::stoi(minOccurs)))
        {
            throw new XSDConstructionException("Error: " Checker.MIN_OCC_ATTR + " attribute value is higher than " + Checker.MAX_OCC_ATTR + " value");
        }
        if(minOccurs.equals(UNBOUNDED_EXP_REG) && supOccurs.equals(UNBOUNDED_EXP_REG))
        {
            stringstream out;
            out << (std::stoi(supOccurs) - std::stoi(minOccurs);
            supOccurs = out.str();
        }

        // Name and ref attributes
        if(name.equals(notFound) && !ref.equals(notFound))
        {
            name = ref;
        }
        else
        {
            Checker.throwMissingAttributeException(Checker.ELEMENT_ELT, Checker.NAME_ATTR);
        }

        // Create the regex
        regex = "(<" + name + ">){" + minOccurs + "}";
        if(!supOccurs.equals("0"))
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
