/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_xml_test_main.cpp
* @version     
* @brief      
* @author	duye
* @date		2014-8-10
* @note 
*
*  1. 2014-8-10 duye Created this file
* 
*/
#include <string>
#include <iostream>
#include <g_system.h>
#include <g_xml.h>

using namespace std;
using namespace gutils;

class XmlTester
{
public:
    XmlTester() {}
    ~XmlTester() {}

    GResult printSchoolXml();
    GResult readSchoolXml();
    GResult writeSchoolXml();
};

GResult XmlTester::printSchoolXml() 
{
    cout << "TEST 1. print file school.xml" << endl;

	XmlDocument doc;  
	const char* xml_file = "school.xml";	
	if (doc.loadFile(xml_file)) 
    {   
		doc.print();  
	} 
    else 
    {
		cout << "can not parse xml school.xml" << endl;
        return G_NO;
	}
    
    return G_YES;
}

GResult XmlTester::readSchoolXml() 
{
	cout << "TEST 2. read file school.xml" << endl;
    
	const char * xmlFile = "school.xml";	
	XmlDocument doc;  
	if (doc.loadFile(xmlFile)) 
    {   
		doc.print();  
	} 
    else 
    {
		cout << "can not parse xml conf/school.xml" << endl;
		return G_NO;
	}
    
	XmlElement* rootElement = doc.rootElement();  //School元素  
	XmlElement* classElement = rootElement->firstChildElement();  // Class元素
	XmlElement* studentElement = classElement->firstChildElement();  //Students  
	for (; studentElement != NULL; studentElement = studentElement->nextSiblingElement()) 
    {  
		XmlAttribute* attributeOfStudent = studentElement->firstAttribute();  //获得student的name属性  
		for (; attributeOfStudent != NULL; attributeOfStudent = attributeOfStudent->next()) 
        {  
			cout << attributeOfStudent->name() << " : " << attributeOfStudent->value() << std::endl;  
		}

		XmlElement* studentContactElement = studentElement->firstChildElement();//获得student的第一个联系方式 
		for (; studentContactElement != NULL; studentContactElement = studentContactElement->nextSiblingElement()) 
        {
			string contactType = studentContactElement->value();
			string contactValue = studentContactElement->getText();
			cout << contactType  << " : " << contactValue << std::endl;  
		}
	}  
    
    return G_YES;
}

GResult XmlTester::writeSchoolXml() 
{
	cout << "TEST 3. write file school.xml" << endl;
    
	const char * xmlFile = "school-write.xml";	
	XmlDocument doc;  
	XmlDeclaration * decl = new XmlDeclaration("1.0", "", "");  
	XmlElement * schoolElement = new XmlElement( "School" );  
	XmlElement * classElement = new XmlElement( "Class" );  
	classElement->setAttribute("name", "C++");

	XmlElement * stu1Element = new XmlElement("Student");
	stu1Element->setAttribute("name", "tinyxml");
	stu1Element->setAttribute("number", "123");
	XmlElement * stu1EmailElement = new XmlElement("email");
	stu1EmailElement->linkEndChild(new XmlText("tinyxml@163.com") );
	XmlElement * stu1AddressElement = new XmlElement("address");
	stu1AddressElement->linkEndChild(new XmlText("dazhou"));
	stu1Element->linkEndChild(stu1EmailElement);
	stu1Element->linkEndChild(stu1AddressElement);

	XmlElement * stu2Element = new XmlElement("Student");
	stu2Element->setAttribute("name", "jsoncpp");
	stu2Element->setAttribute("number", "456");
	XmlElement * stu2EmailElement = new XmlElement("email");
	stu2EmailElement->linkEndChild(new XmlText("jsoncpp@163.com"));
	XmlElement * stu2AddressElement = new XmlElement("address");
	stu2AddressElement->linkEndChild(new XmlText("chengdu"));
	stu2Element->linkEndChild(stu2EmailElement);
	stu2Element->linkEndChild(stu2AddressElement);

	classElement->linkEndChild(stu1Element);  
	classElement->linkEndChild(stu2Element);  
	schoolElement->linkEndChild(classElement);  
	
	doc.linkEndChild(decl);  
	doc.linkEndChild(schoolElement); 
	doc.saveFile(xmlFile);  
    
    return G_YES;
}

void TestShow(const GInt8* function, const GResult result)
{
    printf("%s   %s \n", function, result == G_YES ? "YES" : "NO");
}

int main(int argc, char** argv) 
{
	XmlTester tester;
    
	TestShow("XmlTester::printSchoolXml()", tester.printSchoolXml());
	TestShow("XmlTester::readSchoolXml()", tester.readSchoolXml());
	TestShow("XmlTester::writeSchoolXml()", tester.writeSchoolXml());
    
	return 0;
}
