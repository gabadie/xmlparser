/**
 * \file XmlElement.cpp
 * \brief XML Element
 * \author tcantenot
 * \version 0.1
 * \date 18 mars 2014
 */

#include "Utils.hpp"
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

    Element::ElementList
    Element::elements() const
    {
        ElementList elements;
        for(auto const & c : mChildren)
        {
            if(c->isElement())
            {
                elements.push_back(static_cast<Element const *>(c));
            }
        }
        return elements;
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

    void
    Element::exportToStream(std::ostream & stream, std::size_t level, std::string const & indent) const
    {
        stream << Utils::repeat(indent, level) << "<" << mName << " ";

        for(auto const & a : mAttributes)
        {
            stream << a.first << "=\"" << a.second << "\" ";
        }

        stream << ">\n";

        for(auto const & c : mChildren)
        {
            exportToStream(stream, level + 1, indent);
        }

        stream << Utils::repeat(indent, level) << "</" << mName << ">\n";
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
            == std::end(mChildren)
        );
        #endif

        mChildren.push_back(node);
    }
}

