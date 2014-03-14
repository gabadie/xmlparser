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

        /*
         * Gets all children nodes
         *
         * @return A list of all children nodes (obviously...)
         */
        virtual
        std::list<Node *>
        childrenNodes() = 0;

        virtual
        std::list<Node const *> const
        childrenNodes() const = 0;

        /*
         * Implements standart stream operator
         */
        virtual
        std::ostream &
        operator >> (std::ostream & stream) const = 0;

        /*
         * Destructor
         */
        virtual
        ~Node()
        {

        }

    };

    /*
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
