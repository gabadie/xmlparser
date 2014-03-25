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
        mName(name),
        mRequired(required)
    {
        Xsd::Checker.addAttribute(name, &this); {
        if(!ref)
        {
            Xsd::Checker.getInstance().addTypedAttribute(name, type);
        }
    }

    Attribute::Attribute(const Xml::Element & xmlElement);
    {
        bool required = false, ref = false;
        std::string notFound = "";
        std::string name = xmlElement.attribute(Checker.NAME_ATTR);
        std::string ref = xmlElement.attribute(Checker.REF_ATTR);
        std::string type = xmlElement.attribute(Checker.TYPE_ATTR);
        std::string use = xmlElement.attribute(Checker.USE_ATTR);

        if(!name.compare(notFound))
        {
            // Add an attribute to the attribute type map
            if(!type.compare(notFound) && Xsd::Type.isSimpleType(type))
            {
                    std::vector<std::string> tokens;
                    boost::algorithm::split(tokens, type, is_any_of(":"));
            }
            else
            {
                Checker.throwInvalidAttributeValueException(name, Checker.TYPE_ATTR, type);
            }
        }
        else if(!ref.compare(notFound))
        {
            name = ref;
            ref = true;
        }
        else
        {
            Checker.throwMissingAttributeException(name, Checker.TYPE_ATTR);
        }

        if(!use.compare(notFound))
        {
            if(use.compare(USE_REQUIRED_VALUE))
            {
                required = true;
            }
            else
            {
                Checker.throwInvalidAttributeValueException(name, Checker.USE_ATTR, use);
            }
        }


        this(name, required, type, ref);
    }

    Attribute::~Attribute()
    {
    }
}