/**
 * \file XmlDocument.cpp
 * \brief XML Document
 * \author tcantenot
 * \version 0.1
 * \date 18 mars 2014
 */

#include <algorithm>

#include "XmlDocument.hpp"
#include "XmlElement.hpp"
#include "XmlMiscNode.hpp"

#ifdef APP_DEBUG
#include <cassert>
#endif

namespace Xml
{
    Document::Document(Element * root):
        mRoot(root)
    {

    }

    std::ostream &
    Document::operator >> (std::ostream & stream) const
    {
        for(auto const & c : mChildren)
        {
            stream << c << "\n";
        }
    }

    Document::~Document()
    {
        for(auto & c : mChildren)
        {
            delete c;
        }
    }

    void
    Document::append(DocumentNode * documentNode)
    {
        #ifdef APP_DEBUG
        assert(
            std::find(std::begin(mChildren), std::end(mChildren), documentNode)
            != std::end(mChildren)
        );
        #endif

        mChildren.push_back(documentNode);
    }

    Element *
    Document::root()
    {
        return mRoot;
    }

    Element const *
    Document::root() const
    {
        return mRoot;
    }

    void
    Document::setRoot(Element * root)
    {
        if(root != nullptr && root != mRoot)
        {
            auto it = std::find(std::begin(mChildren), std::end(mChildren), mRoot);

            #ifdef APP_DEBUG
            assert(it != std::end(mChildren));
            #endif

            delete mRoot;

            *it = root;
            mRoot = root;
        }
    }

    bool
    Document::saveToFile(std::string const & path) const
    {
        (void) path;
        //TODO
        return false;
    }
}
