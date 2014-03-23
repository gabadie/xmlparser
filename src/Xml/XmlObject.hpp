#ifndef _H_XML_OBJECT
#define _H_XML_OBJECT

#include <iostream>


namespace Xml
{
    // Forward declarations
    class Element;
    class Test;
    class Document;

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
         * Gets the object's document
         *
         * @return The document
         */
        Document *
        document();

        virtual
        Document const *
        document() const = 0;

        /**
         * Gets the object's parent
         *
         * @return The parent (either a Document or an Element)
         */
        Object *
        parent();

        virtual
        Object const *
        parent() const = 0;

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
