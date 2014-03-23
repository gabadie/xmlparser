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
        friend class Document;

        /**
         * Constructor
         */
        DocumentNode();

        /**
         * Destructor
         */
        virtual
        ~DocumentNode();
    };
}

#endif //_H_XML_DOCUMENT_NODE
