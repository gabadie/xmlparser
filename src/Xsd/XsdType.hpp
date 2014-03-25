#ifndef _H_XSD_TYPE
#define _H_XSD_TYPE

#include "../Xml/XmlElement.hpp"
#include <string>
#include <map>
#include <re2/re2.h>

namespace Xsd
{
    class Type
    {
    public:
        /**
         * Constructor
         * @param xmlElement XML ComplexType element
         */
        Type(const Xml::Element & xmlElement);

        Type(const Xml::Element & xmlElement, const std::string & name);

        Type(const std::string & const regex, std::list<Attribute *> attrs);

         /**
         * Destructor
         */
        virtual
        ~Type();

        static std::string
        parseComplexType(const Xml::Element & xmlElement, std::string separator, bool eltSeqChoice);

        /**
         * Create the type regex
         */
        static bool
        isSimpleType(const std::string & type);

        static bool
        getNameOrRef(const std::string & type);

        static bool
        isReference(const Xml::Element & xmlElement);

        static const Type *
        parseSimpleType(const std::string & type);

        /**
         * Create the regex for an element
         */
        static std::string
        getRegexFromElement(const Xml::Element & xmlElement);

        static std::string
        getRegexFromOccurs(const Xml::Element & xmlElement, const std::string & eltRegex)

        std::list<Attribute *>
        attributes() const;

    protected:

        std::string mRegex;
        std::list<Attribute * const> mAttributes;

        static const std::string TYPE_SUFFIX;
        static const std::string UNBOUNDED;
        static const std::string UNBOUNDED_EXP_REG;
    };
 }

 #endif
