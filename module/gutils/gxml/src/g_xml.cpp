/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_xml.cpp
* @version     
* @brief      
* @author	duye
* @date		2014-8-7
* @note 
*
*  1. 2014-8-7 duye Created this file
* 
*/
#include <ctype.h>
#include <sstream>
#include <iostream>
#include <g_xml.h>

// bool GXmlBase::condenseWhiteSpace = true;

// Microsoft compiler security
FILE* GXmlFOpen(const char* filename, const char* mode)
{
	return fopen(filename, mode);
}

void GXmlBase::encodeString(const std::string& str, std::string* outString)
{
	int i = 0;
	while (i < (int)str.length())
	{
		unsigned char c = (unsigned char)str[i];
		if (c == '&' 
			&& i < ((int)str.length() - 2)
			&& str[i+1] == '#'
			&& str[i+2] == 'x')
		{
			// Hexadecimal character reference.
			// Pass through unchanged.
			// &#xA9;	-- copyright symbol, for example.
			//
			// The -1 is a bug fix from Rob Laveaux. It keeps
			// an overflow from happening if there is no ';'.
			// There are actually 2 ways to exit this loop -
			// while fails (error case) and break (semicolon found).
			// However, there is no mechanism (currently) for
			// this function to return an error.
			while (i < (int)str.length() - 1)
			{
				outString->append(str.c_str() + i, 1);
				++i;
				if (str[i] == ';')
                {
					break;
                }
			}
		}
		else if (c == '&')
		{
			outString->append(m_entity[0].m_str, m_entity[0].m_strLength);
			++i;
		}
		else if (c == '<')
		{
			outString->append(m_entity[1].m_str, m_entity[1].m_strLength);
			++i;
		}
		else if (c == '>')
		{
			outString->append(m_entity[2].m_str, m_entity[2].m_strLength);
			++i;
		}
		else if (c == '\"')
		{
			outString->append(m_entity[3].m_str, m_entity[3].m_strLength);
			++i;
		}
		else if (c == '\'')
		{
			outString->append(m_entity[4].m_str, m_entity[4].m_strLength);
			++i;
		}
		else if (c < 32)
		{
			// Easy pass at non-alpha/numeric/symbol
			// Below 32 is symbolic.
			char buf[32] = {0};
			snprintf(buf, sizeof(buf), "&#x%02X;", (unsigned)(c & 0xff));	

			//*ME:	warning C4267: convert 'size_t' to 'int'
			//*ME:	Int-Cast to make compiler happy ...
			outString->append(buf, (int)strlen(buf));
			++i;
		}
		else
		{
			//char realc = (char) c;
			//outString->append( &realc, 1 );
			*outString += (char)c;	// somewhat more efficient function call.
			++i;
		}
	}
}

GXmlNode::GXmlNode(NodeType type) : GXmlBase()
{
	m_parent = 0;
	m_type = type;
	m_firstChild = 0;
	m_lastChild = 0;
	m_prev = 0;
	m_next = 0;
}

GXmlNode::~GXmlNode()
{
	GXmlNode* node = m_firstChild;
	GXmlNode* temp = nullptr;

	while (node != nullptr)
	{
		temp = node;
		node = node->m_next;
		delete temp;
	}	
}

void GXmlNode::copyTo(GXmlNode* target) const
{
	target->setValue(m_value.c_str());
	target->m_userData = m_userData; 
	target->m_location = m_location;
}

void GXmlNode::clear()
{
	GXmlNode* node = m_firstChild;
	GXmlNode* temp = nullptr;

	while (node != nullptr)
	{
		temp = node;
		node = node->m_next;
		delete temp;
	}	

	m_firstChild = nullptr;
	m_lastChild = nullptr;
}

GXmlNode* GXmlNode::linkEndChild(GXmlNode* node)
{
	assert(node->m_parent == 0 || node->m_parent == this);
	assert(node->getDocument() == 0 || node->getDocument() == this->getDocument());

	if (node->type() == GXmlNode::GNYXML_DOCUMENT)
	{
		delete node;
		if (getDocument()) 
        {
			getDocument()->setError(GXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, GXML_ENCODING_UNKNOWN);
        }
        
		return 0;
	}

	node->m_parent = this;
	node->m_prev = m_lastChild;
	node->m_next = nullptr;
    
	if (m_lastChild != nullptr)
    {
		m_lastChild->m_next = node;
    }
	else
    {
		m_firstChild = node;			// it was an empty list.
	}

	m_lastChild = node;
    
	return node;
}

GXmlNode* GXmlNode::insertEndChild(const GXmlNode& addThis)
{
	if (addThis.type() == GXmlNode::GNYXML_DOCUMENT)
	{
		if (getDocument()) 
        {
			getDocument()->setError(GXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, GXML_ENCODING_UNKNOWN);
        }
        
		return 0;
	}
    
	GXmlNode* node = addThis.clone();
	if (node == nullptr)
    {
		return nullptr;
    }

	return linkEndChild(node);
}

GXmlNode* GXmlNode::insertBeforeChild(GXmlNode* beforeThis, const GXmlNode& addThis)
{	
	if (beforeThis == nullptr || beforeThis->m_parent != this) 
    {
		return 0;
	}
    
	if (addThis.type() == GXmlNode::GNYXML_DOCUMENT)
	{
		if (getDocument() != nullptr) 
        {
			getDocument()->setError(GXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, GXML_ENCODING_UNKNOWN);
        }
        
		return 0;
	}

	GXmlNode* node = addThis.clone();
	if (node == nullptr)
    {
		return 0;
    }
    
	node->m_parent = this;
	node->m_next = beforeThis;
	node->m_prev = beforeThis->m_prev;
	if (beforeThis->m_prev)
	{
		beforeThis->m_prev->m_next = node;
	}
	else
	{
		assert(m_firstChild == beforeThis);
		m_firstChild = node;
	}
    
	beforeThis->m_prev = node;
    
	return node;
}

GXmlNode* GXmlNode::insertAfterChild(GXmlNode* afterThis, const GXmlNode& addThis)
{
	if (afterThis == nullptr || afterThis->m_parent != this) 
    {
		return 0;
	}
    
	if (addThis.type() == GXmlNode::GNYXML_DOCUMENT)
	{
		if (getDocument() != nullptr) 
        {
			getDocument()->setError(GXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, GXML_ENCODING_UNKNOWN);
        }
        
		return 0;
	}

	GXmlNode* node = addThis.clone();
	if (node == nullptr)
    {
		return 0;
    }
    
	node->m_parent = this;
	node->m_prev = afterThis;
	node->m_next = afterThis->m_next;
	if (afterThis->m_next != nullptr)
	{
		afterThis->m_next->m_prev = node;
	}
	else
	{
		assert(m_lastChild == afterThis);
		m_lastChild = node;
	}
    
	afterThis->m_next = node;
    
	return node;
}

GXmlNode* GXmlNode::replaceChild(GXmlNode* replaceThis, const GXmlNode& withThis)
{
	if (replaceThis == nullptr)
    {
		return 0;
    }

	if (replaceThis->m_parent != this)
    {
		return 0;
    }

	if (withThis.toDocument()) 
    {
		// A document can never be a child.	Thanks to Noam.
		if (getDocument() != nullptr) 
        {
			getDocument()->setError(GXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, GXML_ENCODING_UNKNOWN);
        }
        
		return 0;
	}

	GXmlNode* node = withThis.clone();
	if (node == nullptr)
    {
		return 0;
    }

	node->m_next = replaceThis->m_next;
	node->m_prev = replaceThis->m_prev;

	if (replaceThis->m_next)
    {
		replaceThis->m_next->m_prev = node;
    }
	else
    {
		m_lastChild = node;
    }

	if (replaceThis->m_prev)
    {
		replaceThis->m_prev->m_next = node;
	}
    else
    {
		m_firstChild = node;
    }

	delete replaceThis;
	node->m_parent = this;
    
	return node;
}

bool GXmlNode::removeChild(GXmlNode* removeThis)
{
	if (removeThis == nullptr) 
    {
		return false;
	}

	if (removeThis->m_parent != this)
	{	
		return false;
	}

	if (removeThis->m_next)
    {
		removeThis->m_next->m_prev = removeThis->m_prev;
    }
	else
    {
		m_lastChild = removeThis->m_prev;
    }

	if (removeThis->m_prev)
    {
		removeThis->m_prev->m_next = removeThis->m_next;
    }
	else
    {
		m_firstChild = removeThis->m_next;
    }

	delete removeThis;
    
	return true;
}

const GXmlNode* GXmlNode::firstChild(const char* value) const
{
	for (const GXmlNode* node = m_firstChild; node != nullptr; node = node->m_next)
	{
		if (strcmp(node->value(), value) == 0)
        {
			return node;
        }
	}
    
	return 0;
}

const GXmlNode* GXmlNode::lastChild(const char* value) const
{
	for (const GXmlNode* node = m_lastChild; node != nullptr; node = node->m_prev)
	{
		if (strcmp(node->value(), value) == 0)
        {
			return node;
        }
	}
    
	return 0;
}

const GXmlNode* GXmlNode::iterateChildren(const GXmlNode* previous) const
{
	if (previous == nullptr)
	{
		return firstChild();
	}
	else
	{
		assert(previous->m_parent == this);
		return previous->nextSibling();
	}
}

const GXmlNode* GXmlNode::iterateChildren(const char* val, const GXmlNode* previous) const
{
	if (previous == nullptr)
	{
		return firstChild(val);
	}
	else
	{
		assert(previous->m_parent == this);
		return previous->nextSibling(val);
	}
}

const GXmlNode* GXmlNode::nextSibling(const char* value) const 
{
	for (const GXmlNode* node = m_next; node != nullptr; node = node->m_next)
	{
		if (strcmp(node->value(), value) == 0)
        {
			return node;
        }
	}
    
	return 0;
}

const GXmlNode* GXmlNode::previousSibling(const char* value) const
{
	for (const GXmlNode* node = m_prev; node != nullptr; node = node->m_prev)
	{
		if (strcmp(node->value(), value) == 0)
        {
			return node;
        }
	}
    
	return 0;
}

void GXmlElement::removeAttribute(const char* name)
{
	std::string str(name);
	GXmlAttribute* node = m_attributeSet.find(str);
	if (node != nullptr)
	{
		m_attributeSet.remove(node);
		delete node;
	}
}

const GXmlElement* GXmlNode::firstChildElement() const
{
	for (const GXmlNode* node = firstChild(); node != nullptr; node = node->nextSibling())
	{
		if (node->toElement())
        {
			return node->toElement();
        }
	}
    
	return 0;
}

const GXmlElement* GXmlNode::firstChildElement(const char* value) const
{
	for (const GXmlNode* node = firstChild(value); node != nullptr; node = node->nextSibling(value))
	{
		if (node->toElement())
        {
			return node->toElement();
        }
	}
    
	return 0;
}

const GXmlElement* GXmlNode::nextSiblingElement() const
{
	for (const GXmlNode* node = nextSibling(); node != nullptr; node = node->nextSibling())
	{
		if (node->toElement())
        {
			return node->toElement();
        }
	}
    
	return 0;
}

const GXmlElement* GXmlNode::nextSiblingElement(const char* value) const
{
	for (const GXmlNode* node = nextSibling(value); node != nullptr; node = node->nextSibling(value))
	{
		if (node->toElement())
        {
			return node->toElement();
        }
	}
    
	return 0;
}

const GXmlDocument* GXmlNode::getDocument() const
{
	for (const GXmlNode* node = this; node != nullptr; node = node->m_parent)
	{
		if (node->toDocument())
        {
			return node->toDocument();
        }
	}
    
	return 0;
}

GXmlElement::GXmlElement(const char* value) : GXmlNode(GXmlNode::GNYXML_ELEMENT)
{
	m_firstChild = nullptr;
	m_lastChild = nullptr;
	m_value = value;
}

GXmlElement::GXmlElement(const std::string& value) : GXmlNode(GXmlNode::GNYXML_ELEMENT)
{
	m_firstChild = nullptr;
	m_lastChild = nullptr;
	m_value = value;
}

GXmlElement::GXmlElement(const GXmlElement& copy) : GXmlNode(GXmlNode::GNYXML_ELEMENT)
{
	m_firstChild = nullptr;
	m_lastChild = nullptr;
	copy.copyTo(this);	
}

GXmlElement& GXmlElement::operator = (const GXmlElement& base)
{
	clearThis();
	base.copyTo(this);
	return *this;
}

GXmlElement::~GXmlElement()
{
	clearThis();
}

void GXmlElement::clearThis()
{
	clear();
	while (m_attributeSet.first())
	{
		GXmlAttribute* node = m_attributeSet.first();
		m_attributeSet.remove(node);
		delete node;
	}
}

const char* GXmlElement::attribute(const char* name) const
{
	const GXmlAttribute* node = m_attributeSet.find(name);
	if (node != nullptr)
    {
		return node->value();
    }
    
	return 0;
}

const std::string* GXmlElement::attribute(const std::string& name) const
{
	const GXmlAttribute* attrib = m_attributeSet.find(name);
	if (attrib != nullptr)
    {
		return &attrib->valueStr();
    }

    return 0;
}

const char* GXmlElement::attribute(const char* name, int* i) const
{
	const GXmlAttribute* attrib = m_attributeSet.find(name);
	const char* result = nullptr;
	if (attrib != nullptr) 
    {
		result = attrib->value();
		if (i != nullptr) 
        {
			attrib->queryIntValue(i);
		}
	}
    
	return result;
}

const std::string* GXmlElement::attribute(const std::string& name, int* i) const
{
	const GXmlAttribute* attrib = m_attributeSet.find(name);
	const std::string* result = nullptr;
	if (attrib != nullptr)
    {
		result = &attrib->valueStr();
		if (i != nullptr)
        {
			attrib->queryIntValue(i);
		}
	}
    
	return result;
}

const char* GXmlElement::attribute(const char* name, double* d) const
{
	const GXmlAttribute* attrib = m_attributeSet.find(name);
	const char* result = nullptr;
	if (attrib != nullptr)
    {
		result = attrib->value();
		if (d != nullptr) 
        {
			attrib->queryDoubleValue(d);
		}
	}
    
	return result;
}

const std::string* GXmlElement::attribute(const std::string& name, double* d) const
{
	const GXmlAttribute* attrib = m_attributeSet.find( name );
	const std::string* result = nullptr;
	if (attrib != nullptr)
    {
		result = &attrib->valueStr();
		if (d != nullptr) 
        {
			attrib->queryDoubleValue(d);
		}
	}
    
	return result;
}

int GXmlElement::queryIntAttribute(const char* name, int* ival) const
{
	const GXmlAttribute* attrib = m_attributeSet.find(name);
	if (attrib == nullptr)
    {
		return GXML_NO_ATTRIBUTE;
    }
    
	return attrib->queryIntValue(ival);
}

int GXmlElement::queryUnsignedAttribute(const char* name, unsigned* value) const
{
	const GXmlAttribute* node = m_attributeSet.find(name);
	if (node == nullptr)
    {
		return GXML_NO_ATTRIBUTE;
    }

	int ival = 0;
	int result = node->queryIntValue(&ival);
	*value = (unsigned)ival;
    
	return result;
}

int GXmlElement::queryBoolAttribute(const char* name, bool* bval) const
{
	const GXmlAttribute* node = m_attributeSet.find(name);
	if (node == nullptr)
    {
		return GXML_NO_ATTRIBUTE;
    }
	
	int result = GXML_WRONG_TYPE;
	if (stringEqual(node->value(), "true", true, GXML_ENCODING_UNKNOWN) 
		|| stringEqual(node->value(), "yes", true, GXML_ENCODING_UNKNOWN) 
		|| stringEqual(node->value(), "1", true, GXML_ENCODING_UNKNOWN)) 
	{
		*bval = true;
		result = GXML_SUCCESS;
	}
	else if (stringEqual(node->value(), "false", true, GXML_ENCODING_UNKNOWN) 
			 || stringEqual(node->value(), "no", true, GXML_ENCODING_UNKNOWN) 
			 || stringEqual(node->value(), "0", true, GXML_ENCODING_UNKNOWN)) 
	{
		*bval = false;
		result = GXML_SUCCESS;
	}
    
	return result;
}

int GXmlElement::queryIntAttribute(const std::string& name, int* ival) const
{
	const GXmlAttribute* attrib = m_attributeSet.find(name);
	if (attrib == nullptr)
    {
		return GXML_NO_ATTRIBUTE;
    }
    
	return attrib->queryIntValue(ival);
}

int GXmlElement::queryDoubleAttribute(const char* name, double* dval) const
{
	const GXmlAttribute* attrib = m_attributeSet.find(name);
	if (attrib == nullptr)
    {
		return GXML_NO_ATTRIBUTE;
    }
    
	return attrib->queryDoubleValue(dval);
}

int GXmlElement::queryDoubleAttribute(const std::string& name, double* dval) const
{
	const GXmlAttribute* attrib = m_attributeSet.find(name);
	if (attrib == nullptr)
    {
		return GXML_NO_ATTRIBUTE;
    }
    
	return attrib->queryDoubleValue(dval);
}

void GXmlElement::setAttribute(const char* name, int val)
{	
	GXmlAttribute* attrib = m_attributeSet.findOrCreate(name);
	if (attrib != nullptr) 
    {
		attrib->setIntValue(val);
	}
}

void GXmlElement::setAttribute(const std::string& name, int val)
{	
	GXmlAttribute* attrib = m_attributeSet.findOrCreate(name);
	if (attrib != nullptr) 
    {
		attrib->setIntValue(val);
	}
}

void GXmlElement::setDoubleAttribute(const char* name, double val)
{	
	GXmlAttribute* attrib = m_attributeSet.findOrCreate(name);
	if (attrib != nullptr) 
    {
		attrib->setDoubleValue(val);
	}
}

void GXmlElement::setDoubleAttribute(const std::string& name, double val)
{	
	GXmlAttribute* attrib = m_attributeSet.findOrCreate(name);
	if (attrib != nullptr) 
    {
		attrib->setDoubleValue(val);
	}
}

void GXmlElement::setAttribute(const char* cname, const char* cvalue)
{
	GXmlAttribute* attrib = m_attributeSet.findOrCreate(cname);
	if (attrib != nullptr) 
    {
		attrib->setValue(cvalue);
	}
}

void GXmlElement::setAttribute(const std::string& name, const std::string& value)
{
	GXmlAttribute* attrib = m_attributeSet.findOrCreate(name);
	if (attrib != nullptr) 
    {
		attrib->setValue(value);
	}
}

void GXmlElement::print(FILE* cfile, int depth) const
{
	assert(cfile);
	for (int i = 0; i < depth; i++) 
    {
		fprintf(cfile, " ");
	}

	fprintf(cfile, "<%s", m_value.c_str());
	for (const GXmlAttribute* attrib = m_attributeSet.first(); attrib != nullptr; attrib = attrib->next())
	{
		fprintf(cfile, " ");
		attrib->print(cfile, depth);
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	if (m_firstChild == nullptr)
	{
		fprintf(cfile, " />");
	}
	else if (m_firstChild == m_lastChild && m_firstChild->toText() != nullptr)
	{
		fprintf(cfile, ">");
		m_firstChild->print(cfile, depth + 1);
		fprintf(cfile, "</%s>", m_value.c_str());
	}
	else
	{
		fprintf(cfile, ">");
		for (GXmlNode* node = m_firstChild; node != nullptr; node = node->nextSibling())
		{
			if (node->toText() == nullptr)
			{
				fprintf(cfile, "\n");
			}
            
			node->print(cfile, depth + 1);
		}
        
		fprintf(cfile, "\n");
		for (int i = 0; i < depth; i++) 
        {
			fprintf(cfile, " ");
		}
        
		fprintf(cfile, "</%s>", m_value.c_str());
	}
}

void GXmlElement::copyTo(GXmlElement* target) const
{
	// superclass:
	GXmlNode::copyTo(target);

	// Element class: 
	// Clone the attributes, then clone the children.
	for (const GXmlAttribute* attribute = m_attributeSet.first(); attribute != nullptr; attribute = attribute->next())
	{
		target->setAttribute(attribute->name(), attribute->value());
	}

	for (GXmlNode* node = m_firstChild; node != nullptr; node = node->nextSibling())
	{
		target->linkEndChild(node->clone());
	}
}

bool GXmlElement::accept(GXmlVisitor* visitor) const
{
	if (visitor->visitEnter(*this, m_attributeSet.first())) 
	{
		for (const GXmlNode* node = firstChild(); node != nullptr; node = node->nextSibling())
		{
			if (!node->accept(visitor))
            {
				break;
            }
		}
	}
    
	return visitor->visitExit(*this);
}

GXmlNode* GXmlElement::clone() const
{
	GXmlElement* clone = new GXmlElement(value());
	if (clone == nullptr)
    {
		return 0;
    }

	copyTo(clone);
    
	return clone;
}

const char* GXmlElement::getText() const
{
	const GXmlNode* child = this->firstChild();
	if (child != nullptr) 
    {
		const GXmlText* childText = child->toText();
		if (childText != nullptr) 
        {
			return childText->value();
		}
	}
    
	return 0;
}

GXmlDocument::GXmlDocument() : GXmlNode(GXmlNode::GNYXML_DOCUMENT)
{
	m_tabsize = 4;
	m_useMicrosoftBOM = false;
	clearError();
}

GXmlDocument::GXmlDocument(const char* documentName) : GXmlNode(GXmlNode::GNYXML_DOCUMENT)
{
	m_tabsize = 4;
	m_useMicrosoftBOM = false;
	m_value = documentName;
	clearError();
}

GXmlDocument::GXmlDocument(const std::string& documentName) : GXmlNode(GXmlNode::GNYXML_DOCUMENT)
{
	m_tabsize = 4;
	m_useMicrosoftBOM = false;
    m_value = documentName;
	clearError();
}

GXmlDocument::GXmlDocument(const GXmlDocument& copy) : GXmlNode(GXmlNode::GNYXML_DOCUMENT)
{
	copy.copyTo(this);
}

GXmlDocument& GXmlDocument::operator = (const GXmlDocument& copy)
{
	clear();
	copy.copyTo(this);
	return *this;
}

bool GXmlDocument::loadFile(GXmlEncoding encoding)
{
	return loadFile(value(), encoding);
}

bool GXmlDocument::saveFile() const
{
	return saveFile(value());
}

bool GXmlDocument::loadFile(const char* filename, GXmlEncoding encoding)
{
	m_value = filename;
	
	// reading in binary mode so that tinyxml can normalize the EOL
	FILE* file = GXmlFOpen(m_value.c_str(), "rb");	
	if (file == nullptr)
	{
		setError(GXML_ERROR_OPENING_FILE, 0, 0, GXML_ENCODING_UNKNOWN);
		return false;
	}
	
	bool result = loadFile(file, encoding);
	fclose(file);
	
	return result;
}

bool GXmlDocument::loadFile(FILE* file, GXmlEncoding encoding)
{
	if (file == nullptr) 
	{
		setError(GXML_ERROR_OPENING_FILE, 0, 0, GXML_ENCODING_UNKNOWN);
		return false;
	}

	// Delete the existing data:
	clear();
	m_location.clear();

	// Get the file size, so we can pre-allocate the string. HUGE speed impact.
	long length = 0;
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);

	// Strange case, but good to handle up front.
	if (length <= 0)
	{
		setError(GXML_ERROR_DOCUMENT_EMPTY, 0, 0, GXML_ENCODING_UNKNOWN);
		return false;
	}

	// Subtle bug here. GnyXml did use fgets. But from the XML spec:
	// 2.11 End-of-Line Handling
	// <snip>
	// <quote>
	// ...the XML processor MUST behave as if it normalized all line breaks in external 
	// parsed entities (including the document entity) on input, before parsing, by translating 
	// both the two-character sequence #xD #xA and any #xD that is not followed by #xA to 
	// a single #xA character.
	// </quote>
	//
	// It is not clear fgets does that, and certainly isn't clear it works cross platform. 
	// Generally, you expect fgets to translate from the convention of the OS to the c/unix
	// convention, and not work generally.

	/*
	while( fgets( buf, sizeof(buf), file ) )
	{
		data += buf;
	}
	*/

	char* buf = new char[length + 1];
	buf[0] = 0;
	if (fread(buf, length, 1, file) != 1) 
    {
		delete [] buf;
		setError(GXML_ERROR_OPENING_FILE, 0, 0, GXML_ENCODING_UNKNOWN);
		return false;
	}

	// Process the buffer in place to normalize new lines. (See comment above.)
	// Copies from the 'p' to 'q' pointer, where p can advance faster if
	// a newline-carriage return is hit.
	//
	// Wikipedia:
	// Systems based on ASCII or a compatible character set use either LF  (Line feed, '\n', 0x0A, 10 in decimal) or 
	// CR (Carriage return, '\r', 0x0D, 13 in decimal) individually, or CR followed by LF (CR+LF, 0x0D 0x0A)...
	//		* LF:    Multics, Unix and Unix-like systems (GNU/Linux, AIX, Xenix, Mac OS X, FreeBSD, etc.), BeOS, Amiga, RISC OS, and others
    //		* CR+LF: DEC RT-11 and most other early non-Unix, non-IBM OSes, CP/M, MP/M, DOS, OS/2, Microsoft Windows, Symbian OS
    //		* CR:    Commodore 8-bit machines, Apple II family, Mac OS up to version 9 and OS-9

	const char* p = buf;	// the read head
	char* q = buf;			// the write head
	const char CR = 0x0d;
	const char LF = 0x0a;

	buf[length] = 0;
	while (*p) 
    {
		assert(p < (buf + length));
		assert(q <= (buf + length));
		assert(q <= p);

		if (*p == CR) 
        {
			*q++ = LF;
			p++;
            
			if (*p == LF) 
            {       // check for CR+LF (and skip LF)
				p++;
			}
		}
		else 
        {
			*q++ = *p++;
		}
	}
    
	assert(q <= (buf + length));
	*q = 0;

	parse(buf, 0, encoding);

	delete [] buf;
	return !error();
}

bool GXmlDocument::saveFile(const char* filename) const
{
	// The old c stuff lives on...
	FILE* fp = GXmlFOpen(filename, "w");
	if (fp != nullptr)
	{
		bool result = saveFile(fp);
		fclose(fp);
		return result;
	}
    
	return false;
}

bool GXmlDocument::saveFile(FILE* fp) const
{
	if (m_useMicrosoftBOM) 
	{
		const unsigned char GXML_UTF_LEAD_0 = 0xefU;
		const unsigned char GXML_UTF_LEAD_1 = 0xbbU;
		const unsigned char GXML_UTF_LEAD_2 = 0xbfU;

		fputc(GXML_UTF_LEAD_0, fp);
		fputc(GXML_UTF_LEAD_1, fp);
		fputc(GXML_UTF_LEAD_2, fp);
	}
    
	print(fp, 0);
    
	return (ferror(fp) == 0);
}

void GXmlDocument::copyTo(GXmlDocument* target) const
{
	GXmlNode::copyTo(target);

	target->m_error = m_error;
	target->m_errorId = m_errorId;
	target->m_errorDesc = m_errorDesc;
	target->m_tabsize = m_tabsize;
	target->m_errorLocation = m_errorLocation;
	target->m_useMicrosoftBOM = m_useMicrosoftBOM;
	for (GXmlNode* node = m_firstChild; node != nullptr; node = node->nextSibling())
	{
		target->linkEndChild(node->clone());
	}	
}

GXmlNode* GXmlDocument::clone() const
{
	GXmlDocument* clone = new GXmlDocument();
	if (clone == nullptr)
    {
		return 0;
    }

	copyTo(clone);
	return clone;
}

void GXmlDocument::print(FILE* cfile, int depth) const
{
	assert(cfile);
	for (const GXmlNode* node = firstChild(); node != nullptr; node = node->nextSibling())
	{
		node->print(cfile, depth);
		fprintf(cfile, "\n");
	}
}

bool GXmlDocument::accept(GXmlVisitor* visitor) const
{
	if (visitor->visitEnter(*this))
	{
		for (const GXmlNode* node = firstChild(); node != nullptr; node = node->nextSibling())
		{
			if (!node->accept(visitor))
            {
				break;
            }
		}
	}
    
	return visitor->visitExit(*this);
}

const GXmlAttribute* GXmlAttribute::next() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if (m_next->m_value.empty() && m_next->m_name.empty())
	{
		return 0;
	}
    
	return m_next;
}

const GXmlAttribute* GXmlAttribute::previous() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if (m_prev->m_value.empty() && m_prev->m_name.empty())
    {
		return 0;
    }
    
	return m_prev;
}

void GXmlAttribute::print(FILE* cfile, int /*depth*/, std::string* str) const
{
	std::string n;
    std::string v;

	encodeString(m_name, &n);
	encodeString(m_value, &v);

	if (m_value.find('\"') == std::string::npos) 
    {
		if (cfile != nullptr) 
        {
			fprintf(cfile, "%s=\"%s\"", n.c_str(), v.c_str());
		}
        
		if (str != nullptr)
        {
			(*str) += n; 
            (*str) += "=\""; 
            (*str) += v; 
            (*str) += "\"";
		}
	}
	else 
    {
		if (cfile != nullptr) 
        {
			fprintf(cfile, "%s='%s'", n.c_str(), v.c_str());
		}
        
		if (str != nullptr) 
        {
			(*str) += n; 
			(*str) += "='"; 
			(*str) += v; 
			(*str) += "'";
		}
	}
}

int GXmlAttribute::queryIntValue(int* ival) const
{
	if (sscanf(m_value.c_str(), "%d", ival) == 1)
    {
		return GXML_SUCCESS;
    }
    
	return GXML_WRONG_TYPE;
}

int GXmlAttribute::queryDoubleValue(double* dval) const
{
	if (sscanf(m_value.c_str(), "%lf", dval) == 1)
    {
		return GXML_SUCCESS;
    }
    
	return GXML_WRONG_TYPE;
}

void GXmlAttribute::setIntValue(int value)
{
	char buf[64] = {0};
	snprintf(buf, sizeof(buf), "%d", value);
	setValue(buf);
}

void GXmlAttribute::setDoubleValue(double value)
{
	char buf[256] = {0};
	snprintf(buf, sizeof(buf), "%g", value);
	setValue(buf);
}

int GXmlAttribute::intValue() const
{
	return atoi(m_value.c_str());
}

double GXmlAttribute::doubleValue() const
{
	return atof(m_value.c_str());
}

GXmlComment::GXmlComment(const GXmlComment& copy) : GXmlNode(GXmlNode::GNYXML_COMMENT)
{
	copy.copyTo(this);
}

GXmlComment& GXmlComment::operator = (const GXmlComment& base)
{
	clear();
	base.copyTo(this);
	return *this;
}

void GXmlComment::print(FILE* cfile, int depth) const
{
	assert(cfile != nullptr);
	for (int i = 0; i < depth; i++)
	{
		fprintf(cfile, " ");
	}
    
	fprintf(cfile, "<!--%s-->", m_value.c_str());
}

void GXmlComment::copyTo(GXmlComment* target) const
{
	GXmlNode::copyTo(target);
}

bool GXmlComment::accept(GXmlVisitor* visitor) const
{
	return visitor->visit(*this);
}

GXmlNode* GXmlComment::clone() const
{
	GXmlComment* clone = new GXmlComment();

	if (clone == nullptr)
    {
		return 0;
    }

	copyTo(clone);
    
	return clone;
}

void GXmlText::print(FILE* cfile, int depth) const
{
	assert(cfile != nullptr);
    
	if (m_cdata)
	{
		fprintf(cfile, "\n");
		for (int i = 0; i < depth; i++) 
        {
			fprintf(cfile, " ");
		}
        
		fprintf(cfile, "<![CDATA[%s]]>\n", m_value.c_str());	// unformatted output
	}
	else
	{
		std::string buffer;
		encodeString(m_value, &buffer);
		fprintf(cfile, "%s", buffer.c_str());
	}
}

void GXmlText::copyTo(GXmlText* target) const
{
	GXmlNode::copyTo(target);
	target->m_cdata = m_cdata;
}

bool GXmlText::accept(GXmlVisitor* visitor) const
{
	return visitor->visit(*this);
}

GXmlNode* GXmlText::clone() const
{	
	GXmlText* clone = new GXmlText("");
	if (clone == nullptr)
    {
		return 0;
    }

	copyTo(clone);
	return clone;
}

GXmlDeclaration::GXmlDeclaration(const char* version,
	const char* encoding,
	const char* standalone)
	: GXmlNode(GXmlNode::GNYXML_DECLARAGON)
{
	m_version = version;
	m_encoding = encoding;
	m_standalone = standalone;
}

GXmlDeclaration::GXmlDeclaration(const std::string& version,
	const std::string& encoding,
	const std::string& standalone)
	: GXmlNode(GXmlNode::GNYXML_DECLARAGON)
{
	m_version = version;
	m_encoding = encoding;
	m_standalone = standalone;
}

GXmlDeclaration::GXmlDeclaration(const GXmlDeclaration& copy) : GXmlNode(GXmlNode::GNYXML_DECLARAGON)
{
	copy.copyTo(this);	
}

GXmlDeclaration& GXmlDeclaration::operator = (const GXmlDeclaration& copy)
{
	clear();
	copy.copyTo(this);
	return *this;
}

void GXmlDeclaration::print(FILE* cfile, int depth, std::string* str) const
{
	if (cfile != nullptr) 
    {
        fprintf(cfile, "<?xml ");
    }
    
	if (str != nullptr)	
    {
        (*str) += "<?xml ";
    }

	if (!m_version.empty()) 
    {
		if (cfile != nullptr) 
        {
            fprintf(cfile, "version=\"%s\" ", m_version.c_str());
        }
        
		if (str != nullptr) 
        { 
        	(*str) += "version=\""; 
            (*str) += m_version; 
            (*str) += "\" "; 
        }
	}
    
	if (!m_encoding.empty()) 
    {
		if (cfile != nullptr)
        {
            fprintf(cfile, "encoding=\"%s\" ", m_encoding.c_str());
        }
        
		if (str != nullptr) 
        { 
        	(*str) += "encoding=\""; 
            (*str) += m_encoding; 
            (*str) += "\" "; 
        }
	}
    
	if (!m_standalone.empty()) 
    {
		if (cfile != nullptr) 
        {
        	fprintf(cfile, "standalone=\"%s\" ", m_standalone.c_str());
        }
        
		if (str != nullptr) 
        { 
        	(*str) += "standalone=\""; 
            (*str) += m_standalone; 
            (*str) += "\" "; 
        }
	}
    
	if (cfile != nullptr) 
    {
        fprintf(cfile, "?>");
    }
    
	if (str != nullptr)	
    {
        (*str) += "?>";
    }
}

void GXmlDeclaration::copyTo(GXmlDeclaration* target) const
{
	GXmlNode::copyTo(target);
	target->m_version = m_version;
	target->m_encoding = m_encoding;
	target->m_standalone = m_standalone;
}

bool GXmlDeclaration::accept(GXmlVisitor* visitor) const
{
	return visitor->visit(*this);
}

GXmlNode* GXmlDeclaration::clone() const
{	
	GXmlDeclaration* clone = new GXmlDeclaration();
	if (clone == nullptr)
    {
		return 0;
    }

	copyTo(clone);
	return clone;
}

void GXmlUnknown::print(FILE* cfile, int depth) const
{
	for (int i = 0; i < depth; i++)
    {
		fprintf(cfile, " ");
    }
    
	fprintf(cfile, "<%s>", m_value.c_str());
}

void GXmlUnknown::copyTo(GXmlUnknown* target) const
{
	GXmlNode::copyTo(target);
}

bool GXmlUnknown::accept(GXmlVisitor* visitor) const
{
	return visitor->visit(*this);
}

GXmlNode* GXmlUnknown::clone() const
{
	GXmlUnknown* clone = new GXmlUnknown();
	if (clone == nullptr)
    {
		return 0;
    }

	copyTo(clone);
	return clone;
}

GXmlAttributeSet::GXmlAttributeSet()
{
	m_sentinel.m_next = &m_sentinel;
	m_sentinel.m_prev = &m_sentinel;
}

GXmlAttributeSet::~GXmlAttributeSet()
{
	assert(m_sentinel.m_next == &m_sentinel);
	assert(m_sentinel.m_prev == &m_sentinel);
}

void GXmlAttributeSet::add(GXmlAttribute* addMe)
{
	assert(!find(std::string(addMe->name())));	// Shouldn't be multiply adding to the set.
	addMe->m_next = &m_sentinel;
	addMe->m_prev = m_sentinel.m_prev;
	m_sentinel.m_prev->m_next = addMe;
	m_sentinel.m_prev = addMe;
}

void GXmlAttributeSet::remove(GXmlAttribute* removeMe)
{
	for (GXmlAttribute* node = m_sentinel.m_next; node != &m_sentinel; node = node->m_next)
	{
		if (node == removeMe)
		{
			node->m_prev->m_next = node->m_next;
			node->m_next->m_prev = node->m_prev;
			node->m_next = nullptr;
			node->m_prev = nullptr;
			return;
		}
	}
    
	assert(0);		// we tried to remove a non-linked attribute.
}

GXmlAttribute* GXmlAttributeSet::find(const std::string& name) const
{
	for (GXmlAttribute* node = m_sentinel.m_next; node != &m_sentinel; node = node->m_next)
	{
		if (node->m_name == name)
        {
			return node;
        }
	}
    
	return 0;
}

GXmlAttribute* GXmlAttributeSet::findOrCreate(const std::string& name)
{
	GXmlAttribute* attrib = find(name);
	if (attrib == nullptr) 
    {
		attrib = new GXmlAttribute();
		add(attrib);
		attrib->setName(name);
	}
    
	return attrib;
}

GXmlAttribute* GXmlAttributeSet::find(const char* name) const
{
	for (GXmlAttribute* node = m_sentinel.m_next; node != &m_sentinel; node = node->m_next)
	{
		if (strcmp(node->m_name.c_str(), name) == 0)
        {
			return node;
        }
	}
    
	return 0;
}

GXmlAttribute* GXmlAttributeSet::findOrCreate(const char* name)
{
	GXmlAttribute* attrib = find(name);
	if (attrib == nullptr) 
    {
		attrib = new GXmlAttribute();
		add(attrib);
		attrib->setName(name);
	}
    
	return attrib;
}

std::istream& operator >> (std::istream& in, GXmlNode& base)
{
	std::string tag;
	tag.reserve(8 * 1000);
	base.streamIn(&in, &tag);
	base.parse(tag.c_str(), 0, GXML_DEFAULT_ENCODING);
	return in;
}

std::ostream& operator << (std::ostream& out, const GXmlNode& base)
{
	GXmlPrinter printer;
	printer.setStreamPrinting();
	base.accept(&printer);
	out << printer.str();
	return out;
}


std::string& operator << (std::string& out, const GXmlNode& base)
{
	GXmlPrinter printer;
	printer.setStreamPrinting();
	base.accept(&printer);
	out.append(printer.str());
	return out;
}

GXmlHandle GXmlHandle::firstChild() const
{
	if (m_node != nullptr)
	{
		GXmlNode* child = m_node->firstChild();
		if (child != nullptr)
        {
			return GXmlHandle(child);
        }
	}
    
	return GXmlHandle(0);
}

GXmlHandle GXmlHandle::firstChild(const char* value) const
{
	if (m_node != nullptr)
	{
		GXmlNode* child = m_node->firstChild(value);
		if (child != nullptr)
        {
			return GXmlHandle(child);
        }
	}
    
	return GXmlHandle(0);
}

GXmlHandle GXmlHandle::firstChildElement() const
{
	if (m_node != nullptr)
	{
		GXmlElement* child = m_node->firstChildElement();
		if (child != nullptr)
        {
			return GXmlHandle(child);
        }
	}
    
	return GXmlHandle(0);
}

GXmlHandle GXmlHandle::firstChildElement(const char* value) const
{
	if (m_node != nullptr)
	{
		GXmlElement* child = m_node->firstChildElement(value);
		if (child != nullptr)
        {
			return GXmlHandle(child);
        }
	}
    
	return GXmlHandle(0);
}

GXmlHandle GXmlHandle::child(int count) const
{
	if (m_node != nullptr)
	{
		GXmlNode* child = m_node->firstChild();
		for (int i = 0; child != nullptr && i < count; child = child->nextSibling(), i++)
		{
			// nothing
		}
        
		if (child != nullptr)
        {
			return GXmlHandle(child);
        }
	}
    
	return GXmlHandle(0);
}

GXmlHandle GXmlHandle::child(const char* value, int count) const
{
	if (m_node != nullptr)
	{
		GXmlNode* child = m_node->firstChild(value);
		for (int i = 0; child && i < count; child = child->nextSibling(value), i++)
		{
			// nothing
		}
        
		if (child != nullptr)
        {
			return GXmlHandle(child);
        }
	}
    
	return GXmlHandle(0);
}

GXmlHandle GXmlHandle::childElement(int count) const
{
	if (m_node)
	{
		GXmlElement* child = m_node->firstChildElement();
		for (int i = 0; child != nullptr && i < count; child = child->nextSiblingElement(), i++)
		{
			// nothing
		}
        
		if (child != nullptr)
        {
			return GXmlHandle(child);
        }
	}
    
	return GXmlHandle(0);
}

GXmlHandle GXmlHandle::childElement(const char* value, int count) const
{
	if (m_node)
	{
		GXmlElement* child = m_node->firstChildElement(value);
		for (int i = 0; child != nullptr && i < count; child = child->nextSiblingElement(value), i++)
		{
			// nothing
		}
        
		if (child != nullptr)
        {
			return GXmlHandle(child);
        }
	}
    
	return GXmlHandle(0);
}

bool GXmlPrinter::visitEnter(const GXmlDocument&)
{
	return true;
}

bool GXmlPrinter::visitExit(const GXmlDocument&)
{
	return true;
}

bool GXmlPrinter::visitEnter(const GXmlElement& element, const GXmlAttribute* firstAttribute)
{
	doIndent();
	m_buffer += "<";
	m_buffer += element.value();
	for (const GXmlAttribute* attrib = firstAttribute; attrib != nullptr; attrib = attrib->next())
	{
		m_buffer += " ";
		attrib->print(0, 0, &m_buffer);
	}

	if (element.firstChild() == nullptr) 
	{
		m_buffer += " />";
		doLineBreak();
	}
	else 
	{
		m_buffer += ">";
		if (element.firstChild()->toText()
			&& element.lastChild() == element.firstChild()
			&& element.firstChild()->toText()->cdata() == false)
		{
			m_simpleTextPrint = true;
		}
		else
		{
			doLineBreak();
		}
	}
    
	++m_depth;
    
	return true;
}

bool GXmlPrinter::visitExit(const GXmlElement& element)
{
	--m_depth;
	if (element.firstChild() == nullptr) 
	{
		// nothing.
	}
	else 
	{
		if (m_simpleTextPrint)
		{
			m_simpleTextPrint = false;
		}
		else
		{
			doIndent();
		}
        
		m_buffer += "</";
		m_buffer += element.value();
		m_buffer += ">";
		doLineBreak();
	}
    
	return true;
}

bool GXmlPrinter::visit(const GXmlText& text)
{
	if (text.cdata())
	{
		doIndent();
		m_buffer += "<![CDATA[";
		m_buffer += text.value();
		m_buffer += "]]>";
		doLineBreak();
	}
	else if (m_simpleTextPrint)
	{
		std::string str;
		GXmlBase::encodeString(text.valueTStr(), &str);
		m_buffer += str;
	}
	else
	{
		doIndent();
		std::string str;
		GXmlBase::encodeString(text.valueTStr(), &str);
		m_buffer += str;
		doLineBreak();
	}
    
	return true;
}

bool GXmlPrinter::visit(const GXmlDeclaration& declaration)
{
	doIndent();
	declaration.print(0, 0, &m_buffer);
	doLineBreak();
	return true;
}

bool GXmlPrinter::visit(const GXmlComment& comment)
{
	doIndent();
	m_buffer += "<!--";
	m_buffer += comment.value();
	m_buffer += "-->";
	doLineBreak();
	return true;
}

bool GXmlPrinter::visit(const GXmlUnknown& unknown)
{
	doIndent();
	m_buffer += "<";
	m_buffer += unknown.value();
	m_buffer += ">";
	doLineBreak();
	return true;
}
