/**
 * \file XsdType.cpp
 * \brief XSD Type
 * \author jcourlet
 * \version 0.1
 * \date 18 mars 2014
 */

#include "XsdType.hpp"
#include "XsdChecker.hpp"
#include "XsdException.hpp"
#include "../Utils.hpp"

#include <sstream>

namespace Xsd
{

    const std::string Type::TYPE_SUFFIX = "TYPE";
    const std::string Type::UNBOUNDED = "unbounded";
    const std::string Type::UNBOUNDED_EXP_REG = "*";

    Type::Type(const Xml::Element * const xmlElement, Checker * checker)
    {
        mAttributes = new std::map<std::string, Attribute *>();
        mRegex = parseComplexType(xmlElement, "", false, mAttributes, true, checker, false);
    }


    Type::Type(const std::string & regex, std::map<std::string, Attribute *> * attrs):
        mRegex(regex)
    {
        mAttributes = attrs;
    }

    Type::~Type()
    {
		if(mAttributes != NULL)
		{
			for(auto iterator : *mAttributes)
			{
				delete iterator.second;
			}
			mAttributes->clear();
		}
    }


    void
    Type::checkRootValidity(const Xml::Element * const root, Checker * checker)
    {
        std::string rootElement = "<" + root->name() + ">";
        std::string regex = checker->getElementType(checker->ROOT)->mRegex;
        if(!RE2::FullMatch(rootElement,  regex))
        {
            throw new XSDValidationException("Error: invalid element or missing children for element: " + root->name());
        }

        checker->getElementType(root->name())->checkValidity(root, checker);
    }

    bool
    Type::isValid(const std::string & str)
    {
        return RE2::FullMatch(str, mRegex);
    }

    void
    Type::checkValidity(const Xml::Element * const element, Checker * checker)
    {
        std::map<std::string, std::string> attributeMapElement = element->attributes();

		if(mAttributes != NULL)
		{
			//Checking attributes
			for (auto iterAttr : *mAttributes)
			{
				auto iterMap = attributeMapElement.find(iterAttr.second->name());
				if (iterMap == attributeMapElement.end())
				{
					if (iterAttr.second->isRequired())
					{
						throw new XSDValidationException("Error: attribute " + iterAttr.second->name() + "  in element " + element->name() + " is missing");
					}
				}
				else
				{
					if(!checker->getAttributeType(iterAttr.second->name())->isValid(iterMap->second))
					{
						throw new XSDValidationException("Error: Invalid attribute value for attribute " + iterAttr.second->name() + ": " + iterMap->second);
					}
				}
			}

			for (auto iterAttr : attributeMapElement)
			{
				auto iteratorFind = mAttributes->find(iterAttr.first);
				if (iteratorFind == mAttributes->end())
				{
					if((iterAttr.first != "xmlns:xsi") && (iterAttr.first.substr(0,4) != "xsi:"))
					{
						throw new XSDValidationException("Error: Unexpected attribute " + iterAttr.first + "  in element " + element->name());
					}
				}
			}
		}

		//Checking children elements
        if(!isValid(childrenToString(element->elements()) + element->text()))
        {
            throw new XSDValidationException("Error: Invalid element or missing children for element: " + element->name());
        }

		//Recursive call on every child element
        for (auto iter : element->elements())
        {
            Type * typePt = checker-> getElementType(iter->name());
            typePt->checkValidity(iter, checker);
        }
    }

    std::string
    Type::childrenToString(std::vector<Xml::Element const *> childrenElt)
    {
        std::string str = "";
        for (auto iter : childrenElt)
        {
            str += "<" + iter->name() + ">";
        }
        return str;
    }

    //Should work, still have to check the algorithm for choice or sequence inside choice or sequence
    std::string
    Type::parseComplexType(const Xml::Element * const xmlElement, std::string separator, bool eltSeqChoice, std::map<std::string, Attribute *> * attributes, bool acceptAttributes, Checker * checker, bool root)
    {
        std::string regex = "(";

        if(xmlElement->attribute(checker->MIXED_ATTR) == "true")
        {
            separator += ".*";
        }

        for (auto ci : xmlElement->elements())
        {
            if(ci->name() == checker->SEQUENCE_ELT)
            {
                regex += getRegexFromOccurs(ci, parseComplexType(ci, checker->AND_SEPARATOR, true, attributes, false, checker, false), checker) + separator;
            }
            else if(ci->name() == checker->CHOICE_ELT)
            {
                regex += getRegexFromOccurs(ci, parseComplexType(ci, checker->OR_SEPARATOR, true, attributes, false, checker, false), checker) + separator;
            }
            else if(ci->name() == checker->ELEMENT_ELT && eltSeqChoice)
            {
                regex += parseElement(ci, checker) + separator;
            }
            else if(ci->name() == checker->ATTRIBUTE_ELT)
            {
                if(!acceptAttributes)
                {
                    checker->throwInvalidElementException(getNameOrRef(ci), checker->ELEMENT_ELT);
                }
                Attribute * attr = Xsd::Attribute::parseAttribute(ci, checker);
                attributes->insert(std::make_pair(attr->name(), attr));
            }
            else if(root && ci->name() == Checker::COMPLEX_TYP_ELT)
            {
                std::string typeName = ci->attribute(Checker::NAME_ATTR);
                if(typeName == "")
                {
                    Checker::throwMissingAttributeException(Checker::COMPLEX_TYP_ELT, Checker::NAME_ATTR);
                }
                std::map<std::string, Attribute *> * typeAttrs = new std::map<std::string, Attribute *>();
                std::string typeRegex = Xsd::Type::parseComplexType(ci, "", false, typeAttrs, true, checker, false);
                checker->addType(typeName, new Type(typeRegex, typeAttrs));
            }
            else
            {
                throw new XSDConstructionException("Error: Invalid XSD element received: " + ci->name() + " (" + checker->SEQUENCE_ELT + ", " + checker->CHOICE_ELT + ", " + checker->ELEMENT_ELT + " or " + checker->ATTRIBUTE_ELT + ", " + " expected)");
            }
        }

        if(regex.back() == '|')
        {
            regex = regex.substr(0, regex.size()-1);
        }

        return regex + ")";
    }

    bool
    Type::isSimpleType(const std::string & type, Checker * checker)
    {
        //throw new NotImplementedException("Not implemented yet");
        return (type == checker->getStringType())
            || (type == checker->getDateType());
    }

    std::string
    Type::getOccursFromElement(const Xml::Element & xmlElement, const std::string & occursAttrName, const std::string & occursAttrValue, Checker * checker)
    {
        if(occursAttrValue == UNBOUNDED)
        {
            return UNBOUNDED_EXP_REG;
        }

        try
        {
            if(std::stoi(occursAttrValue) < 0)
            {
                throw new XSDConstructionException("Error: Invalid value for " + occursAttrName + "attribute in element " + xmlElement.name() + ": " + occursAttrValue);
            }
        }
        catch(std::exception * e)
        {
            delete e;
            checker->throwInvalidAttributeValueException(checker->ELEMENT_ELT, occursAttrName, occursAttrValue);
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
        if(name != notFound)
        {
            return false;
        }
        if(ref != notFound)
        {
            return true;
        }
        Checker::throwMissingAttributeException(Checker::ELEMENT_ELT, Checker::NAME_ATTR);
        return false;
    }

    std::string
    Type::getTagFromName(const std::string & name)
    {
        std::stringstream out;
        out << "<";
        out << name;
        out << ">";
        return out.str();
    }

    /**
     * The regex does not contain the validation for the element children, it's only about the element name or ref itself
     */
    std::string
    Type::parseElement(const Xml::Element * const xmlElement, Checker * checker)
    {
        bool ref = false;
        std::string regex, name;

        // Name and ref attributes
        if(isReference(xmlElement))
        {
            name = xmlElement->attribute(checker->REF_ATTR);
            ref = true;
        }
        else
        {
            name = xmlElement->attribute(checker->NAME_ATTR);
        }

        regex = getRegexFromOccurs(xmlElement, getTagFromName(name), checker);

        if(!ref)
        {
            //Type management
            std::string type = xmlElement->attribute(checker->TYPE_ATTR);
            if(type != "")
            {
				checker->addTypedElement(name, type);
			}
            else if(xmlElement->elements().size() > 0)
            {
                auto typeElement = xmlElement->elements().front();
                if(typeElement->name() == Checker::COMPLEX_TYP_ELT)
                {
                    std::string typeName = name + "Type";
                    Type * type = new Type(typeElement, checker);
                    checker->addType(typeName, type);
                    checker->addTypedElement(name, typeName);
                }
                else
                {
                    throw new XSDConstructionException("Error: type attribute or element cannot be found for " + name + " element");
                }
            }
            else
            {
                throw new XSDConstructionException("Error: type description (attribute or element) cannot be found for " + name + " element");
            }
        }

        return regex;
    }

    std::string
    Type::getRegexFromOccurs(const Xml::Element * const xmlElement, const std::string & eltRegex, Checker * checker)
    {
        std::string notFound = "", regex;
        std::string name = xmlElement->attribute(checker->NAME_ATTR);
        std::string ref = xmlElement->attribute(checker->REF_ATTR);
        std::string minOccurs = xmlElement->attribute(checker->MIN_OCC_ATTR);
        std::string supOccurs = xmlElement->attribute(checker->MAX_OCC_ATTR);

        // Min and max occurs attributes
        if(minOccurs != notFound)
        {
            minOccurs = getOccursFromElement(*xmlElement, checker->MIN_OCC_ATTR, minOccurs, checker);
        }
        else
        {
            minOccurs = "1";
        }

        if(supOccurs != notFound)
        {
            supOccurs = getOccursFromElement(*xmlElement, checker->MAX_OCC_ATTR, supOccurs, checker);
        }
        else
        {
            supOccurs = "1";
        }

        if(minOccurs == UNBOUNDED_EXP_REG)
        {
            throw new XSDConstructionException("Error: Invalid value for attribute " + checker->MIN_OCC_ATTR + ": " + UNBOUNDED_EXP_REG);
        }

        if(supOccurs != UNBOUNDED_EXP_REG && std::stoi(supOccurs) < std::stoi(minOccurs))
        {
            throw new XSDConstructionException("Error: " + checker->MIN_OCC_ATTR + " attribute value is higher than " + checker->MAX_OCC_ATTR + " value");
        }
        if(supOccurs != UNBOUNDED_EXP_REG)
        {
            std::stringstream out;
            out << (std::stoi(supOccurs) - std::stoi(minOccurs));
            supOccurs = out.str();
        }

        // Create the regex
        regex = "(" + eltRegex + "){" + minOccurs + "}";
        if(supOccurs != "0")
        {
            if(supOccurs != UNBOUNDED_EXP_REG)
            {
                supOccurs = "{" + supOccurs + "}";
            }
            regex += "((" + eltRegex + ")?)" + supOccurs;
        }

        return regex;
    }

    std::map<std::string, Attribute *> *
    Type::attributes() const
    {
        return mAttributes;
    }
}
