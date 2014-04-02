#include <mk_test.h>
#include <regex>
#include <iostream>
#include <dirent.h>
#include <sstream>
#include <string>
#include <re2/re2.h>


#include "testhelper.hpp"
#include "../src/Xsd/XsdChecker.hpp"

#define DEBUG_MODE

using namespace Xsd;

std::string xmlFolder = "./tests/xml_files";
std::string xsdFolder = "./tests/xsd_files";
std::string invalidFileRegexp = "^[0-9]*_err_.*$";

std::string
getFilePath(std::string folderPath, std::string fileName)
{
    std::ostringstream out;
    out << folderPath << "/" << fileName;
    return out.str();
}

std::string
getFileNameNoExt(std::string fileName)
{
    std::size_t idx = fileName.find(".");
    if(idx != std::string::npos)
    {
        return fileName.substr(0, idx);
    }
    return fileName;
}

std::string
getXsdFileNameFromXmlError(std::string fileName)
{
    int idx = fileName.find("_", 0);
    std::string start = fileName.substr(0, idx+1);
    std::string end = fileName.substr(idx+3, std::string::npos);
    return start + getFileNameNoExt(end) + ".xsd";
}

void
test_construction()
{
    //TODO test the construction of the intermediary XSD structure
    //Example: test_assert(doc->root()->name() == "hello");
}

void
test_validation()
{
    DIR * dir;
    struct dirent * ent;
    std::string xmlFilePath;
    std::string xsdFilePath;
    Xml::Log xmlLog;
    bool valid;

    dir = opendir (xmlFolder.c_str());

    if(dir == NULL)
    {
        std::cerr << "Error: Cannot find " << xmlFolder << " folder" << std::endl;
        return;
    }

    while ((ent = readdir (dir)) != NULL)
    {
        xmlFilePath = getFilePath(xmlFolder, ent->d_name);

        if (RE2::FullMatch(std::string(ent->d_name), invalidFileRegexp))
        {
            xsdFilePath = getXsdFileNameFromXmlError(ent->d_name);
            valid = false;
            std::cout << "Invalidation of " << xmlFilePath << " from " << xsdFilePath << std::endl;
        }
        else
        {
            xsdFilePath = getFilePath(xsdFolder, getFileNameNoExt(ent->d_name) + ".xsd");
            valid = true;
            std::cout << "Validation of " << xmlFilePath << " from " << xsdFilePath << std::endl;
        }


        Xml::Document * xmlDoc = Xml::load(xmlFilePath, &xmlLog);
        Xml::Document * xsdDoc = Xml::load(xsdFilePath, &xmlLog);

        // Building XSD Checker
        Xsd::Checker * checker = Xsd::Checker::parseXsd(xsdDoc);
        if(Xsd::Checker::parseXsd(xsdDoc) == NULL)
        {
            return;
        }

        // Validation process
        if(valid)
        {
            test_assert(checker->isValid(xmlDoc, checker));
        }
        else
        {
            test_assert(!checker->isValid(xmlDoc, checker));
        }

        delete xmlDoc;
        delete checker;
    }
    closedir (dir);
}

//=============================================================================
void testValidation(const std::string & xmlContent, const std::string & xmlWrongContent, const std::string & xsdContent)
{
    Xml::Log log;
    Xml::Document * xmlDoc = Xml::parse(xmlContent, &log);
    Xml::Document * xmlWrongDoc = Xml::parse(xmlWrongContent, &log);
    Xml::Document * xsdDoc = Xml::parse(xsdContent, &log);

    if(xmlDoc == nullptr || xmlWrongDoc == nullptr || xsdDoc == nullptr)
    {
        std::cerr << "Error: A document could not be parsed" << std::endl;
        return;
    }

    Xsd::Checker * checker = Xsd::Checker::parseXsd(xsdDoc);
    test_assert(checker != NULL);

    if(checker == nullptr)
    {
        std::cerr << "Error: XSD document could not be built" << std::endl;
        return;
    }
    test_assert(checker->isValid(xmlDoc, checker));
    test_assert(!checker->isValid(xmlWrongDoc, checker));

    delete xmlDoc;
    delete xmlWrongDoc;
    delete xsdDoc;
    delete checker;
}

void testValidation(const std::string & xmlContent, const std::string & xsdContent)
{
    Xml::Log log;
    Xml::Document * xmlDoc = Xml::parse(xmlContent, &log);
    Xml::Document * xsdDoc = Xml::parse(xsdContent, &log);

    test_assert(xmlDoc != nullptr && xsdDoc != nullptr);
    Xsd::Checker * checker = Xsd::Checker::parseXsd(xsdDoc);
    test_assert(checker != NULL);
    test_assert(checker->isValid(xmlDoc, checker));

    delete xmlDoc;
    delete xsdDoc;
    delete checker;
}

void
testOneElement()
{
    std::string xmlContent (xml_code(
        <?xml version="1.0" encoding="UTF-8"?>
        <person xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../xsd_files/1_one_element.xsd">
            test
        </person>
    ));

    std::string xmlWrongContent (xml_code(
        <?xml version="1.0" encoding="UTF-8"?>
        <test xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../xsd_files/1_one_element.xsd">
            test
        </test>
    ));

    std::string xsdContent (xml_code(
        <?xml version="1.0" encoding="UTF-8" ?>
        <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
          <xs:element name="person" type="xs:string"/>
        </xs:schema>
    ));

    testValidation(xmlContent, xmlWrongContent, xsdContent);
    //TODO: check the XSD construction
}

void
testComplexType()
{
    std::string xmlContent (xml_code(
        <?xml version="1.0" encoding="UTF-8"?>
        <person xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../xsd_files/2_complex_type.xsd">
            <address>myAddress</address>
        </person>
    ));

    std::string xmlWrongContent (xml_code(
        <?xml version="1.0" encoding="UTF-8"?>
        <person xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../xsd_files/2_complex_type.xsd">
            <test>myAddress</test>
        </person>
    ));

    std::string xsdContent (xml_code(
        <?xml version="1.0" encoding="UTF-8" ?>
        <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
          <xs:element name="person">
            <xs:complexType>
                <xs:choice>
                    <xs:element name="address" type="xs:string"/>
                </xs:choice>
            </xs:complexType>
          </xs:element>
        </xs:schema>
    ));

    testValidation(xmlContent, xmlWrongContent, xsdContent);
}

void
testRecursiveComplexType()
{
    std::string xmlContent (xml_code(
        <?xml version="1.0" encoding="UTF-8"?>
        <person xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../xsd_files/3_recursive_complex_type.xsd">
            <address validity="24-02-2014">myAddress</address>
        </person>
    ));

    std::string xmlWrongContent (xml_code(
        <?xml version="1.0" encoding="UTF-8"?>
        <person xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../xsd_files/3_recursive_complex_type.xsd">
            <address validity="24-02-214">myAddress</address>
        </person>
    ));

    std::string xsdContent (xml_code(
        <?xml version="1.0" encoding="UTF-8" ?>
        <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
            <xs:element name="person">
                <xs:complexType>
                    <xs:choice>
                    <xs:element name="address">
                        <xs:complexType>
                            <xs:attribute name="validity" type="xs:date" use="required"/>
                        </xs:complexType>
                    </xs:element name="address">
                    </xs:choice>
                </xs:complexType>
            </xs:element>
        </xs:schema>
    ));

    testValidation(xmlContent, xmlWrongContent, xsdContent);
}

void
testSequence()
{
    std::string xmlContent (xml_code(
        <?xml version="1.0" encoding="UTF-8"?>
        <person xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../xsd_files/4_sequence.xsd">
            <infos>
                <surname>King</surname>
                <name>Julian</name>
                <birth>02-04-2022</birth>
            </infos>
        </person>
    ));

    std::string xmlWrongContent (xml_code(
        <?xml version="1.0" encoding="UTF-8"?>
        <person xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../xsd_files/4_sequence.xsd">
            <infos>
                <surname>King</surname>
                <birth>02-04-2022</birth>
            </infos>
        </person>
    ));

    std::string xsdContent (xml_code(
        <?xml version="1.0" encoding="UTF-8" ?>
        <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
            <xs:element name="person">
                <xs:complexType>
                    <xs:choice>
                    <xs:element name="infos">
                        <xs:complexType>
                            <xs:sequence>
                                <xs:element name="surname" type="xs:string"/>
                                <xs:element name="name" type="xs:string"/>
                                <xs:element name="birth" type="xs:date"/>
                            </xs:sequence>
                        </xs:complexType>
                    </xs:element>
                    </xs:choice>
                </xs:complexType>
            </xs:element>
        </xs:schema>
    ));

    testValidation(xmlContent, xmlWrongContent, xsdContent);
}

void
testChoice()
{
    std::string xmlContent (xml_code(
        <?xml version="1.0" encoding="UTF-8"?>
        <person xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../xsd_files/5_choice.xsd">
            <infos>
                <surname>King</surname>
                <name>Julian</name>
                <birth>02-04-2022</birth>
                <mobile>lalala</mobile>
            </infos>
        </person>
    ));

    std::string xmlWrongContent (xml_code(
        <?xml version="1.0" encoding="UTF-8"?>
        <person xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../xsd_files/5_choice.xsd">
            <infos>
                <surname>King</surname>
                <name>Julian</name>
                <birth>02-04-2022</birth>
                <mobile>lalala</mobile>
                <tel>lalala</tel>
            </infos>
        </person>
    ));

    std::string xsdContent (xml_code(
        <?xml version="1.0" encoding="UTF-8" ?>
        <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
            <xs:element name="person">
                <xs:complexType>
                    <xs:element name="infos">
                        <xs:complexType>
                            <xs:sequence>
                                <xs:element name="surname" type="xs:string"/>
                                <xs:element name="name" type="xs:string"/>
                                <xs:element name="birth" type="xs:date"/>
                                <xs:choice>
                                    <xs:element name="tel" type="xs:string"/>
                                    <xs:element name="mobile" type="xs:string"/>
                                </xs:choice>
                            </xs:sequence>
                        </xs:complexType>
                    </xs:element>
                </xs:complexType>
            </xs:element>

            <xs:element name="animal" type="xs:string"/>
        </xs:schema>
    ));

    testValidation(xmlContent, xmlWrongContent, xsdContent);
}

void
testRefAttribute()
{
    std::string xmlContent (xml_code(
        <?xml version="1.0" encoding="UTF-8"?>
        <person xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../xsd_files/5_choice.xsd">
            <infos>
                <surname>King</surname>
                <name>Julian</name>
                <birth>02-04-2022</birth>
                <mobile>lalala</mobile>
            </infos>
        </person>
    ));

    std::string xmlWrongContent (xml_code(
        <?xml version="1.0" encoding="UTF-8"?>
        <person xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../xsd_files/5_choice.xsd">
            <infos>
                <surname>King</surname>
                <name>Julian</name>
                <birth>02-04-2022</birth>
                <mobile>lalala</mobile>
            </infos>
        </person>
    ));

    std::string xsdContent (xml_code(
        <?xml version="1.0" encoding="UTF-8" ?>
        <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
            <xs:element name="person">
                <xs:complexType>
                    <xs:choice>
                    <xs:element name="address">
                        <xs:complexType>
                            <xs:attribute name="patate" type="xs:string"/>
                            <xs:attribute ref="validity" use="required"/>
                        </xs:complexType>
                    </xs:element>
                    </xs:choice>
                </xs:complexType>
            </xs:element>

            <xs:attribute name="validity" type="xs:date"/>
        </xs:schema>
    ));

    testValidation(xmlContent, xmlWrongContent, xsdContent);
}

void
testRefType()
{
    std::string xmlContent (xml_code(
        <?xml version="1.0" encoding="UTF-8"?>
        <person xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../xsd_files/5_choice.xsd">
            <infos>
                <surname>King</surname>
                <name>Julian</name>
                <birth>02-04-2022</birth>
                <mobile>lalala</mobile>
            </infos>
        </person>
    ));

    std::string xsdContent (xml_code(
        <?xml version="1.0" encoding="UTF-8" ?>
        <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
            <xs:element name="person">
                <xs:complexType>
                    <xs:choice>
                        <xs:element name="address" type="addressType"/>
                    </xs:choice>
                </xs:complexType>
            </xs:element>

            <xs:complexType name="addressType">
                <xs:attribute ref="validity" use="required"/>
            </xs:complexType>

            <xs:attribute name="validity" type="xs:date"/>
        </xs:schema>
    ));

    testValidation(xmlContent, xsdContent);
}

void
testRefElement()
{
    std::string xmlContent (xml_code(
        <?xml version="1.0" encoding="UTF-8"?>
        <person xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../xsd_files/5_choice.xsd">
            <infos>
                <surname>King</surname>
                <name>Julian</name>
                <birth>02-04-2022</birth>
                <mobile>lalala</mobile>
            </infos>
        </person>
    ));

    std::string xsdContent (xml_code(
        <?xml version="1.0" encoding="UTF-8" ?>
        <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
            <xs:element name="person">
                <xs:complexType>
                    <xs:choice>
                        <xs:element ref="address"/>
                    <xs:choice>
                </xs:complexType>
            </xs:element>

            <xs:element name="address" type="addressType"/>

            <xs:complexType name="addressType">
                <xs:attribute ref="validity" use="required"/>
            </xs:complexType>

            <xs:attribute name="validity" type="xs:date"/>
        </xs:schema>
    ));

    testValidation(xmlContent, xsdContent);
}

void
testOccurs()
{
    std::string xmlContent (xml_code(
        <?xml version="1.0" encoding="UTF-8"?>
        <person xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../xsd_files/9_occurs.xsd">
            <infos validity="02-05-2014" invalidity="02-05-2018" test="patate">
                <surname>Kin1</surname>
                <surname>King2</surname>
                <surname>King3</surname>
                <surname>King4</surname>
                <name>Julian</name>
                <mobile>lalala</mobile>
                <patate>lalala</patate>
                <pomme>lalala</pomme>
            </infos>
        </person>
    ));

    std::string xmlWrongContent (xml_code(
        <?xml version="1.0" encoding="UTF-8"?>
        <person xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="../xsd_files/9_occurs.xsd">
            <infos validity="02-05-2014" invalidity="02-05-2018" test="patate">
                <surname>Kin1</surname>
                <surname>King2</surname>
                <surname>King3</surname>
                <surname>King4</surname>
                <surname>King4</surname>
                <surname>King4</surname>
                <name>Julian</name>
                <mobile>lalala</mobile>
                <mobile>lalala</mobile>
                <mobile>lalala</mobile>
                <patate>lalala</patate>
                <pomme>lalala</pomme>
            </infos>
        </person>
    ));

    std::string xsdContent (xml_code(
        <?xml version="1.0" encoding="UTF-8" ?>
        <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
            <xs:element name="person" type="personType"/>

            <xs:complexType name="personType">
                <xs:choice>
                    <xs:element ref="infos"/>
                <xs:choice>
            </xs:complexType>

            <xs:element name="infos">
                <xs:complexType>
                    <xs:sequence>
                        <xs:element ref="surname" minOccurs="2" maxOccurs="unbounded"/>
                        <xs:element name="name" type="xs:string"/>
                        <xs:element name="birth" type="xs:date"/>
                        <xs:choice minOccurs="2" maxOccurs="5">
                            <xs:element name="tel" type="xs:string"/>
                            <xs:choice>
                                <xs:element name="patate" type="xs:string"/>
                                <xs:element name="pomme" type="xs:string"/>
                            </xs:choice>
                            <xs:element name="mobile" type="xs:string"/>
                        </xs:choice>
                    </xs:sequence>
                    <xs:attribute ref="validity" use="required"/>
                    <xs:attribute name="invalidity" type="xs:date" use="required"/>
                    <xs:attribute name="test" type="xs:string"/>
                    <xs:attribute name="test2" type="xs:string"/>
                </xs:complexType>
            </xs:element>

            <xs:attribute name="validity" type="xs:date"/>

            <xs:element name="surname" type="xs:string"/>
        </xs:schema>
    ));

    testValidation(xmlContent, xmlWrongContent, xsdContent);
}

void
testWrongXsd()
{
    std::string xsdContent (xml_code(
        <?xml version="1.0" encoding="UTF-8" ?>
        <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
            <xs:element name="person" type="personType"/>

            <xs:complexType name="personType">
                <xs:element ref=""/>
            </xs:complexType>

            <xs:element name="infos">
                <xs:complexType>
                    <xs:sequence>
                        <xs:element ref="surname" minOccurs="unbounded" maxOccurs="2"/>
                        <xs:element name="name" type="xs:string"/>
                        <xs:element name="birth" type="xs:date"/>
                        <xs:choice minOccurs="2" maxOccurs="5">
                            <xs:element name="tel" type="xs:string"/>
                            <xs:choice>
                                <xs:element name="patate" type="xs:string"/>
                                <xs:element name="pomme" type="xs:string"/>
                            </xs:choice>
                            <xs:element name="mobile" type="xs:string"/>
                        </xs:choice>
                    </xs:sequence>
                    <xs:attribute ref="validity" use="456"/>
                    <xs:attribute name="invalidity" type="xs:date" use="required"/>
                    <xs:attribute name="test" type="xs:string"/>
                    <xs:attribute name="test2" type="xs:string"/>
                </xs:complexType>
            </xs:element>

            <xs:attr name="validity" type="date"/>

            <element name="surname" type="xs:string"/>
        </xs:schema>
    ));

    Xml::Log log;
    Xml::Document * xsdDoc = Xml::parse(xsdContent, &log);
    Xsd::Checker * checker = Xsd::Checker::parseXsd(xsdDoc);
    test_assert(checker == NULL);

    delete xsdDoc;
}

int
main()
{
    //testOneElement();
    testComplexType();
    //testRecursiveComplexType();
    // testSequence();
    // testChoice();
    // testRefAttribute();
    // testRefType();
    // testRefElement();
    // testOccurs();
    //testWrongXsd();

    test_construction();    //TODO
    //test_validation();

    return 0;
}
