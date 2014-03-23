#ifndef _H_XML_OBJECT
#define _H_XML_OBJECT

#include <iostream>


namespace Xml
{
    // Forward declaration
    class Element;
    class Test;

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
         * Gets the parent xml object (non-const version)
         *
         * @return The parent xml object
         */
        Object *
        parent();

        /**
         * Gets the parent xml object (const version)
         *
         * @return The parent xml object
         */
        Object const *
        parent() const;

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

        /**
         * Implements standart stream operator
         */
        inline
        std::ostream &
        operator >> (std::ostream & stream) const
        {
            exportToStream(stream, 0, "  ");
            return stream;
        }

    protected:
        friend class Xml::Element;

        /**
         * Exports to a <stream> with a given <indent>
         *
         * @param stream The stream to export to
         * @param level  Level of the token
         * @param indent The indentation prefix
         */
        virtual
        void
        exportToStream(std::ostream & stream, std::size_t level,
            std::string const & indent) const = 0;

        /**
         * Tells whether or not the node is an Element
         *
         * @return True if the node is an Element, false otherwise.
         */
        virtual
        bool
        isElement() const;

    protected:
        // Pointer to the parent object
        Object * mParent;

        friend class Xml::Test;
    };

    /**
     * Defines a sexier standart stream operator
     */
    inline
    std::ostream &
    operator << (std::ostream & stream, Object const & object)
    {
        return object >> stream;
    }

}

#endif //_H_XML_OBJECT
