#include "XsdType.hpp"
#include "XsdChecker.hpp"

namespace Xsd
{

    const std::string TYPE_SUFFIX = "TYPE";
    const std::string UNBOUNDED = "unbounded";
    const std::string UNBOUNDED_EXP_REG = "*";

    Type::Type(const Xml::Element & xmlElement, const std::string & name)
    {
        mRegex = parseComplexType(xmlElement, "", false, );
        Checker::getInstance().addType(name, this);
    }

    Type::Type(const Xml::Element & xmlElement, const std::string & name)
    {
        mRegex = getRegexFromElement(xmlElement);
        Checker::getInstance().addType(name, this);
    }


    Type::Type(const std::string & name, const std::string & regex, std::list<Attribute *> attrs):
        mRegex(regex), mAttributes(attrs)
    {
        Checker::getInstance().addType(name, this);
    }

    Type::~Type()
    {

    }

    //Should work, still have to check the algorithm for choice or sequence inside choice or sequence
    static std::string
    Type::parseComplexType(const Xml::Element & xmlElement, std::string separator, bool eltSeqChoice)
    {
        bool eltParsed = false;
        std::string regex = "(";

        if(ci->attribute(Checker::MIXED_ATTR).compare("true") == 0)
        {
            separator += ".*";
        }

        for (std::list<Xml::Element>::const_iterator ci = xmlElement.elements().begin(); ci != xmlElement.elements().end(); ++ci)
        {
            if(ci->name().compare(Checker::SEQUENCE_ELT) == 0)
            {
                regex += getRegexFromOccurs(*ci, parseComplexType(*ci, "", true)) + separator;
            }
            else if(ci->name().compare(Checker::CHOICE_ELT) == 0)
            {
                regex += getRegexFromOccurs(*ci, parseComplexType(*ci, "|", true)) + separator;
            }
            else if(ci->name().compare(Checker.ELEMENT_ELT) == 0)
            {
                if(eltSeqChoice || !eltParsed)
                {
                    eltParsed = true;
                    regex += parseElement(*ci) + separator;
                }
                else
                {
                    Checker::throwInvalidElementException(Checker.ELEMENT_ELT, getNameOrRef(*ci);
                }
            }
            else if(ci->name().compare(Checker::ATTRIBUTE_ELT) == 0)
            {
                mAttributes.push_back(Xsd::Attribute.parseAttribute(*ci));
            }
            else
            {
                Checker::throwInvalidElementException(Checker::COMPLEX_TYPE_ELT, ci->name());
            }
        }

        if(regex.back() == '|')
        {
            regex = regex.substr(0, regex.size()-1);
        }

        return regex + ")";
    }

    static bool
    isSimpleType(const std::string & type)
    {
        return (type.compare(Checker::getInstance().getStringTypeValue()) == 0)
            || (type.compare(Checker::getInstance().getDateTypeValue()) == 0);
    }

    static std::string getOccursFromElement(const Xml::Element & xmlElement, const std::string & occursAttrName, const std::string & occursAttrValue)
    {
        if(occursAttrValue.compare(UNBOUNDED) == 0)
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
            Checker::throwInvalidAttributeValueException(Checker::ELEMENT_ELT, occursAttrName, occursAttrValue);
        }

        return occursAttrValue;
    }

    static bool
    Type::getNameOrRef(const Xml::Element & xmlElement)
    {
        if(isReference(xmlElement))
        {
            return xmlElement.attribute(Checker.REF_ATTR);
        }
        return xmlElement.attribute(Checker.NAME_ATTR);
    }

    static bool
    Type::isReference(const Xml::Element & xmlElement)
    {
        std::string notFound = "";
        std::string name = xmlElement.attribute(Checker::NAME_ATTR);
        std::string ref = xmlElement.attribute(Checker::REF_ATTR);

        // Name and ref attributes
        if((name.compare(notFound) == 0) && (ref.compare(notFound) != 0))
        {
            return true;
        }
        else
        {
            Checker::throwMissingAttributeException(Checker::ELEMENT_ELT, Checker::NAME_ATTR);
        }

        return false;
    }

    /**
     * Returns the regex of an element, adds its type and type relation to the maps if it's not a ref
     * The regex does not contain the validation for the element children, it's only about the element name or ref itself
     */
    static std::string
    Type::parseElement(const Xml::Element & xmlElement)
    {
        bool ref = false;
        std::string regex, name;

        // Name and ref attributes
        if(isReference(xmlElement))
        {
            name = xmlElement.attribute(Checker.REF_ATTR);
            ref = true;
        }
        else
        {
            name = xmlElement.attribute(Checker.NAME_ATTR);
        }

        regex = getRegexFromOccurs(xmlElement, name);

        if(!ref)
        {
            //Type manaement
            std::string type = xmlElement.attribute(Checker.TYPE_ATTR);
            if(!type.compare("") && Xsd::Type.isSimpleType(type))
            {
                std::vector<std::string> tokens;
                boost::algorithm::split(tokens, type, boost::algorithm::is_any_of(":"));
                Checker::getInstance().addTypedElement(name, tokens.back());

            }
            else if(xmlElement.elements().size() > 0)
            {
                Xml::Element & typeElement = xmlElement.elements().front();
                if(typeElement.name().compare(Checker::COMPLEX_TYPE_ELEMENT) == 0)
                {
                    parseComplexType(*ci, "", true);
                }
                else
                {
                    throw new XSDConstructionException("Error: type attribute or element cannot be found for " + Checker::ELEMENT_ELT + " element");
                }
            }
            else
            {
                throw new XSDConstructionException("Error: type attribute or element cannot be found for " + Checker::ELEMENT_ELT + " element");
            }
        }

        return regex;
    }

    /**
     * Modify and returns the element regex given in parameter in order to add
     * regex expression for the occurs attributes values
     */
    static std::string
    Type::getRegexFromOccurs(const Xml::Element & xmlElement, const std::string & eltRegex)
    {
        std::string notFound = "", regex;
        std::string name = xmlElement.attribute(Xsd::Checker::NAME_ATTR);
        std::string ref = xmlElement.attribute(Xsd::Checker::REF_ATTR);
        std::string minOccurs = xmlElement.attribute(Checker::MIN_OCC_ATTR);
        std::string supOccurs = xmlElement.attribute(Checker::MAX_OCC_ATTR);

        // Min and max occurs attributes
        if(!(minOccurs.compare(notFound) == 0))
        {
            minOccurs = getOccursFromElement(xmlElement, Checker::MIN_OCC_ATTR, sMinOccurs);
        }
        else
        {
            minOccurs = "1";
        }

        if(!(supOccurs.compare(notFound) == 0))
        {
            supOccurs = getOccursFromElement(xmlElement, Checker::MAX_OCC_ATTR, supOccurs);
        }
        else
        {
            supOccurs = "0";
        }

        if((minOccurs.compare(UNBOUNDED_EXP_REG) == 0) && !(supOccurs.compare(UNBOUNDED_EXP_REG) == 0)
            || (!(supOccurs.compare(UNBOUNDED_EXP_REG) ==0) && std::stoi(supOccurs) < std::stoi(minOccurs)))
        {
            throw new XSDConstructionException("Error: " Checker::MIN_OCC_ATTR + " attribute value is higher than " + Checker::MAX_OCC_ATTR + " value");
        }
        if((minOccurs.compare(UNBOUNDED_EXP_REG) == 0) && (supOccurs.compare(UNBOUNDED_EXP_REG) == 0))
        {
            stringstream out;
            out << (std::stoi(supOccurs) - std::stoi(minOccurs);
            supOccurs = out.str();
        }

        // Name and ref attributes
        if(isReference(xmlElement))
        {
            name = ref;
        }

        // Create the regex
        regex = "(<" + eltRegex + ">){" + minOccurs + "}";
        if(!(supOccurs.compare("0") == 0))
        {
            regex += "((<" + eltRegex + ">?){" + supOccurs + "})";
        }

        return regex;
    }

    std::list<Attribute *>
    attributes() const
    {
        return mAttributes;
    }
}
