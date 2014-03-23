#ifndef _H_XML_NODE
#define _H_XML_NODE

#include <list>
#include <string>

#include "XmlObject.hpp"

namespace Xml
{
    class Element;

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
         * Destructor
         */
        virtual
        ~Node();

        /**
         * Gets the root element of the document (const version)
         *
         * @return The root element of the document
         */
        virtual
        Element const *
        root() const override final;

    protected:
        friend class Xml::Element;

        /**
         * Gets the content text if is a Xml::Text
         */
        virtual
        std::string const &
        contentText() const;
    };

}


#endif //_H_XML_NODE
