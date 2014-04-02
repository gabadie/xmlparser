
#include "../AppDebug.hpp"
#include "XmlObject.hpp"
#include "XmlComment.hpp"
#include "XmlDocument.hpp"
#include "XmlElement.hpp"
#include "XmlProcessingInstruction.hpp"

#include "../MemoryLeakTrackerOn.hpp"

namespace Xml
{
    Object::~Object()
    {

    }

    std::string const &
    Object::attribute(std::string const &) const
    {
        static std::string const empty = "";

        return empty;
    }

    Document *
    Object::document()
    {
        return const_cast<Document *>(static_cast<Object const *>(this)->document());
    }

    Object *
    Object::parent()
    {
        return const_cast<Object *>(static_cast<Object const *>(this)->parent());
    }

    ElementList
    Object::elements() const
    {
        /*
         * Object::remove is overloaded in Xml::Document and Xml::Element.
         * but other classes don't because they are not supposed to have
         * children elements
         */
        app_unreachable();

        return {};
    }

    ElementList
    Object::elementsByTag(std::string const & tag) const
    {
        ElementList filteredElements;
        ElementList allElements = elements();

        for(auto const & c : allElements)
        {
            app_assert(c != nullptr);
            app_assert(c->mParent == this);
            app_assert(c->isElement() == true);

            auto const element = static_cast<Element const *>(c);
            if(element->tag() == tag)
            {
                filteredElements.push_back(element);
            }
        }

        return filteredElements;
    }

    void
    Object::appendComment(std::string const & comment)
    {
        if (comment == "")
        {
            return;
        }

        this->appendNode(new Comment(comment));
    }

    void
    Object::appendProcessingInstruction(ProcessingInstruction * pi)
    {
        this->appendNode(pi);
    }

    bool
    Object::remove(Node *)
    {
        /*
         * Object::remove is overloaded in Xml::Document and Xml::Element.
         * but other classes don't because they are not supposed to have
         * children elements
         */
        app_unreachable();

        return false;
    }

    std::list<Object const *>
    Object::select(std::string const & xPathQuery) const
    {
        if (xPathQuery == "")
        {
            return {};
        }

        if (xPathQuery == "/")
        {
            auto doc = this->document();

            if(doc == nullptr)
            {
                return {};
            }

            return {static_cast<Object const *>(doc)};
        }

        if (xPathQuery == ".")
        {
            return {this};
        }

        else if (xPathQuery == "..")
        {
            auto p = parent();

            if (p == nullptr)
            {
                return {};
            }

            return {p};
        }
        // If the XPath query has no '/'
        else if (xPathQuery.find("/") == std::string::npos)
        {
            auto elementList = elementsByTag(xPathQuery);

            std::list<Object const *> results;

            for (auto elem : elementList)
            {
                results.push_back(elem);
            }

            return results;
        }

        // If '/' is the first char, we start the query from the root
        if (xPathQuery[0] == '/')
        {
            auto doc = this->document();

            if (doc != nullptr)
            {
                return doc->select(xPathQuery.substr(1));
            }

            return {};
        }

        std::list<Object const *> results;

        auto slashPos = xPathQuery.find("/");

        app_assert(slashPos != std::string::npos);
        app_assert(slashPos != xPathQuery.size() - 1);

        auto token = xPathQuery.substr(0, slashPos);

        // And apply the rest of the query recursively to the Element children
        for (auto const & c : elementsByTag(token))
        {
            auto res = c->select(xPathQuery.substr(slashPos + 1));

            results.splice(std::end(results), res); // Concatenate the results
        }

        return results;
    }

    std::string
    Object::valueOf(std::string const & xPathQuery) const
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

            if (results.size() == 0)
            {
                return "";
            }

            auto node = *std::begin(results);

            if (node->isElement() == false)
            {
                return "";
            }

            // Keep only the first result
            return static_cast<Element const *>(node)->attribute(xPathQuery.substr(atPos + 2));
        }
        // Else if we request the content of an element
        else
        {
            auto results = this->select(xPathQuery);

            if (results.size() == 0)
            {
                return "";
            }

            auto node = *std::begin(results);

            if (node->isElement() == false)
            {
                return "";
            }

            // Keep only the first result
            return static_cast<Element const *>(node)->fullText(); // TODO: move fullText -> Object
        }

        return "";
    }

    void
    Object::appendNode(Node *)
    {
        /*
         * Object::appendNode is overloaded in Xml::Document and Xml::Element.
         * but other classes don't because they are not supposed to have
         * children elements
         */
        app_unreachable();
    }

    bool
    Object::hasChild(Node const *) const
    {
        /*
         * Object::hasChild is overloaded in Xml::Document and Xml::Element.
         * but other classes don't because they are not supposed to have
         * children elements
         */
        app_unreachable();

        return 0;
    }
}

#include "../MemoryLeakTrackerOff.hpp"
