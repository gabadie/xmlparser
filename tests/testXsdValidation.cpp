#include <mk_test.h>
#include <regex>
#include <iostream>
#include <dirent.h>
#include <sstream>
#include <string>
#include <re2/re2.h>

#include "../src/Xsd/XsdChecker.hpp"

#define DEBUG_MODE

using namespace Xsd;

std::string xmlFolder = "../../tests/xml_files";
std::string xsdFolder = "../../tests/xsd_files";
std::string invalidFileRegexp = "^[0-9]*_err_.*$";

std::string
getFilePath(std::string folderPath, std::string fileName)
{
    std::ostringstream out;
    out << folderPath << "/" << fileName;
    return out.str();
}

std::string
getXsdFileNameFromXmlError(std::string fileName)
{
    int idx = fileName.find("_", 0);
    std::string start = fileName.substr(0, idx+1);
    std::string end = fileName.substr(idx+3, std::string::npos);
    return start + end;
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
        std::cerr << "Cannot find " << xmlFolder << " folder" << std::endl;
        test_assert(false);
        std::cerr << "Cannot find " << xmlFolder << " folder" << std::endl;
    }

    while ((ent = readdir (dir)) != NULL)
    {
        xmlFilePath = getFilePath(xmlFolder, ent->d_name);
        std::cout << "test" <<std::endl;
        if (RE2::FullMatch(std::string(ent->d_name), invalidFileRegexp))
        {
            xsdFilePath = getXsdFileNameFromXmlError(ent->d_name);
            valid = false;
        }
        else
        {
            xsdFilePath = getFilePath(xsdFolder, ent->d_name);
            valid = true;
        }

        std::cout << "Validation of " << xmlFilePath << " from " << xsdFilePath << std::endl;

        Xml::Document * xmlDoc = Xml::load(xmlFilePath, &xmlLog);
        test_assert(xmlDoc != NULL);
        Xml::Document * xsdDoc = Xml::load(xsdFilePath, &xmlLog);
        test_assert(xsdDoc != NULL);

        // Building XSD Checker
        Xsd::Checker::initialize(xsdDoc);

        Xsd::Checker * checker = Xsd::Checker::getInstance();

        // Validation process
        if(valid)
        {
            test_assert(checker->isValid(xmlDoc));
        }
        else
        {
            test_assert(!checker->isValid(xmlDoc));
        }

        delete xmlDoc;
        delete checker;
    }
    closedir (dir);
}

int
main()
{
    std::string fnames[] = {"foo.txt", "bar.txt", "baz.dat", "zoidberg"};
    std::string txt_regex("[a-z]+\\.txt");

    for (const auto &fname : fnames) {
        std::cout << fname << ": " << RE2::FullMatch(fname, txt_regex) << '\n';
    }

    test_construction();
    test_validation();

    return 0;
}
