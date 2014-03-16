#ifndef _H_XML_NODE
#define _H_XML_NODE

#include <iostream>
#include <list>
#include <string>

namespace Xml
{

    /**
     * Defines the abstract class of a node for interface purpose
     */
    class Node
    {
    public:

        /**
         * Constructor
         *
         * @param parent Parent of the node
         */
        Node(Node * parent = nullptr);

        /**
         * Implements standart stream operator
         */
        inline
        std::ostream &
        operator >> (std::ostream & stream) const
        {
            exportToStream(stream, "");
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

    protected:

        // Pointer to the parent node
        Node * mParent;

        /**
         * Exports to a <stream> with a given <indent>
         *
         * @param <stream> is the stream to export to
         * @param <indent> is the the indentation prefix
         */
        virtual
        void
        exportToStream(std::ostream & stream, std::string const & indent) const = 0;

        /**
         * Gets the content text if is a Xml::Text
         */
        virtual
        std::string
        contentText() const;

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
