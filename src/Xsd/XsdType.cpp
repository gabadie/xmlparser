#include "XsdType.hpp"
#include "XsdChecker.hpp"

namespace Xsd
{
    Type::Type(const Xml::Element & xmlElement)
    {
        mName = xmlElement.attribute(Checker.NAME_ATTR);

        if(mName == null)
        {
            Checker.throwMissingAttributeException(Checker.COMPLEX_TYP_ELT, Checker.NAME_ATTR);
        }

        mRegex = getRegexFromElement(xmlElement);
        Xsd::Checker.addType(name, &this);
    }

    Type::Type(const Xml::Element & xmlElement, std::string & name):
    {
        mRegex = getRegexFromElement(xmlElement);
        Xsd::Checker.addType(name, &this);
    }

    Type::~Type()
    {

    }

    static std::string
    Type::parseComplexType(const Xml::Element & xmlElement)
    {
        std::string regex = "";


static const std::string NAME_ATTR = "name";
static const std::string TYPE_ATTR = "type";
static const std::string REF_ATTR = "ref";
static const std::string USE_ATTR = "use";
static const std::string MIN_OCC_ATTR = "minOccurs";
static const std::string MAX_OCC_ATTR = "maxOccurs";

        for (std::list<Xml::Element>::const_iterator ci = xmlElement.elements().begin(); ci != xmlElement.elements().end(); ++ci)
        {
            if(ci->name().equals(Checker.SEQUENCE_ELT))
            {
                std
            }
            else if(ci->name().equals(Checker.CHOICE_ELT)
            {

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

    static std::string
    Type::getRegexFromElement(const Xml::Element & xmlElement)
    {
        std::string name = xmlElement.attribute("name");
        std::string ref = xmlElement.attribute("ref");
        std::string min = xmlElement.attribute("minOccurs");
        std::string max = xmlElement.attribute("maxOccurs");

        std::string regex;
        std::string attributeName;
        std::string minOccurs = "1";
        std::string maxOccurs = "*";

        // Get the element name
        if(name != "")
        {
            attributeName = name;
        }
        else if(ref != "")
        {
            attributeName = ref;
        }
        else
        {
            // TODO Gestion exception
            attributeName = "";
            std::cout << "Error : Attribute element doesn't have any name attribute" << std::endl;
        }

        // Get the occurence
        if(min != "")
        {
            minOccurs = min;
        }
        if(max != "")
        {
            maxOccurs = max;
        }

        // Create the regex
        regex = "<(" + attributeName + ">){" + minOccurs + "})((<" + attributeName + ">?){" + maxOccurs + "}";
        return regex;
    }

    std::list<Attribute *>
    attributes()
    {
        return mAttributes;
    }
}
