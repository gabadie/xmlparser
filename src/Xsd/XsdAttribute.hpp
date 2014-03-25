#ifndef _H_XSD_ATTRIBUTE
#define _H_XSD_ATTRIBUTE

#include <string>

namespace Xsd
{
    class Type;

    /**
     * Defines the schema of an XML attribute
     */
    class Attribute
    {
        public:
            /**
             * Constructor
             */
            Attribute(const Xml::Element & xmlElement);

            Attribute(const std::string & name, bool required, const std::string & typeName, bool ref);

            /**
             * Destructor
             */
            virtual ~Attribute() {}

        protected:
            std::string mName;
            bool mRequired;

            void
            init(const std::string & name, bool required, const std::string & typeName, bool ref);
    };
}

#endif //_H_XSD_ATTRIBUTE