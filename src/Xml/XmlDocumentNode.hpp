#ifndef _H_XML_DOCUMENT_NODE
#define _H_XML_DOCUMENT_NODE

#include "XmlNode.hpp"

namespace Xml
{

    /**
     * Defines the abstract class of a document node for interface purpose
     */
    class DocumentNode : public Node
    {
    public:
        /**
         * Destructor
         */
        virtual
        ~DocumentNode();

    protected:
        /**
         * Constructor
         */
        DocumentNode();

        friend class Xml::Comment;
        friend class Xml::Element;
        friend class Xml::ProcessingInstruction;
    };
}

#endif //_H_XML_DOCUMENT_NODE
