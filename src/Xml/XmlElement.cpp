/**
 * \file XmlElement.cpp
 * \brief XML Element
 * \author tcantenot
 * \version 0.1
 * \date 18 mars 2014
 */

#include <algorithm>
#include <sstream>

#include "../AppDebug.hpp"
#include "../Utils.hpp"
#include "XmlConsts.hpp"
#include "XmlDocument.hpp"
#include "XmlElement.hpp"
#include "XmlText.hpp"

#include "../MemoryLeakTrackerOn.hpp"

namespace Xml
{
    Element::Element(std::string const & name, std::string const & namespaceName):
        DocumentNode(),
        mName(name),
        mNamespaceName(namespaceName),
        mAttributes(),
        mChildren()
    {
        app_assert(name != "");
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

    ObjectLabel
    Element::objectLabel() const
    {
        return ObjectLabel::Element;
    }

    Node *
    Element::clone() const
    {
        Xml::Element* elementClone =  new Element(this->mName, this->mNamespaceName);
        elementClone->mAttributes = AttributesMap(this->mAttributes);
        return elementClone;
    }


    NodeList const  &
    Element::children() const
    {
        return mChildren;
    }

    ElementList
    Element::elements() const
    {
        ElementList elements;
        for(auto const & c : mChildren)
        {
            app_assert(c != nullptr);
            app_assert(c->mParent == this);

            if(c->isElement())
            {
                elements.push_back(static_cast<Element const *>(c));
            }
        }
        return elements;
    }

    ElementList
    Element::elements(std::string const & tag) const
    {
        ElementList elements;
        for(auto const & c : mChildren)
        {
            app_assert(c != nullptr);
            app_assert(c->mParent == this);

            if(c->isElement())
            {
                auto const element = static_cast<Element const *>(c);
                if(element->tag() == tag)
                {
                    elements.push_back(element);
                }
            }
        }
        return elements;
    }

    std::string
    Element::text() const
    {
        std::ostringstream contentStream;

        for(auto const & c : mChildren)
        {
            app_assert(c != nullptr);
            app_assert(c->mParent == this);

            std::string const text = c->contentText();

            if(text.size() == 0)
            {
                continue;
            }

            if (contentStream.tellp() != 0)
            {
                contentStream << Xml::CAT_SEPARATOR;
            }

            contentStream << text;
        }

        return contentStream.str();
    }

    std::string
    Element::fullText() const
    {
        std::ostringstream contentStream;

        for(auto const & c : mChildren)
        {
            app_assert(c != nullptr);
            app_assert(c->mParent == this);

            if(c->isElement())
            {
                std::string const text = static_cast<Element *>(c)->fullText();

                if(text.size() > 0)
                {
                    if (contentStream.tellp() != 0)
                    {
                        contentStream << Xml::CAT_SEPARATOR;
                    }

                    contentStream << text;
                }
            }

            std::string const text = c->contentText();

            if(text.size() == 0)
            {
                continue;
            }

            if (contentStream.tellp() != 0)
            {
                contentStream << Xml::CAT_SEPARATOR;
            }

            contentStream << text;
        }

        return contentStream.str();
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
        for (auto node : mChildren)
        {
            app_assert(node != nullptr);
            app_assert(node->mParent == this);

            node->mParent = nullptr;
            delete node;
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
        if (text == "")
        {
            return;
        }

        this->appendNode(new Text(text));
    }

    bool
    Element::remove(Node * node)
    {
        app_assert(node != nullptr);

        if (node->mParent != this)
        {
            return false;
        }

        auto it = std::find(std::begin(mChildren), std::end(mChildren), node);

        app_assert(it != std::end(mChildren));

        mChildren.erase(it);
        node->mParent = nullptr;

        return true;
    }

    std::string const &
    Element::name() const
    {
        return mName;
    }

    void
    Element::setName(std::string const & name)
    {
        app_assert(name != "");

        mName = name;
    }

    std::string const &
    Element::namespaceName() const
    {
        return mNamespaceName;
    }

    void
    Element::setNamespaceName(std::string const & namespaceName)
    {
        app_assert(namespaceName != "");

        mNamespaceName = namespaceName;
    }

    std::string
    Element::tag() const
    {
        if (mNamespaceName == "")
        {
            return mName;
        }
        else
        {
            std::string tag = mNamespaceName + ":" + mName;
            return tag;
        }
    }

    std::string const &
    Element::attribute(std::string const & name) const
    {
        static std::string const notFound = "";

        auto it = mAttributes.find(name);

        return (it != std::end(mAttributes)) ? it->second : notFound;
    }

    AttributesMap const &
    Element::attributes() const
    {
        return mAttributes;
    }

    AttributesMap
    Element::namespaceAttributes(std::string const & name) const
    {
        AttributesMap map;

        if (name == "")
        {
            for (auto const & i : mAttributes)
            {
                if (i.first.find(":") == std::string::npos)
                {
                    map.insert(i);
                }
            }

            return map;
        }

        std::string prefix = name + ":";

        for (auto const & i : mAttributes)
        {
            if (i.first.substr(0, prefix.size()) == prefix)
            {
                std::string name = i.first.substr(prefix.size(), std::string::npos);
                map.insert({name, i.second});
            }
        }

        return map;
    }

    void
    Element::setAttribute(std::string const & name, std::string const & value)
    {
        app_assert(name != "");

        mAttributes[name] = value;
    }

    std::list<Element const *>
    Element::select(std::string const & xPathQuery) const
    {
        std::list<Element const *> results;

        if(xPathQuery == "")
        {
            return results;
        }

        if(xPathQuery == "/")
        {
            auto doc = this->document();
            auto root = doc ? doc->root() : nullptr;

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
                app_assert(c != nullptr);
                app_assert(c->mParent == this);

                if(!c->isElement())
                {
                    continue;
                }

                auto elt = static_cast<Element *>(c);
                if(elt->tag() == xPathQuery)
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
                auto doc = this->document();
                auto root = doc ? doc->root() : nullptr;
                if(root != nullptr)
                {
                    return root->select(xPathQuery.substr(1));
                }
            }
            // Otherwise we get the first token...
            else
            {
                auto slashPos = xPathQuery.find("/");

                app_assert(slashPos != std::string::npos);
                app_assert(slashPos != xPathQuery.size() - 1);

                auto token = xPathQuery.substr(0, slashPos);

                // And apply the rest of the query recursively to the Element children
                for(auto const & c : mChildren)
                {
                    app_assert(c != nullptr);

                    if(!c->isElement())
                    {
                        continue;
                    }

                    auto elt = static_cast<Element *>(c);
                    if(elt->tag() == token)
                    {
                        auto res = elt->select(xPathQuery.substr(slashPos + 1));
                        results.splice(std::end(results), res); // Concatenate the results
                    }
                }
            }
        }

        return results;
    }


    bool
    Element::matches(std::string const & pattern) const {
        if (pattern == "") {
            return true;
        }

        if (pattern == "/")
        {
            return this->document()->root() == this;
        }

        auto slashPos = pattern.find_last_of("/");
        if (slashPos == std::string::npos) {
            return this->name() == pattern;
        }
        else {
            auto lastToken = pattern.substr(slashPos + 1, pattern.size() - 1);
            if (this->name() != lastToken || this->parent() == nullptr || !this->parent()->isElement()) {
                return false;
            }
            auto parent = static_cast<Xml::Element const *>(this->parent());
            auto parentPattern = pattern.substr(0, slashPos);
            return parent->matches(parentPattern);
        }
    }

    std::string
    Element::valueOf(std::string const & xPathQuery) const
    {
        if(xPathQuery == "")
        {
            return "";
        }

        // If we request an attribute of the current element
        if(xPathQuery[0] == '@')
        {
            return this->attribute(xPathQuery.substr(1));
        }

        // If we request the attribute of another element
        auto atPos = xPathQuery.find("/@");
        if(atPos != std::string::npos)
        {
            auto results = this->select(xPathQuery.substr(0, atPos));
            // Keep only the first result
            return results.size() > 0 ?
                (*std::begin(results))->attribute(xPathQuery.substr(atPos + 2)) : "";
        }
        // Else if we request the content of an element
        else
        {
            auto results = this->select(xPathQuery);
            // Keep only the first result
            return results.size() > 0 ? (*std::begin(results))->fullText() : "";
        }

        return "";
    }

    void
    Element::exportToStream(std::ostream & stream, std::size_t level, std::string const & indent) const
    {
        stream << Utils::repeat(indent, level) << "<" << (mNamespaceName.size() > 0 ? mNamespaceName + ":" : "") << mName;

        for(auto const & a : mAttributes)
        {
            stream << " " << a.first << "=\"" << a.second << "\"";
        }

        // If the element has no child, we close the tag and stop
        if(mChildren.size() == 0)
        {
            stream << "/>";
            return;
        }

        stream << ">\n";

        // else we display each child
        for(auto const & c : mChildren)
        {
            app_assert(c != nullptr);
            app_assert(c->mParent == this);

            c->exportToStream(stream, level + 1, indent);
            stream << "\n";
        }

        stream << Utils::repeat(indent, level) << "</" << (mNamespaceName.size() > 0 ? mNamespaceName + ":" : "") << mName << ">";
    }

    void
    Element::appendNode(Node * node)
    {
        app_assert(node != nullptr);
        app_assert(node != this);
        app_assert(canAppend(node));

        node->detach();

        mChildren.push_back(node);
        node->mParent = this;
    }

    bool
    Element::hasChild(Node const * node) const
    {
        for(auto const & c : mChildren)
        {
            app_assert(c != nullptr);
            app_assert(c->mParent == this);

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

    bool
    Element::canAppend(Node const * node)
    {
        app_assert(node != nullptr);

        auto objectLabel = node->objectLabel();

        return objectLabel == ObjectLabel::CharacterData ||
            objectLabel == ObjectLabel::Comment ||
            objectLabel == ObjectLabel::Element ||
            objectLabel == ObjectLabel::ProcessingInstruction ||
            objectLabel == ObjectLabel::Text;
    }

}

#include "../MemoryLeakTrackerOff.hpp"
