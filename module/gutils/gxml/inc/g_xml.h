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
* @date		2014-07-18
* @note 
*
*  1. 2014-07-18 duye Created this file
* 
*/

#pragma once

#include <string>
#include <list>
#include <g_system.h>

#define G_ERROR_XML_INVALID_SYNTAX       (G_ERROR_UTILS_BASE - 1)
#define G_ERROR_XML_INVALID_NESTING      (G_ERROR_UTILS_BASE - 2)
#define G_ERROR_XML_TAG_MISMATCH         (G_ERROR_UTILS_BASE - 3)
#define G_ERROR_XML_NO_ROOT              (G_ERROR_UTILS_BASE - 4)
#define G_ERROR_XML_MULTIPLE_ROOTS       (G_ERROR_UTILS_BASE - 5)

class GXmlProcessor;
class GXmlAttribute;
class GXmlNode;
class GXmlElementNode;
class GXmlTextNode;

typedef std::list<GXmlAttribute*> GXmlAttributeList;
typedef std::list<GXmlNode*> GXmlNodeList;

class GXmlAttribute
{
public:
    GXmlAttribute(const GInt8* name, const GInt8* value);
    GXmlAttribute(const GInt8* prefix, 
        const GInt8* name, 
        const GInt8* value) 
        : m_prefix(prefix)
        , m_name(name)
        , m_value(value) {}
        
    const std::string& GetPrefix() const { return m_prefix; }
    const std::string& GetName() const { return m_name;   }
    const std::string& GetValue() const { return m_value;  }
    void SetValue(const GInt8* value) { m_value = value; }

private:
    std::string m_prefix;
    std::string m_name;
    std::string m_value;

    GXmlAttribute(const GXmlAttribute& attribute) 
        : m_prefix(attribute.m_prefix)
        , m_name(attribute.m_name)
        , m_value(attribute.m_value) {}
    
    GXmlAttribute& operator=(const GXmlAttribute& a);

    friend class GXmlAttributeFinder;
    friend class GXmlAttributeFinderWithPrefix;
};

class GXmlNamespaceMap
{
public:
    ~GXmlNamespaceMap();

    GResult SetNamespaceUri(const GInt8* prefix, const GInt8* uri);
    const std::string* GetNamespaceUri(const GInt8* prefix);
    const std::string* GetNamespacePrefix(const GInt8* uri);

private:
    class Entry 
    {
    public:
        Entry(const GInt8* prefix, const GInt8* uri) : 
            m_prefix(prefix), m_uri(uri) {}

        std::string m_prefix;
        std::string m_uri;
    };

    typedef std::list<Entry*> EntryList;

    EntryList   m_entries;

    friend class GXmlWriter;
    friend class GXmlNodeWriter;
    friend class GXmlNodeCanonicalWriter;
};

class GXmlNode
{
 public:
    typedef enum 
    {
        DOCUMENT,
        ELEMENT,
        TEXT
    } Type;
    
    GXmlNode(Type type) : m_type(type), m_parent(NULL) {}
    virtual ~GXmlNode() {}
    Type GetType() const { return m_type; }
    GXmlNode* GetParent() const { return m_parent; }

    virtual GXmlElementNode* AsElementNode() { return NULL; }
    virtual const GXmlElementNode* AsElementNode() const { return NULL; }
    virtual GXmlTextNode* AsTextNode() { return NULL; }
    virtual const GXmlTextNode* AsTextNode() const { return NULL; }

 protected:
    virtual void SetParent(GXmlNode* parent) { m_parent = parent; }

    Type        m_type;
    GXmlNode*   m_parent;

    friend class GXmlNodeFinder;
    friend class GXmlSerializer;
    friend class GXmlWriter;
    friend class GXmlElementNode;
};

class GXmlElementNode : public GXmlNode
{
public:
    GXmlElementNode(const GInt8* tag);
    GXmlElementNode(const GInt8* prefix, const GInt8* tag);
    virtual ~GXmlElementNode();
    GXmlNodeList& GetChildren() { return m_children; }
    const GXmlNodeList& GetChildren() const { return m_children; }
    GXmlElementNode* GetChild(const GInt8* tag, 
             const GInt8* namespc = NULL,
             GUint32 n=0) const;
             
    GResult AddChild(GXmlNode* child);
    GResult SetAttribute(const GInt8* prefix,
        const GInt8* name, 
        const GInt8* value);
                 
    GResult SetAttribute(const GInt8* name, const GInt8* value);
    GResult AddText(const GInt8* text); 
    GXmlAttributeList& GetAttributes() { return m_attributes; }
    const GXmlAttributeList& GetAttributes() const { return m_attributes; }
    const std::string* GetAttribute(const GInt8* name, const GInt8* namespc = NULL) const;
    const std::string& GetPrefix() const { return m_prefix; }
    const std::string& GetTag() const { return m_tag;    }
    const std::string* GetText(GUint32 n=0) const;

    // bring all the namespace definitions used in this element of one of its descendants
    // into the namespace map of this element so that it may be serialized as a
    // standalone element without any prefixes with undefined namespace uris
    GResult MakeStandalone();

    // namespace methods   
    const std::string* GetNamespace() const;
    GResult SetNamespaceUri(const GInt8* prefix, const GInt8* uri);
    const std::string* GetNamespaceUri(const GInt8* prefix) const;
    const std::string* GetNamespacePrefix(const GInt8* uri) const;

    // type casting
    GXmlElementNode* AsElementNode() { return this; }
    const GXmlElementNode* AsElementNode() const { return this; }

protected:
    void SetParent(GXmlNode* parent);
    void SetNamespaceParent(GXmlElementNode* parent);
    void RelinkNamespaceMaps();
    GResult AddAttribute(const GInt8* name, const GInt8* value);

    std::string                 m_prefix;
    std::string                 m_tag;
    GXmlNodeList                m_children;
    GXmlAttributeList           m_attributes;
    GXmlNamespaceMap*           m_namespaceMap;
    GXmlElementNode*            m_namespaceParent;

    friend class GXmlTagFinder;
    friend class GXmlSerializer;
    friend class GXmlWriter;
    friend class GXmlNodeWriter;
    friend class GXmlNodeCanonicalWriter;
    friend class GXmlParser;
    friend class GXmlProcessor;
    friend class GXmlNamespaceCollapser;
};

class GXmlTextNode : public GXmlNode
{
 public:
    typedef enum {
        CHARACTER_DATA,
        IGNORABLE_WHITESPACE,
        CDATA_SECTION,
        ENTITY_REFERENCE,
        COMMENT
    } TokenType;

    GXmlTextNode(TokenType token_type, const GInt8* text);
    const std::string& GetString() const { return m_text; }
    GXmlTextNode* AsTextNode() { return this; }
    const GXmlTextNode* AsTextNode() const { return this; }

 private:
    TokenType       m_tokenType;
    std::string     m_text;
};

class GXmlParser
{
public:
    GXmlParser(bool keep_whitespace = true);
    virtual ~GXmlParser();
    virtual GResult Parse(const GInt8* xml, 
        GXmlNode*& tree,
        bool incremental = false);
        
    virtual  GResult Parse(const GInt8* xml, 
        GUint32 size,
        GXmlNode*& tree,
        bool incremental = false);
        
    virtual GResult Parse(const std::string& stream, 
        GXmlNode*& tree,
        bool incremental = false);
        
    virtual  GResult Parse(const std::string& stream, 
        GUint32& size,
        GXmlNode*& tree,
        bool incremental = false);
        
protected:
    GResult OnStartElement(const GInt8* name);
    GResult OnElementAttribute(const GInt8* name, const GInt8* value);
    GResult OnEndElement(const GInt8* name);
    GResult OnCharacterData(const GInt8* data, unsigned long size);
    void RemoveIgnorableWhitespace();

    GXmlProcessor*      m_processor;
    GXmlElementNode*    m_root;
    GXmlElementNode*    m_currentElement;
    bool                m_keepWhitespace;

private:
    void Reset();

    friend class GXmlProcessor;
};

class GXmlSerializer
{
public:
    GXmlSerializer(std::string output,
        GUint32 indentation = 0,
        bool shrink_empty_elements = true,
        bool add_xml_decl = false);
        
    virtual ~GXmlSerializer();
    virtual GResult StartDocument();
    virtual GResult EndDocument();
    virtual GResult StartElement(const GInt8* prefix, const GInt8* name);
    virtual GResult EndElement(const GInt8* prefix, const GInt8* name);
    virtual GResult Attribute(const GInt8* prefix, const GInt8* name, const GInt8* value);
    virtual GResult Text(const GInt8* text);
    virtual GResult CdataSection(const GInt8* data);
    virtual GResult Comment(const GInt8* comment);

protected:
    void EscapeChar(unsigned char c, char* text);
    GResult ProcessPending();
    GResult OutputEscapedString(const GInt8* text, bool attribute);
    void OutputIndentation(bool start);

    std::string         m_output;
    bool                m_elementPending;
    GUint32             m_depth;
    GUint32             m_indentation;
    std::string         m_indentationPrefix;
    bool                m_elementHasText;
    bool                m_shrinkEmptyElements;
    bool                m_addXmlDecl;
};

class GXmlWriter
{
public:
    explicit GXmlWriter(GUint32 indentation = 0) 
        : m_indentation(indentation) {}
        
    GResult Serialize(GXmlNode& node, 
        std::string& stream, 
        bool add_xml_decl = false);

private:
    GUint32 m_indentation;
};

class GXmlCanonicalizer
{
public:
    GResult Serialize(GXmlNode& node, 
        std::string& stream, 
        bool add_xml_decl = false);
};
