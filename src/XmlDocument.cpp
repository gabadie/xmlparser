/**
 * \file XmlDocument.cpp
 * \brief XML Document
 * \author tcantenot
 * \version 0.1
 * \date 18 mars 2014
 */

#include "XmlDocument.hpp"
#include "XmlElement.hpp"
#include "XmlMiscNode.hpp"

namespace Xml
{
    Document::Document(Element * root):
        mRoot(root)
    {

    }

    Document::~Document()
    {
        delete mRoot;
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
        if(root != mRoot)
        {
            delete mRoot;
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

    Document *
    Document::loadFromFile(std::string const & path)
    {
        (void) path;
        //TODO
        return nullptr;
    }

    void
    Document::exportToStream(std::ostream & stream, std::string const & indent) const
    {
        (void) stream;
        (void) indent;
        __builtin_trap();
    }
}
