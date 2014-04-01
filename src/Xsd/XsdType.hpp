#ifndef _H_XSD_TYPE
#define _H_XSD_TYPE

#include "../Xml/XmlElement.hpp"
#include "XsdAttribute.hpp"
#include "XsdException.hpp"
#include <string>
#include <vector>
#include <map>
#include <re2/re2.h>

namespace Xsd
{
    /**
     * Define the type of an element
     */
    class Type
    {
    public:
        /*
         * Constructor
         *
         * @param xmlElement xml parsed element
         */
        Type(const Xml::Element * const xmlElement);

        Type(const std::string & regex, std::list<Attribute *> attrs);

         /**
         * Destructor
         */
        virtual
        ~Type();

        /*
         * Create the regex with an xml parsed element
         *
         * @param xmlElement xml parsed element
         * @param separator the separator of the regex
         * @param eltSeqChoice indicates if the xml element is in a sequence or choice element
         * @param acceptAttributes indicates if the element accepts attributes
         */
        static std::string
        parseComplexType(const Xml::Element * const xmlElement, std::string separator, bool eltSeqChoice, std::list<Attribute *> attributes, bool acceptAttributes);

        /*
         * Returns the regex of an element, adds its type and type relation to the maps if it's not a ref
         *
         * @param xmlElement xml parsed element
         */
        static std::string
        parseElement(const Xml::Element * const xmlElement);

        /*
         * Check if the string value matches with the type regex
         *
         * @param str the string to match
         */
        bool
        isValid(const std::string & str);

        /**
         * Create the type regex
         */
        static bool
        isSimpleType(const std::string & type);

        void
        checkValidity(const Xml::Element * const element);

        static std::string
        getNameOrRef(const Xml::Element * const xmlElement);

        static bool
        isReference(const Xml::Element * const xmlElement);

        static const Type *
        parseSimpleType(const std::string & type);

        std::string
        childrenToString(std::vector<Xml::Element const *> childrenElt);

        static std::string
        getRegexFromOccurs(const Xml::Element * const xmlElement, const std::string & eltRegex);

        static std::string
        getOccursFromElement(const Xml::Element & xmlElement, const std::string & occursAttrName, const std::string & occursAttrValue);

        std::list<Attribute *>
        attributes() const;

    protected:

        std::string mRegex;
        std::list<Attribute *> mAttributes;

        static const std::string TYPE_SUFFIX;
        static const std::string UNBOUNDED;
        static const std::string UNBOUNDED_EXP_REG;
    };
 }

 #endif
