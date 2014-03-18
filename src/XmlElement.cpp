/**
 * \file XmlElement.cpp
 * \brief XML Element
 * \author tcantenot
 * \version 0.1
 * \date 18 mars 2014
 */

#include "XmlComment.hpp"
#include "XmlElement.hpp"
#include "XmlText.hpp"

#ifdef APP_DEBUG
#include <algorithm>
#include <cassert>
#endif

namespace Xml
{
    Element::Element(std::string const & name, Node * parent):
        Node(parent),
        mName(name),
        mAttributes(),
        mChildren()
    {

    }

    Element::~Element()
    {
        // Free memory
        this->clearContent();
    }

    Element::NodeList const &
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
            if(parent->isElement())
            {
                return static_cast<Element const *>(parent);
            }
        }
        while((parent = mParent->parent()) != nullptr);

        return nullptr;
    }

    std::string
    Element::text() const
    {
        std::string content = "";

        for(auto const & c : mChildren)
        {
            if(c != nullptr)
            {
                content += c->contentText();
            }
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
                static_cast<Element *>(c)->clearContent();
            }

            delete c;
        }
    }

    void
    Element::append(Element * element)
    {
        this->appendNode(element);
    }

    void
    Element::appendText(std::string const & text)
    {
        this->appendNode(new Text(text));
    }

    void
    Element::appendComment(std::string const & comment)
    {
        this->appendNode(new Comment(comment));
    }

    void
    Element::appendPI(std::string const & pi)
    {
        (void) pi;
        //TODO
        //this->appendNode(new PI(content));
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

        return (it != std::end(mAttributes)) ? it->second : notFound;
    }

    void
    Element::setAttribute(std::string const & name, std::string const & value)
    {
        mAttributes[name] = value;
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

    void
    Element::appendNode(Node * node)
    {
        #ifdef APP_DEBUG
        assert(node != this);

        assert(
            std::find(std::begin(mChildren), std::end(mChildren), node)
            != std::end(mChildren)
        );
        #endif

        mChildren.push_back(node);
    }
}

