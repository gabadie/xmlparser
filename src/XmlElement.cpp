#include "XmlElement.hpp"

#ifdef APP_DEBUG
#include <algorithm>
#endif

namespace Xml
{
    Element::Element(std::string const & name, Node * parent):
        Node(parent),
        mParent(parent),
        mchildren(),
        mComments(),
        mPI()
    {

    }

    Element::~Element()
    {
        // Free memory
        this->clearContent();
    }

    NodeList const &
    Element::elements() const
    {
        return mChildren;
    }

    Element const *
    Element::parentElement() const
    {
        Node * parent = mParent;
        do
        {
            if(parent->isElement()) break;
        }
        while((parent = mParent->parent()) != nullptr);

        return parent;
    }

    std::string
    Element::text() const
    {
        std::string content = "";

        for(auto const & c : mChildren)
        {
            content += contentText();
        }

        return content;
    }

    void
    Element::setContent(std::string const & content)
    {
        this->clearContent();
        this->appendText(content);
    }

    void
    Element::clearContent()
    {
        for(auto & c : mChildren)
        {
            if(c->isElement())
            {
                c->clearContent();
            }

            delete c;
        }
    }

    void
    Element::append(Node * node)
    {
        #ifdef APP_DEBUG
        assert(node != this);

        assert(
            std::find(std::begin(mChildren), std::end(mChildren), node)
            != std::end(mChildren)
        );
        #endif

        mChildren.append(node);
    }


    void
    Element::appendText(std::string const & text)
    {
        this->appendNode(new Text(content));
    }

    void
    Element::appendComment(std::string const & comment)
    {
        this->appendNode(new Comment(content));
    }

    void
    Element::appendPI(std::string const & pi)
    {
        this->appendNode(new PI(content));
    }

    std::string const &
    Element::name() const
    {
        return mName;
    }

    void
    Element::setName(std::string const & name)
    {
        mName = name;
    }

    std::string const &
    Element::attribute(std::string const & name) const
    {
        static std::string const notFound = "";

        auto it = mAttributes.find(name);

        return it != std::end(mAttributes) ? *it : notFound;
    }

    void
    Element::setAttribute(std::string const & name, std::string const & value)
    {
        m_attributes[name] = value;
    }

    //TODO
    void
    Element::exportToStream(std::ostream & stream, std::string const & indent) const
    {
        for(auto const & c : mChildren)
        {
            stream << indent << c->contentText();
        }
    }

    bool
    Element::isElement() const
    {
        return true;
    }
}