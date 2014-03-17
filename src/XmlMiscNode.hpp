#ifndef _H_XML_MISC_NODE
#define _H_XML_MISC_NODE

#include <iosfwd>
#include <string>

#include "XmlDocumentNode.hpp"

namespace Xml
{
    /**
     * Defines an abstract misc node for interface purpose
     */
    class MiscNode : public DocumentNode
    {
    public:

        /**
         * Constructor
         *
         * @param parent Parent of the node
         */
        MiscNode(Node * parent = nullptr);

        /**
         * Destructor
         */
        virtual
        ~MiscNode();
    };
}


#endif //_H_XML_MISC_NODE
