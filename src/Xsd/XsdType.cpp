#include "XsdType.hpp"


namespace Xsd
{
    Type::Type()
    {
        mRegex = "";
    }

    Type::~Type()
    {

    }

    void
    Type::parseTypeFromComplexType(XmlElement xmlElement)
    {

    }

    std::string
    Type::getRegexFromElement(XmlElement xmlElement)
    {
        map<std::string,std::string>::iterator itName = xmlElement.mAttributes.find('name');
        map<std::string,std::string>::iterator itRef = xmlElement.mAttributes.find('ref');
        map<std::string,std::string>::iterator itMin = xmlElement.mAttributes.find('minOccurs');
        map<std::string,std::string>::iterator itMax = xmlElement.mAttributes.find('maxOccurs');
        bool hasName = (itName != xmlElement.mAttributes.end());
        bool hasRef = (itRef != xmlElement.mAttributes.end());
        bool hasMin = (itMin != xmlElement.mAttributes.end());
        bool hasMax = (itMax != xmlElement.mAttributes.end());

        std:string regex;
        std:string name;
        string minOccurs = "1";
        string maxOccurs = "*";

        // Get the element name
        if(hasName)
        {
            name = (*itName).second;
        }
        else if(hasRef)
        {
            name = (*itRef).second;
        }
        else
        {
            // TODO Gestion exception
            name = "";
            cout << "Error : Attribute element doesn't have any name attribute" << endl;
        }

        // Get the occurence
        if(hasMin)
        {
            minOccurs = (*itMin).second;
        }
        if(hasMax)
        {
            maxOccurs = (*itMax).second;
        }

        // Create the regex
        regex = "<(" + name + ">){" + minOccurs + "})((<" + name + ">?){" + maxOccurs + "}";
        return regex;
    }
}