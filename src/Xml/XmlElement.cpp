/**
 * \file XmlElement.cpp
 * \brief XML Element
 * \author tcantenot
 * \version 0.1
 * \date 18 mars 2014
 */

#include <algorithm>

#include "../Utils.hpp"
#include "XmlComment.hpp"
#include "XmlElement.hpp"
#include "XmlProcessingInstruction.hpp"
#include "XmlText.hpp"

#ifdef APP_DEBUG
#include <cassert>
#endif

namespace Xml
{
    Element::Element(std::string const & name):
        DocumentNode(),
        mName(name),
        mAttributes(),
        mChildren()
    {

    }

    Element::~Element()
    {
        // Free memory
        this->clearContent();

        // Remove the element from its parent
        if(mParent != nullptr && mParent->isElement())
        {
            static_cast<Element *>(mParent)->remove(this);
        }
    }


    bool
    Element::hasChild(Node * node) const
    {
        for(auto const & c : mChildren)
        {
            if(c == node)
            {
                return true;
            }

            if(c->isElement())
            {
                if(static_cast<Element *>(c)->hasChild(node))
                {
                    return true;
                }
            }
        }

        return false;
    }

    Element::NodeList const  &
    Element::children() const
    {
        return mChildren;
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

    Element::ElementList
    Element::elements(std::string const & tag) const
    {
        ElementList elements;
        for(auto const & c : mChildren)
        {
            if(c->isElement())
            {
                auto const element = static_cast<Element const *>(c);
                if(element->mName == tag)
                {
                    elements.push_back(element);
                }
            }
        }
        return elements;
    }

    Element const *
    Element::parentElement() const
    {
        auto parent = mParent;

        while(parent != nullptr)
        {
            if(parent->isElement())
            {
                return static_cast<Element const *>(parent);
            }

            parent = parent->parent();
        }

        return nullptr;
    }

    std::string
    Element::text() const
    {
        std::string content = "";

        for(auto const & c : mChildren)
        {
            #ifdef APP_DEBUG
            assert(c != nullptr);
            #endif

            std::string text = c->contentText();
            if(text.size() > 0)
            {
                content += text + "\n";
            }
        }

        return content.size() > 0 ? content.substr(0, content.size() - 1) : content;
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
        auto it = std::begin(mChildren);

        while(it != std::end(mChildren))
        {
            (*it)->mParent = nullptr;
            delete (*it++);
        }

        mChildren.clear();
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
    Element::appendProcessingInstruction(ProcessingInstruction * pi)
    {
        this->appendNode(pi);
    }

    bool
    Element::remove(Node * node)
    {
        auto it = std::find(std::begin(mChildren), std::end(mChildren), node);

        if(it != std::end(mChildren))
        {
            node->mParent = nullptr;
            delete node;
            mChildren.erase(it);
            return true;
        }

        return false;
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

    std::list<Element const *>
    Element::select(std::string const & xPathQuery) const
    {
        std::list<Element const *> results;

        if(xPathQuery == "/")
        {
            auto root = this->root();
            if(root != nullptr)
            {
                results.push_back(root);
            }
        }
        else if(xPathQuery == ".")
        {
            results.push_back(this);
        }
        else if(xPathQuery == "..")
        {
            if(mParent != nullptr && mParent->isElement())
            {
                results.push_back(static_cast<Element *>(mParent));
            }
        }
        // If the XPath query has no '/'
        else if(xPathQuery.find("/") == std::string::npos)
        {
            // We retrieve the element children that match the query
            for(auto const & c : mChildren)
            {
                #ifdef APP_DEBUG
                assert(c != nullptr);
                #endif

                if(!c->isElement()) continue;

                auto elt = static_cast<Element *>(c);
                if(elt->name() == xPathQuery)
                {
                    results.push_back(elt);
                }
            }
        }
        // Else if the XPath query has at least one '/'
        else
        {
            // If '/' is the first char, we start the query from the root
            if(xPathQuery[0] == '/')
            {
                auto root = this->root();
                if(root != nullptr)
                {
                    return root->select(xPathQuery.substr(1));
                }
            }
            // Otherwise we get the first token...
            else
            {
                auto slashPos = xPathQuery.find("/");

                #ifdef APP_DEBUG
                assert(slashPos != std::string::npos);
                assert(slashPos != xPathQuery.size() - 1);
                #endif

                auto token = xPathQuery.substr(0, slashPos);

                // And apply the rest of the query recursively to the Element children
                for(auto const & c : mChildren)
                {
                    #ifdef APP_DEBUG
                    assert(c != nullptr);
                    #endif

                    if(!c->isElement()) continue;

                    auto elt = static_cast<Element *>(c);
                    if(elt->name() == token)
                    {
                        auto res = elt->select(xPathQuery.substr(slashPos + 1));
                        results.splice(std::end(results), res); // Concatenate the results
                    }
                }
            }
        }

        return results;
    }

    void
    Element::exportToStream(std::ostream & stream, std::size_t level, std::string const & indent) const
    {
        stream << Utils::repeat(indent, level) << "<" << mName;

        for(auto const & a : mAttributes)
        {
            stream << " " << a.first << "=\"" << a.second << "\"";
        }

        stream << ">\n";

        for(auto const & c : mChildren)
        {
            #ifdef APP_DEBUG
            assert(c != nullptr);
            #endif

            c->exportToStream(stream, level + 1, indent);
            stream << "\n";
        }

        stream << Utils::repeat(indent, level) << "</" << mName << ">";
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
        assert(node != nullptr);
        assert(node != this);
        assert(
            std::find(std::begin(mChildren), std::end(mChildren), node)
            == std::end(mChildren)
        );
        #endif

        mChildren.push_back(node);
        node->mParent = this;
    }
}

