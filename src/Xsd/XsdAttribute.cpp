#include "XsdAttribute.hpp"
#include "XsdChecker.hpp"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#ifdef APP_DEBUG
#include <cassert>
#endif

namespace Xsd
{
    Attribute::Attribute(const std::string & name, bool required, const std::string & typeName, bool ref):
    {
        init(name, required, typeName, ref);
    }

    void
    Attribute:init(const std::string & name, bool required, const std::string & typeName, bool ref)
    {
        mName = name;
        mRequired = required;
        Xsd::Checker::addAttribute(name, &this);
        if(!ref)
        {
            Xsd::Checker::getInstance().addTypedAttribute(name, typeName);
        }

    }

    Attribute::Attribute(const Xml::Element & xmlElement):
    {
        bool required = false, hasRef = false;
        std::string notFound = "";
        std::string name = xmlElement.attribute(Checker::NAME_ATTR);
        std::string ref = xmlElement.attribute(Checker::REF_ATTR);
        std::string type = xmlElement.attribute(Checker::TYPE_ATTR);
        std::string use = xmlElement.attribute(Checker::USE_ATTR);

        if(!(name.compare(notFound) == 0))
        {
            // Add an attribute to the attribute type map
            if(!(type.compare(notFound) == 0) && Xsd::Type::isSimpleType(type))
            {
                    std::vector<std::string> tokens;
                    boost::algorithm::split(tokens, type, boost::algorithm::is_any_of(":"));
            }
            else
            {
                Checker::throwInvalidAttributeValueException(name, Checker::TYPE_ATTR, type);
            }
        }
        else if(!(ref.compare(notFound) == 0))
        {
            name = ref;
            hasRef = true;
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


        init(name, required, type, hasRef);
    }
}