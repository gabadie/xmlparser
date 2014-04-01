
#include "../AppDebug.hpp"
#include "XmlObject.hpp"
#include "XmlComment.hpp"
#include "XmlProcessingInstruction.hpp"

namespace Xml
{
    Object::~Object()
    {

    }

    Document *
    Object::document()
    {
        return const_cast<Document *>(static_cast<Object const *>(this)->document());
    }

    Object *
    Object::parent()
    {
        return const_cast<Object *>(static_cast<Object const *>(this)->parent());
    }

    void
    Object::appendComment(std::string const & comment)
    {
        if (comment == "")
        {
            return;
        }

        this->appendNode(new Comment(comment));
    }

    void
    Object::appendProcessingInstruction(ProcessingInstruction * pi)
    {
        this->appendNode(pi);
    }

    bool
    Object::remove(Node *)
    {
        /*
         * Object::remove is overloaded in Xml::Document and Xml::Element.
         * but other classes don't because they are not supposed to have
         * children elements
         */
        app_unreachable();

        return false;
    }

    bool
    Object::isElement() const
    {
        return objectLabel() == ObjectLabel::Element;
    }

    void
    Object::appendNode(Node *)
    {
        /*
         * Object::appendNode is overloaded in Xml::Document and Xml::Element.
         * but other classes don't because they are not supposed to have
         * children elements
         */
        app_unreachable();
    }

    bool
    Object::hasChild(Node const *) const
    {
        /*
         * Object::hasChild is overloaded in Xml::Document and Xml::Element.
         * but other classes don't because they are not supposed to have
         * children elements
         */
        app_unreachable();

        return 0;
    }
}
