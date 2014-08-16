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

	GXmlDocument doc;  
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
	GXmlDocument doc;  
	if (doc.loadFile(xmlFile)) 
    {   
		doc.print();  
	} 
    else 
    {
		cout << "can not parse xml conf/school.xml" << endl;
		return;
	}
    
	GXmlElement* rootElement = doc.rootElement();  //School元素  
	GXmlElement* classElement = rootElement->firstChildElement();  // Class元素
	GXmlElement* studentElement = classElement->firstChildElement();  //Students  
	for (; studentElement != NULL; studentElement = studentElement->nextSiblingElement()) 
    {  
		GXmlAttribute* attributeOfStudent = studentElement->firstAttribute();  //获得student的name属性  
		for (; attributeOfStudent != NULL; attributeOfStudent = attributeOfStudent->next()) 
        {  
			cout << attributeOfStudent->name() << " : " << attributeOfStudent->value() << std::endl;  
		}

		GXmlElement* studentContactElement = studentElement->firstChildElement();//获得student的第一个联系方式 
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
	GXmlDocument doc;  
	GXmlDeclaration * decl = new GXmlDeclaration("1.0", "", "");  
	GXmlElement * schoolElement = new GXmlElement( "School" );  
	GXmlElement * classElement = new GXmlElement( "Class" );  
	classElement->setAttribute("name", "C++");

	GXmlElement * stu1Element = new GXmlElement("Student");
	stu1Element->setAttribute("name", "tinyxml");
	stu1Element->setAttribute("number", "123");
	GXmlElement * stu1EmailElement = new GXmlElement("email");
	stu1EmailElement->linkEndChild(new GXmlText("tinyxml@163.com") );
	GXmlElement * stu1AddressElement = new GXmlElement("address");
	stu1AddressElement->linkEndChild(new GXmlText("dazhou"));
	stu1Element->linkEndChild(stu1EmailElement);
	stu1Element->linkEndChild(stu1AddressElement);

	GXmlElement * stu2Element = new GXmlElement("Student");
	stu2Element->setAttribute("name", "jsoncpp");
	stu2Element->setAttribute("number", "456");
	GXmlElement * stu2EmailElement = new GXmlElement("email");
	stu2EmailElement->linkEndChild(new GXmlText("jsoncpp@163.com"));
	GXmlElement * stu2AddressElement = new GXmlElement("address");
	stu2AddressElement->linkEndChild(new GXmlText("chengdu"));
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
