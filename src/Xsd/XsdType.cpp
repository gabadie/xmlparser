#include "XsdType.hpp"
#include "XsdChecker.hpp"
#include <sstream>
#include "../Utils.hpp"

namespace Xsd
{

    const std::string Type::TYPE_SUFFIX = "TYPE";
    const std::string Type::UNBOUNDED = "unbounded";
    const std::string Type::UNBOUNDED_EXP_REG = "*";

    Type::Type(const Xml::Element * const xmlElement, const std::string & name)
    {
        mRegex = parseComplexType(xmlElement, "", false, mAttributes, true);
        Checker::getInstance()->addType(name, this);
    }


    Type::Type(const std::string & name, const std::string & regex, std::list<Attribute *> attrs):
        mRegex(regex),
        mAttributes(attrs)
    {
        Checker::getInstance()->addType(name, this);
    }

    Type::~Type()
    {
        for(auto iterator = mAttributes.begin(); iterator != mAttributes.end(); iterator++)
        {
            delete *iterator;
        }
        mAttributes.clear();
    }

    bool
    Type::isValid(const std::string & str)
    {
        return RE2::FullMatch(str, mRegex);
    }

    void
    Type::checkValidity(const Xml::Element * const element)
    {
        //TODO: add function attributes() which returns the mAttributes map in Xml::Element
        for (auto iterAttr = element->attributesValue().begin(); iterAttr != element->attributesValue().end(); ++iterAttr)
        {
            //pour chaque attibut d'un element
            //  obtenir l'attribut xsd avec une map depuis le type (map attribut de type)
            //  xsdAttribute->checkValidity(iterAttr->second)
            // mAttributes.find(iterAttr->first)
            // iterAttr->checkValidity(iterAttr->second);
        }

        if(!RE2::FullMatch(childrenToString(element->elements()), mRegex))
        {
            throw new XSDValidationException("Invalid element: " + element->name());
        }

        for (auto iter = element->elements().begin(); iter != element->elements().end(); ++iter)
        {
            Checker * checker = Checker::getInstance();
            Type * typePt = checker-> getElementType((*iter)->name());
            typePt->checkValidity(*iter);
        }
    }

    std::string
    Type::childrenToString(std::vector<Xml::Element const *> childrenElt)
    {
        std::string str = "";
        for (auto iter = childrenElt.begin(); iter != childrenElt.end(); ++iter)
        {
            str += "<" + (*iter)->name() + ">";
        }
        return str;
    }

    //Should work, still have to check the algorithm for choice or sequence inside choice or sequence
    std::string
    Type::parseComplexType(const Xml::Element * const xmlElement, std::string separator, bool eltSeqChoice, std::list<Attribute *> attributes, bool acceptAttributes)
    {
        bool eltParsed = false;
        std::string regex = "(";

        if(xmlElement->attribute(Checker::MIXED_ATTR).compare("true") == 0)
        {
            separator += ".*";
        }

        for (auto ci = xmlElement->elements().begin(); ci != xmlElement->elements().end(); ++ci)
        {
            if((*ci)->name().compare(Checker::SEQUENCE_ELT) == 0)
            {
                regex += getRegexFromOccurs(*ci, parseComplexType(*ci, Checker::AND_SEPARATOR, true, attributes, false)) + separator;
            }
            else if((*ci)->name().compare(Checker::CHOICE_ELT) == 0)
            {
                regex += getRegexFromOccurs(*ci, parseComplexType(*ci, Checker::OR_SEPARATOR, true, attributes, false)) + separator;
            }
            else if((*ci)->name().compare(Checker::ELEMENT_ELT) == 0)
            {
                if(eltSeqChoice || !eltParsed)
                {
                    eltParsed = true;
                    regex += parseElement(*ci) + separator;
                }
                else
                {
                    Checker::throwInvalidElementException(Checker::ELEMENT_ELT, getNameOrRef(*ci));
                }
            }
            else if((*ci)->name().compare(Checker::ATTRIBUTE_ELT) == 0)
            {
                if(!acceptAttributes)
                {
                    Checker::throwInvalidElementException(Checker::ELEMENT_ELT, getNameOrRef(*ci));
                }
                attributes.push_back(Xsd::Attribute::parseAttribute(*ci));
            }
            else
            {
                Checker::throwInvalidElementException(Checker::COMPLEX_TYP_ELT, (*ci)->name());
            }
        }

        if(regex.back() == '|')
        {
            regex = regex.substr(0, regex.size()-1);
        }

        return regex + ")";
    }

    bool
    Type::isSimpleType(const std::string & type)
    {
        //throw new NotImplementedException("Not implemented yet");
        return (type.compare(Checker::STRING_TYPE) == 0)
            || (type.compare(Checker::DATE_TYPE) == 0);
    }

    std::string
    Type::getOccursFromElement(const Xml::Element & xmlElement, const std::string & occursAttrName, const std::string & occursAttrValue)
    {
        if(occursAttrValue.compare(UNBOUNDED) == 0)
        {
            return UNBOUNDED_EXP_REG;
        }

        try
        {
            if(std::stoi(occursAttrValue) < 0)
            {
                throw new XSDConstructionException("Invalid value for " + occursAttrName + "attribute in element " + xmlElement.name() + ": " + occursAttrValue);
            }
        }
        catch(const std::exception& e)
        {
            Checker::throwInvalidAttributeValueException(Checker::ELEMENT_ELT, occursAttrName, occursAttrValue);
        }

        return occursAttrValue;
    }


    std::string
    Type::getNameOrRef(const Xml::Element * const xmlElement)
    {
        if(isReference(xmlElement))
        {
            return xmlElement->attribute(Checker::REF_ATTR);
        }
        return xmlElement->attribute(Checker::NAME_ATTR);
    }

    bool
    Type::isReference(const Xml::Element * const xmlElement)
    {
        std::string notFound = "";
        std::string name = xmlElement->attribute(Checker::NAME_ATTR);
        std::string ref = xmlElement->attribute(Checker::REF_ATTR);

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
    std::string
    Type::parseElement(const Xml::Element * const xmlElement)
    {
        bool ref = false;
        std::string regex, name;

        // Name and ref attributes
        if(isReference(xmlElement))
        {
            name = xmlElement->attribute(Checker::REF_ATTR);
            ref = true;
        }
        else
        {
            name = xmlElement->attribute(Checker::NAME_ATTR);
        }

        regex = getRegexFromOccurs(xmlElement, name);

        if(!ref)
        {
            //Type manaement
            std::string type = xmlElement->attribute(Checker::TYPE_ATTR);
            if(!type.compare("") && isSimpleType(type))
            {
                std::vector<std::string> tokens;
                //throw new NotImplementedException("Not implemented yet"); tokens = NULL; //todo remove
                //tokens = Utils::split(type, ':'));
                Checker::getInstance()->addTypedElement(name, tokens.back());

            }
            else if(xmlElement->elements().size() > 0)
            {
                auto typeElement = xmlElement->elements().front();
                if(typeElement->name().compare(Checker::COMPLEX_TYP_ELT) == 0)
                {
                    std::string typeName = name + "Type";
                    new Type(typeElement, typeName);
                    Checker::getInstance()->addTypedElement(name, typeName);
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
    std::string
    Type::getRegexFromOccurs(const Xml::Element * const xmlElement, const std::string & eltRegex)
    {
        std::string notFound = "", regex;
        std::string name = xmlElement->attribute(Xsd::Checker::NAME_ATTR);
        std::string ref = xmlElement->attribute(Xsd::Checker::REF_ATTR);
        std::string minOccurs = xmlElement->attribute(Checker::MIN_OCC_ATTR);
        std::string supOccurs = xmlElement->attribute(Checker::MAX_OCC_ATTR);

        // Min and max occurs attributes
        if(!(minOccurs.compare(notFound) == 0))
        {
            minOccurs = getOccursFromElement(*xmlElement, Checker::MIN_OCC_ATTR, minOccurs);
        }
        else
        {
            minOccurs = "1";
        }

        if(!(supOccurs.compare(notFound) == 0))
        {
            supOccurs = getOccursFromElement(*xmlElement, Checker::MAX_OCC_ATTR, supOccurs);
        }
        else
        {
            supOccurs = "0";
        }

        if((minOccurs.compare(UNBOUNDED_EXP_REG) == 0 && supOccurs.compare(UNBOUNDED_EXP_REG) != 0)
            || (supOccurs.compare(UNBOUNDED_EXP_REG) !=0 && std::stoi(supOccurs) < std::stoi(minOccurs)))
        {
            throw new XSDConstructionException("Error: " + Checker::MIN_OCC_ATTR + " attribute value is higher than " + Checker::MAX_OCC_ATTR + " value");
        }
        if((minOccurs.compare(UNBOUNDED_EXP_REG) == 0) && (supOccurs.compare(UNBOUNDED_EXP_REG) == 0))
        {
            std::stringstream out;
            out << (std::stoi(supOccurs) - std::stoi(minOccurs));
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
    Type::attributes() const
    {
        return mAttributes;
    }
}
