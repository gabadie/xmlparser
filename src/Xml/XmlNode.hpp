#ifndef _H_XML_NODE
#define _H_XML_NODE

#include <list>
#include <string>

#include "XmlObject.hpp"

namespace Xml
{
    // Forward decalarations
    class Document;
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
         * Gets the node's document
         *
         * @return The document
         */
        Document const *
        document() const override final;

        /**
         * Gets the parent xml object (const version)
         *
         * @return The parent xml object
         */
        Object const *
        parent() const override final;


    protected:
        // Pointer to the parent object
        Object * mParent;

        friend class Xml::Document;
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
