#ifndef _H_DOCUMENT_NODE
#define _H_DOCUMENT_NODE

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
        ~DocumentNode()
        {

        }
    };
}


#endif //_H_DOCUMENT_NODE
