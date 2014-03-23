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

namespace Xml
{
    Document::Document(Element * root):
        mRoot(root),
        mChildren()
    {
        if(root != nullptr)
        {
            mChildren.push_back(root);
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

    void
    Document::setRoot(Element * root)
    {
        if(root == mRoot)
        {
            return;
        }

        if(root != nullptr)
        {
            // If there is already a root node...
            if(mRoot != nullptr)
            {
                auto it = std::find(std::begin(mChildren), std::end(mChildren), mRoot);

                app_assert(it != std::end(mChildren));

                // ...we delete it
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
        else
        {
            auto it = std::find(std::begin(mChildren), std::end(mChildren), mRoot);
            mChildren.erase(it);
            delete mRoot;
        }

        mRoot = root;
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
        for(auto i = 0u; i < mChildren.size(); ++i)
        {
            auto const & c = mChildren[i];

            app_assert(c != nullptr);

            c->exportToStream(stream, level, indent);

            stream << (i == mChildren.size() - 1 ? "" : "\n");
        }
    }

    void
    Document::appendNode(Node * documentNode)
    {
        app_assert(documentNode != nullptr);
        app_assert(documentNode->contentText() == ""); // make sure we are not appending a Xml::Text

        app_assert(
            std::find(std::begin(mChildren), std::end(mChildren), documentNode)
            == std::end(mChildren)
        );

        app_assert(mRoot == nullptr || !mRoot->hasChild(documentNode));

        // A document has only one Xml::Element
        if(documentNode->isElement() && mRoot != nullptr)
        {
            auto it = std::find(std::begin(mChildren), std::end(mChildren), mRoot);
            mChildren.erase(it);
            delete mRoot;
        }

        mChildren.push_back((DocumentNode *) documentNode);
        documentNode->mParent = this;
    }

}
