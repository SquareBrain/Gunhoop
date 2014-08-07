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
#include <tinyxml.h>

FILE* GXmlFOpen( const char* filename, const char* mode );

bool GXmlBase::condenseWhiteSpace = true;

// Microsoft compiler security
FILE* GXmlFOpen( const char* filename, const char* mode )
{
	return fopen( filename, mode );
}

void GXmlBase::EncodeString( const std::string& str, std::string* outString )
{
	int i=0;

	while( i<(int)str.length() )
	{
		unsigned char c = (unsigned char) str[i];

		if (    c == '&' 
		     && i < ( (int)str.length() - 2 )
			 && str[i+1] == '#'
			 && str[i+2] == 'x' )
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
			while ( i<(int)str.length()-1 )
			{
				outString->append( str.c_str() + i, 1 );
				++i;
				if ( str[i] == ';' )
					break;
			}
		}
		else if ( c == '&' )
		{
			outString->append( entity[0].str, entity[0].strLength );
			++i;
		}
		else if ( c == '<' )
		{
			outString->append( entity[1].str, entity[1].strLength );
			++i;
		}
		else if ( c == '>' )
		{
			outString->append( entity[2].str, entity[2].strLength );
			++i;
		}
		else if ( c == '\"' )
		{
			outString->append( entity[3].str, entity[3].strLength );
			++i;
		}
		else if ( c == '\'' )
		{
			outString->append( entity[4].str, entity[4].strLength );
			++i;
		}
		else if ( c < 32 )
		{
			// Easy pass at non-alpha/numeric/symbol
			// Below 32 is symbolic.
			char buf[ 32 ];
			
			#if defined(GXML_SNPRINTF)		
				GXML_SNPRINTF( buf, sizeof(buf), "&#x%02X;", (unsigned) ( c & 0xff ) );
			#else
				sprintf( buf, "&#x%02X;", (unsigned) ( c & 0xff ) );
			#endif		

			//*ME:	warning C4267: convert 'size_t' to 'int'
			//*ME:	Int-Cast to make compiler happy ...
			outString->append( buf, (int)strlen( buf ) );
			++i;
		}
		else
		{
			//char realc = (char) c;
			//outString->append( &realc, 1 );
			*outString += (char) c;	// somewhat more efficient function call.
			++i;
		}
	}
}


GXmlNode::GXmlNode( NodeType _type ) : GXmlBase()
{
	parent = 0;
	type = _type;
	firstChild = 0;
	lastChild = 0;
	prev = 0;
	next = 0;
}


GXmlNode::~GXmlNode()
{
	GXmlNode* node = firstChild;
	GXmlNode* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}	
}


void GXmlNode::CopyTo( GXmlNode* target ) const
{
	target->SetValue (value.c_str() );
	target->userData = userData; 
	target->location = location;
}


void GXmlNode::clear()
{
	GXmlNode* node = firstChild;
	GXmlNode* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}	

	firstChild = 0;
	lastChild = 0;
}


GXmlNode* GXmlNode::LinkEndChild( GXmlNode* node )
{
	assert( node->parent == 0 || node->parent == this );
	assert( node->GetDocument() == 0 || node->GetDocument() == this->GetDocument() );

	if ( node->Type() == GXmlNode::GNYXML_DOCUMENT )
	{
		delete node;
		if ( GetDocument() ) 
			GetDocument()->SetError( GXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, GXML_ENCODING_UNKNOWN );
		return 0;
	}

	node->parent = this;

	node->prev = lastChild;
	node->next = 0;

	if ( lastChild )
		lastChild->next = node;
	else
		firstChild = node;			// it was an empty list.

	lastChild = node;
	return node;
}


GXmlNode* GXmlNode::InsertEndChild( const GXmlNode& addThis )
{
	if ( addThis.Type() == GXmlNode::GNYXML_DOCUMENT )
	{
		if ( GetDocument() ) 
			GetDocument()->SetError( GXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, GXML_ENCODING_UNKNOWN );
		return 0;
	}
	GXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;

	return LinkEndChild( node );
}


GXmlNode* GXmlNode::InsertBeforeChild( GXmlNode* beforeThis, const GXmlNode& addThis )
{	
	if ( !beforeThis || beforeThis->parent != this ) {
		return 0;
	}
	if ( addThis.Type() == GXmlNode::GNYXML_DOCUMENT )
	{
		if ( GetDocument() ) 
			GetDocument()->SetError( GXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, GXML_ENCODING_UNKNOWN );
		return 0;
	}

	GXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;
	node->parent = this;

	node->next = beforeThis;
	node->prev = beforeThis->prev;
	if ( beforeThis->prev )
	{
		beforeThis->prev->next = node;
	}
	else
	{
		assert( firstChild == beforeThis );
		firstChild = node;
	}
	beforeThis->prev = node;
	return node;
}


GXmlNode* GXmlNode::InsertAfterChild( GXmlNode* afterThis, const GXmlNode& addThis )
{
	if ( !afterThis || afterThis->parent != this ) {
		return 0;
	}
	if ( addThis.Type() == GXmlNode::GNYXML_DOCUMENT )
	{
		if ( GetDocument() ) 
			GetDocument()->SetError( GXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, GXML_ENCODING_UNKNOWN );
		return 0;
	}

	GXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;
	node->parent = this;

	node->prev = afterThis;
	node->next = afterThis->next;
	if ( afterThis->next )
	{
		afterThis->next->prev = node;
	}
	else
	{
		assert( lastChild == afterThis );
		lastChild = node;
	}
	afterThis->next = node;
	return node;
}


GXmlNode* GXmlNode::ReplaceChild( GXmlNode* replaceThis, const GXmlNode& withThis )
{
	if ( !replaceThis )
		return 0;

	if ( replaceThis->parent != this )
		return 0;

	if ( withThis.ToDocument() ) {
		// A document can never be a child.	Thanks to Noam.
		GXmlDocument* document = GetDocument();
		if ( document ) 
			document->SetError( GXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, GXML_ENCODING_UNKNOWN );
		return 0;
	}

	GXmlNode* node = withThis.Clone();
	if ( !node )
		return 0;

	node->next = replaceThis->next;
	node->prev = replaceThis->prev;

	if ( replaceThis->next )
		replaceThis->next->prev = node;
	else
		lastChild = node;

	if ( replaceThis->prev )
		replaceThis->prev->next = node;
	else
		firstChild = node;

	delete replaceThis;
	node->parent = this;
	return node;
}


bool GXmlNode::RemoveChild( GXmlNode* removeThis )
{
	if ( !removeThis ) {
		return false;
	}

	if ( removeThis->parent != this )
	{	
		assert( 0 );
		return false;
	}

	if ( removeThis->next )
		removeThis->next->prev = removeThis->prev;
	else
		lastChild = removeThis->prev;

	if ( removeThis->prev )
		removeThis->prev->next = removeThis->next;
	else
		firstChild = removeThis->next;

	delete removeThis;
	return true;
}

const GXmlNode* GXmlNode::FirstChild( const char * _value ) const
{
	const GXmlNode* node;
	for ( node = firstChild; node; node = node->next )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


const GXmlNode* GXmlNode::LastChild( const char * _value ) const
{
	const GXmlNode* node;
	for ( node = lastChild; node; node = node->prev )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


const GXmlNode* GXmlNode::IterateChildren( const GXmlNode* previous ) const
{
	if ( !previous )
	{
		return FirstChild();
	}
	else
	{
		assert( previous->parent == this );
		return previous->NextSibling();
	}
}


const GXmlNode* GXmlNode::IterateChildren( const char * val, const GXmlNode* previous ) const
{
	if ( !previous )
	{
		return FirstChild( val );
	}
	else
	{
		assert( previous->parent == this );
		return previous->NextSibling( val );
	}
}


const GXmlNode* GXmlNode::NextSibling( const char * _value ) const 
{
	const GXmlNode* node;
	for ( node = next; node; node = node->next )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


const GXmlNode* GXmlNode::PreviousSibling( const char * _value ) const
{
	const GXmlNode* node;
	for ( node = prev; node; node = node->prev )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


void GXmlElement::RemoveAttribute( const char * name )
{
    #ifdef GXML_USE_STL
	std::string str( name );
	GXmlAttribute* node = attributeSet.Find( str );
	#else
	GXmlAttribute* node = attributeSet.Find( name );
	#endif
	if ( node )
	{
		attributeSet.Remove( node );
		delete node;
	}
}

const GXmlElement* GXmlNode::FirstChildElement() const
{
	const GXmlNode* node;

	for (	node = FirstChild();
			node;
			node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const GXmlElement* GXmlNode::FirstChildElement( const char * _value ) const
{
	const GXmlNode* node;

	for (	node = FirstChild( _value );
			node;
			node = node->NextSibling( _value ) )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const GXmlElement* GXmlNode::NextSiblingElement() const
{
	const GXmlNode* node;

	for (	node = NextSibling();
			node;
			node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const GXmlElement* GXmlNode::NextSiblingElement( const char * _value ) const
{
	const GXmlNode* node;

	for (	node = NextSibling( _value );
			node;
			node = node->NextSibling( _value ) )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const GXmlDocument* GXmlNode::GetDocument() const
{
	const GXmlNode* node;

	for( node = this; node; node = node->parent )
	{
		if ( node->ToDocument() )
			return node->ToDocument();
	}
	return 0;
}


GXmlElement::GXmlElement (const char * _value)
	: GXmlNode( GXmlNode::GNYXML_ELEMENT )
{
	firstChild = lastChild = 0;
	value = _value;
}


#ifdef GXML_USE_STL
GXmlElement::GXmlElement( const std::string& _value ) 
	: GXmlNode( GXmlNode::GNYXML_ELEMENT )
{
	firstChild = lastChild = 0;
	value = _value;
}
#endif


GXmlElement::GXmlElement( const GXmlElement& copy)
	: GXmlNode( GXmlNode::GNYXML_ELEMENT )
{
	firstChild = lastChild = 0;
	copy.CopyTo( this );	
}


GXmlElement& GXmlElement::operator=( const GXmlElement& base )
{
	ClearThis();
	base.CopyTo( this );
	return *this;
}


GXmlElement::~GXmlElement()
{
	ClearThis();
}


void GXmlElement::ClearThis()
{
	clear();
	while( attributeSet.First() )
	{
		GXmlAttribute* node = attributeSet.First();
		attributeSet.Remove( node );
		delete node;
	}
}


const char* GXmlElement::Attribute( const char* name ) const
{
	const GXmlAttribute* node = attributeSet.Find( name );
	if ( node )
		return node->Value();
	return 0;
}


#ifdef GXML_USE_STL
const std::string* GXmlElement::Attribute( const std::string& name ) const
{
	const GXmlAttribute* attrib = attributeSet.Find( name );
	if ( attrib )
		return &attrib->ValueStr();
	return 0;
}
#endif


const char* GXmlElement::Attribute( const char* name, int* i ) const
{
	const GXmlAttribute* attrib = attributeSet.Find( name );
	const char* result = 0;

	if ( attrib ) {
		result = attrib->Value();
		if ( i ) {
			attrib->QueryIntValue( i );
		}
	}
	return result;
}


#ifdef GXML_USE_STL
const std::string* GXmlElement::Attribute( const std::string& name, int* i ) const
{
	const GXmlAttribute* attrib = attributeSet.Find( name );
	const std::string* result = 0;

	if ( attrib ) {
		result = &attrib->ValueStr();
		if ( i ) {
			attrib->QueryIntValue( i );
		}
	}
	return result;
}
#endif


const char* GXmlElement::Attribute( const char* name, double* d ) const
{
	const GXmlAttribute* attrib = attributeSet.Find( name );
	const char* result = 0;

	if ( attrib ) {
		result = attrib->Value();
		if ( d ) {
			attrib->QueryDoubleValue( d );
		}
	}
	return result;
}


#ifdef GXML_USE_STL
const std::string* GXmlElement::Attribute( const std::string& name, double* d ) const
{
	const GXmlAttribute* attrib = attributeSet.Find( name );
	const std::string* result = 0;

	if ( attrib ) {
		result = &attrib->ValueStr();
		if ( d ) {
			attrib->QueryDoubleValue( d );
		}
	}
	return result;
}
#endif


int GXmlElement::QueryIntAttribute( const char* name, int* ival ) const
{
	const GXmlAttribute* attrib = attributeSet.Find( name );
	if ( !attrib )
		return GXML_NO_ATTRIBUTE;
	return attrib->QueryIntValue( ival );
}


int GXmlElement::QueryUnsignedAttribute( const char* name, unsigned* value ) const
{
	const GXmlAttribute* node = attributeSet.Find( name );
	if ( !node )
		return GXML_NO_ATTRIBUTE;

	int ival = 0;
	int result = node->QueryIntValue( &ival );
	*value = (unsigned)ival;
	return result;
}


int GXmlElement::QueryBoolAttribute( const char* name, bool* bval ) const
{
	const GXmlAttribute* node = attributeSet.Find( name );
	if ( !node )
		return GXML_NO_ATTRIBUTE;
	
	int result = GXML_WRONG_TYPE;
	if (    StringEqual( node->Value(), "true", true, GXML_ENCODING_UNKNOWN ) 
		 || StringEqual( node->Value(), "yes", true, GXML_ENCODING_UNKNOWN ) 
		 || StringEqual( node->Value(), "1", true, GXML_ENCODING_UNKNOWN ) ) 
	{
		*bval = true;
		result = GXML_SUCCESS;
	}
	else if (    StringEqual( node->Value(), "false", true, GXML_ENCODING_UNKNOWN ) 
			  || StringEqual( node->Value(), "no", true, GXML_ENCODING_UNKNOWN ) 
			  || StringEqual( node->Value(), "0", true, GXML_ENCODING_UNKNOWN ) ) 
	{
		*bval = false;
		result = GXML_SUCCESS;
	}
	return result;
}



#ifdef GXML_USE_STL
int GXmlElement::QueryIntAttribute( const std::string& name, int* ival ) const
{
	const GXmlAttribute* attrib = attributeSet.Find( name );
	if ( !attrib )
		return GXML_NO_ATTRIBUTE;
	return attrib->QueryIntValue( ival );
}
#endif


int GXmlElement::QueryDoubleAttribute( const char* name, double* dval ) const
{
	const GXmlAttribute* attrib = attributeSet.Find( name );
	if ( !attrib )
		return GXML_NO_ATTRIBUTE;
	return attrib->QueryDoubleValue( dval );
}


#ifdef GXML_USE_STL
int GXmlElement::QueryDoubleAttribute( const std::string& name, double* dval ) const
{
	const GXmlAttribute* attrib = attributeSet.Find( name );
	if ( !attrib )
		return GXML_NO_ATTRIBUTE;
	return attrib->QueryDoubleValue( dval );
}
#endif


void GXmlElement::SetAttribute( const char * name, int val )
{	
	GXmlAttribute* attrib = attributeSet.FindOrCreate( name );
	if ( attrib ) {
		attrib->SetIntValue( val );
	}
}


#ifdef GXML_USE_STL
void GXmlElement::SetAttribute( const std::string& name, int val )
{	
	GXmlAttribute* attrib = attributeSet.FindOrCreate( name );
	if ( attrib ) {
		attrib->SetIntValue( val );
	}
}
#endif


void GXmlElement::SetDoubleAttribute( const char * name, double val )
{	
	GXmlAttribute* attrib = attributeSet.FindOrCreate( name );
	if ( attrib ) {
		attrib->SetDoubleValue( val );
	}
}


#ifdef GXML_USE_STL
void GXmlElement::SetDoubleAttribute( const std::string& name, double val )
{	
	GXmlAttribute* attrib = attributeSet.FindOrCreate( name );
	if ( attrib ) {
		attrib->SetDoubleValue( val );
	}
}
#endif 


void GXmlElement::SetAttribute( const char * cname, const char * cvalue )
{
	GXmlAttribute* attrib = attributeSet.FindOrCreate( cname );
	if ( attrib ) {
		attrib->SetValue( cvalue );
	}
}


#ifdef GXML_USE_STL
void GXmlElement::SetAttribute( const std::string& _name, const std::string& _value )
{
	GXmlAttribute* attrib = attributeSet.FindOrCreate( _name );
	if ( attrib ) {
		attrib->SetValue( _value );
	}
}
#endif


void GXmlElement::Print( FILE* cfile, int depth ) const
{
	int i;
	assert( cfile );
	for ( i=0; i<depth; i++ ) {
		fprintf( cfile, "    " );
	}

	fprintf( cfile, "<%s", value.c_str() );

	const GXmlAttribute* attrib;
	for ( attrib = attributeSet.First(); attrib; attrib = attrib->Next() )
	{
		fprintf( cfile, " " );
		attrib->Print( cfile, depth );
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	GXmlNode* node;
	if ( !firstChild )
	{
		fprintf( cfile, " />" );
	}
	else if ( firstChild == lastChild && firstChild->ToText() )
	{
		fprintf( cfile, ">" );
		firstChild->Print( cfile, depth + 1 );
		fprintf( cfile, "</%s>", value.c_str() );
	}
	else
	{
		fprintf( cfile, ">" );

		for ( node = firstChild; node; node=node->NextSibling() )
		{
			if ( !node->ToText() )
			{
				fprintf( cfile, "\n" );
			}
			node->Print( cfile, depth+1 );
		}
		fprintf( cfile, "\n" );
		for( i=0; i<depth; ++i ) {
			fprintf( cfile, "    " );
		}
		fprintf( cfile, "</%s>", value.c_str() );
	}
}


void GXmlElement::CopyTo( GXmlElement* target ) const
{
	// superclass:
	GXmlNode::CopyTo( target );

	// Element class: 
	// Clone the attributes, then clone the children.
	const GXmlAttribute* attribute = 0;
	for(	attribute = attributeSet.First();
	attribute;
	attribute = attribute->Next() )
	{
		target->SetAttribute( attribute->Name(), attribute->Value() );
	}

	GXmlNode* node = 0;
	for ( node = firstChild; node; node = node->NextSibling() )
	{
		target->LinkEndChild( node->Clone() );
	}
}

bool GXmlElement::Accept( GXmlVisitor* visitor ) const
{
	if ( visitor->VisitEnter( *this, attributeSet.First() ) ) 
	{
		for ( const GXmlNode* node=FirstChild(); node; node=node->NextSibling() )
		{
			if ( !node->Accept( visitor ) )
				break;
		}
	}
	return visitor->VisitExit( *this );
}


GXmlNode* GXmlElement::Clone() const
{
	GXmlElement* clone = new GXmlElement( Value() );
	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


const char* GXmlElement::GetText() const
{
	const GXmlNode* child = this->FirstChild();
	if ( child ) {
		const GXmlText* childText = child->ToText();
		if ( childText ) {
			return childText->Value();
		}
	}
	return 0;
}


GXmlDocument::GXmlDocument() : GXmlNode( GXmlNode::GNYXML_DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
	ClearError();
}

GXmlDocument::GXmlDocument( const char * documentName ) : GXmlNode( GXmlNode::GNYXML_DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
	value = documentName;
	ClearError();
}


#ifdef GXML_USE_STL
GXmlDocument::GXmlDocument( const std::string& documentName ) : GXmlNode( GXmlNode::GNYXML_DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
    value = documentName;
	ClearError();
}
#endif


GXmlDocument::GXmlDocument( const GXmlDocument& copy ) : GXmlNode( GXmlNode::GNYXML_DOCUMENT )
{
	copy.CopyTo( this );
}


GXmlDocument& GXmlDocument::operator=( const GXmlDocument& copy )
{
	clear();
	copy.CopyTo( this );
	return *this;
}


bool GXmlDocument::LoadFile( GXmlEncoding encoding )
{
	return LoadFile( Value(), encoding );
}


bool GXmlDocument::SaveFile() const
{
	return SaveFile( Value() );
}

bool GXmlDocument::LoadFile( const char* _filename, GXmlEncoding encoding )
{
	std::string filename( _filename );
	value = filename;

	// reading in binary mode so that tinyxml can normalize the EOL
	FILE* file = GXmlFOpen( value.c_str (), "rb" );	

	if ( file )
	{
		bool result = LoadFile( file, encoding );
		fclose( file );
		return result;
	}
	else
	{
		SetError( GXML_ERROR_OPENING_FILE, 0, 0, GXML_ENCODING_UNKNOWN );
		return false;
	}
}

bool GXmlDocument::LoadFile( FILE* file, GXmlEncoding encoding )
{
	if ( !file ) 
	{
		SetError( GXML_ERROR_OPENING_FILE, 0, 0, GXML_ENCODING_UNKNOWN );
		return false;
	}

	// Delete the existing data:
	clear();
	location.clear();

	// Get the file size, so we can pre-allocate the string. HUGE speed impact.
	long length = 0;
	fseek( file, 0, SEEK_END );
	length = ftell( file );
	fseek( file, 0, SEEK_SET );

	// Strange case, but good to handle up front.
	if ( length <= 0 )
	{
		SetError( GXML_ERROR_DOCUMENT_EMPTY, 0, 0, GXML_ENCODING_UNKNOWN );
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

	char* buf = new char[ length+1 ];
	buf[0] = 0;

	if ( fread( buf, length, 1, file ) != 1 ) {
		delete [] buf;
		SetError( GXML_ERROR_OPENING_FILE, 0, 0, GXML_ENCODING_UNKNOWN );
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
	while( *p ) {
		assert( p < (buf+length) );
		assert( q <= (buf+length) );
		assert( q <= p );

		if ( *p == CR ) {
			*q++ = LF;
			p++;
			if ( *p == LF ) {		// check for CR+LF (and skip LF)
				p++;
			}
		}
		else {
			*q++ = *p++;
		}
	}
	assert( q <= (buf+length) );
	*q = 0;

	Parse( buf, 0, encoding );

	delete [] buf;
	return !Error();
}


bool GXmlDocument::SaveFile( const char * filename ) const
{
	// The old c stuff lives on...
	FILE* fp = GXmlFOpen( filename, "w" );
	if ( fp )
	{
		bool result = SaveFile( fp );
		fclose( fp );
		return result;
	}
	return false;
}


bool GXmlDocument::SaveFile( FILE* fp ) const
{
	if ( useMicrosoftBOM ) 
	{
		const unsigned char GXML_UTF_LEAD_0 = 0xefU;
		const unsigned char GXML_UTF_LEAD_1 = 0xbbU;
		const unsigned char GXML_UTF_LEAD_2 = 0xbfU;

		fputc( GXML_UTF_LEAD_0, fp );
		fputc( GXML_UTF_LEAD_1, fp );
		fputc( GXML_UTF_LEAD_2, fp );
	}
	Print( fp, 0 );
	return (ferror(fp) == 0);
}


void GXmlDocument::CopyTo( GXmlDocument* target ) const
{
	GXmlNode::CopyTo( target );

	target->error = error;
	target->errorId = errorId;
	target->errorDesc = errorDesc;
	target->tabsize = tabsize;
	target->errorLocation = errorLocation;
	target->useMicrosoftBOM = useMicrosoftBOM;

	GXmlNode* node = 0;
	for ( node = firstChild; node; node = node->NextSibling() )
	{
		target->LinkEndChild( node->Clone() );
	}	
}


GXmlNode* GXmlDocument::Clone() const
{
	GXmlDocument* clone = new GXmlDocument();
	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void GXmlDocument::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	for ( const GXmlNode* node=FirstChild(); node; node=node->NextSibling() )
	{
		node->Print( cfile, depth );
		fprintf( cfile, "\n" );
	}
}


bool GXmlDocument::Accept( GXmlVisitor* visitor ) const
{
	if ( visitor->VisitEnter( *this ) )
	{
		for ( const GXmlNode* node=FirstChild(); node; node=node->NextSibling() )
		{
			if ( !node->Accept( visitor ) )
				break;
		}
	}
	return visitor->VisitExit( *this );
}


const GXmlAttribute* GXmlAttribute::Next() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}

/*
GXmlAttribute* GXmlAttribute::Next()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}
*/

const GXmlAttribute* GXmlAttribute::Previous() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}

/*
GXmlAttribute* GXmlAttribute::Previous()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}
*/

void GXmlAttribute::Print( FILE* cfile, int /*depth*/, std::string* str ) const
{
	std::string n, v;

	EncodeString( name, &n );
	EncodeString( value, &v );

	if (value.find ('\"') == std::string::npos) {
		if ( cfile ) {
			fprintf (cfile, "%s=\"%s\"", n.c_str(), v.c_str() );
		}
		if ( str ) {
			(*str) += n; (*str) += "=\""; (*str) += v; (*str) += "\"";
		}
	}
	else {
		if ( cfile ) {
			fprintf (cfile, "%s='%s'", n.c_str(), v.c_str() );
		}
		if ( str ) {
			(*str) += n; (*str) += "='"; (*str) += v; (*str) += "'";
		}
	}
}


int GXmlAttribute::QueryIntValue( int* ival ) const
{
	if ( sscanf( value.c_str(), "%d", ival ) == 1 )
		return GXML_SUCCESS;
	return GXML_WRONG_TYPE;
}

int GXmlAttribute::QueryDoubleValue( double* dval ) const
{
	if ( sscanf( value.c_str(), "%lf", dval ) == 1 )
		return GXML_SUCCESS;
	return GXML_WRONG_TYPE;
}

void GXmlAttribute::SetIntValue( int _value )
{
	char buf [64];
	#if defined(GXML_SNPRINTF)		
		GXML_SNPRINTF(buf, sizeof(buf), "%d", _value);
	#else
		sprintf (buf, "%d", _value);
	#endif
	SetValue (buf);
}

void GXmlAttribute::SetDoubleValue( double _value )
{
	char buf [256];
	#if defined(GXML_SNPRINTF)		
		GXML_SNPRINTF( buf, sizeof(buf), "%g", _value);
	#else
		sprintf (buf, "%g", _value);
	#endif
	SetValue (buf);
}

int GXmlAttribute::IntValue() const
{
	return atoi (value.c_str ());
}

double  GXmlAttribute::DoubleValue() const
{
	return atof (value.c_str ());
}


GXmlComment::GXmlComment( const GXmlComment& copy ) : GXmlNode( GXmlNode::GNYXML_COMMENT )
{
	copy.CopyTo( this );
}


GXmlComment& GXmlComment::operator=( const GXmlComment& base )
{
	clear();
	base.CopyTo( this );
	return *this;
}


void GXmlComment::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	for ( int i=0; i<depth; i++ )
	{
		fprintf( cfile,  "    " );
	}
	fprintf( cfile, "<!--%s-->", value.c_str() );
}


void GXmlComment::CopyTo( GXmlComment* target ) const
{
	GXmlNode::CopyTo( target );
}


bool GXmlComment::Accept( GXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}


GXmlNode* GXmlComment::Clone() const
{
	GXmlComment* clone = new GXmlComment();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void GXmlText::Print( FILE* cfile, int depth ) const
{
	assert( cfile );
	if ( cdata )
	{
		int i;
		fprintf( cfile, "\n" );
		for ( i=0; i<depth; i++ ) {
			fprintf( cfile, "    " );
		}
		fprintf( cfile, "<![CDATA[%s]]>\n", value.c_str() );	// unformatted output
	}
	else
	{
		std::string buffer;
		EncodeString( value, &buffer );
		fprintf( cfile, "%s", buffer.c_str() );
	}
}


void GXmlText::CopyTo( GXmlText* target ) const
{
	GXmlNode::CopyTo( target );
	target->cdata = cdata;
}


bool GXmlText::Accept( GXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}


GXmlNode* GXmlText::Clone() const
{	
	GXmlText* clone = 0;
	clone = new GXmlText( "" );

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


GXmlDeclaration::GXmlDeclaration( const char * _version,
									const char * _encoding,
									const char * _standalone )
	: GXmlNode( GXmlNode::GNYXML_DECLARAGON )
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}


#ifdef GXML_USE_STL
GXmlDeclaration::GXmlDeclaration(	const std::string& _version,
									const std::string& _encoding,
									const std::string& _standalone )
	: GXmlNode( GXmlNode::GNYXML_DECLARAGON )
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}
#endif


GXmlDeclaration::GXmlDeclaration( const GXmlDeclaration& copy )
	: GXmlNode( GXmlNode::GNYXML_DECLARAGON )
{
	copy.CopyTo( this );	
}


GXmlDeclaration& GXmlDeclaration::operator=( const GXmlDeclaration& copy )
{
	clear();
	copy.CopyTo( this );
	return *this;
}


void GXmlDeclaration::Print( FILE* cfile, int /*depth*/, std::string* str ) const
{
	if ( cfile ) fprintf( cfile, "<?xml " );
	if ( str )	 (*str) += "<?xml ";

	if ( !version.empty() ) {
		if ( cfile ) fprintf (cfile, "version=\"%s\" ", version.c_str ());
		if ( str ) { (*str) += "version=\""; (*str) += version; (*str) += "\" "; }
	}
	if ( !encoding.empty() ) {
		if ( cfile ) fprintf (cfile, "encoding=\"%s\" ", encoding.c_str ());
		if ( str ) { (*str) += "encoding=\""; (*str) += encoding; (*str) += "\" "; }
	}
	if ( !standalone.empty() ) {
		if ( cfile ) fprintf (cfile, "standalone=\"%s\" ", standalone.c_str ());
		if ( str ) { (*str) += "standalone=\""; (*str) += standalone; (*str) += "\" "; }
	}
	if ( cfile ) fprintf( cfile, "?>" );
	if ( str )	 (*str) += "?>";
}


void GXmlDeclaration::CopyTo( GXmlDeclaration* target ) const
{
	GXmlNode::CopyTo( target );

	target->version = version;
	target->encoding = encoding;
	target->standalone = standalone;
}


bool GXmlDeclaration::Accept( GXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}


GXmlNode* GXmlDeclaration::Clone() const
{	
	GXmlDeclaration* clone = new GXmlDeclaration();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void GXmlUnknown::Print( FILE* cfile, int depth ) const
{
	for ( int i=0; i<depth; i++ )
		fprintf( cfile, "    " );
	fprintf( cfile, "<%s>", value.c_str() );
}


void GXmlUnknown::CopyTo( GXmlUnknown* target ) const
{
	GXmlNode::CopyTo( target );
}


bool GXmlUnknown::Accept( GXmlVisitor* visitor ) const
{
	return visitor->Visit( *this );
}


GXmlNode* GXmlUnknown::Clone() const
{
	GXmlUnknown* clone = new GXmlUnknown();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


GXmlAttributeSet::GXmlAttributeSet()
{
	sentinel.next = &sentinel;
	sentinel.prev = &sentinel;
}


GXmlAttributeSet::~GXmlAttributeSet()
{
	assert( sentinel.next == &sentinel );
	assert( sentinel.prev == &sentinel );
}


void GXmlAttributeSet::Add( GXmlAttribute* addMe )
{
    #ifdef GXML_USE_STL
	assert( !Find( std::string( addMe->Name() ) ) );	// Shouldn't be multiply adding to the set.
	#else
	assert( !Find( addMe->Name() ) );	// Shouldn't be multiply adding to the set.
	#endif

	addMe->next = &sentinel;
	addMe->prev = sentinel.prev;

	sentinel.prev->next = addMe;
	sentinel.prev      = addMe;
}

void GXmlAttributeSet::Remove( GXmlAttribute* removeMe )
{
	GXmlAttribute* node;

	for( node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node == removeMe )
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = 0;
			node->prev = 0;
			return;
		}
	}
	assert( 0 );		// we tried to remove a non-linked attribute.
}


#ifdef GXML_USE_STL
GXmlAttribute* GXmlAttributeSet::Find( const std::string& name ) const
{
	for( GXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node->name == name )
			return node;
	}
	return 0;
}

GXmlAttribute* GXmlAttributeSet::FindOrCreate( const std::string& _name )
{
	GXmlAttribute* attrib = Find( _name );
	if ( !attrib ) {
		attrib = new GXmlAttribute();
		Add( attrib );
		attrib->SetName( _name );
	}
	return attrib;
}
#endif


GXmlAttribute* GXmlAttributeSet::Find( const char* name ) const
{
	for( GXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( strcmp( node->name.c_str(), name ) == 0 )
			return node;
	}
	return 0;
}


GXmlAttribute* GXmlAttributeSet::FindOrCreate( const char* _name )
{
	GXmlAttribute* attrib = Find( _name );
	if ( !attrib ) {
		attrib = new GXmlAttribute();
		Add( attrib );
		attrib->SetName( _name );
	}
	return attrib;
}


#ifdef GXML_USE_STL	
std::istream& operator>> (std::istream & in, GXmlNode & base)
{
	std::string tag;
	tag.reserve( 8 * 1000 );
	base.StreamIn( &in, &tag );

	base.Parse( tag.c_str(), 0, GXML_DEFAULT_ENCODING );
	return in;
}
#endif


#ifdef GXML_USE_STL	
std::ostream& operator<< (std::ostream & out, const GXmlNode & base)
{
	GXmlPrinter printer;
	printer.SetStreamPrinting();
	base.Accept( &printer );
	out << printer.Str();

	return out;
}


std::string& operator<< (std::string& out, const GXmlNode& base )
{
	GXmlPrinter printer;
	printer.SetStreamPrinting();
	base.Accept( &printer );
	out.append( printer.Str() );

	return out;
}
#endif


GXmlHandle GXmlHandle::FirstChild() const
{
	if ( node )
	{
		GXmlNode* child = node->FirstChild();
		if ( child )
			return GXmlHandle( child );
	}
	return GXmlHandle( 0 );
}


GXmlHandle GXmlHandle::FirstChild( const char * value ) const
{
	if ( node )
	{
		GXmlNode* child = node->FirstChild( value );
		if ( child )
			return GXmlHandle( child );
	}
	return GXmlHandle( 0 );
}


GXmlHandle GXmlHandle::FirstChildElement() const
{
	if ( node )
	{
		GXmlElement* child = node->FirstChildElement();
		if ( child )
			return GXmlHandle( child );
	}
	return GXmlHandle( 0 );
}


GXmlHandle GXmlHandle::FirstChildElement( const char * value ) const
{
	if ( node )
	{
		GXmlElement* child = node->FirstChildElement( value );
		if ( child )
			return GXmlHandle( child );
	}
	return GXmlHandle( 0 );
}


GXmlHandle GXmlHandle::Child( int count ) const
{
	if ( node )
	{
		int i;
		GXmlNode* child = node->FirstChild();
		for (	i=0;
				child && i<count;
				child = child->NextSibling(), ++i )
		{
			// nothing
		}
		if ( child )
			return GXmlHandle( child );
	}
	return GXmlHandle( 0 );
}


GXmlHandle GXmlHandle::Child( const char* value, int count ) const
{
	if ( node )
	{
		int i;
		GXmlNode* child = node->FirstChild( value );
		for (	i=0;
				child && i<count;
				child = child->NextSibling( value ), ++i )
		{
			// nothing
		}
		if ( child )
			return GXmlHandle( child );
	}
	return GXmlHandle( 0 );
}


GXmlHandle GXmlHandle::ChildElement( int count ) const
{
	if ( node )
	{
		int i;
		GXmlElement* child = node->FirstChildElement();
		for (	i=0;
				child && i<count;
				child = child->NextSiblingElement(), ++i )
		{
			// nothing
		}
		if ( child )
			return GXmlHandle( child );
	}
	return GXmlHandle( 0 );
}


GXmlHandle GXmlHandle::ChildElement( const char* value, int count ) const
{
	if ( node )
	{
		int i;
		GXmlElement* child = node->FirstChildElement( value );
		for (	i=0;
				child && i<count;
				child = child->NextSiblingElement( value ), ++i )
		{
			// nothing
		}
		if ( child )
			return GXmlHandle( child );
	}
	return GXmlHandle( 0 );
}


bool GXmlPrinter::VisitEnter( const GXmlDocument& )
{
	return true;
}

bool GXmlPrinter::VisitExit( const GXmlDocument& )
{
	return true;
}

bool GXmlPrinter::VisitEnter( const GXmlElement& element, const GXmlAttribute* firstAttribute )
{
	DoIndent();
	buffer += "<";
	buffer += element.Value();

	for( const GXmlAttribute* attrib = firstAttribute; attrib; attrib = attrib->Next() )
	{
		buffer += " ";
		attrib->Print( 0, 0, &buffer );
	}

	if ( !element.FirstChild() ) 
	{
		buffer += " />";
		DoLineBreak();
	}
	else 
	{
		buffer += ">";
		if (    element.FirstChild()->ToText()
			  && element.LastChild() == element.FirstChild()
			  && element.FirstChild()->ToText()->CDATA() == false )
		{
			simpleTextPrint = true;
			// no DoLineBreak()!
		}
		else
		{
			DoLineBreak();
		}
	}
	++depth;	
	return true;
}


bool GXmlPrinter::VisitExit( const GXmlElement& element )
{
	--depth;
	if ( !element.FirstChild() ) 
	{
		// nothing.
	}
	else 
	{
		if ( simpleTextPrint )
		{
			simpleTextPrint = false;
		}
		else
		{
			DoIndent();
		}
		buffer += "</";
		buffer += element.Value();
		buffer += ">";
		DoLineBreak();
	}
	return true;
}


bool GXmlPrinter::Visit( const GXmlText& text )
{
	if ( text.CDATA() )
	{
		DoIndent();
		buffer += "<![CDATA[";
		buffer += text.Value();
		buffer += "]]>";
		DoLineBreak();
	}
	else if ( simpleTextPrint )
	{
		std::string str;
		GXmlBase::EncodeString( text.ValueTStr(), &str );
		buffer += str;
	}
	else
	{
		DoIndent();
		std::string str;
		GXmlBase::EncodeString( text.ValueTStr(), &str );
		buffer += str;
		DoLineBreak();
	}
	return true;
}


bool GXmlPrinter::Visit( const GXmlDeclaration& declaration )
{
	DoIndent();
	declaration.Print( 0, 0, &buffer );
	DoLineBreak();
	return true;
}


bool GXmlPrinter::Visit( const GXmlComment& comment )
{
	DoIndent();
	buffer += "<!--";
	buffer += comment.Value();
	buffer += "-->";
	DoLineBreak();
	return true;
}


bool GXmlPrinter::Visit( const GXmlUnknown& unknown )
{
	DoIndent();
	buffer += "<";
	buffer += unknown.Value();
	buffer += ">";
	DoLineBreak();
	return true;
}

