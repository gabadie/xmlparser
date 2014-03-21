#ifndef _H_XML_NODE
#define _H_XML_NODE

#include <iostream>
#include <list>
#include <string>

#include "XmlObject.hpp"

namespace Xml
{
    /**
     * Defines the abstract class of a node for interface purpose
     */
    class Node : public Object
    {
    public:

        /**
         * Constructor
         */
        Node();

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

        /**
         * Destructor
         */
        virtual
        ~Node();

        /**
         * Gets the parent node (non-const version)
         *
         * @return The parent node
         */
        Node *
        parent();

        /**
         * Gets the parent node (const version)
         *
         * @return The parent node
         */
        Node const *
        parent() const;

        /**
         * Gets the root element of the document (const version)
         *
         * @return The root element of the document
         */
        virtual
        Element const *
        root() const override final;

    protected:
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
         * Gets the content text if is a Xml::Text
         */
        virtual
        std::string const &
        contentText() const;

    protected:
        friend class Element;

        /**
         * Tells whether or not the node is an Element
         *
         * @return True if the node is an Element, false otherwise.
         */
        virtual
        bool
        isElement() const;

    protected:
        // Pointer to the parent node
        Node * mParent;
    };

    /**
     * Defines a sexier standart stream operator
     */
    inline
    std::ostream &
    operator << (std::ostream & stream, Node const & node)
    {
        node >> stream;
        return stream;
    }

}


#endif //_H_XML_NODE
