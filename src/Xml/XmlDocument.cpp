/**
 * \file XmlDocument.cpp
 * \brief XML Document
 * \author tcantenot
 * \version 0.1
 * \date 18 mars 2014
 */

#include <algorithm>
#include <iostream>
#include <fstream>

#include "XmlComment.hpp"
#include "XmlDocument.hpp"
#include "XmlDocumentNode.hpp"
#include "XmlElement.hpp"
#include "XmlProcessingInstruction.hpp"

#ifdef APP_DEBUG
#include <cassert>
#endif

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

    std::ostream &
    Document::operator >> (std::ostream & stream) const
    {
        for(auto i = 0u; i < mChildren.size(); ++i)
        {
            auto const & c = mChildren[i];

            #ifdef APP_DEBUG
            assert(c != nullptr);
            #endif

            stream << (*c) << (i == mChildren.size() - 1 ? "" : "\n");
        }

        return stream;
    }

    Document::~Document()
    {
        for(auto & c : mChildren)
        {
            delete c;
        }
    }

    Element const *
    Document::root() const
    {
        return mRoot;
    }

    void
    Document::appendComment(std::string const & comment)
    {
        this->appendNode(new Comment(comment));
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

                #ifdef APP_DEBUG
                assert(it != std::end(mChildren));
                #endif

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
    Document::appendNode(DocumentNode * documentNode)
    {
        #ifdef APP_DEBUG
        assert(documentNode != nullptr);
        assert(
            std::find(std::begin(mChildren), std::end(mChildren), documentNode)
            == std::end(mChildren)
        );
        /* TODO
         * Check that the document node is not a child of root
         */
        #endif

        // A document has only one Xml::Element
        if(documentNode->isElement() && mRoot != nullptr)
        {
            delete mRoot;
        }

        mChildren.push_back(documentNode);
        documentNode->mParent = this;
    }

}
