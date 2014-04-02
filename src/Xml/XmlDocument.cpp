/**
 * \file XmlDocument.cpp
 * \brief XML Document
 * \author tcantenot
 * \version 0.1
 * \date 18 mars 2014
 */

#include <algorithm>
#include <fstream>
#include <iosfwd>
#include <iostream>

#include "../AppDebug.hpp"
#include "XmlDocument.hpp"
#include "XmlDocumentNode.hpp"
#include "XmlElement.hpp"

#include "../MemoryLeakTrackerOn.hpp"

namespace Xml
{
    Document::Document(Element * root):
        mRoot(root),
        mChildren()
    {
        if(root != nullptr)
        {
            app_assert(root->mParent == nullptr);

            mChildren.push_back(root);

            root->mParent = this;
        }
    }

    Document::~Document()
    {
        for (auto node : mChildren)
        {
            node->mParent = nullptr;
            delete node;
        }

        mChildren.clear();
    }

    ObjectLabel
    Document::objectLabel() const
    {
        return ObjectLabel::Document;
    }

    Document const *
    Document::document() const
    {
        return this;
    }

    Object const *
    Document::parent() const
    {
        return nullptr;
    }

    Document::NodesList const &
    Document::children() const
    {
        return mChildren;
    }

    bool
    Document::remove(Node * node)
    {
        app_assert(node != nullptr);

        if (node->mParent != this)
        {
            return false;
        }

        {
            auto it = std::find(std::begin(mChildren), std::end(mChildren), node);

            app_assert(it != std::end(mChildren));

            mChildren.erase(it);
        }

        if (node == (Node *) mRoot)
        {
            mRoot = nullptr;
        }

        node->mParent = nullptr;

        return true;
    }

    void
    Document::setRoot(Element * root)
    {
        if(root == mRoot)
        {
            return;
        }

        if(root == nullptr)
        {
            delete mRoot;

            return;
        }

        // If there is already a root node...
        if(mRoot != nullptr)
        {
            auto it = std::find(std::begin(mChildren), std::end(mChildren), mRoot);

            app_assert(it != std::end(mChildren));

            // ...we delete it
            mRoot->mParent = nullptr;
            delete mRoot;

            // and replaces it by the new one
            *it = root;
            root->mParent = this;
        }
        else
        {
            this->appendNode(root);
        }
    }

    bool
    Document::saveToFile(std::string const & path) const
    {
        std::ofstream file(path);

        if(!file.good())
        {
            std::cerr << "Failed to open file: \"" << path << "\"" << std::endl;
            return false;
        }

        file << *this;

        return true;
    }

    void
    Document::exportToStream(std::ostream & stream, std::size_t level, std::string const & indent) const
    {
        for(size_t i = 0u; i < mChildren.size(); ++i)
        {
            auto const & c = mChildren[i];

            app_assert(c != nullptr);
            app_assert(c->mParent == this);

            c->exportToStream(stream, level, indent);

            if (i != (mChildren.size() - 1))
            {
                stream << "\n";
            }
        }
    }

    void
    Document::appendNode(Node * documentNode)
    {
        app_assert(documentNode != nullptr);
        app_assert(canAppend(documentNode)); // make sure we are not appending a Xml::Text

        documentNode->detach();

        // A document has only one Xml::Element
        if (documentNode->isElement())
        {
            if (mRoot != nullptr)
            {
                delete mRoot;
            }

            mRoot = (Element *) documentNode;
        }

        mChildren.push_back((DocumentNode *) documentNode);
        documentNode->mParent = this;
    }

    bool
    Document::hasChild(Node const * node) const
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
                if(c->hasChild(node))
                {
                    return true;
                }
            }
        }

        return false;
    }

    bool
    Document::canAppend(Node const * node)
    {
        app_assert(node != nullptr);

        auto objectLabel = node->objectLabel();

        return objectLabel == ObjectLabel::Comment ||
            objectLabel == ObjectLabel::Doctype ||
            objectLabel == ObjectLabel::Element ||
            objectLabel == ObjectLabel::ProcessingInstruction;
    }

}

#include "../MemoryLeakTrackerOff.hpp"
