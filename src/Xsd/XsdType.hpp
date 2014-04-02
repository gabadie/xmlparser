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
    class Checker;

    /**
     * Define the type of an element
     */
    class Type
    {
    public:
        /**
         * Constructor
         *
         * @param xmlElement xml parsed element
         * @param checker The checker
         */
        Type(const Xml::Element * const xmlElement, Checker * checker);


        /**
         * Constructor
         *
         * @param regex The regex
         * @param attrs The list of attributes
         */
        Type(const std::string & regex, std::map<std::string, Attribute *> * attrs);

         /**
         * Destructor
         */
        virtual
        ~Type();

        /**
         * Create the regex with an xml parsed element
         *
         * @param xmlElement The xml parsed element
         * @param separator The separator of the regex
         * @param eltSeqChoice Indicates if the xml element is in a sequence or choice element
         * @param acceptAttributes Indicates if the element accepts attributes
         * @param checker The checker
         *
         * @return The regex created
         */
        static std::string
        parseComplexType(const Xml::Element * const xmlElement, std::string separator, bool eltSeqChoice, std::map<std::string, Attribute *> * attributes, bool acceptAttributes, Checker * checker);

        /**
         * Returns the regex of an element, adds its type and type relation to the maps if it's not a ref
         *
         * @param xmlElement The xml parsed element
         * @param checker The checker
         *
         * @return The regex created
         */
        static std::string
        parseElement(const Xml::Element * const xmlElement, Checker * checker);

        /**
         * Check if the string value matches with the type regex
         *
         * @param str The string to match
         *
         * @return True if the values match, false otherwise
         */
        bool
        isValid(const std::string & str);

        /**
         * Checks if the type attribute is of date or string type
         *
         * @param type The value of the type attribute
         * @param checker The checker
         *
         * @return True if it is a date or string type, false otherwise
         */
        static bool
        isSimpleType(const std::string & type, Checker * checker);


        /**
         * Check the validity of the element by checking the validity of his attributes/children
         *
         * @param element The xml parsed element
         * @param checker The checker
         */
        void
        checkValidity(const Xml::Element * const element, Checker * checker);

        /**
         * Check the validity of the root element
         *
         * @param element The xml parsed element
         * @param checker The checker
         */
        static void
        checkRootValidity(const Xml::Element * const root, Checker * checker);

        /**
         * Get the value of the ref or name attribute
         *
         * @param xmlElement The xml parsed element
         *
         * @return The name of the element
         */
        static std::string
        getNameOrRef(const Xml::Element * const xmlElement);

        /**
         * Check if the type is a reference
         *
         * @param xmlElement The xml parsed element
         *
         * @ return True if the element is a reference, false otherwise
         */
        static bool
        isReference(const Xml::Element * const xmlElement);

        /**
         * Transforms the children nodes to string
         *
         * @param childrenElt The list of children element
         *
         * @return The string created
         */
        std::string
        childrenToString(std::vector<Xml::Element const *> childrenElt);

         /**
         * Returns the string given in parameter put between < and >
         *
         * @param name The name
         *
         * @return The string created
         */
        static std::string
        getTagFromName(const std::string & name);

        /**
         * Modify and returns the element regex given in parameter in order to add
         * regex expression for the occurs attributes values
         *
         * @param xmlElement The xml parsed element
         * @param eltRegex The name of the element whose we want the regex
         * @param checker The checker
         *
         * @return The string created
         */
        static std::string
        getRegexFromOccurs(const Xml::Element * const xmlElement, const std::string & eltRegex, Checker * checker);

        /**
         * Modifies the occurence of an element to be used in the regex
         *
         * @param xmlElement The xml parsed element
         * @param occursAttrName The name of the occurence attribute
         * @param occursAttrValue The value of the occurence attribute
         * @param checker The checker
         *
         * @return The new value of the occurence value
         */
        static std::string
        getOccursFromElement(const Xml::Element & xmlElement, const std::string & occursAttrName, const std::string & occursAttrValue, Checker * checker);

        /**
         * Gets the list of attributes of the type
         *
         * @return The list of attributes
         */
        std::map<std::string, Attribute *> *
        attributes() const;

    protected:

        std::string mRegex;                             /// regex created for the type
        std::map<std::string, Attribute *> * mAttributes; /// the map of attributes

        static const std::string TYPE_SUFFIX;           /// the "TYPE" suffix
        static const std::string UNBOUNDED;             /// the "UNBOUNDED" value
        static const std::string UNBOUNDED_EXP_REG;     /// the unbounded transformed value
    };
 }

 #endif
