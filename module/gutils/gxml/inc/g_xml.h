/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_xml.h
* @version     
* @brief      
* @author   duye
* @date     2013-11-15
* @note 
*
*  2. 2014-06-24 duye Migration to Gohoop project
*  1. 2013-11-15 duye Created this file
* 
*/

#pragma once

#include <g_system.h>
#include <string>

const int NPT_ERROR_XML_INVALID_NESTING = NPT_ERROR_BASE_XML - 0;
const int NPT_ERROR_XML_TAG_MISMATCH    = NPT_ERROR_BASE_XML - 1;
const int NPT_ERROR_XML_NO_ROOT         = NPT_ERROR_BASE_XML - 2;
const int NPT_ERROR_XML_MULTIPLE_ROOTS  = NPT_ERROR_BASE_XML - 3;

#define NPT_XML_ANY_NAMESPACE "*"
#define NPT_XML_NO_NAMESPACE  NULL

class NPT_XmlProcessor;

class GXmlAttribute
{
 public:
    GXmlAttribute(const std::string& name, const std::string& value);
    ~GXmlAttribute();
    const std::string& getName() const;
    void setValue(const std::string& value);
    const std::string& getValue() const;
    
 private:
    std::string     m_name;
    std::string     m_value;
};

typedef std::map<std::string name, GXmlAttribute*> GXmlAttributeMap;

class GXmlNode
{
 public:
    typedef enum {
        DOCUMENT,
        ELEMENT,
        TEXT
    } GNodeType;

    explicit GXmlNode(GNodeType type);
    virtual ~GXmlNode();
    
    GNodeType getType();
    GXmlNode* getParent();
    void setParent(GXmlNode* parent);
    
private:
    GNodeType       m_type;
    GXmlNode*       m_parent;
};

typedef std::list<GXmlNode*> GXmlNodeList;

class GXmlElementNode : public GXmlNode
{
public:
    GXmlElementNode(const std::string& tag);
    virtual GXmlElementNode();
    
    GXmlNodeList& getChildren();
    const GXmlNodeList& getChildren() const;
    GXmlElementNode* getChild(std::string& tag) const;
    
    GResult addChild(GXmlNode* child);
    GResult setAttribute(const std::string& name, const std::string& value);
    GResult AddText(const std::string& text); 
    NPT_List<NPT_XmlAttribute*>& 
                            GetAttributes() { return m_Attributes; }
    const NPT_List<NPT_XmlAttribute*>&
                            GetAttributes() const { return m_Attributes; }
    const NPT_String*       GetAttribute(const char* name,
                                         const char* namespc = NPT_XML_NO_NAMESPACE) const;
    const NPT_String&       GetPrefix() const { return m_Prefix; }
    const NPT_String&       GetTag()    const { return m_Tag;    }
    const NPT_String*       GetText(NPT_Ordinal n=0) const;

    // bring all the namespace definitions used in this element of one of its descendants
    // into the namespace map of this element so that it may be serialized as a
    // standalone element without any prefixes with undefined namespace uris
    NPT_Result              MakeStandalone();

    // namespace methods   
    const NPT_String* GetNamespace() const;
    NPT_Result        SetNamespaceUri(const char* prefix, const char* uri);
    const NPT_String* GetNamespaceUri(const char* prefix) const;
    const NPT_String* GetNamespacePrefix(const char* uri) const;

    // type casting
    NPT_XmlElementNode*       AsElementNode()       { return this; }
    const NPT_XmlElementNode* AsElementNode() const { return this; }

protected:
    // methods
    void SetParent(NPT_XmlNode* parent);
    void SetNamespaceParent(NPT_XmlElementNode* parent);
    void RelinkNamespaceMaps();

    NPT_Result AddAttribute(const char* name, const char* value);

    // members  
    NPT_String                  m_Prefix;
    NPT_String                  m_Tag;
    NPT_List<NPT_XmlNode*>      m_Children;
    NPT_List<NPT_XmlAttribute*> m_Attributes;
    NPT_XmlNamespaceMap*        m_NamespaceMap;
    NPT_XmlElementNode*         m_NamespaceParent;

    // friends
    friend class NPT_XmlTagFinder;
    friend class NPT_XmlSerializer;
    friend class NPT_XmlWriter;
    friend class NPT_XmlNodeWriter;
    friend class NPT_XmlNodeCanonicalWriter;
    friend class NPT_XmlParser;
    friend class NPT_XmlProcessor;
    friend class NPT_XmlNamespaceCollapser;
};

class GXmlTextNode : public NPT_XmlNode
{
 public:
    typedef enum {
        CHARACTER_DATA,
        IGNORABLE_WHITESPACE,
        CDATA_SECTION,
        ENTITY_REFERENCE,
        COMMENT
    } GTokenType;

    GXmlTextNode(GTokenType tokenType, const std::string& text);

    const std::string& getString();

 private:
    GTokenType        m_TokenType;
    std::string&      m_Text;
};

class GXmlDoc
{
 public:
    GXmlDoc();
    explicit GXmlDoc(const GInt8* xmlDocPath);
    ~GXmlDoc();
    
    GResult Load(const GInt8* xmlDocPath);
    
    GResult OnStartElement(const char* name);
    GResult OnElementAttribute(const char* name, const char* value);
    GResult OnEndElement(const char* name);
    GResult OnCharacterData(const char* data, unsigned long size);
    void RemoveIgnorableWhitespace();

private:
    GXmlProcessor*      m_Processor;
    GXmlElementNode*    m_Root;
    GXmlElementNode*    m_CurrentElement;
    bool                m_KeepWhitespace;
};

class GXmlSerializer
{
public:
   GXmlSerializer();
    virtual ~GXmlSerializer();
    virtual NPT_Result StartDocument();
    virtual NPT_Result EndDocument();
    virtual NPT_Result StartElement(const char* prefix, const char* name);
    virtual NPT_Result EndElement(const char* prefix, const char* name);
    virtual NPT_Result Attribute(const char* prefix, const char* name, const char* value);
    virtual NPT_Result Text(const char* text);
    virtual NPT_Result CdataSection(const char* data);
    virtual NPT_Result Comment(const char* comment);

protected:
    // methods
    void       EscapeChar(unsigned char c, char* text);
    NPT_Result ProcessPending();
    NPT_Result OutputEscapedString(const char* text, bool attribute);
    void       OutputIndentation(bool start);

    // members
    NPT_OutputStream* m_Output;
    bool              m_ElementPending;
    NPT_Cardinal      m_Depth;
    NPT_Cardinal      m_Indentation;
    NPT_String        m_IndentationPrefix;
    bool              m_ElementHasText;
    bool              m_ShrinkEmptyElements;
    bool              m_AddXmlDecl;
};

/*----------------------------------------------------------------------
|   NPT_XmlWriter
+---------------------------------------------------------------------*/
class NPT_XmlWriter
{
public:
    // constructor
    explicit NPT_XmlWriter(NPT_Cardinal indentation = 0) : m_Indentation(indentation) {}

    // methods
    NPT_Result Serialize(NPT_XmlNode&      node, 
                         NPT_OutputStream& stream, 
                         bool              add_xml_decl = false);

private:
    // members
    NPT_Cardinal m_Indentation;
};

/*----------------------------------------------------------------------
|   NPT_XmlCanonicalizer
+---------------------------------------------------------------------*/
class NPT_XmlCanonicalizer
{
public:
    // methods
    NPT_Result Serialize(NPT_XmlNode&      node, 
                         NPT_OutputStream& stream, 
                         bool              add_xml_decl = false);
};
