/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_xml.h
* @version     
* @brief      
* @author	duye
* @date		2014-8-7
* @note 
*
*  1. 2014-8-7 duye Created this file
* 
*/
#pragma once

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <sstream>

class GXmlDocument;
class GXmlElement;
class GXmlComment;
class GXmlUnknown;
class GXmlAttribute;
class GXmlText;
class GXmlDeclaration;
class GXmlParsingData;

const int GXML_MAJOR_VERSION = 2;
const int GXML_MINOR_VERSION = 6;
const int GXML_PATCH_VERSION = 2;

/*	Internal structure for tracking location of items 
	in the XML file.
*/
class GXmlCursor
{
public:
	GXmlCursor() 
	{ 
		clear(); 
	}
	
	void clear() 
	{ 
		m_row = m_col = -1; 
	}

	int m_row;	// 0 based.
	int m_col;	// 0 based.
};

/**
	Implements the interface to the "Visitor pattern" (see the Accept() method.)
	If you call the Accept() method, it requires being passed a GXmlVisitor
	class to handle callbacks. For nodes that contain other nodes (Document, Element)
	you will get called with a VisitEnter/VisitExit pair. Nodes that are always leaves
	are simply called with Visit().

	If you return 'true' from a Visit method, recursive parsing will continue. If you return
	false, <b>no children of this node or its sibilings</b> will be Visited.

	All flavors of Visit methods have a default implementation that returns 'true' (continue 
	visiting). You need to only override methods that are interesting to you.

	Generally Accept() is called on the GXmlDocument, although all nodes suppert Visiting.

	You should never change the document from a callback.

	@sa GXmlNode::Accept()
*/
class GXmlVisitor
{
public:
	virtual ~GXmlVisitor() {}

	/// Visit a document.
	virtual bool visitEnter(const GXmlDocument& /*doc*/) { return true; }
	/// Visit a document.
	virtual bool visitExit(const GXmlDocument& /*doc*/)	{ return true; }

	/// Visit an element.
	virtual bool visitEnter(const GXmlElement& /*element*/, const GXmlAttribute* /*firstAttribute*/) { return true; }
	/// Visit an element.
	virtual bool visitExit(const GXmlElement& /*element*/) { return true; }

	/// Visit a declaration
	virtual bool visit(const GXmlDeclaration& /*declaration*/) { return true; }
	/// Visit a text node
	virtual bool visit(const GXmlText& /*text*/) { return true; }
	/// Visit a comment node
	virtual bool visit(const GXmlComment& /*comment*/) { return true; }
	/// Visit an unknown node
	virtual bool visit(const GXmlUnknown& /*unknown*/) { return true; }
};

// Only used by Attribute::Query functions
enum 
{
	GXML_SUCCESS,
	GXML_NO_ATTRIBUTE,
	GXML_WRONG_TYPE
};


// Used by the parsing routines.
enum GXmlEncoding
{
	GXML_ENCODING_UNKNOWN,
	GXML_ENCODING_UTF8,
	GXML_ENCODING_LEGACY
};

const GXmlEncoding GXML_DEFAULT_ENCODING = GXML_ENCODING_UNKNOWN;

/** GXmlBase is a base class for every class in GnyXml.
	It does little except to establish that GnyXml classes
	can be printed and provide some utility functions.

	In XML, the document and elements can contain
	other elements and other types of nodes.

	@verbatim
	A Document can contain:	Element	(container or leaf)
							Comment (leaf)
							Unknown (leaf)
							Declaration( leaf )

	An Element can contain:	Element (container or leaf)
							Text	(leaf)
							Attributes (not on tree)
							Comment (leaf)
							Unknown (leaf)

	A Decleration contains: Attributes (not on tree)
	@endverbatim
*/
class GXmlBase
{
	friend class GXmlNode;
	friend class GXmlElement;
	friend class GXmlDocument;

public:
	GXmlBase() : m_userData(0) {}
	virtual ~GXmlBase()	{}

	/**	All GnyXml classes can print themselves to a filestream
		or the string class (GXmlString in non-STL mode, std::string
		in STL mode.) Either or both cfile and str can be null.
		
		This is a formatted print, and will insert 
		tabs and newlines.
		
		(For an unformatted stream, use the << operator.)
	*/
	virtual void print(FILE* cfile, int depth) const = 0;

	/**	The world does not agree on whether white space should be kept or
		not. In order to make everyone happy, these global, static functions
		are provided to set whether or not GnyXml will condense all white space
		into a single space or not. The default is to condense. Note changing this
		value is not thread safe.
	*/
	static void setCondenseWhiteSpace(bool condense) { m_condenseWhiteSpace = condense; }

	/// Return the current white space setting.
	static bool isWhiteSpaceCondensed() { return m_condenseWhiteSpace; }

	/** Return the position, in the original source file, of this node or attribute.
		The row and column are 1-based. (That is the first row and first column is
		1,1). If the returns values are 0 or less, then the parser does not have
		a row and column value.

		Generally, the row and column value will be set when the GXmlDocument::Load(),
		GXmlDocument::LoadFile(), or any GXmlNode::Parse() is called. It will NOT be set
		when the DOM was created from operator>>.

		The values reflect the initial load. Once the DOM is modified programmatically
		(by adding or changing nodes and attributes) the new values will NOT update to
		reflect changes in the document.

		There is a minor performance cost to computing the row and column. Computation
		can be disabled if GXmlDocument::SetTabSize() is called with 0 as the value.

		@sa GXmlDocument::SetTabSize()
	*/
	int row() const	{ return m_location.m_row + 1; }
	int column() const { return m_location.m_col + 1; }    ///< See Row()

	void setUserData(void* user) { m_userData = user; }	///< Set a pointer to arbitrary user data.
	void* getUserData() { return m_userData; }	///< Get a pointer to arbitrary user data.
	const void* getUserData() const { return m_userData; }	///< Get a pointer to arbitrary user data.

	// Table that returs, for a given lead byte, the total number of bytes
	// in the UTF-8 sequence.
	static const int m_utf8ByteTable[256];

	virtual const char* parse(const char* p, 
		GXmlParsingData* data, 
		GXmlEncoding encoding /*= GXML_ENCODING_UNKNOWN */) = 0;

	/** Expands entities in a string. Note this should not contian the tag's '<', '>', etc, 
		or they will be transformed into entities!
	*/
	static void encodeString(const std::string& str, std::string* out);

	enum
	{
		GXML_NO_ERROR = 0,
		GXML_ERROR,
		GXML_ERROR_OPENING_FILE,
		GXML_ERROR_PARSING_ELEMENT,
		GXML_ERROR_FAILED_TO_READ_ELEMENT_NAME,
		GXML_ERROR_READING_ELEMENT_VALUE,
		GXML_ERROR_READING_ATTRIBUTES,
		GXML_ERROR_PARSING_EMPTY,
		GXML_ERROR_READING_END_TAG,
		GXML_ERROR_PARSING_UNKNOWN,
		GXML_ERROR_PARSING_COMMENT,
		GXML_ERROR_PARSING_DECLARAGON,
		GXML_ERROR_DOCUMENT_EMPTY,
		GXML_ERROR_EMBEDDED_NULL,
		GXML_ERROR_PARSING_CDATA,
		GXML_ERROR_DOCUMENT_TOP_ONLY,
		GXML_ERROR_STRING_COUNT
	};

protected:
	static const char* skipWhiteSpace(const char*, GXmlEncoding encoding);

	inline static bool isWhiteSpace(char c)		
	{ 
		return (isspace((unsigned char)c) || c == '\n' || c == '\r'); 
	}
    
	inline static bool isWhiteSpace(int c)
	{
		if (c < 256)
        {
			return isWhiteSpace((char)c);
        }
        
		return false;	// Again, only truly correct for English/Latin...but usually works.
	}

	static bool	streamWhiteSpace(std::istream* in, std::string* tag);
	static bool streamTo(std::istream* in, int character, std::string* tag);

	/*	Reads an XML name into the string provided. Returns
		a pointer just past the last character of the name,
		or 0 if the function has an error.
	*/
	static const char* readName(const char* p, std::string* name, GXmlEncoding encoding);

	/*	Reads text. Returns a pointer past the given end tag.
		Wickedly complex options, but it keeps the (sensitive) code in one place.
	*/
	static const char* readText(const char* in,				// where to start
		std::string* text,			// the string read
		bool ignoreWhiteSpace,		// whether to keep the white space
		const char* endTag,			// what ends this text
		bool ignoreCase,			// whether to ignore case in the end tag
		GXmlEncoding encoding);	// the current encoding

	// If an entity has been found, transform it into a character.
	static const char* getEntity(const char* in, char* value, int* length, GXmlEncoding encoding);

	// Get a character, while interpreting entities.
	// The length can be from 0 to 4 bytes.
	inline static const char* getChar(const char* p, char* value, int* length, GXmlEncoding encoding)
	{
		assert(p);
		if (encoding == GXML_ENCODING_UTF8)
		{
			*length = m_utf8ByteTable[*((const unsigned char*)p)];
			assert(*length >= 0 && *length < 5);
		}
		else
		{
			*length = 1;
		}

		if (*length == 1)
		{
			if (*p == '&')
            {
				return getEntity(p, value, length, encoding);
            }
            
			*value = *p;
			return p + 1;
		}
		else if (*length)
		{
			//strncpy( _value, p, *length );	// lots of compilers don't like this function (unsafe),
												// and the null terminator isn't needed
			for (int i = 0; p[i] && i < *length; i++) 
            {
				value[i] = p[i];
			}
            
			return p + (*length);
		}
		else
		{
			// Not valid text.
			return 0;
		}
	}

	// Return true if the next characters in the stream are any of the endTag sequences.
	// Ignore case only works for english, and should only be relied on when comparing
	// to English words: StringEqual( p, "version", true ) is fine.
	static bool stringEqual(const char* p,
		const char* endTag,
		bool ignoreCase,
		GXmlEncoding encoding);

	static const char* m_errorString[GXML_ERROR_STRING_COUNT];

	GXmlCursor m_location;

    /// Field containing a generic user pointer
	void* m_userData;
	
	// None of these methods are reliable for any language except English.
	// Good for approximation, not great for accuracy.
	static int isAlpha(unsigned char anyByte, GXmlEncoding encoding);
	static int isAlphaNum(unsigned char anyByte, GXmlEncoding encoding);
	inline static int toLower(int v, GXmlEncoding encoding)
	{
		if (encoding == GXML_ENCODING_UTF8)
		{
			if (v < 128) return tolower(v);
			return v;
		}
		else
		{
			return tolower(v);
		}
	}
	static void convertUTF32ToUTF8(unsigned long input, char* output, int* length);

private:
	GXmlBase(const GXmlBase&);				// not implemented.
	void operator = (const GXmlBase& base);	// not allowed.

	struct Entity
	{
		const char*     m_str;
		unsigned int	m_strLength;
		char		    m_chr;
	};
    
	enum
	{
		NUM_ENTITY = 5,
		NUM_ENTITY_LENGTH = 6
	};
    
	static Entity m_entity[NUM_ENTITY];
	static bool m_condenseWhiteSpace;
};


/** The parent class for everything in the Document Object Model.
	(Except for attributes).
	Nodes have siblings, a parent, and children. A node can be
	in a document, or stand on its own. The type of a GXmlNode
	can be queried, and it can be cast to its more defined type.
*/
class GXmlNode : public GXmlBase
{
	friend class GXmlDocument;
	friend class GXmlElement;

public:
    /** An input stream operator, for every class. Tolerant of newlines and
	    formatting, but doesn't expect them.
    */
    friend std::istream& operator >> (std::istream& in, GXmlNode& base);

    /** An output stream operator, for every class. Note that this outputs
	    without any newlines or formatting, as opposed to Print(), which
	    includes tabs and new lines.

	    The operator<< and operator>> are not completely symmetric. Writing
	    a node to a stream is very well defined. You'll get a nice stream
	    of output, without any extra whitespace or newlines.
	    
	    But reading is not as well defined. (As it always is.) If you create
	    a GXmlElement (for example) and read that from an input stream,
	    the text needs to define an element or junk will result. This is
	    true of all input streams, but it's worth keeping in mind.

	    A GXmlDocument will read nodes until it reads a root element, and
		all the children of that root element.
    */	
    friend std::ostream& operator << (std::ostream& out, const GXmlNode& base);

	/// Appends the XML node or attribute to a std::string.
	friend std::string& operator << (std::string& out, const GXmlNode& base);

	/** The types of XML nodes supported by GnyXml. (All the
			unsupported types are picked up by UNKNOWN.)
	*/
	enum NodeType
	{
		GNYXML_DOCUMENT,
		GNYXML_ELEMENT,
		GNYXML_COMMENT,
		GNYXML_UNKNOWN,
		GNYXML_TEXT,
		GNYXML_DECLARAGON,
		GNYXML_TYPECOUNT
	};

	virtual ~GXmlNode();

	/** The meaning of 'value' changes for the specific type of
		GXmlNode.
		@verbatim
		Document:	filename of the xml file
		Element:	name of the element
		Comment:	the comment text
		Unknown:	the tag contents
		Text:		the text string
		@endverbatim

		The subclasses will wrap this function.
	*/
	const char* value() const { return m_value.c_str(); }

	/** Return Value() as a std::string. If you only use STL,
	    this is more efficient than calling Value().
		Only available in STL mode.
	*/
	const std::string& valueStr() const { return m_value; }

	const std::string& valueTStr() const { return m_value; }

	/** Changes the value of the node. Defined as:
		@verbatim
		Document:	filename of the xml file
		Element:	name of the element
		Comment:	the comment text
		Unknown:	the tag contents
		Text:		the text string
		@endverbatim
	*/
	void setValue(const char* value) { m_value = value;}

	/// STL std::string form.
	void setValue(const std::string& value) { m_value = value; }

	/// Delete all the children of this node. Does not affect 'this'.
	void clear();

	/// One step up the DOM.
	GXmlNode* parent() { return m_parent; }
	const GXmlNode* parent() const{ return m_parent; }

	const GXmlNode* firstChild() const { return m_firstChild; }	///< The first child of this node. Will be null if there are no children.
	GXmlNode* firstChild() { return m_firstChild; }
	const GXmlNode* firstChild(const char* value) const;			///< The first child of this node with the matching 'value'. Will be null if none found.
	/// The first child of this node with the matching 'value'. Will be null if none found.
	GXmlNode* firstChild(const char* value) 
	{
		// Call through to the const version - safe since nothing is changed. Exiting syntax: cast this to a const (always safe)
		// call the method, cast the return back to non-const.
		return const_cast<GXmlNode*>((const_cast<const GXmlNode*>(this))->firstChild(value));
	}
    
	const GXmlNode* lastChild() const { return m_lastChild; }		/// The last child of this node. Will be null if there are no children.
	GXmlNode* lastChild() { return m_lastChild; }
	
	const GXmlNode* lastChild(const char* value) const;			/// The last child of this node matching 'value'. Will be null if there are no children.
	GXmlNode* lastChild(const char* value) 
    {
		return const_cast<GXmlNode*>((const_cast<const GXmlNode*>(this))->lastChild(value));
	}

	const GXmlNode* FirstChild(const std::string& value) const	
    { 
    	return firstChild(value.c_str ()); 
    }    ///< STL std::string form.
    
	GXmlNode* firstChild(const std::string& value)	
    {   
    	return firstChild(value.c_str()); 
    }  ///< STL std::string form.
    
	const GXmlNode* LastChild(const std::string& value) const
    { 
    	return lastChild(value.c_str ()); 
    } ///< STL std::string form.
    
	GXmlNode* lastChild(const std::string& value)
    {   
    	return lastChild(value.c_str()); 
    }   ///< STL std::string form.

	/** An alternate way to walk the children of a node.
		One way to iterate over nodes is:
		@verbatim
			for( child = parent->FirstChild(); child; child = child->NextSibling() )
		@endverbatim

		IterateChildren does the same thing with the syntax:
		@verbatim
			child = 0;
			while( child = parent->IterateChildren( child ) )
		@endverbatim

		IterateChildren takes the previous child as input and finds
		the next one. If the previous child is null, it returns the
		first. IterateChildren will return null when done.
	*/
	const GXmlNode* iterateChildren(const GXmlNode* previous) const;
	GXmlNode* iterateChildren(const GXmlNode* previous) 
    {
		return const_cast<GXmlNode*>((const_cast<const GXmlNode*>(this))->iterateChildren(previous));
	}

	/// This flavor of IterateChildren searches for children with a particular 'value'
	const GXmlNode* iterateChildren(const char* value, const GXmlNode* previous) const;
	GXmlNode* iterateChildren(const char* value, const GXmlNode* previous) 
    {
		return const_cast<GXmlNode*>((const_cast<const GXmlNode*>(this))->iterateChildren(value, previous));
	}

	const GXmlNode* iterateChildren(const std::string& value, const GXmlNode* previous) const	
    {   
    	return iterateChildren (value.c_str (), previous); 
    }   ///< STL std::string form.
    
	GXmlNode* iterateChildren(const std::string& value, const GXmlNode* previous) 
    {   
    	return iterateChildren(value.c_str(), previous); 
    }   ///< STL std::string form.

	/** Add a new node related to this. Adds a child past the LastChild.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	GXmlNode* insertEndChild(const GXmlNode& addThis);

	/** Add a new node related to this. Adds a child past the LastChild.

		NOTE: the node to be added is passed by pointer, and will be
		henceforth owned (and deleted) by tinyXml. This method is efficient
		and avoids an extra copy, but should be used with care as it
		uses a different memory model than the other insert functions.

		@sa InsertEndChild
	*/
	GXmlNode* linkEndChild(GXmlNode* addThis);

	/** Add a new node related to this. Adds a child before the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	GXmlNode* insertBeforeChild(GXmlNode* beforeThis, const GXmlNode& addThis);

	/** Add a new node related to this. Adds a child after the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	GXmlNode* insertAfterChild(GXmlNode* afterThis, const GXmlNode& addThis);

	/** Replace a child of this node.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	GXmlNode* replaceChild(GXmlNode* replaceThis, const GXmlNode& withThis);

	/// Delete a child of this node.
	bool removeChild(GXmlNode* removeThis);

	/// Navigate to a sibling node.
	const GXmlNode* previousSibling() const	{ return m_prev; }
	GXmlNode* previousSibling()	{ return m_prev; }

	/// Navigate to a sibling node.
	const GXmlNode* previousSibling(const char* prev) const;
	GXmlNode* previousSibling(const char* prev) 
    {
		return const_cast<GXmlNode*>((const_cast<const GXmlNode*>(this))->previousSibling(prev));
	}

	const GXmlNode* previousSibling(const std::string& value) const	
    {   
    	return previousSibling(value.c_str());   
    }   ///< STL std::string form.
    
	GXmlNode* previousSibling(const std::string& value) 
    {   
    	return previousSibling(value.c_str()); 
    }   ///< STL std::string form.
    
	const GXmlNode* nextSibling(const std::string& value) const		
    {   
    	return nextSibling(value.c_str());   
    }   ///< STL std::string form.
    
	GXmlNode* nextSibling(const std::string& value) 
    {   
    	return nextSibling(value.c_str());   
    }   ///< STL std::string form.

	/// Navigate to a sibling node.
	const GXmlNode* nextSibling() const	{ return m_next; }
	GXmlNode* nextSibling()	{ return m_next; }

	/// Navigate to a sibling node with the given 'value'.
	const GXmlNode* nextSibling(const char* next) const;
	GXmlNode* nextSibling(const char* next) 
    {
		return const_cast<GXmlNode*>((const_cast<const GXmlNode*>(this))->nextSibling(next));
	}

	/** Convenience function to get through elements.
		Calls NextSibling and ToElement. Will skip all non-Element
		nodes. Returns 0 if there is not another element.
	*/
	const GXmlElement* nextSiblingElement() const;
	GXmlElement* nextSiblingElement() 
    {
		return const_cast<GXmlElement*>((const_cast<const GXmlNode*>(this))->nextSiblingElement());
	}

	/** Convenience function to get through elements.
		Calls NextSibling and ToElement. Will skip all non-Element
		nodes. Returns 0 if there is not another element.
	*/
	const GXmlElement* nextSiblingElement(const char* next) const;
	GXmlElement* nextSiblingElement(const char* next) 
    {
		return const_cast<GXmlElement*>((const_cast<const GXmlNode*>(this))->nextSiblingElement(next) );
	}

	const GXmlElement* nextSiblingElement( const std::string& value) const	
	{	
		return nextSiblingElement(value.c_str());	
    }	///< STL std::string form.
    
	GXmlElement* nextSiblingElement(const std::string& value)	
    {   
    	return nextSiblingElement(value.c_str());    
    }   ///< STL std::string form.

	/// Convenience function to get through elements.
	const GXmlElement* firstChildElement()	const;
	GXmlElement* firstChildElement() 
    {
		return const_cast<GXmlElement*>((const_cast<const GXmlNode*>(this))->firstChildElement());
	}

	/// Convenience function to get through elements.
	const GXmlElement* firstChildElement(const char* value) const;
	GXmlElement* firstChildElement(const char* value) 
    {
		return const_cast<GXmlElement*>((const_cast<const GXmlNode*>(this))->firstChildElement(value));
	}

	const GXmlElement* firstChildElement(const std::string& value) const	
    {   
    	return firstChildElement(value.c_str()); 
    }   ///< STL std::string form.
    
	GXmlElement* firstChildElement(const std::string& value)
    {   
    	return firstChildElement(value.c_str()); 
    }   ///< STL std::string form.

	/** Query the type (as an enumerated value, above) of this node.
		The possible types are: GNYXML_DOCUMENT, GNYXML_ELEMENT, GNYXML_COMMENT,
								GNYXML_UNKNOWN, GNYXML_TEXT, and GNYXML_DECLARAGON.
	*/
	int type() const { return m_type; }

	/** Return a pointer to the Document this node lives in.
		Returns null if not in a document.
	*/
	const GXmlDocument* getDocument() const;
	GXmlDocument* getDocument() 
    {
		return const_cast<GXmlDocument*>((const_cast<const GXmlNode*>(this))->getDocument());
	}

	/// Returns true if this node has no children.
	bool noChildren() const	{ return !m_firstChild; }

	virtual const GXmlDocument* toDocument() const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const GXmlElement* toElement() const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const GXmlComment* toComment() const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const GXmlUnknown* toUnknown() const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const GXmlText* toText() const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual const GXmlDeclaration* toDeclaration() const { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.

	virtual GXmlDocument* toDocument() { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual GXmlElement* toElement() { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual GXmlComment* toComment() { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual GXmlUnknown* toUnknown() { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual GXmlText* toText() { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual GXmlDeclaration* toDeclaration() { return 0; } ///< Cast to a more defined type. Will return null if not of the requested type.

	/** Create an exact duplicate of this node and return it. The memory must be deleted
		by the caller. 
	*/
	virtual GXmlNode* clone() const = 0;

	/** Accept a hierchical visit the nodes in the GnyXML DOM. Every node in the 
		XML tree will be conditionally visited and the host will be called back
		via the GXmlVisitor interface.

		This is essentially a SAX interface for GnyXML. (Note however it doesn't re-parse
		the XML for the callbacks, so the performance of GnyXML is unchanged by using this
		interface versus any other.)

		The interface has been based on ideas from:

		- http://www.saxproject.org/
		- http://c2.com/cgi/wiki?HierarchicalVisitorPattern 

		Which are both good references for "visiting".

		An example of using Accept():
		@verbatim
		GXmlPrinter printer;
		tinyxmlDoc.Accept( &printer );
		const char* xmlcstr = printer.CStr();
		@endverbatim
	*/
	virtual bool accept(GXmlVisitor* visitor) const = 0;

protected:
	GXmlNode(NodeType type);

	// Copy to the allocated object. Shared functionality between Clone, Copy constructor,
	// and the assignment operator.
	void copyTo(GXmlNode* target) const;

    // The real work of the input operator.
	virtual void streamIn(std::istream* in, std::string* tag) = 0;

	// Figure out what is at *p, and parse it. Returns null if it is not an xml node.
	GXmlNode* identify(const char* start, GXmlEncoding encoding);

	GXmlNode* 	m_parent;
	NodeType 	m_type;
	GXmlNode*	m_firstChild;
	GXmlNode*	m_lastChild;
	std::string	m_value;

	GXmlNode*	m_prev;
	GXmlNode*	m_next;

private:
	GXmlNode(const GXmlNode&);				// not implemented.
	void operator = (const GXmlNode& base);	// not allowed.
};

/** An attribute is a name-value pair. Elements have an arbitrary
	number of attributes, each with a unique name.

	@note The attributes are not GXmlNodes, since they are not
		  part of the tinyXML document object model. There are other
		  suggested ways to look at this problem.
*/
class GXmlAttribute : public GXmlBase
{
	friend class GXmlAttributeSet;

public:
	/// Construct an empty attribute.
	GXmlAttribute() : GXmlBase()
	{
		m_document = nullptr;
		m_prev = nullptr;
        m_next = nullptr;
	}

	/// std::string constructor.
	GXmlAttribute(const std::string& name, const std::string& value)
	{
		m_name = name;
		m_value = value;
		m_document = nullptr;
		m_prev = m_next = nullptr;
	}

	/// Construct an attribute with a name and value.
	GXmlAttribute(const char* name, const char* value)
	{
		m_name = name;
		m_value = value;
		m_document = nullptr;
		m_prev = nullptr;
        m_next = nullptr;
	}

	const char*	name() const { return m_name.c_str(); }		///< Return the name of this attribute.
	const char*	value() const { return m_value.c_str(); }		///< Return the value of this attribute.
	const std::string& valueStr() const	{ return m_value; }				///< Return the value of this attribute.
	int	intValue() const;									///< Return the value of this attribute, converted to an integer.
	double doubleValue() const;								///< Return the value of this attribute, converted to a double.

	// Get the tinyxml string representation
	const std::string& nameTStr() const { return m_name; }

	/** QueryIntValue examines the value string. It is an alternative to the
		IntValue() method with richer error checking.
		If the value is an integer, it is stored in 'value' and 
		the call returns GXML_SUCCESS. If it is not
		an integer, it returns GXML_WRONG_TYPE.

		A specialized but useful call. Note that for success it returns 0,
		which is the opposite of almost all other GnyXml calls.
	*/
	int queryIntValue(int* value) const;
	/// QueryDoubleValue examines the value string. See QueryIntValue().
	int queryDoubleValue(double* value) const;

	void setName(const char* name)	{ m_name = name; }				///< Set the name of this attribute.
	void setValue(const char* value) { m_value = value; }				///< Set the value.

	void setIntValue(int value);										///< Set the value from an integer.
	void setDoubleValue(double value);								///< Set the value from a double.

	/// STL std::string form.
	void setName(const std::string& name) { m_name = name; }	
	/// STL std::string form.	
	void setValue(const std::string& value) { m_value = value; }

	/// Get the next sibling attribute in the DOM. Returns null at end.
	const GXmlAttribute* next() const;
	GXmlAttribute* next() 
    {
		return const_cast<GXmlAttribute*>((const_cast<const GXmlAttribute*>(this))->next()); 
	}

	/// Get the previous sibling attribute in the DOM. Returns null at beginning.
	const GXmlAttribute* previous() const;
	GXmlAttribute* previous() 
    {
		return const_cast<GXmlAttribute*>((const_cast<const GXmlAttribute*>(this))->previous()); 
	}

	bool operator == (const GXmlAttribute& rhs) const { return rhs.m_name == m_name; }
	bool operator < (const GXmlAttribute& rhs) const { return m_name < rhs.m_name; }
	bool operator > (const GXmlAttribute& rhs) const { return m_name > rhs.m_name; }

	/*	Attribute parsing starts: first letter of the name
						 returns: the next char after the value end quote
	*/
	virtual const char* parse(const char* p, GXmlParsingData* data, GXmlEncoding encoding);

	// Prints this Attribute to a FILE stream.
	virtual void print(FILE* cfile, int depth) const 
	{
		print(cfile, depth, 0);
	}
    
	void print(FILE* cfile, int depth, std::string* str) const;

	// [internal use]
	// Set the document pointer so the attribute can report errors.
	void setDocument(GXmlDocument* doc)	{ m_document = doc; }

private:
	GXmlAttribute(const GXmlAttribute&);				// not implemented.
	void operator = (const GXmlAttribute& base);	// not allowed.

	GXmlDocument*	m_document;	// A pointer back to a document, for error reporting.
	std::string 	m_name;
	std::string 	m_value;
	GXmlAttribute*	m_prev;
	GXmlAttribute*	m_next;
};

/*	A class used to manage a group of attributes.
	It is only used internally, both by the ELEMENT and the DECLARAGON.
	
	The set can be changed transparent to the Element and Declaration
	classes that use it, but NOT transparent to the Attribute
	which has to implement a next() and previous() method. Which makes
	it a bit problematic and prevents the use of STL.

	This version is implemented with circular lists because:
		- I like circular lists
		- it demonstrates some independence from the (typical) doubly linked list.
*/
class GXmlAttributeSet
{
public:
	GXmlAttributeSet();
	~GXmlAttributeSet();

	void add(GXmlAttribute* attribute);
	void remove(GXmlAttribute* attribute);

	const GXmlAttribute* first( )const
    { 
    	return (m_sentinel.m_next == &m_sentinel) ? 0 : m_sentinel.m_next; 
    }
    
	GXmlAttribute* first()
    { 
    	return (m_sentinel.m_next == &m_sentinel) ? 0 : m_sentinel.m_next; 
    }
    
	const GXmlAttribute* last() const
    { 
    	return (m_sentinel.m_prev == &m_sentinel) ? 0 : m_sentinel.m_prev; 
    }
    
	GXmlAttribute* last()
    { 
    	return (m_sentinel.m_prev == &m_sentinel) ? 0 : m_sentinel.m_prev; 
    }

	GXmlAttribute* find(const char* name) const;
	GXmlAttribute* findOrCreate(const char* name);

	GXmlAttribute* find(const std::string& name) const;
	GXmlAttribute* findOrCreate(const std::string& name);

private:
	//*ME:	Because of hidden/disabled copy-construktor in GXmlAttribute (sentinel-element),
	//*ME:	this class must be also use a hidden/disabled copy-constructor !!!
	GXmlAttributeSet(const GXmlAttributeSet&);	// not allowed
	void operator = (const GXmlAttributeSet&);	// not allowed (as GXmlAttribute)

	GXmlAttribute m_sentinel;
};

/** The element is a container class. It has a value, the element name,
	and can contain other elements, text, comments, and unknowns.
	Elements also contain an arbitrary number of attributes.
*/
class GXmlElement : public GXmlNode
{
public:
	/// Construct an element.
	GXmlElement(const char* value);

	/// std::string constructor.
	GXmlElement(const std::string& value);

	GXmlElement(const GXmlElement& element);

	GXmlElement& operator = (const GXmlElement& base);

	virtual ~GXmlElement();

	/** Given an attribute name, Attribute() returns the value
		for the attribute of that name, or null if none exists.
	*/
	const char* attribute(const char* name) const;

	/** Given an attribute name, Attribute() returns the value
		for the attribute of that name, or null if none exists.
		If the attribute exists and can be converted to an integer,
		the integer value will be put in the return 'i', if 'i'
		is non-null.
	*/
	const char* attribute(const char* name, int* i) const;

	/** Given an attribute name, Attribute() returns the value
		for the attribute of that name, or null if none exists.
		If the attribute exists and can be converted to an double,
		the double value will be put in the return 'd', if 'd'
		is non-null.
	*/
	const char* attribute(const char* name, double* d) const;

	/** QueryIntAttribute examines the attribute - it is an alternative to the
		Attribute() method with richer error checking.
		If the attribute is an integer, it is stored in 'value' and 
		the call returns GXML_SUCCESS. If it is not
		an integer, it returns GXML_WRONG_TYPE. If the attribute
		does not exist, then GXML_NO_ATTRIBUTE is returned.
	*/	
	int queryIntAttribute(const char* name, int* value) const;
	/// QueryUnsignedAttribute examines the attribute - see QueryIntAttribute().
	int queryUnsignedAttribute(const char* name, unsigned* value) const;
	/** QueryBoolAttribute examines the attribute - see QueryIntAttribute(). 
		Note that '1', 'true', or 'yes' are considered true, while '0', 'false'
		and 'no' are considered false.
	*/
	int queryBoolAttribute(const char* name, bool* value) const;
	/// QueryDoubleAttribute examines the attribute - see QueryIntAttribute().
	int queryDoubleAttribute(const char* name, double* value) const;
	/// QueryFloatAttribute examines the attribute - see QueryIntAttribute().
	int queryFloatAttribute(const char* name, float* value) const 
	{
		double d = 0.0;
		int result = queryDoubleAttribute(name, &d);
		if (result == GXML_SUCCESS) 
        	{
			*value = (float)d;
		}
        
		return result;
	}

	/// QueryStringAttribute examines the attribute - see QueryIntAttribute().
	int queryStringAttribute(const char* name, std::string* value) const 
	{
		const char* cstr = attribute(name);
		if (cstr != nullptr) 
        {
			*value = std::string(cstr);
			return GXML_SUCCESS;
		}
        
		return GXML_NO_ATTRIBUTE;
	}

	/** Template form of the attribute query which will try to read the
		attribute into the specified type. Very easy, very powerful, but
		be careful to make sure to call this with the correct type.
		
		NOTE: This method doesn't work correctly for 'string' types that contain spaces.

		@return GXML_SUCCESS, GXML_WRONG_TYPE, or GXML_NO_ATTRIBUTE
	*/
	template<typename T> int queryValueAttribute(const std::string& name, T* outValue) const
	{
		const GXmlAttribute* node = m_attributeSet.find(name);
		if (node == nullptr)
        {
			return GXML_NO_ATTRIBUTE;
        }

		std::stringstream sstream(node->valueStr());
		sstream >> *outValue;
		if (!sstream.fail())
        {
			return GXML_SUCCESS;
        }
        
		return GXML_WRONG_TYPE;
	}

	int queryValueAttribute(const std::string& name, std::string* outValue) const
	{
		const GXmlAttribute* node = m_attributeSet.find(name);
		if (node == nullptr)
        {
			return GXML_NO_ATTRIBUTE;
        }
        
		*outValue = node->valueStr();
		return GXML_SUCCESS;
	}

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void setAttribute(const char* name, const char* value);
	const std::string* attribute(const std::string& name) const;
	const std::string* attribute(const std::string& name, int* i) const;
	const std::string* attribute(const std::string& name, double* d) const;
	int queryIntAttribute(const std::string& name, int* value) const;
	int queryDoubleAttribute(const std::string& name, double* value) const;

	/// STL std::string form.
	void setAttribute(const std::string& name, const std::string& value);
	///< STL std::string form.
	void setAttribute(const std::string& name, int value);
	///< STL std::string form.
	void setDoubleAttribute(const std::string& name, double value);

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void setAttribute(const char* name, int value);

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void setDoubleAttribute(const char* name, double value);

	/** Deletes an attribute with the given name.
	*/
	void removeAttribute(const char* name);
	void removeAttribute(const std::string& name) 
    {   
    	removeAttribute(name.c_str());    
    }   ///< STL std::string form.

	const GXmlAttribute* firstAttribute() const	
    { 
    	return m_attributeSet.first(); 
    }        ///< Access the first attribute in this element.
    
	GXmlAttribute* firstAttribute() 
    { 
    	return m_attributeSet.first(); 
    }
    
	const GXmlAttribute* lastAttribute()	const 	
    { 
    	return m_attributeSet.last(); 
    }     ///< Access the last attribute in this element.
    
	GXmlAttribute* lastAttribute()					
    { 
    	return m_attributeSet.last(); 
    }

	/** Convenience function for easy access to the text inside an element. Although easy
		and concise, GetText() is limited compared to getting the GXmlText child
		and accessing it directly.
	
		If the first child of 'this' is a GXmlText, the GetText()
		returns the character string of the Text node, else null is returned.

		This is a convenient method for getting the text of simple contained text:
		@verbatim
		<foo>This is text</foo>
		const char* str = fooElement->GetText();
		@endverbatim

		'str' will be a pointer to "This is text". 
		
		Note that this function can be misleading. If the element foo was created from
		this XML:
		@verbatim
		<foo><b>This is text</b></foo> 
		@endverbatim

		then the value of str would be null. The first child node isn't a text node, it is
		another element. From this XML:
		@verbatim
		<foo>This is <b>text</b></foo> 
		@endverbatim
		GetText() will return "This is ".

		WARNING: GetText() accesses a child node - don't become confused with the 
				 similarly named GXmlHandle::Text() and GXmlNode::ToText() which are 
				 safe type casts on the referenced node.
	*/
	const char* getText() const;

	/// Creates a new Element and returns it - the returned element is a copy.
	virtual GXmlNode* clone() const;
	// Print the Element to a FILE stream.
	virtual void print(FILE* cfile, int depth) const;

	/*	Attribtue parsing starts: next char past '<'
						 returns: next char past '>'
	*/
	virtual const char* parse(const char* p, GXmlParsingData* data, GXmlEncoding encoding);
	virtual const GXmlElement* toElement() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual GXmlElement* toElement() { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool accept(GXmlVisitor* visitor) const;

protected:
	void copyTo(GXmlElement* target) const;
	void clearThis();	// like clear, but initializes 'this' object as well

	// Used to be public [internal use]
	virtual void streamIn(std::istream* in, std::string* tag);
	/*	[internal use]
		Reads the "value" of the element -- another element, or text.
		This should terminate with the current end tag.
	*/
	const char* readValue(const char* in, GXmlParsingData* prevData, GXmlEncoding encoding);

private:
	GXmlAttributeSet m_attributeSet;
};

/**	An XML comment.
*/
class GXmlComment : public GXmlNode
{
public:
	/// Constructs an empty comment.
	GXmlComment() : GXmlNode(GXmlNode::GNYXML_COMMENT) {}
    
	/// Construct a comment from text.
	GXmlComment(const char* value) : GXmlNode(GXmlNode::GNYXML_COMMENT) 
	{
		setValue(value);
	}
    
	GXmlComment(const GXmlComment&);
	GXmlComment& operator=(const GXmlComment& base);

	virtual ~GXmlComment() {}

	/// Returns a copy of this Comment.
	virtual GXmlNode* clone() const;
	// Write this Comment to a FILE stream.
	virtual void print(FILE* cfile, int depth) const;

	/*	Attribtue parsing starts: at the ! of the !--
						 returns: next char past '>'
	*/
	virtual const char* parse(const char* p, GXmlParsingData* data, GXmlEncoding encoding);

	virtual const GXmlComment* toComment() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual GXmlComment* toComment() { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool accept(GXmlVisitor* visitor) const;

protected:
	void copyTo(GXmlComment* target) const;

	// used to be public
	virtual void streamIn(std::istream* in, std::string* tag);
};

/** XML text. A text node can have 2 ways to output the next. "normal" output 
	and CDATA. It will default to the mode it was parsed from the XML file and
	you generally want to leave it alone, but you can change the output mode with 
	SetCDATA() and query it with CDATA().
*/
class GXmlText : public GXmlNode
{
	friend class GXmlElement;
    
public:
	/** Constructor for text element. By default, it is treated as 
		normal, encoded text. If you want it be output as a CDATA text
		element, set the parameter _cdata to 'true'
	*/
	GXmlText(const char* initValue) : GXmlNode(GXmlNode::GNYXML_TEXT)
	{
		setValue(initValue);
		m_cdata = false;
	}
    
	virtual ~GXmlText() {}

	/// Constructor.
	GXmlText(const std::string& initValue) : GXmlNode(GXmlNode::GNYXML_TEXT)
	{
		setValue(initValue);
		m_cdata = false;
	}

	GXmlText(const GXmlText& copy) : GXmlNode(GXmlNode::GNYXML_TEXT)	
    { 
    	copy.copyTo(this); 
    }
    
	GXmlText& operator = (const GXmlText& base)	
    { 
    	base.copyTo(this); 
        return *this; 
    }

	// Write this text object to a FILE stream.
	virtual void print(FILE* cfile, int depth) const;

	/// Queries whether this represents text using a CDATA section.
	bool cdata() const { return m_cdata; }
	/// Turns on or off a CDATA representation of text.
	void setCdata(bool cdata) { m_cdata = cdata; }

	virtual const char* parse(const char* p, GXmlParsingData* data, GXmlEncoding encoding);

	virtual const GXmlText* toText() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual GXmlText* toText() { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool accept(GXmlVisitor* content) const;

protected :
	///  [internal use] Creates a new Element and returns it.
	virtual GXmlNode* clone() const;
	void copyTo(GXmlText* target) const;

	bool blank() const;	// returns true if all white space and new lines
	// [internal use]
	virtual void streamIn(std::istream* in, std::string* tag);

private:
	bool m_cdata;			// true if this should be input and output as a CDATA style text element
};

/** In correct XML the declaration is the first entry in the file.
	@verbatim
		<?xml version="1.0" standalone="yes"?>
	@endverbatim

	GnyXml will happily read or write files without a declaration,
	however. There are 3 possible attributes to the declaration:
	version, encoding, and standalone.

	Note: In this version of the code, the attributes are
	handled as special cases, not generic attributes, simply
	because there can only be at most 3 and they are always the same.
*/
class GXmlDeclaration : public GXmlNode
{
public:
	/// Construct an empty declaration.
	GXmlDeclaration() : GXmlNode(GXmlNode::GNYXML_DECLARAGON) {}

	/// Constructor.
	GXmlDeclaration(const std::string& version,
		const std::string& encoding,
		const std::string& standalone);

	/// Construct.
	GXmlDeclaration(const char* version,
		const char* encoding,
		const char* standalone);

	GXmlDeclaration(const GXmlDeclaration& copy);
	GXmlDeclaration& operator = (const GXmlDeclaration& copy);

	virtual ~GXmlDeclaration() {}

	/// Version. Will return an empty string if none was found.
	const char* version() const { return m_version.c_str(); }
	/// Encoding. Will return an empty string if none was found.
	const char* encoding() const { return m_encoding.c_str(); }
	/// Is this a standalone document?
	const char* standalone() const { return m_standalone.c_str(); }

	/// Creates a copy of this Declaration and returns it.
	virtual GXmlNode* clone() const;
	// Print this declaration to a FILE stream.
	virtual void print(FILE* cfile, int depth, std::string* str) const;
	virtual void print(FILE* cfile, int depth) const 
    {
		print(cfile, depth, 0);
	}

	virtual const char* parse(const char* p, GXmlParsingData* data, GXmlEncoding encoding);
	virtual const GXmlDeclaration* toDeclaration() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual GXmlDeclaration* toDeclaration() { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool accept(GXmlVisitor* visitor) const;

protected:
	void copyTo(GXmlDeclaration* target) const;
	// used to be public
	virtual void streamIn(std::istream* in, std::string* tag);

private:
	std::string m_version;
	std::string m_encoding;
	std::string m_standalone;
};

/** Any tag that tinyXml doesn't recognize is saved as an
	unknown. It is a tag of text, but should not be modified.
	It will be written back to the XML, unchanged, when the file
	is saved.

	DTD tags get thrown into GXmlUnknowns.
*/
class GXmlUnknown : public GXmlNode
{
public:
	GXmlUnknown() : GXmlNode(GXmlNode::GNYXML_UNKNOWN)	{}
	virtual ~GXmlUnknown() {}

	GXmlUnknown(const GXmlUnknown& copy) : GXmlNode(GXmlNode::GNYXML_UNKNOWN)		
    { 
    	copy.copyTo(this); 
    }
    
	GXmlUnknown& operator = (const GXmlUnknown& copy)										
    { 
    	copy.copyTo(this); 
        return *this; 
    }

	/// Creates a copy of this Unknown and returns it.
	virtual GXmlNode* clone() const;
	// Print this Unknown to a FILE stream.
	virtual void print(FILE* cfile, int depth) const;
	virtual const char* parse(const char* p, GXmlParsingData* data, GXmlEncoding encoding);
	virtual const GXmlUnknown* toUnknown() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual GXmlUnknown* toUnknown() { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool accept(GXmlVisitor* content) const;

protected:
	void copyTo(GXmlUnknown* target) const;
	virtual void streamIn(std::istream* in, std::string* tag);
};

/** Always the top level node. A document binds together all the
	XML pieces. It can be saved, loaded, and printed to the screen.
	The 'value' of a document node is the xml file name.
*/
class GXmlDocument : public GXmlNode
{
public:
	/// Create an empty document, that has no name.
	GXmlDocument();
	/// Create a document with a name. The name of the document is also the filename of the xml.
	GXmlDocument(const char* documentName);

	/// Constructor.
	GXmlDocument(const std::string& documentName);
	GXmlDocument(const GXmlDocument& copy);
	GXmlDocument& operator = (const GXmlDocument& copy);

	virtual ~GXmlDocument() {}

	/** Load a file using the current document value.
		Returns true if successful. Will delete any existing
		document data before loading.
	*/
	bool loadFile(GXmlEncoding encoding = GXML_DEFAULT_ENCODING);
	/// Save a file using the current document value. Returns true if successful.
	bool saveFile() const;
	/// Load a file using the given filename. Returns true if successful.
	bool loadFile(const char* filename, GXmlEncoding encoding = GXML_DEFAULT_ENCODING);
	/// Save a file using the given filename. Returns true if successful.
	bool saveFile(const char* filename) const;
	/** Load a file using the given FILE*. Returns true if successful. Note that this method
		doesn't stream - the entire object pointed at by the FILE*
		will be interpreted as an XML file. GnyXML doesn't stream in XML from the current
		file location. Streaming may be added in the future.
	*/
	bool loadFile(FILE* file, GXmlEncoding encoding = GXML_DEFAULT_ENCODING);
	/// Save a file using the given FILE*. Returns true if successful.
	bool saveFile(FILE* file) const;

	bool loadFile(const std::string& filename, GXmlEncoding encoding = GXML_DEFAULT_ENCODING)			///< STL std::string version.
	{
		return loadFile(filename.c_str(), encoding);
	}
    
	bool saveFile(const std::string& filename) const		///< STL std::string version.
	{
		return saveFile(filename.c_str());
	}

	/** Parse the given null terminated block of xml data. Passing in an encoding to this
		method (either GXML_ENCODING_LEGACY or GXML_ENCODING_UTF8 will force GnyXml
		to use that encoding, regardless of what GnyXml might otherwise try to detect.
	*/
	virtual const char* parse(const char* p, GXmlParsingData* data = 0, GXmlEncoding encoding = GXML_DEFAULT_ENCODING);

	/** Get the root element -- the only top level element -- of the document.
		In well formed XML, there should only be one. GnyXml is tolerant of
		multiple elements at the document level.
	*/
	const GXmlElement* rootElement() const { return firstChildElement(); }
	GXmlElement* rootElement() { return firstChildElement(); }

	/** If an error occurs, Error will be set to true. Also,
		- The ErrorId() will contain the integer identifier of the error (not generally useful)
		- The ErrorDesc() method will return the name of the error. (very useful)
		- The ErrorRow() and ErrorCol() will return the location of the error (if known)
	*/	
	bool error() const { return m_error; }

	/// Contains a textual (english) description of the error if one occurs.
	const char* errorDesc() const { return m_errorDesc.c_str(); }

	/** Generally, you probably want the error string ( ErrorDesc() ). But if you
		prefer the ErrorId, this function will fetch it.
	*/
	int errorId() const { return m_errorId; }

	/** Returns the location (if known) of the error. The first column is column 1, 
		and the first row is row 1. A value of 0 means the row and column wasn't applicable
		(memory errors, for example, have no row/column) or the parser lost the error. (An
		error in the error reporting, in that case.)

		@sa SetTabSize, Row, Column
	*/
	int errorRow() const { return m_errorLocation.m_row + 1; }
	int errorCol() const { return m_errorLocation.m_col + 1; }	///< The column where the error occured. See ErrorRow()

	/** SetTabSize() allows the error reporting functions (ErrorRow() and ErrorCol())
		to report the correct values for row and column. It does not change the output
		or input in any way.
		
		By calling this method, with a tab size
		greater than 0, the row and column of each node and attribute is stored
		when the file is loaded. Very useful for tracking the DOM back in to
		the source file.

		The tab size is required for calculating the location of nodes. If not
		set, the default of 4 is used. The tabsize is set per document. Setting
		the tabsize to 0 disables row/column tracking.

		Note that row and column tracking is not supported when using operator>>.

		The tab size needs to be enabled before the parse or load. Correct usage:
		@verbatim
		GXmlDocument doc;
		doc.SetTabSize( 8 );
		doc.Load( "myfile.xml" );
		@endverbatim

		@sa Row, Column
	*/
	void setTabSize(int tabsize) { m_tabsize = tabsize; }

	int tabSize() const	{ return m_tabsize; }

	/** If you have handled the error, it can be reset with this call. The error
		state is automatically cleared if you Parse a new XML block.
	*/
	void clearError()						
	{	
		m_error = false; 
		m_errorId = 0; 
		m_errorDesc = ""; 
		m_errorLocation.m_row = 0;
        m_errorLocation.m_col = 0; 
	}

	/** Write the document to standard out using formatted printing ("pretty print"). */
	void print() const { print(stdout, 0); }

	/* Write the document to a string using formatted printing ("pretty print"). This
		will allocate a character array (new char[]) and return it as a pointer. The
		calling code pust call delete[] on the return char* to avoid a memory leak.
	*/
	//char* PrintToMemory() const; 

	/// Print this Document to a FILE stream.
	virtual void print(FILE* cfile, int depth = 0) const;
	// [internal use]
	void setError(int err, 
		const char* errorLocation, 
		GXmlParsingData* prevData, 
		GXmlEncoding encoding);

	virtual const GXmlDocument* toDocument() const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual GXmlDocument* toDocument() { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children. 
	*/
	virtual bool accept(GXmlVisitor* content) const;

protected :
	// [internal use]
	virtual GXmlNode* clone() const;
	virtual void streamIn(std::istream* in, std::string* tag);

private:
	void copyTo(GXmlDocument* target) const;

	bool 		m_error;
	int  		m_errorId;
	std::string m_errorDesc;
	int 		m_tabsize;
	GXmlCursor 	m_errorLocation;
	bool 		m_useMicrosoftBOM;		// the UTF-8 BOM were found when read. Note this, and try to write.
};

/**
	A GXmlHandle is a class that wraps a node pointer with null checks; this is
	an incredibly useful thing. Note that GXmlHandle is not part of the GnyXml
	DOM structure. It is a separate utility class.

	Take an example:
	@verbatim
	<Document>
		<Element attributeA = "valueA">
			<Child attributeB = "value1" />
			<Child attributeB = "value2" />
		</Element>
	<Document>
	@endverbatim

	Assuming you want the value of "attributeB" in the 2nd "Child" element, it's very 
	easy to write a *lot* of code that looks like:

	@verbatim
	GXmlElement* root = document.FirstChildElement( "Document" );
	if ( root )
	{
		GXmlElement* element = root->FirstChildElement( "Element" );
		if ( element )
		{
			GXmlElement* child = element->FirstChildElement( "Child" );
			if ( child )
			{
				GXmlElement* child2 = child->NextSiblingElement( "Child" );
				if ( child2 )
				{
					// Finally do something useful.
	@endverbatim

	And that doesn't even cover "else" cases. GXmlHandle addresses the verbosity
	of such code. A GXmlHandle checks for null	pointers so it is perfectly safe 
	and correct to use:

	@verbatim
	GXmlHandle docHandle( &document );
	GXmlElement* child2 = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", 1 ).ToElement();
	if ( child2 )
	{
		// do something useful
	@endverbatim

	Which is MUCH more concise and useful.

	It is also safe to copy handles - internally they are nothing more than node pointers.
	@verbatim
	GXmlHandle handleCopy = handle;
	@endverbatim

	What they should not be used for is iteration:

	@verbatim
	int i=0; 
	while ( true )
	{
		GXmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", i ).ToElement();
		if ( !child )
			break;
		// do something
		++i;
	}
	@endverbatim

	It seems reasonable, but it is in fact two embedded while loops. The Child method is 
	a linear walk to find the element, so this code would iterate much more than it needs 
	to. Instead, prefer:

	@verbatim
	GXmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).FirstChild( "Child" ).ToElement();

	for( child; child; child=child->NextSiblingElement() )
	{
		// do something
	}
	@endverbatim
*/
class GXmlHandle
{
public:
	/// Create a handle from any node (at any depth of the tree.) This can be a null pointer.
	GXmlHandle(GXmlNode* node)	{ m_node = node; }
	/// Copy constructor
	GXmlHandle(const GXmlHandle& ref) { m_node = ref.m_node; }
	GXmlHandle operator=(const GXmlHandle& ref) 
    { 
    	if (&ref != this) 
        {
        	m_node = ref.m_node;
            return *this; 
        }
    }

	/// Return a handle to the first child node.
	GXmlHandle firstChild() const;
	/// Return a handle to the first child node with the given name.
	GXmlHandle firstChild(const char* value) const;
	/// Return a handle to the first child element.
	GXmlHandle firstChildElement() const;
	/// Return a handle to the first child element with the given name.
	GXmlHandle firstChildElement(const char* value) const;

	/** Return a handle to the "index" child with the given name. 
		The first child is 0, the second 1, etc.
	*/
	GXmlHandle child(const char* value, int index) const;
	/** Return a handle to the "index" child. 
		The first child is 0, the second 1, etc.
	*/
	GXmlHandle child(int index) const;
	/** Return a handle to the "index" child element with the given name. 
		The first child element is 0, the second 1, etc. Note that only GXmlElements
		are indexed: other types are not counted.
	*/
	GXmlHandle childElement(const char* value, int index) const;
	/** Return a handle to the "index" child element. 
		The first child element is 0, the second 1, etc. Note that only GXmlElements
		are indexed: other types are not counted.
	*/
	GXmlHandle childElement(int index) const;

	GXmlHandle firstChild(const std::string& value) const 
    { 
    	return firstChild(value.c_str()); 
    }
    
	GXmlHandle firstChildElement(const std::string& value) const 
    { 
    	return firstChildElement(value.c_str()); 
    }

	GXmlHandle child(const std::string& value, int index) const			
    { 
    	return child(value.c_str(), index); 
    }
    
	GXmlHandle childElement(const std::string& value, int index) const	
    { 
    	return childElement(value.c_str(), index); 
    }

	/** Return the handle as a GXmlNode. This may return null.
	*/
	GXmlNode* toNode() const { return m_node; } 
	/** Return the handle as a GXmlElement. This may return null.
	*/
	
	GXmlElement* toElement() const 
	{ 
		return ((m_node && m_node->toElement()) ? m_node->toElement() : 0); 
    }
    
	/**	Return the handle as a GXmlText. This may return null.
	*/
	GXmlText* toText() const			
	{ 
		return ((m_node && m_node->toText()) ? m_node->toText() : 0); 
    }
    
	/** Return the handle as a GXmlUnknown. This may return null.
	*/
	GXmlUnknown* toUnknown() const	
	{ 
		return ((m_node && m_node->toUnknown() ) ? m_node->toUnknown() : 0); 
    }

	/** @deprecated use ToNode. 
		Return the handle as a GXmlNode. This may return null.
	*/
	GXmlNode* node() const { return toNode(); } 
	/** @deprecated use ToElement. 
		Return the handle as a GXmlElement. This may return null.
	*/
	GXmlElement* element() const { return toElement(); }
	/**	@deprecated use ToText()
		Return the handle as a GXmlText. This may return null.
	*/
	GXmlText* text() const { return toText(); }
	/** @deprecated use ToUnknown()
		Return the handle as a GXmlUnknown. This may return null.
	*/
	GXmlUnknown* unknown() const { return toUnknown(); }

private:
	GXmlNode* m_node;
};

/** Print to memory functionality. The GXmlPrinter is useful when you need to:

	-# Print to memory (especially in non-STL mode)
	-# Control formatting (line endings, etc.)

	When constructed, the GXmlPrinter is in its default "pretty printing" mode.
	Before calling Accept() you can call methods to control the printing
	of the XML document. After GXmlNode::Accept() is called, the printed document can
	be accessed via the CStr(), Str(), and Size() methods.

	GXmlPrinter uses the Visitor API.
	@verbatim
	GXmlPrinter printer;
	printer.SetIndent( "\t" );

	doc.Accept( &printer );
	fprintf( stdout, "%s", printer.CStr() );
	@endverbatim
*/
class GXmlPrinter : public GXmlVisitor
{
public:
	GXmlPrinter() 
        : m_depth(0)
        , m_simpleTextPrint(false)
        , m_buffer()
        , m_indent("")
        , m_lineBreak("\n") {}

	virtual bool visitEnter(const GXmlDocument& doc);
	virtual bool visitExit(const GXmlDocument& doc);
	
	virtual bool visitEnter(const GXmlElement& element, const GXmlAttribute* firstAttribute);
	virtual bool visitExit(const GXmlElement& element);

	virtual bool visit(const GXmlDeclaration& declaration);
	virtual bool visit(const GXmlText& text);
	virtual bool visit(const GXmlComment& comment);
	virtual bool visit(const GXmlUnknown& unknown);

	/** Set the indent characters for printing. By default 4 spaces
		but tab (\t) is also useful, or null/empty string for no indentation.
	*/
	void setIndent(const char* indent)	{ m_indent = indent ? indent : ""; }
	/// Query the indention string.
	const char* indent() { return m_indent.c_str(); }
	/** Set the line breaking string. By default set to newline (\n). 
		Some operating systems prefer other characters, or can be
		set to the null/empty string for no indenation.
	*/
	void setLineBreak(const char* lineBreak) { m_lineBreak = lineBreak ? lineBreak : ""; }
	/// Query the current line breaking string.
	const char* lineBreak()	{ return m_lineBreak.c_str(); }

	/** Switch over to "stream printing" which is the most dense formatting without 
		linebreaks. Common when the XML is needed for network transmission.
	*/
	void setStreamPrinting()						
	{ 
		m_indent = "";
		m_lineBreak = "";
	}	
    
	/// Return the result.
	const char* cStr() { return m_buffer.c_str(); }
	/// Return the length of the result string.
	size_t size() { return m_buffer.size(); }

	/// Return the result.
	const std::string& str() { return m_buffer; }

private:
	void doIndent()	
    {
		for (int i = 0; i < m_depth; i++)
        {
			m_buffer += m_indent;
        }
	}
    
	void doLineBreak() 
    {
		m_buffer += m_lineBreak;
	}

	int 		m_depth;
	bool 		m_simpleTextPrint;
	std::string m_buffer;
	std::string m_indent;
	std::string m_lineBreak;
};
