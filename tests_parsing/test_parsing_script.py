
import os
import sys
import subprocess

def testXmlParsing(path, success):
    test_dir = os.path.dirname(__file__) + '/'
    xmlparser_path = os.path.abspath(os.path.join(test_dir, '../build-debug/products/xmltool'))

    xml_path = test_dir + path

    assert os.path.isfile(xml_path)
    assert os.path.isfile(xmlparser_path)

    s = subprocess.Popen([xmlparser_path, xml_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    s.wait()

    assert s.returncode != 1

    successed = ((s.returncode == 0) == success)

    if success:
        return

    print 'test {} failed !'.format(path)
    print '# ----------------------------------------------------------- stdout'
    print s.stdout.read()

    print '# ----------------------------------------------------------- stderr'
    print s.stderr.read()

    sys.exit(1)


if __name__ == "__main__":
    testXmlParsing('sample_01.xml', True)
    sys.exit(0)
