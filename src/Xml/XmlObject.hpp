#ifndef _H_XML_OBJECT
#define _H_XML_OBJECT

namespace Xml
{
    // Forward declaration
    class Element;

    /**
     * Defines a abstract interface for all Xml classes
     */
    class Object
    {
    public:
        /**
         * Constructor
         */
        Object() = default;

        /**
         * Destructor
         */
        virtual
        ~Object();

        /**
         * Gets the root element of the document (non-const version)
         *
         * @return The root element of the document
         */
        Element *
        root();

        /**
         * Gets the root element of the document (const version)
         *
         * @return The root element of the document
         */
        virtual
        Element const *
        root() const = 0;
    };
}

#endif //_H_XML_OBJECT
