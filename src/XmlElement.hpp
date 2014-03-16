#ifndef _H_XML_ELEMENT
#define _H_XML_ELEMENT

#include <deque>
#include <map>
#include <string>
#include <vector>

#include "XmlNode.hpp"

namespace Xml
{
    //Forward declarations
    class MiscNode;

    /**
     * Defines an XML element
     */
    class Element : public Node
    {
    public:

        // Type aliases
        using AttributesMap = std::map<std::string, std::string>;
        using ElementsList = std::deque<Node *>;
        using MiscContainer = std::vector<MiscNode *>;

        /**
         * Constructor
         *
         * @param name Name of the element
         * @param parent Parent of the element
         */
        Element(std::string const & name, Node * parent = nullptr);

        /**
         * Destructor
         */
        virtual
        ~Element()
        {

        }

        /**
         * Gets the ordered list of the children of the element
         *
         * @return Return a list of Node *
         */
        ElementsList const &
        elements() const;


        //TODO
        //std::string text() const; Inherited from Node ?

        //TODO
        //void setContent(std::string const &) -> setText() ? Inherited from Node ?

        //TODO
        //void clearContent() ?

        /**
         * Appends a node child
         *
         * @param node Child node to append
         */
        void
        append(Node * node);

        //TODO
        //void appendText(std::string const & text) -> Inherited from Node ?

        //TODO
        /**
         * Appends a comment to the element
         *
         * @param comment Text of the comment to append
         */
        //void
        //appendComment(std::string const & comment);

        //TODO
        /**
         * Appends a PI to the element
         *
         * @param pi Text of the PI to append
         */
        //void
        //appendPI(std::string const & pi);

        /**
         * Gets the name of the element
         *
         * @return The name of the element
         */
        std::string const &
        name() const;

        /**
         * Set the name of the element
         *
         * @param name Name to set
         */
        void
        setName(std::string const & name);

        /**
         * Gets the value of an attribute by name
         *
         * @param name Name of the attribute to get.
         *
         * @return The value of the attribute if found, an empty string otherwise.
         */
        std::string const &
        attribute(std::string const & name) const;

        /**
         * Sets the value of an attribute.
         *
         * If the attribute does not exist, it is created.
         *
         * @param name  Name of the attribute to set
         * @param value Value to set
         */
        void
        setAttribute(std::string const & name, std::string const & value);

        /**
         * Gets the comments attached to the element
         *
         * @return The list of comments attached to the element
         */
        MiscContainer const &
        comments();

        /**
         * Gets the PI attached to the element
         *
         * @return The list of PI attached to the element
         */
        MiscContainer const &
        PI();

    protected:

        /**
         * Exports to a <stream> with a given <indent>
         *
         * @param <stream> is the stream to export to
         * @param <indent> is the the indentation prefix
         */
        virtual
        void
        exportToStream(std::ostream & stream, std::string const & indent) const override;


    protected:
        std::string nName;         ///< Name of the element
        AttributesMap mAttributes; ///< Attributes of the element
        ElementsList mChildren;    ///< Children elements
        MiscContainer mComments;   ///< Comments attached to the elements
        MiscContainer mPI;         ///< PI attached to the elements
    };
}

#endif //_H_XML_ELEMENT
