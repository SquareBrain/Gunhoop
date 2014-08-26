/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		gxml_main.h
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
#include <g_xml.h>

using namespace std;

class GXmlTest
{
public:
    GXmlTest() {}
    ~GXmlTest() {}

    void printSchoolXml();
    void readSchoolXml();
    void writeSchoolXml();
};

void GXmlTest::printSchoolXml() 
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
	}
}

void GXmlTest::readSchoolXml() 
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
		return;
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
}

void GXmlTest::writeSchoolXml() 
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
}

int main(int argc, char** argv) 
{
	GXmlTest test;
	test.printSchoolXml();
	test.readSchoolXml();
	test.writeSchoolXml();
    
	return 0;
}
