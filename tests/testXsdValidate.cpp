#include "testhelper.hpp"

/*
 * To test the Xml parser, we need to access some protected/private field
 */
#define protected public
#define private public

#include "../src/Xsd/XsdValidate.hpp"

void
testXsdValidateIsDate()
{
    test_assert(Xsd::isDate("2014-01-01"));
    test_assert(Xsd::isDate("2014-12-31"));
    test_assert(!Xsd::isDate("01-01-2014"));
    test_assert(!Xsd::isDate("01-2014-01"));
}

void
testXsdValidateHasRequired()
{
    std::string content (xml_code(
        <?xml version="1.0" encoding="UTF-8" ?>
        <xsd:schema xmlns:xsd="http://www.w3.org/2001/XMLSchema">
            <xsd:element name="infos">
                <xsd:complexType>
                    <xsd:attribute ref="validity" use="required"/>
                    <xsd:attribute name="test2" type="xsd:string"/>
                </xsd:complexType>
            </xsd:element>

            <xsd:attribute name="validity" type="xsd:date"/>
        </xsd:schema>
    ));

    Xml::Log log;
    Xml::Document * xsdDoc = Xml::parse(content, &log);

    test_assert(xsdDoc != nullptr);

    test_assert(Xsd::hasRequired(xsdDoc, "attribute", "validity"));
    test_assert(Xsd::hasRequired(xsdDoc, "element", "infos"));
    test_assert(!Xsd::hasRequired(xsdDoc, "attribute", "foo"));
    test_assert(!Xsd::hasRequired(xsdDoc, "foo", "bar"));
}

int
main()
{
    testXsdValidateIsDate();
    testXsdValidateHasRequired();

    return 0;
}
