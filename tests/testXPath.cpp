#include <mk_test.h>

#include <utility>

/*
 * To test the Xml parser, we need to access some protected/private field
 */
#define protected public
#define private public

#include "../src/Xml/XmlDocument.hpp"
#include "../src/Xml/XmlElement.hpp"

/*
 * <root>
 *   <elt1>
 *     <elt11></elt11>
 *     <elt11></elt11>
 *  </elt1>
 *  <elt2>
 *     <elt21></elt21>
 *     <elt22></elt22>
 *  </elt2>
 * </root>
 */
void
testSelect()
{
    Xml::Document doc;

    std::string root = "root";
    auto xmlRoot = new Xml::Element(root);
    doc.setRoot(xmlRoot);

    std::string elt1 = "elt1";
    auto xmlElt1 = new Xml::Element(elt1);
    xmlRoot->append(xmlElt1);

    std::string elt11 = "elt11";
    auto xmlElt11_1 = new Xml::Element(elt11);
    xmlElt1->append(xmlElt11_1);
    auto xmlElt11_2 = new Xml::Element(elt11);
    xmlElt1->append(xmlElt11_2);

    std::string elt2 = "elt2";
    auto xmlElt2 = new Xml::Element(elt2);
    xmlRoot->append(xmlElt2);

    std::string elt21 = "elt21";
    auto xmlElt21 = new Xml::Element(elt21);
    xmlElt2->append(xmlElt21);

    std::string elt22 = "elt22";
    auto xmlElt22 = new Xml::Element(elt22);
    xmlElt2->append(xmlElt22);

    // Lambda to get the nth element of the container returned by Element::select
    static
    auto const
    get = [](decltype(std::declval<Xml::Element &>().select("")) const & r, std::size_t n)
    {
        auto it = std::begin(r);
        std::advance(it, n);
        return *it;
    };

    // Query: "/"
    {
        static
        auto const
        rootQueryTest = [&](Xml::Element const * elt)
        {
            auto results = elt->select("/");
            test_assert(results.size() == 1);
            test_assert(get(results, 0) == xmlRoot);
        };

        rootQueryTest(xmlRoot);
        rootQueryTest(xmlElt1);
        rootQueryTest(xmlElt11_1);
        rootQueryTest(xmlElt11_2);
        rootQueryTest(xmlElt2);
        rootQueryTest(xmlElt21);
        rootQueryTest(xmlElt22);
    }

    // Query: "."
    {

        static
        auto const
        selfQueryTest = [&](Xml::Element const * elt)
        {
            auto results = elt->select(".");
            test_assert(results.size() == 1);
            test_assert(get(results, 0) == elt);
        };

        selfQueryTest(xmlRoot);
        selfQueryTest(xmlElt1);
        selfQueryTest(xmlElt11_1);
        selfQueryTest(xmlElt11_2);
        selfQueryTest(xmlElt2);
        selfQueryTest(xmlElt21);
        selfQueryTest(xmlElt22);
    }

    // Query: ".."
    {

        static
        auto const
        parentQueryTest = [&](Xml::Element const * elt, Xml::Element const * result)
        {
            auto results = elt->select("..");
            if(result != nullptr)
            {
                test_assert(results.size() == 1);
                test_assert(get(results, 0) == result);
            }
            else
            {
                test_assert(results.size() == 0);
            }
        };

        parentQueryTest(xmlRoot, nullptr);
        parentQueryTest(xmlElt1, xmlRoot);
        parentQueryTest(xmlElt11_1, xmlElt1);
        parentQueryTest(xmlElt11_2, xmlElt1);
        parentQueryTest(xmlElt2, xmlRoot);
        parentQueryTest(xmlElt21, xmlElt2);
        parentQueryTest(xmlElt22, xmlElt2);
    }

    // Query: "tag"
    {
        {
            auto results = xmlRoot->select(elt1);
            test_assert(results.size() == 1);
            test_assert(get(results, 0) == xmlElt1);
        }

        {
            auto results = xmlRoot->select(elt2);
            test_assert(results.size() == 1);
            test_assert(get(results, 0) == xmlElt2);
        }

        {
            auto results = xmlElt1->select(elt11);
            test_assert(results.size() == 2);
            test_assert(get(results, 0) == xmlElt11_1);
            test_assert(get(results, 1) == xmlElt11_2);
        }

        {
            auto results = xmlElt2->select(elt21);
            test_assert(results.size() == 1);
            test_assert(get(results, 0) == xmlElt21);
        }

        {
            auto results = xmlElt2->select(elt22);
            test_assert(results.size() == 1);
            test_assert(get(results, 0) == xmlElt22);
        }

        {
            auto results = xmlRoot->select("12456789");
            test_assert(results.size() == 0);
        }
    }

    // Query: "/tag"
    {
        static
        auto const
        rootQueryTest = [&](Xml::Element const * elt, Xml::Element const * rootChild)
        {
            auto results = elt->select("/" + rootChild->name());
            test_assert(results.size() == 1);
            test_assert(get(results, 0) == rootChild);
        };

        rootQueryTest(xmlRoot, xmlElt1);
        rootQueryTest(xmlElt1, xmlElt1);
        rootQueryTest(xmlElt11_1, xmlElt1);
        rootQueryTest(xmlElt11_2, xmlElt1);
        rootQueryTest(xmlElt2, xmlElt1);
        rootQueryTest(xmlElt21, xmlElt1);
        rootQueryTest(xmlElt22, xmlElt1);

        rootQueryTest(xmlRoot, xmlElt2);
        rootQueryTest(xmlElt1, xmlElt2);
        rootQueryTest(xmlElt11_1, xmlElt2);
        rootQueryTest(xmlElt11_2, xmlElt2);
        rootQueryTest(xmlElt2, xmlElt2);
        rootQueryTest(xmlElt21, xmlElt2);
        rootQueryTest(xmlElt22, xmlElt2);

        {
            auto results = xmlElt1->select("/" + root);
            test_assert(results.size() == 0);
        }
    }

    // Query: "tag1/tag2"
    {
        {
            auto results = xmlRoot->select(elt1 + "/" + elt11);
            test_assert(results.size() == 2);
            test_assert(get(results, 0) == xmlElt11_1);
            test_assert(get(results, 1) == xmlElt11_2);
        }

        {
            auto results = xmlRoot->select(elt2 + "/" + elt21);
            test_assert(results.size() == 1);
            test_assert(get(results, 0) == xmlElt21);
        }

        {
            auto results = xmlRoot->select(elt2 + "/" + elt22);
            test_assert(results.size() == 1);
            test_assert(get(results, 0) == xmlElt22);
        }

        {
            auto results = xmlRoot->select(elt1 + "/" + "12456789");
            test_assert(results.size() == 0);
        }
    }

    // Query: "/tag1/tag2"
    {
        {
            auto results = xmlRoot->select("/" + elt1 + "/" + elt11);
            test_assert(results.size() == 2);
            test_assert(get(results, 0) == xmlElt11_1);
            test_assert(get(results, 1) == xmlElt11_2);
        }

        {
            auto results = xmlRoot->select("/" + elt2 + "/" + elt21);
            test_assert(results.size() == 1);
            test_assert(get(results, 0) == xmlElt21);
        }

        {
            auto results = xmlRoot->select("/" + elt2 + "/" + elt22);
            test_assert(results.size() == 1);
            test_assert(get(results, 0) == xmlElt22);
        }

        {
            auto results = xmlRoot->select("/" + elt1 + "/" + "12456789");
            test_assert(results.size() == 0);
        }
    }

    std::string elt111 = "elt111";
    auto xmlElt111_1 = new Xml::Element(elt111);
    xmlElt11_1->append(xmlElt111_1);
    auto xmlElt111_2 = new Xml::Element(elt111);
    xmlElt11_1->append(xmlElt111_2);

    std::string elt1111 = "elt1111";
    auto xmlElt111_21 = new Xml::Element(elt1111);
    xmlElt111_2->append(xmlElt111_21);

    // Query: "tag1/.../tagN"
    {
        {
            auto results = xmlRoot->select(elt1 + "/" + elt11 + "/" + elt111);
            test_assert(results.size() == 2);
            test_assert(get(results, 0) == xmlElt111_1);
            test_assert(get(results, 1) == xmlElt111_2);
        }

        {
            auto results = xmlRoot->select(elt1 + "/" + elt11 + "/" + elt111 + "/" + elt1111);
            test_assert(results.size() == 1);
            test_assert(get(results, 0) == xmlElt111_21);
        }
    }

    // Query: "/tag1/.../tagN"
    {
        {
            auto results = xmlRoot->select("/" + elt1 + "/" + elt11 + "/" + elt111);
            test_assert(results.size() == 2);
            test_assert(get(results, 0) == xmlElt111_1);
            test_assert(get(results, 1) == xmlElt111_2);
        }

        {
            auto results = xmlRoot->select("/" + elt1 + "/" + elt11 + "/" + elt111 + "/" + elt1111);
            test_assert(results.size() == 1);
            test_assert(get(results, 0) == xmlElt111_21);
        }
    }
}

void
testValueOf()
{
    Xml::Document doc;

    std::string root = "root";
    Xml::Element * xmlRoot = new Xml::Element(root);
    doc.setRoot(xmlRoot);

    std::string attr1 = "attr1";
    std::string value1 = "value1";
    xmlRoot->setAttribute(attr1, value1);

    {
        test_assert(xmlRoot->valueOf(".") == "");
        test_assert(xmlRoot->valueOf("@" + attr1) == value1);
        test_assert(xmlRoot->valueOf("@" + attr1 + "foo") == "");
    }

    std::string elt1 = "elt1";
    Xml::Element * xmlElt1 = new Xml::Element(elt1);
    xmlRoot->append(xmlElt1);

    {
        std::string text = "This is a text in elt1.";
        xmlElt1->appendText(text);

        test_assert(xmlElt1->valueOf("@") == "");
        test_assert(xmlElt1->valueOf(".") == text);
        test_assert(xmlElt1->valueOf("/" + elt1) == text);
        test_assert(xmlRoot->valueOf("/" + elt1) == text);
        test_assert(xmlElt1->valueOf("../@" + attr1) == value1);
    }

    std::string elt11 = "elt11";
    Xml::Element * xmlElt11_1 = new Xml::Element(elt11);
    Xml::Element * xmlElt11_2 = new Xml::Element(elt11);
    xmlElt1->append(xmlElt11_1);
    xmlElt1->append(xmlElt11_2);

    {
        std::string text = "This is a text in elt11.";
        xmlElt11_1->appendText(text);

        std::string attr = "attr11";
        std::string value = "value11";
        xmlElt11_2->setAttribute(attr, value);

        test_assert(xmlElt11_1->valueOf(".") == text);
        test_assert(xmlElt1->valueOf(elt11) == text);
        test_assert(xmlElt11_1->valueOf("/" + elt1 + "/" + elt11) == text);
        test_assert(xmlElt11_1->valueOf("/" + elt1 + "/" + elt11 + "/@" + attr) == "");
        test_assert(xmlElt11_2->valueOf("@" + attr) == value);

        std::string attr2 = "attr12";
        std::string value2 = "value12";
        xmlElt11_1->setAttribute(attr2, value2);
        test_assert(xmlElt1->valueOf("/" + elt1 + "/" + elt11 + "/@" + attr2) == value2);
    }
}

int
main()
{
    testSelect();
    testValueOf();

    return 0;
}
