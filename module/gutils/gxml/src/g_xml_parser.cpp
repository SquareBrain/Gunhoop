/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_xml_parser.cpp
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
#include <stddef.h>
#include <g_xml.h>

namespace gutils {

// Note tha "PutString" hardcodes the same list. This
// is less flexible than it appears. Changing the entries
// or order will break putstring.
XmlBase::Entity XmlBase::m_entity[XmlBase::NUM_ENTITY] = 
{
	{"&amp;", 5, '&'},
	{"&lt;", 4, '<'},
	{"&gt;", 4, '>'},
	{"&quot;", 6, '\"'},
	{"&apos;", 6, '\''}
};

bool XmlBase::m_condenseWhiteSpace = false;

// Bunch of unicode info at:
//		http://www.unicode.org/faq/utf_bom.html
// Including the basic of this table, which determines the #bytes in the
// sequence from the lead byte. 1 placed for invalid sequences --
// although the result will be junk, pass it through as much as possible.
// Beware of the non-characters in UTF-8:	
//				ef bb bf (Microsoft "lead bytes")
//				ef bf be
//				ef bf bf 

const unsigned char GXML_UTF_LEAD_0 = 0xefU;
const unsigned char GXML_UTF_LEAD_1 = 0xbbU;
const unsigned char GXML_UTF_LEAD_2 = 0xbfU;
const int XmlBase::m_utf8ByteTable[256] = 
{
	//	0	1	2	3	4	5	6	7	8	9	a	b	c	d	e	f
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x00
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x10
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x20
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x30
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x40
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x50
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x60
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x70	End of ASCII range
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x80 0x80 to 0xc1 invalid
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x90 
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0xa0 
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0xb0 
	1,	1,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	// 0xc0 0xc2 to 0xdf 2 byte
	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	// 0xd0
	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	// 0xe0 0xe0 to 0xef 3 byte
	4,	4,	4,	4,	4,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1	// 0xf0 0xf0 to 0xf4 4 byte, 0xf5 and higher invalid
};

void XmlBase::convertUTF32ToUTF8(unsigned long input, char* output, int* length)
{
	const unsigned long BYTE_MASK = 0xBF;
	const unsigned long BYTE_MARK = 0x80;
	const unsigned long FIRST_BYTE_MARK[7] = {0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};

	if (input < 0x80)
	{
		*length = 1;
	}
	else if (input < 0x800)
	{
		*length = 2;
	}
	else if (input < 0x10000)
	{
		*length = 3;
	}
	else if (input < 0x200000)
	{
		*length = 4;
	}
	else
	{ 
		*length = 0; 
		return;
	}	// This code won't covert this correctly anyway.

	output += *length;

	// Scary scary fall throughs.
	switch (*length) 
	{
	case 4:
		--output; 
		*output = (char)((input | BYTE_MARK) & BYTE_MASK); 
		input >>= 6;
	case 3:
		--output; 
		*output = (char)((input | BYTE_MARK) & BYTE_MASK); 
		input >>= 6;
	case 2:
		--output; 
		*output = (char)((input | BYTE_MARK) & BYTE_MASK); 
		input >>= 6;
	case 1:
		--output; 
		*output = (char)(input | FIRST_BYTE_MARK[*length]);
	default:
		break;
	}
}

int XmlBase::isAlpha(unsigned char any_byte, XmlEncoding /*encoding*/)
{
	// This will only work for low-ascii, everything else is assumed to be a valid
	// letter. I'm not sure this is the best approach, but it is quite tricky trying
	// to figure out alhabetical vs. not across encoding. So take a very 
	// conservative approach.

//	if (encoding == GXML_ENCODING_UTF8)
//	{
		if (any_byte < 127)
		{
			return isalpha(any_byte);
		}
		else
		{
			return 1;	// What else to do? The unicode set is huge...get the english ones right.
		}
//	}
//	else
//	{
//		return isalpha(any_byte);
//	}
}


int XmlBase::isAlphaNum(unsigned char any_byte, XmlEncoding /*encoding*/)
{
	// This will only work for low-ascii, everything else is assumed to be a valid
	// letter. I'm not sure this is the best approach, but it is quite tricky trying
	// to figure out alhabetical vs. not across encoding. So take a very 
	// conservative approach.

//	if (encoding == GXML_ENCODING_UTF8)
//	{
		if (any_byte < 127)
		{
			return isalnum(any_byte);
		}
		else
		{
			return 1;	// What else to do? The unicode set is huge...get the english ones right.
		}
//	}
//	else
//	{
//		return isalnum(any_byte);
//	}
}

class XmlParsingData
{
	friend class XmlDocument;
	
public:
	void stamp(const char* now, XmlEncoding encoding);
    
	const XmlCursor& cursor() const	
	{ 
		return m_cursor; 
	}

private:
	// Only used by the document!
	XmlParsingData(const char* start, int tabsize, int row, int col)
	{
		assert(start);
		m_stamp = start;
		m_tabsize = tabsize;
		m_cursor.m_row = row;
		m_cursor.m_col = col;
	}

	XmlCursor		m_cursor;
	const char*		m_stamp;
	int				m_tabsize;
};

void XmlParsingData::stamp(const char* now, XmlEncoding encoding)
{
	assert(now);

	// Do nothing if the tabsize is 0.
	if (m_tabsize < 1)
	{
		return;
	}

	// Get the current row, column.
	int row = m_cursor.m_row;
	int col = m_cursor.m_col;
	const char* p = m_stamp;
	assert(p);

	while (p < now)
	{
		// Treat p as unsigned, so we have a happy compiler.
		const unsigned char* pU = (const unsigned char*)p;

		// Code contributed by Fletcher Dunn: (modified by lee)
		switch (*pU) 
		{
		case 0:
			// We *should* never get here, but in case we do, don't
			// advance past the terminating null character, ever
			return;
		case '\r':
			// bump down to the next line
			++row;
			col = 0;				
			// Eat the character
			++p;

			// Check for \r\n sequence, and treat this as a single character
			if (*p == '\n') 
			{
				++p;
			}
			break;
		case '\n':
			// bump down to the next line
			++row;
			col = 0;
	
			// Eat the character
			++p;

			// Check for \n\r sequence, and treat this as a single
			// character.  (Yes, this bizarre thing does occur still
			// on some arcane platforms...)
			if (*p == '\r') 
			{
				++p;
			}
			break;
		case '\t':
			// Eat the character
			++p;

			// Skip to next tab stop
			col = (col / m_tabsize + 1) * m_tabsize;
			break;
		case GXML_UTF_LEAD_0:
			if (encoding == GXML_ENCODING_UTF8)
			{
				if (*(p + 1) && *(p + 2))
				{
					// In these cases, don't advance the column. These are
					// 0-width spaces.
					if (*(pU + 1) == GXML_UTF_LEAD_1 && *(pU + 2) == GXML_UTF_LEAD_2)
					{
						p += 3;	
					}
					else if (*(pU + 1) == 0xbfU && *(pU + 2) == 0xbeU)
					{
						p += 3;
					}
					else if (*(pU + 1) == 0xbfU && *(pU + 2) == 0xbfU)
					{
						p += 3;	
					}
					else
					{ 
						p += 3; 
						++col;
					}	// A normal character.
				}
			}
			else
			{
				++p;
				++col;
			}
			break;
		default:
			if (encoding == GXML_ENCODING_UTF8)
			{
				// Eat the 1 to 4 byte utf8 character.
				int step = XmlBase::m_utf8ByteTable[*((const unsigned char*)p)];
				if (step == 0)
				{
					step = 1;		// Error case from bad encoding, but handle gracefully.
				}
				p += step;

				// Just advance one column, of course.
				++col;
			}
			else
			{
				++p;
				++col;
			}
			break;
		}
	}
	
	m_cursor.m_row = row;
	m_cursor.m_col = col;
	assert(m_cursor.m_row >= -1);
	assert(m_cursor.m_col >= -1);
	m_stamp = p;
	assert(m_stamp);
}

const char* XmlBase::skipWhiteSpace(const char* p, XmlEncoding encoding)
{
	if (p == nullptr || !*p)
	{
		return 0;
	}
	
	if (encoding == GXML_ENCODING_UTF8)
	{
		while (*p)
		{
			const unsigned char* pU = (const unsigned char*)p;
			
			// Skip the stupid Microsoft UTF-8 Byte order marks
			if (*(pU + 0) == GXML_UTF_LEAD_0
				 && *(pU + 1) == GXML_UTF_LEAD_1 
				 && *(pU + 2) == GXML_UTF_LEAD_2)
			{
				p += 3;
				continue;
			}
			else if(*(pU + 0) == GXML_UTF_LEAD_0
				 && *(pU + 1) == 0xbfU
				 && *(pU + 2) == 0xbeU)
			{
				p += 3;
				continue;
			}
			else if(*(pU + 0) == GXML_UTF_LEAD_0
				 && *(pU + 1) == 0xbfU
				 && *(pU + 2) == 0xbfU)
			{
				p += 3;
				continue;
			}

			if (isWhiteSpace(*p))		// Still using old rules for white space.
			{
				++p;
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		while (*p && isWhiteSpace(*p))
		{
			++p;
		}
	}

	return p;
}

bool XmlBase::streamWhiteSpace(std::istream* in, std::string* tag)
{
	for (;;)
	{
		if (!in->good()) 
		{
			return false;
		}

		int c = in->peek();
		// At this scope, we can't get to a document. So fail silently.
		if (!isWhiteSpace(c) || c <= 0)
		{
			return true;
		}

		*tag += (char)in->get();
	}
}

bool XmlBase::streamTo(std::istream* in, int character, std::string* tag)
{
	//assert( character > 0 && character < 128 );	// else it won't work in utf-8
	while (in->good())
	{
		int c = in->peek();
		if (c == character)
		{
			return true;
		}
		
		if (c <= 0)		// Silent failure: can't get document at this scope
		{
			return false;
		}

		in->get();
		*tag += (char)c;
	}
	
	return false;
}

// One of GnyXML's more performance demanding functions. Try to keep the memory overhead down. The
// "assign" optimization removes over 10% of the execution time.
//
const char* XmlBase::readName(const char* p, std::string* name, XmlEncoding encoding)
{
	// Oddly, not supported on some comilers,
	//name->clear();
	// So use this:
	*name = "";
	assert(p);

	// Names start with letters or underscores.
	// Of course, in unicode, tinyxml has no idea what a letter *is*. The
	// algorithm is generous.
	//
	// After that, they can be letters, underscores, numbers,
	// hyphens, or colons. (Colons are valid ony for namespaces,
	// but tinyxml can't tell namespaces from names.)
	if (p && *p 
		&& (isAlpha((unsigned char)*p, encoding) 
		|| *p == '_'))
	{
		const char* start = p;
		while (p && *p
			&& (isAlphaNum((unsigned char)*p, encoding) 
				|| *p == '_'
				|| *p == '-'
				|| *p == '.'
				|| *p == ':'))
		{
			//(*name) += *p; // expensive
			++p;
		}
		
		if (p - start > 0) 
		{
			name->assign(start, p - start);
		}
		
		return p;
	}
	
	return 0;
}

const char* XmlBase::getEntity(const char* p, char* value, int* length, XmlEncoding encoding)
{
	// Presume an entity, and pull it out.
    std::string ent;
	*length = 0;

	if (*(p + 1) && *(p + 1) == '#' && *(p + 2))
	{
		unsigned long ucs = 0;
		ptrdiff_t delta = 0;
		unsigned mult = 1;

		if (*(p + 2) == 'x')
		{
			// Hexadecimal.
			if (!*(p + 3))
			{
				return 0;
			}

			const char* q = p + 3;
			q = strchr(q, ';');

			if (!q || !*q) 
			{
				return 0;
			}

			delta = q - p;
			--q;

			while (*q != 'x')
			{
				if (*q >= '0' && *q <= '9')
				{
					ucs += mult * (*q - '0');
				}
				else if (*q >= 'a' && *q <= 'f')
				{
					ucs += mult * (*q - 'a' + 10);
				}
				else if (*q >= 'A' && *q <= 'F')
				{
					ucs += mult * (*q - 'A' + 10);
				}
				else 
				{
					return 0;
				}
				
				mult *= 16;
				--q;
			}
		}
		else
		{
			// Decimal.
			if (!*(p + 2)) 
			{
				return 0;
			}

			const char* q = p + 2;
			q = strchr(q, ';');

			if (!q || !*q)
			{	
				return 0;
			}

			delta = q - p;
			--q;

			while (*q != '#')
			{
				if (*q >= '0' && *q <= '9')
				{
					ucs += mult * (*q - '0');
				}
				else 
				{
					return 0;
				}
				
				mult *= 10;
				--q;
			}
		}
		
		if (encoding == GXML_ENCODING_UTF8)
		{
			// convert the UCS to UTF-8
			convertUTF32ToUTF8(ucs, value, length);
		}
		else
		{
			*value = (char)ucs;
			*length = 1;
		}
		
		return p + delta + 1;
	}

	// Now try to match it.
	for (int i = 0; i < NUM_ENTITY; i++)
	{
		if (strncmp(m_entity[i].m_str, p, m_entity[i].m_strLength) == 0)
		{
			assert(strlen(m_entity[i].m_str ) == m_entity[i].m_strLength);
			*value = m_entity[i].m_chr;
			*length = 1;
			return (p + m_entity[i].m_strLength);
		}
	}

	// So it wasn't an entity, its unrecognized, or something like that.
	*value = *p;	// Don't put back the last one, since we return it!
	//*length = 1;	// Leave unrecognized entities - this doesn't really work.
					// Just writes strange XML.
	return p + 1;
}

bool XmlBase::stringEqual(const char* p,
	const char* tag,
	bool ignoreCase,
	XmlEncoding encoding)
{
	assert(p);
	assert(tag);
	
	if (!p || !*p)
	{
		assert(0);
		return false;
	}

	const char* q = p;

	if (ignoreCase)
	{
		while (*q && *tag && toLower(*q, encoding) == toLower(*tag, encoding))
		{
			++q;
			++tag;
		}

		if (*tag == 0)
		{
			return true;
		}
	}
	else
	{
		while (*q && *tag && *q == *tag)
		{
			++q;
			++tag;
		}

		if (*tag == 0)		// Have we found the end of the tag, and everything equal?
		{
			return true;
		}
	}
	
	return false;
}

const char* XmlBase::readText(const char* p, 
	std::string * text, 
	bool trimWhiteSpace, 
	const char* endTag, 
	bool caseInsensitive,
	XmlEncoding encoding)
{
    *text = "";
	if (!trimWhiteSpace			// certain tags always keep whitespace
		|| !m_condenseWhiteSpace)	// if true, whitespace is always kept
	{
		// Keep all the white space.
		while (p && *p
			&& !stringEqual(p, endTag, caseInsensitive, encoding))
		{
			int len;
			char cArr[4] = {0, 0, 0, 0};
			p = getChar(p, cArr, &len, encoding);
			text->append(cArr, len);
		}
	}
	else
	{
		bool whitespace = false;

		// Remove leading white space:
		p = skipWhiteSpace(p, encoding);
		while (p && *p
			&& !stringEqual(p, endTag, caseInsensitive, encoding))
		{
			if (*p == '\r' || *p == '\n')
			{
				whitespace = true;
				++p;
			}
			else if (isWhiteSpace(*p))
			{
				whitespace = true;
				++p;
			}
			else
			{
				// If we've found whitespace, add it before the
				// new character. Any whitespace just becomes a space.
				if (whitespace)
				{
					(*text) += ' ';
					whitespace = false;
				}
				int len;
				char cArr[4] = {0, 0, 0, 0};
				p = getChar(p, cArr, &len, encoding);
				if (len == 1)
				{
					(*text) += cArr[0];	// more efficient
				}
				else
				{
					text->append(cArr, len);
				}
			}
		}
	}
	
	if (p && *p)
	{
		p += strlen(endTag);
	}
	
	return (p && *p) ? p : 0;
}

void XmlDocument::streamIn(std::istream* in, std::string* tag)
{
	// The basic issue with a document is that we don't know what we're
	// streaming. Read something presumed to be a tag (and hope), then
	// identify it, and call the appropriate stream method on the tag.
	//
	// This "pre-streaming" will never read the closing ">" so the
	// sub-tag can orient itself.

	if (!streamTo(in, '<', tag )) 
	{
		setError(GXML_ERROR_PARSING_EMPTY, 0, 0, GXML_ENCODING_UNKNOWN);
		return;
	}

	while (in->good())
	{
		int tagIndex = (int)tag->length();
		while (in->good() && in->peek() != '>')
		{
			int c = in->get();
			if (c <= 0)
			{
				setError(GXML_ERROR_EMBEDDED_NULL, 0, 0, GXML_ENCODING_UNKNOWN);
				break;
			}
			(*tag) += (char) c;
		}

		if (in->good())
		{
			// We now have something we presume to be a node of 
			// some sort. Identify it, and call the node to
			// continue streaming.
			XmlNode* node = identify(tag->c_str() + tagIndex, GXML_DEFAULT_ENCODING);

			if (node)
			{
				node->streamIn(in, tag);
				bool isElement = node->toElement() != 0;
				delete node;
				node = 0;

				// If this is the root element, we're done. Parsing will be
				// done by the >> operator.
				if (isElement)
				{
					return;
				}
			}
			else
			{
				setError(GXML_ERROR, 0, 0, GXML_ENCODING_UNKNOWN);
				return;
			}
		}
	}
	
	// We should have returned sooner.
	setError(GXML_ERROR, 0, 0, GXML_ENCODING_UNKNOWN);
}

const char* XmlDocument::parse(const char* p, XmlParsingData* prevData, XmlEncoding encoding)
{
	clearError();

	// Parse away, at the document level. Since a document
	// contains nothing but other tags, most of what happens
	// here is skipping white space.
	if (!p || !*p)
	{
		setError(GXML_ERROR_DOCUMENT_EMPTY, 0, 0, GXML_ENCODING_UNKNOWN);
		return 0;
	}

	// Note that, for a document, this needs to come
	// before the while space skip, so that parsing
	// starts from the pointer we are given.
	m_location.clear();
	if (prevData)
	{
		m_location.m_row = prevData->m_cursor.m_row;
		m_location.m_col = prevData->m_cursor.m_col;
	}
	else
	{
		m_location.m_row = 0;
		m_location.m_col = 0;
	}
	
	XmlParsingData data(p, tabSize(), m_location.m_row, m_location.m_col);
	m_location = data.cursor();

	if (encoding == GXML_ENCODING_UNKNOWN)
	{
		// Check for the Microsoft UTF-8 lead bytes.
		const unsigned char* pU = (const unsigned char*)p;
		if (*(pU + 0) && *(pU + 0) == GXML_UTF_LEAD_0
			 && *(pU + 1) && *(pU + 1) == GXML_UTF_LEAD_1
			 && *(pU + 2) && *(pU + 2) == GXML_UTF_LEAD_2)
		{
			encoding = GXML_ENCODING_UTF8;
			m_useMicrosoftBOM = true;
		}
	}

    p = skipWhiteSpace(p, encoding);
	if (!p)
	{
		setError(GXML_ERROR_DOCUMENT_EMPTY, 0, 0, GXML_ENCODING_UNKNOWN);
		return 0;
	}

	while (p && *p)
	{
		XmlNode* node = identify(p, encoding);
		if (node)
		{
			p = node->parse(p, &data, encoding);
			linkEndChild(node);
		}
		else
		{
			break;
		}

		// Did we get encoding info?
		if (encoding == GXML_ENCODING_UNKNOWN
			&& node->toDeclaration())
		{
			XmlDeclaration* dec = node->toDeclaration();
			const char* enc = dec->encoding();
			assert(enc);

			if (*enc == 0)
			{
				encoding = GXML_ENCODING_UTF8;
			}
			else if (stringEqual( enc, "UTF-8", true, GXML_ENCODING_UNKNOWN))
			{
				encoding = GXML_ENCODING_UTF8;
			}
			else if (stringEqual(enc, "UTF8", true, GXML_ENCODING_UNKNOWN))
			{
				encoding = GXML_ENCODING_UTF8;	// incorrect, but be nice
			}
			else 
			{
				encoding = GXML_ENCODING_LEGACY;
			}
		}

		p = skipWhiteSpace(p, encoding);
	}

	// Was this empty?
	if (!m_firstChild) 
	{
		setError(GXML_ERROR_DOCUMENT_EMPTY, 0, 0, encoding);
		return 0;
	}

	// All is well.
	return p;
}

void XmlDocument::setError(int err, const char* pError, XmlParsingData* data, XmlEncoding encoding)
{	
	// The first error in a chain is more accurate - don't set again!
	if (m_error)
	{
		return;
	}

	assert(err > 0 && err < GXML_ERROR_STRING_COUNT);
	m_error = true;
	m_errorId = err;
	m_errorDesc = m_errorString[m_errorId];

	m_errorLocation.clear();
	if (pError && data)
	{
		data->stamp(pError, encoding);
		m_errorLocation = data->cursor();
	}
}

XmlNode* XmlNode::identify(const char* p, XmlEncoding encoding)
{
	XmlNode* returnNode = 0;

	p = skipWhiteSpace(p, encoding);
	if (!p || !*p || *p != '<')
	{
		return 0;
	}

	p = skipWhiteSpace(p, encoding);

	if (!p || !*p)
	{
		return 0;
	}

	// What is this thing? 
	// - Elements start with a letter or underscore, but xml is reserved.
	// - Comments: <!--
	// - Decleration: <?xml
	// - Everthing else is unknown to tinyxml.
	//

	const char* xmlHeader = {"<?xml"};
	const char* commentHeader = {"<!--"};
	const char* dtdHeader = {"<!"};
	const char* cdataHeader = {"<![CDATA["};

	if (stringEqual(p, xmlHeader, true, encoding))
	{
		returnNode = new XmlDeclaration();
	}
	else if (stringEqual(p, commentHeader, false, encoding))
	{
		returnNode = new XmlComment();
	}
	else if (stringEqual(p, cdataHeader, false, encoding))
	{
		XmlText* text = new XmlText("");
		text->setCdata(true);
		returnNode = text;
	}
	else if (stringEqual(p, dtdHeader, false, encoding))
	{
		returnNode = new XmlUnknown();
	}
	else if (isAlpha(*(p + 1), encoding)
		|| *(p + 1) == '_')
	{
		returnNode = new XmlElement("");
	}
	else
	{
		returnNode = new XmlUnknown();
	}

	if (returnNode)
	{
		// Set the parent, so it can report errors
		returnNode->m_parent = this;
	}
	
	return returnNode;
}

void XmlElement::streamIn(std::istream* in, std::string* tag)
{
	// We're called with some amount of pre-parsing. That is, some of "this"
	// element is in "tag". Go ahead and stream to the closing ">"
	while (in->good())
	{
		int c = in->get();
		if (c <= 0)
		{
			XmlDocument* document = getDocument();
			if (document)
			{
				document->setError(GXML_ERROR_EMBEDDED_NULL, 0, 0, GXML_ENCODING_UNKNOWN);
			}
			
			return;
		}
		
		(*tag) += (char)c;
		
		if (c == '>')
		{
			break;
		}
	}

	if (tag->length() < 3) 
	{
		return;
	}

	// Okay...if we are a "/>" tag, then we're done. We've read a complete tag.
	// If not, identify and stream.

	if (tag->at( tag->length() - 1 ) == '>' 
		&& tag->at( tag->length() - 2 ) == '/')
	{
		// All good!
		return;
	}
	else if (tag->at(tag->length() - 1) == '>')
	{
		// There is more. Could be:
		//		text
		//		cdata text (which looks like another node)
		//		closing tag
		//		another node.
		for (;;)
		{
			streamWhiteSpace(in, tag);

			// Do we have text?
			if (in->good() && in->peek() != '<') 
			{
				// Yep, text.
				XmlText text("");
				text.streamIn(in, tag);

				// What follows text is a closing tag or another node.
				// Go around again and figure it out.
				continue;
			}

			// We now have either a closing tag...or another node.
			// We should be at a "<", regardless.
			if (!in->good()) 
			{
				return;
			}
			
			assert(in->peek() == '<');
			int tagIndex = (int) tag->length();

			bool closingTag = false;
			bool firstCharFound = false;

			for(;;)
			{
				if (!in->good())
				{
					return;
				}

				int c = in->peek();
				if (c <= 0)
				{
					XmlDocument* document = getDocument();
					if (document)
					{
						document->setError(GXML_ERROR_EMBEDDED_NULL, 0, 0, GXML_ENCODING_UNKNOWN);
					}
					
					return;
				}
				
				if (c == '>')
				{
					break;
				}

				*tag += (char)c;
				in->get();

				// Early out if we find the CDATA id.
				if (c == '[' && tag->size() >= 9)
				{
					size_t len = tag->size();
					const char* start = tag->c_str() + len - 9;
					if (strcmp( start, "<![CDATA[") == 0) 
					{
						assert(!closingTag);
						break;
					}
				}

				if (!firstCharFound && c != '<' && !isWhiteSpace(c))
				{
					firstCharFound = true;
					if (c == '/')
					{
						closingTag = true;
					}
				}
			}
			// If it was a closing tag, then read in the closing '>' to clean up the input stream.
			// If it was not, the streaming will be done by the tag.
			if (closingTag)
			{
				if (!in->good())
				{
					return;
				}

				int c = in->get();
				if (c <= 0)
				{
					XmlDocument* document = getDocument();
					if (document)
					{
						document->setError(GXML_ERROR_EMBEDDED_NULL, 0, 0, GXML_ENCODING_UNKNOWN);
					}
					
					return;
				}
				
				assert(c == '>');
				*tag += (char)c;

				// We are done, once we've found our closing tag.
				return;
			}
			else
			{
				// If not a closing tag, id it, and stream.
				const char* tagloc = tag->c_str() + tagIndex;
				XmlNode* node = identify(tagloc, GXML_DEFAULT_ENCODING);
				if (!node)
				{
					return;
				}
				
				node->streamIn(in, tag);
				delete node;
				node = 0;

				// No return: go around from the beginning: text, closing tag, or node.
			}
		}
	}
}

const char* XmlElement::parse(const char* p, XmlParsingData* data, XmlEncoding encoding)
{
	p = skipWhiteSpace(p, encoding);
	XmlDocument* document = getDocument();

	if (!p || !*p)
	{
		if (document) 
		{
			document->setError(GXML_ERROR_PARSING_ELEMENT, 0, 0, encoding);
		}
		
		return 0;
	}

	if (data)
	{
		data->stamp(p, encoding);
		m_location = data->cursor();
	}

	if (*p != '<')
	{
		if (document) 
		{
			document->setError(GXML_ERROR_PARSING_ELEMENT, p, data, encoding);
		}
		
		return 0;
	}

	p = skipWhiteSpace(p + 1, encoding);

	// Read the name.
	const char* pErr = p;

    p = readName(p, &m_value, encoding);
	if (!p || !*p)
	{
		if (document)	
		{
			document->setError(GXML_ERROR_FAILED_TO_READ_ELEMENT_NAME, pErr, data, encoding);
		}
		return 0;
	}

    std::string endTag ("</");
	endTag += m_value;

	// Check for and read attributes. Also look for an empty
	// tag or an end tag.
	while (p && *p)
	{
		pErr = p;
		p = skipWhiteSpace(p, encoding);
		if (!p || !*p)
		{
			if (document) 
			{
				document->setError(GXML_ERROR_READING_ATTRIBUTES, pErr, data, encoding);
			}
			
			return 0;
		}
		
		if (*p == '/')
		{
			++p;
			// Empty tag.
			if (*p  != '>')
			{
				if (document) 
				{
					document->setError(GXML_ERROR_PARSING_EMPTY, p, data, encoding);		
				}
				return 0;
			}
			
			return (p + 1);
		}
		else if (*p == '>')
		{
			// Done with attributes (if there were any.)
			// Read the value -- which can include other
			// elements -- read the end tag, and return.
			++p;
			p = readValue(p, data, encoding);		// Note this is an Element method, and will set the error if one happens.
			
			if (!p || !*p) 
			{
				// We were looking for the end tag, but found nothing.
				// Fix for [ 1663758 ] Failure to report error on bad XML
				if (document) 
				{
					document->setError(GXML_ERROR_READING_END_TAG, p, data, encoding);
				}
				
				return 0;
			}

			// We should find the end tag now
			// note that:
			// </foo > and
			// </foo> 
			// are both valid end tags.
			if (stringEqual(p, endTag.c_str(), false, encoding))
			{
				p += endTag.length();
				p = skipWhiteSpace(p, encoding);
				if (p && *p && *p == '>') 
				{
					++p;
					return p;
				}
				
				if (document) 
				{
					document->setError(GXML_ERROR_READING_END_TAG, p, data, encoding);
				}
				
				return 0;
			}
			else
			{
				if (document) 
				{
					document->setError(GXML_ERROR_READING_END_TAG, p, data, encoding);
				}
				
				return 0;
			}
		}
		else
		{
			// Try to read an attribute:
			XmlAttribute* attrib = new XmlAttribute();
			if (!attrib)
			{
				return 0;
			}

			attrib->setDocument(document);
			pErr = p;
			p = attrib->parse(p, data, encoding);

			if (!p || !*p)
			{
				if (document) 
				{
					document->setError(GXML_ERROR_PARSING_ELEMENT, pErr, data, encoding);
				}
				
				delete attrib;
				
				return 0;
			}

			// Handle the strange case of double attributes:
			XmlAttribute* node = m_attributeSet.find(attrib->nameTStr());
			if (node)
			{
				if (document)
				{
					document->setError(GXML_ERROR_PARSING_ELEMENT, pErr, data, encoding);
				}
				
				delete attrib;
				return 0;
			}

			m_attributeSet.add(attrib);
		}
	}
	
	return p;
}

const char* XmlElement::readValue(const char* p, XmlParsingData* data, XmlEncoding encoding)
{
	XmlDocument* document = getDocument();

	// Read in text and elements in any order.
	const char* pWithWhiteSpace = p;
	p = skipWhiteSpace(p, encoding);

	while (p && *p)
	{
		if (*p != '<')
		{
			// Take what we have, make a text element.
			XmlText* textNode = new XmlText("");

			if (!textNode)
			{
			    return 0;
			}

			if (XmlBase::isWhiteSpaceCondensed())
			{
				p = textNode->parse(p, data, encoding);
			}
			else
			{
				// Special case: we want to keep the white space
				// so that leading spaces aren't removed.
				p = textNode->parse(pWithWhiteSpace, data, encoding);
			}

			if (!textNode->blank())
			{
				linkEndChild(textNode);
			}
			else
			{
				delete textNode;
			}
		} 
		else 
		{
			// We hit a '<'
			// Have we hit a new element or an end tag? This could also be
			// a XmlText in the "CDATA" style.
			if (stringEqual(p, "</", false, encoding))
			{
				return p;
			}
			else
			{
				XmlNode* node = identify(p, encoding);
				if (node)
				{
					p = node->parse(p, data, encoding);
					linkEndChild(node);
				}				
				else
				{
					return 0;
				}
			}
		}
		
		pWithWhiteSpace = p;
		p = skipWhiteSpace(p, encoding);
	}

	if (!p)
	{
		if (document) 
		{
			document->setError(GXML_ERROR_READING_ELEMENT_VALUE, 0, 0, encoding);
		}
	}	
	
	return p;
}

void XmlUnknown::streamIn(std::istream* in, std::string* tag)
{
	while (in->good())
	{
		int c = in->get();	
		if (c <= 0)
		{
			XmlDocument* document = getDocument();
			if (document)
			{
				document->setError(GXML_ERROR_EMBEDDED_NULL, 0, 0, GXML_ENCODING_UNKNOWN);
			}
			
			return;
		}
		
		(*tag) += (char)c;

		if (c == '>')
		{
			// All is well.
			return;		
		}
	}
}

const char* XmlUnknown::parse(const char* p, XmlParsingData* data, XmlEncoding encoding)
{
	XmlDocument* document = getDocument();
	p = skipWhiteSpace(p, encoding);

	if (data)
	{
		data->stamp(p, encoding);
		m_location = data->cursor();
	}
	
	if (!p || !*p || *p != '<')
	{
		if (document) 
		{
			document->setError(GXML_ERROR_PARSING_UNKNOWN, p, data, encoding);
		}
		
		return 0;
	}
	
	++p;
    m_value = "";

	while (p && *p && *p != '>')
	{
		m_value += *p;
		++p;
	}

	if (!p)
	{
		if (document)	
		{
			document->setError(GXML_ERROR_PARSING_UNKNOWN, 0, 0, encoding);
		}
	}
	
	if (p && *p == '>')
	{
		return p + 1;
	}
	
	return p;
}

void XmlComment::streamIn(std::istream* in, std::string* tag)
{
	while (in->good())
	{
		int c = in->get();	
		if (c <= 0)
		{
			XmlDocument* document = getDocument();
			if (document)
			{
				document->setError(GXML_ERROR_EMBEDDED_NULL, 0, 0, GXML_ENCODING_UNKNOWN);
			}
			
			return;
		}

		(*tag) += (char)c;

		if (c == '>' 
			&& tag->at(tag->length() - 2 ) == '-'
			&& tag->at(tag->length() - 3 ) == '-')
		{
			// All is well.
			return;		
		}
	}
}

const char* XmlComment::parse(const char* p, XmlParsingData* data, XmlEncoding encoding)
{
	XmlDocument* document = getDocument();
	m_value = "";

	p = skipWhiteSpace(p, encoding);

	if (data)
	{
		data->stamp(p, encoding);
		m_location = data->cursor();
	}
	
	const char* startTag = "<!--";
	const char* endTag = "-->";

	if (!stringEqual(p, startTag, false, encoding))
	{
		if (document)
		{
			document->setError(GXML_ERROR_PARSING_COMMENT, p, data, encoding);
		}
		
		return 0;
	}
	
	p += strlen(startTag);

	// [ 1475201 ] GnyXML parses entities in comments
	// Oops - ReadText doesn't work, because we don't want to parse the entities.
	// p = ReadText( p, &value, false, endTag, false, encoding );
	//
	// from the XML spec:
	/*
	 [Definition: Comments may appear anywhere in a document outside other markup; in addition, 
	              they may appear within the document type declaration at places allowed by the grammar. 
				  They are not part of the document's character data; an XML processor MAY, but need not, 
				  make it possible for an application to retrieve the text of comments. For compatibility, 
				  the string "--" (double-hyphen) MUST NOT occur within comments.] Parameter entity 
				  references MUST NOT be recognized within comments.

				  An example of a comment:

				  <!-- declarations for <head> & <body> -->
	*/

    m_value = "";
	// Keep all the white space.
	while (p && *p && !stringEqual(p, endTag, false, encoding))
	{
		m_value.append(p, 1);
		++p;
	}
	
	if (p && *p) 
	{
		p += strlen(endTag);
	}

	return p;
}

const char* XmlAttribute::parse(const char* p, XmlParsingData* data, XmlEncoding encoding)
{
	p = skipWhiteSpace(p, encoding);
	if (!p || !*p)
	{
		return 0;
	}

	if (data)
	{
		data->stamp(p, encoding);
		m_location = data->cursor();
	}
	
	// Read the name, the '=' and the value.
	const char* pErr = p;
	p = readName(p, &m_name, encoding);
	if (!p || !*p)
	{
		if (m_document) 
		{
			m_document->setError(GXML_ERROR_READING_ATTRIBUTES, pErr, data, encoding);
		}
		
		return 0;
	}
	
	p = skipWhiteSpace(p, encoding);
	if (!p || !*p || *p != '=')
	{
		if (m_document) 
		{
			m_document->setError(GXML_ERROR_READING_ATTRIBUTES, p, data, encoding);
		}
		
		return 0;
	}

	++p;	// skip '='
	p = skipWhiteSpace(p, encoding);
	if (!p || !*p)
	{
		if (m_document) 
		{
			m_document->setError(GXML_ERROR_READING_ATTRIBUTES, p, data, encoding);
		}
		
		return 0;
	}
	
	const char* end;
	const char SINGLE_QUOTE = '\'';
	const char DOUBLE_QUOTE = '\"';

	if (*p == SINGLE_QUOTE)
	{
		++p;
		end = "\'";		// single quote in string
		p = readText(p, &m_value, false, end, false, encoding);
	}
	else if (*p == DOUBLE_QUOTE)
	{
		++p;
		end = "\"";		// double quote in string
		p = readText(p, &m_value, false, end, false, encoding);
	}
	else
	{
		// All attribute values should be in single or double quotes.
		// But this is such a common error that the parser will try
		// its best, even without them.
		m_value = "";
		while (p && *p											// existence
			&& !isWhiteSpace(*p)								// whitespace
			&& *p != '/' && *p != '>')							// tag end
		{
			if (*p == SINGLE_QUOTE || *p == DOUBLE_QUOTE) 
			{
				// [ 1451649 ] Attribute values with trailing quotes not handled correctly
				// We did not have an opening quote but seem to have a 
				// closing one. Give up and throw an error.
				if (m_document) 
				{
					m_document->setError(GXML_ERROR_READING_ATTRIBUTES, p, data, encoding);
				}
				
				return 0;
			}
			
			m_value += *p;
			++p;
		}
	}
	
	return p;
}

void XmlText::streamIn(std::istream* in, std::string* tag)
{
	while (in->good())
	{
		int c = in->peek();	
		if (!m_cdata && (c == '<')) 
		{
			return;
		}
		
		if (c <= 0)
		{
			XmlDocument* document = getDocument();
			if (document)
			{
				document->setError(GXML_ERROR_EMBEDDED_NULL, 0, 0, GXML_ENCODING_UNKNOWN);
			}
			
			return;
		}

		(*tag) += (char)c;
		in->get();	// "commits" the peek made above

		if (m_cdata && c == '>' && tag->size() >= 3) 
		{
			size_t len = tag->size();
			if ((*tag)[len-2] == ']' && (*tag)[len-3] == ']') 
			{
				// terminator of cdata.
				return;
			}
		}    
	}
}

const char* XmlText::parse(const char* p, XmlParsingData* data, XmlEncoding encoding)
{
	m_value = "";
	XmlDocument* document = getDocument();

	if (data)
	{
		data->stamp(p, encoding);
		m_location = data->cursor();
	}

	const char* const startTag = "<![CDATA[";
	const char* const endTag = "]]>";

	if (m_cdata || stringEqual(p, startTag, false, encoding))
	{
		m_cdata = true;

		if (!stringEqual(p, startTag, false, encoding))
		{
			if (document)
			{
				document->setError(GXML_ERROR_PARSING_CDATA, p, data, encoding);
			}
			
			return 0;
		}
		
		p += strlen(startTag);

		// Keep all the white space, ignore the encoding, etc.
		while (p && *p
			&& !stringEqual(p, endTag, false, encoding))
		{
			m_value += *p;
			++p;
		}

		std::string dummy; 
		p = readText(p, &dummy, false, endTag, false, encoding);
		
		return p;
	}
	else
	{
		bool ignoreWhite = true;
		const char* end = "<";
		p = readText(p, &m_value, ignoreWhite, end, false, encoding);
		if (p && *p)
		{
			return p-1;	// don't truncate the '<'
		}
		
		return 0;
	}
}

void XmlDeclaration::streamIn(std::istream* in, std::string* tag)
{
	while (in->good())
	{
		int c = in->get();
		if (c <= 0)
		{
			XmlDocument* document = getDocument();
			if (document)
			{
				document->setError(GXML_ERROR_EMBEDDED_NULL, 0, 0, GXML_ENCODING_UNKNOWN);
			}
			
			return;
		}
		
		(*tag) += (char)c;

		if (c == '>')
		{
			// All is well.
			return;
		}
	}
}

const char* XmlDeclaration::parse(const char* p, XmlParsingData* data, XmlEncoding encoding)
{
	p = skipWhiteSpace(p, encoding);
	// Find the beginning, find the end, and look for
	// the stuff in-between.
	XmlDocument* document = getDocument();
	if (!p || !*p || !stringEqual( p, "<?xml", true, encoding))
	{
		if (document) 
		{
			document->setError(GXML_ERROR_PARSING_DECLARAGON, 0, 0, encoding);
		}
		
		return 0;
	}
	
	if (data)
	{
		data->stamp(p, encoding);
		m_location = data->cursor();
	}
	
	p += 5;

	m_version = "";
	m_encoding = "";
	m_standalone = "";

	while (p && *p)
	{
		if (*p == '>')
		{
			++p;
			return p;
		}

		p = skipWhiteSpace(p, encoding);
		if (stringEqual(p, "version", true, encoding))
		{
			XmlAttribute attrib;
			p = attrib.parse(p, data, encoding);		
			m_version = attrib.value();
		}
		else if (stringEqual(p, "encoding", true, encoding))
		{
			XmlAttribute attrib;
			p = attrib.parse(p, data, encoding);		
			m_encoding = attrib.value();
		}
		else if (stringEqual(p, "standalone", true, encoding))
		{
			XmlAttribute attrib;
			p = attrib.parse(p, data, encoding);		
			m_standalone = attrib.value();
		}
		else
		{
			// Read over whatever it is.
			while (p && *p && *p != '>' && !isWhiteSpace(*p))
			{
				++p;
			}
		}
	}
	
	return 0;
}

bool XmlText::blank() const
{
	for (unsigned int i = 0; i < m_value.length(); i++)
	{
		if (!isWhiteSpace(m_value[i]))
		{
			return false;
		}
	}
	
	return true;
}
}
