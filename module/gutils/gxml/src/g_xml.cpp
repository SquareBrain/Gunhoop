/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_logger.h
* @version     
* @brief      
* @author   duye
* @date     2014-07-18
* @note 
*
*  1. 2014-07-18 duye Created this file
* 
*/
#include <map>
#include <Gxml.h>

//#define G_XML_PARSER_DEBUG
#ifdef G_XML_PARSER_DEBUG
#define G_XML_DEBUG_0(s) G_Debug(s)
#define G_XML_DEBUG_1(s,x0) G_Debug(s,x0)
#define G_XML_DEBUG_2(s,x0,x1) G_Debug(s,x0,x1)
#define G_XML_DEBUG_3(s,x0,x1,x2) G_Debug(s,x0,x1,x2)
#define G_XML_DEBUG_4(s,x0,x1,x2,x3) G_Debug(s,x0,x1,x2,x3)
#else
#define G_XML_DEBUG_0(s)
#define G_XML_DEBUG_1(s,x0)
#define G_XML_DEBUG_2(s,x0,x1)
#define G_XML_DEBUG_3(s,x0,x1,x2)
#define G_XML_DEBUG_4(s,x0,x1,x2,x3)
#endif

static const std::string 
GXmlNamespaceUri_Xml("http://www.w3.org/XML/1998/namespace");

class GXmlAttributeFinder
{
public:
    // if 'namespc' is NULL, we're looking for ANY namespace
    // if 'namespc' is '\0', we're looking for NO namespace
    // if 'namespc' is non-empty, look for that SPECIFIC namespace
    GXmlAttributeFinder(const GXmlElementNode& element, 
        const GInt8* name, 
        const GInt8* namespc) 
        : m_element(element)
        , m_name(name)
        , m_namespace(namespc) {}

    bool operator()(const GXmlAttribute* const & attribute) const 
    {
        if (attribute->m_name == m_name) 
        {
            if (m_namespace) 
            {
                const std::string& prefix = attribute->GetPrefix();
                if (m_namespace[0] == '\0') 
                {
                    // match if the attribute has NO namespace
                    return prefix.empty();
                } 
                else 
                {
                    // match if the attribute has the SPECIFIC namespace
                    // we're looking for
                    if (prefix.empty()) 
                    {
                        // attributes without a prefix don't have a namespace
                        return false;
                    } 
                    else 
                    {
                        const std::string* namespc = m_element.GetNamespaceUri(prefix);
                        return namespc && *namespc == m_namespace;
                    }
                }
            } 
            else 
            {
                // ANY namespace will match
                return true;
            }
        } 
        else 
        {
            return false;
        }
    }

private:
    const GXmlElementNode&  m_element;
    const GInt8*            m_name;
    const GInt8*            m_namespace;
};

class GXmlAttributeFinderWithPrefix
{
public:
    GXmlAttributeFinderWithPrefix(const GInt8* prefix, const GInt8* name) : 
      m_prefix(prefix?prefix:""), m_name(name) {}

    bool operator()(const GXmlAttribute* const & attribute) const 
    {
        return attribute->m_prefix == m_prefix && attribute->m_name == m_name;
    }

private:
    const GInt8* m_prefix;
    const GInt8* m_name;
};

class GXmlTagFinder
{
public:
    // if 'namespc' is NULL, we're looking for ANY namespace
    // if 'namespc' is '\0', we're looking for NO namespace
    // if 'namespc' is non-empty, look for that SPECIFIC namespace
    GXmlTagFinder(const GInt8* tag, const GInt8* namespc) 
        : m_tag(tag), m_namespace(namespc) {}

    bool operator()(const GXmlNode* const & node) const 
    {
        const GXmlElementNode* element = node->AsElementNode();
        if (element && element->m_tag == m_tag) 
        {
            if (m_namespace) 
            {
                // look for a SPECIFIC namespace or NO namespace
                const std::string* namespc = element->GetNamespace();
                if (namespc) 
                {
                    // the element has a namespace, match if it is equal to
                    // what we're looking for
                    return *namespc == m_namespace;
                } 
                else 
                {
                    // the element does not have a namespace, match if we're
                    // looking for NO namespace
                    return m_namespace[0] == '\0';
                }
            } 
            else 
            {
                // ANY namespace will match
                return true;
            }
        } 
        else 
        {
            return false;
        }
    }

private:
    const GInt8* m_tag;
    const GInt8* m_namespace;
};

class GXmlTextFinder
{
public:
    bool operator()(const GXmlNode* const& node) const 
    {
        return node->AsTextNode() != NULL;
    }
};

class GXmlNamespaceCollapser
{
public:
    GXmlNamespaceCollapser(GXmlElementNode* element) : m_Root(element) {}

    void operator()(GXmlNode*& node) const 
    {
        GXmlElementNode* element = node->AsElementNode();
        if (element == NULL) 
        {
            return;
        }

        // collapse the namespace for this element
        CollapseNamespace(element, element->GetPrefix());

        // collapse the namespaces for the attributes
        std::list<GXmlAttribute*>::Iterator item = element->GetAttributes().GetFirstItem();
        while (item) 
        {
            GXmlAttribute* attribute = *item;
            CollapseNamespace(element, attribute->GetPrefix());
            ++item;
        }

        // recurse to the children
        element->GetChildren().Apply(*this);
    }

private:
    // methods
    void CollapseNamespace(GXmlElementNode* element, const std::string& prefix) const;

    // members
    GXmlElementNode* m_Root;
};

void GXmlNamespaceCollapser::CollapseNamespace(GXmlElementNode* element, 
    const std::string& prefix) const
{
    if (m_Root->m_namespaceMap == NULL ||
        (m_Root->m_namespaceMap->GetNamespaceUri(prefix) == NULL && prefix != "xml")) 
    {
        // the root element does not have that prefix in the map
        const std::string* uri = element->GetNamespaceUri(prefix);
        if (uri) 
        {
            m_Root->SetNamespaceUri(prefix, uri->GetGInt8*s());
        }
    }
}

GXmlAttribute::GXmlAttribute(const GInt8* name, const GInt8* value) : m_value(value)
{
    const GInt8* cursor = name;
    while (GInt8* c = *cursor++) 
    {
        if (c == ':') 
        {
            unsigned int prefix_length = (unsigned int)(cursor - name) - 1;
            m_prefix.Assign(name, prefix_length);
            name = cursor;
            break;
        }
    }
    m_name = name;
}

GXmlElementNode::GXmlElementNode(const GInt8* prefix, const GInt8* tag) 
    : GXmlNode(ELEMENT)
    , m_prefix(prefix)
    , m_tag(tag)
    , m_namespaceMap(NULL)
    , m_namespaceParent(NULL)
{
}

GXmlElementNode::GXmlElementNode(const GInt8* tag) 
    : GXmlNode(ELEMENT)
    , m_namespaceMap(NULL)
    , m_namespaceParent(NULL)
{
    const GInt8* cursor = tag;
    while (GInt8* c = *cursor++) 
    {
        if (c == ':') 
        {
            unsigned int prefix_length = (unsigned int)(cursor - tag) - 1;
            m_prefix.Assign(tag, prefix_length);
            tag = cursor;
            break;
        }
    }
    
    m_tag = tag;
}

GXmlElementNode::~GXmlElementNode()
{
    m_children.Apply(GObjectDeleter<GXmlNode>());
    m_attributes.Apply(GObjectDeleter<GXmlAttribute>());
    delete m_namespaceMap;
}

void GXmlElementNode::SetParent(GXmlNode* parent)
{
    // update our parent
    m_Parent = parent;

    // update out namespace linkage
    GXmlElementNode* parent_element =
        parent?parent->AsElementNode():NULL;
    GXmlElementNode* namespace_parent;
    if (parent_element) 
    {
        namespace_parent = 
            parent_element->m_namespaceMap ? 
            parent_element:
            parent_element->m_namespaceParent;
    } 
    else 
    {
        namespace_parent = NULL;
    }
    
    if (namespace_parent != m_namespaceParent) 
    {
        m_namespaceParent = namespace_parent;
        RelinkNamespaceMaps();
    }
}

GResult GXmlElementNode::AddChild(GXmlNode* child)
{
    if (child == NULL) 
    {
        return G_ERROR_INVALID_PARAMETERS;
    }
    
    child->SetParent(this);
    return m_children.Add(child);
}

GXmlElementNode* GXmlElementNode::GetChild(const GInt8* tag, const GInt8* namespc, GUint32 n) const
{
    // remap the requested namespace to match the semantics of the finder
    // and allow for "" to also mean NO namespace
    if (namespc == NULL || namespc[0] == '\0') 
    {
        namespc = ""; // for the finder, empty string means NO namespace
    } 
    else if (namespc[0] == '*' && namespc[1] == '\0') 
    {
        namespc = NULL; // for the finder, NULL means ANY namespace
    }

    // find the child
    std::list<GXmlNode*>::Iterator item;
    item = m_children.Find(GXmlTagFinder(tag, namespc), n);
    return item ? (*item)->AsElementNode() : NULL;
}

GResult GXmlElementNode::AddAttribute(const GInt8* name, const GInt8* value)
{
    if (name == NULL || value == NULL) 
    {
        return G_ERROR_INVALID_PARAMETERS;
    }
    
    return m_attributes.Add(new GXmlAttribute(name, value));
}

GResult GXmlElementNode::SetAttribute(const GInt8* prefix,
    const GInt8* name, 
    const GInt8* value)
{
    if (name == NULL || value == NULL) 
    {
        return G_ERROR_INVALID_PARAMETERS;
    }

    /* see if this attribute is already set */
    std::list<GXmlAttribute*>::Iterator attribute;
    attribute = m_attributes.Find(GXmlAttributeFinderWithPrefix(prefix, name));
    if (attribute) 
    {
        // an attribute with this name and prefix already exists, 
        // GInt8*nge its value
        (*attribute)->SetValue(value); 
        return G_YES;
    }
    return m_attributes.Add(new GXmlAttribute(prefix, name, value));
}

GResult GXmlElementNode::SetAttribute(const GInt8* name, const GInt8* value)
{
    return SetAttribute(NULL, name, value);
}

const std::string* GXmlElementNode::GetAttribute(const GInt8* name, const GInt8* namespc) const
{
    // remap the requested namespace to match the semantics of the finder
    // and allow for "" to also mean NO namespace
    if (namespc == NULL || namespc[0] == '\0') 
    {
        namespc = ""; // for the finder, empty string means NO namespace
    } 
    else if (namespc[0] == '*' && namespc[1] == '\0')
    {
        namespc = NULL; // for the finder, NULL means ANY namespace
    }

    // find the attribute
    std::list<GXmlAttribute*>::Iterator attribute;
    attribute = m_attributes.Find(GXmlAttributeFinder(*this, name, namespc));
    if (attribute) 
    { 
        return &(*attribute)->GetValue();
    } 
    else 
    {
        return NULL;
    }
}

GResult GXmlElementNode::AddText(const GInt8* text)
{
    return AddChild(new GXmlTextNode(GXmlTextNode::GInt8*ACTER_DATA, text));
}

const std::string* GXmlElementNode::GetText(GUint32 n) const
{
    std::list<GXmlNode*>::Iterator node;
    node = m_children.Find(GXmlTextFinder(), n);
    return node ? &(*node)->AsTextNode()->GetString() : NULL;
}

GResult GXmlElementNode::MakeStandalone()
{
    GXmlNamespaceCollapser collapser(this);
    GXmlNode* node_pointer = this;
    collapser(node_pointer);

    return G_YES;
}

void GXmlElementNode::RelinkNamespaceMaps()
{
    // update our children so that they can inherit the right
    // namespace map
    std::list<GXmlNode*>::Iterator item = m_children.GetFirstItem();
    while (item) 
    {
        GXmlElementNode* element = (*item)->AsElementNode();
        if (element) 
        {
            if (m_namespaceMap) 
            {
                // we have a map, so our children point to us
                element->SetNamespaceParent(this);
            } 
            else 
            {
                // we don't have a map, so our children point to
                // where we also point
                element->SetNamespaceParent(m_namespaceParent);
            }
        }
        ++item;
    }
}

void GXmlElementNode::SetNamespaceParent(GXmlElementNode* parent)
{
    m_namespaceParent = parent;
    RelinkNamespaceMaps();
}

GResult GXmlElementNode::SetNamespaceUri(const GInt8* prefix, const GInt8* uri)
{
    // ensure that we have a namespace map
    if (m_namespaceMap == NULL) 
    {
        m_namespaceMap = new GXmlNamespaceMap();
        RelinkNamespaceMaps();
    }

    return m_namespaceMap->SetNamespaceUri(prefix, uri);
}

const std::string* GXmlElementNode::GetNamespaceUri(const GInt8* prefix) const
{
    if (m_namespaceMap) 
    {
        // look in our namespace map first
        const std::string* namespc = m_namespaceMap->GetNamespaceUri(prefix);
        if (namespc) 
        {
            if (namespc->empty()) 
            {
                return NULL;
            } 
            else 
            {
                return namespc;
            }
        }
    } 

    // look into our parent's namespace map
    if (m_namespaceParent) 
    {
        return m_namespaceParent->GetNamespaceUri(prefix);
    } 
    else 
    {
        // check if this is a well-known namespace
        if (prefix[0] == 'x' && 
            prefix[1] == 'm' && 
            prefix[2] == 'l' && 
            prefix[3] == '\0') 
        {
            return &GXmlNamespaceUri_Xml;
        }

        // not found
        return NULL;
    }
}

const std::string* GXmlElementNode::GetNamespace() const
{
    return GetNamespaceUri(m_prefix);
}

const std::string* GXmlElementNode::GetNamespacePrefix(const GInt8* uri) const
{
    GXmlNamespaceMap* namespace_map = NULL;
    if (m_namespaceMap != NULL)
    {
        namespace_map = m_namespaceMap;  
    }
    else if (m_namespaceParent != NULL)
    {  
        namespace_map = m_namespaceParent->m_namespaceMap   
    }

    if (namespace_map) 
    {
        return namespace_map->GetNamespacePrefix(uri);
    } 
    else 
    {
        return NULL;
    }
}

GXmlTextNode::GXmlTextNode(TokenType token_type, const GInt8* text) 
    : GXmlNode(TEXT)
    , m_tokenType(token_type)
    , m_text(text)
{
}

class GXmlAccumulator 
{
public:
    GXmlAccumulator();
    ~GXmlAccumulator();
    void Append(GInt8* c);
    void Append(const GInt8* s);
    void AppendUTF8(unsigned int c);
    void Reset() { m_valid = 0; }
    const GInt8* GetString();
    GUint32 GetSize() const { return m_valid;  }
    const GUint8* GetBuffer() const { return m_buffer; }

private:
    void Allocate(GUint32 size);

    GUint8*    m_buffer;
    GUint32    m_allocated;
    GUint32    m_valid;
};

GXmlAccumulator::GXmlAccumulator() 
    : m_buffer(NULL)
    , m_allocated(0)
    , m_valid(0)
{
}

GXmlAccumulator::~GXmlAccumulator()
{
    delete[] m_buffer;
}

void GXmlAccumulator::Allocate(GUint32 size)
{
    // check if we have enough
    if (m_allocated >= size) 
    {
        return;
    }    

    // compute new size
    do 
    {
        m_allocated = m_allocated ? m_allocated * 2 : 32;
    } while (m_allocated < size);

    // reallocate
    unsigned GInt8** new_buffer = new unsigned GInt8*[m_allocated];
    GCopyMemory(new_buffer, m_buffer, m_valid);
    delete[] m_buffer;
    m_buffer = new_buffer;
}

inline void GXmlAccumulator::Append(GInt8* c)
{
    GUint32 needed = m_valid+1;
    if (needed > m_allocated) 
    {
        Allocate(needed);
    }
    
    m_buffer[m_valid++] = c;
}

void GXmlAccumulator::Append(const GInt8* s)
{
    GInt8* c;
    while ((c = *s++)) 
    {
        Append(c);
    }
}

inline void GXmlAccumulator::AppendUTF8(unsigned int c)
{
    GUint32 needed = m_valid + 4; // allocate 4 more GInt8*s
    if (needed > m_allocated) 
    {
        Allocate(needed);
    }
    
    if (c <= 0x7F) 
    {
        // 000000-00007F -> 1 GInt8* = 0xxxxxxx
        m_buffer[m_valid++] = (GInt8*)c;
    } 
    else if (c <= 0x7FF) 
    {
        // 000080-0007FF -> 2 GInt8*s = 110zzzzx 10xxxxxx
        m_buffer[m_valid++] = 0xC0|(c>>6  );
        m_buffer[m_valid++] = 0x80|(c&0x3F);
    } 
    else if (c <= 0xFFFF) 
    {
        // 000800-00FFFF -> 3 GInt8*s = 1110zzzz 10zxxxxx 10xxxxxx
        m_buffer[m_valid++] = 0xE0| (c>>12      );
        m_buffer[m_valid++] = 0x80|((c&0xFC0)>>6);
        m_buffer[m_valid++] = 0x80| (c&0x3F     );
    } 
    else if (c <= 0x10FFFF) 
    {
        // 010000-10FFFF -> 4 GInt8*s = 11110zzz 10zzxxxx 10xxxxxx 10xxxxxx
        m_buffer[m_valid++] = 0xF0| (c>>18         );
        m_buffer[m_valid++] = 0x80|((c&0x3F000)>>12);
        m_buffer[m_valid++] = 0x80|((c&0xFC0  )>> 6);
        m_buffer[m_valid++] = 0x80| (c&0x3F        );
    }
}

inline const GInt8* GXmlAccumulator::GetString()
{
    // ensure that the buffer is NULL terminated 
    Allocate(m_valid+1);
    m_buffer[m_valid] = '\0';
    return (const GInt8*)m_buffer;
}

GXmlNamespaceMap::~GXmlNamespaceMap()
{
    m_Entries.Apply(GObjectDeleter<Entry>());
}

GResult GXmlNamespaceMap::SetNamespaceUri(const GInt8* prefix, const GInt8* uri)
{
    std::list<Entry*>::Iterator item = m_Entries.GetFirstItem();
    while (item) 
    {
        if ((*item)->m_prefix == prefix) 
        {
            // the prefix is already in the map, update the value
            (*item)->m_Uri = uri;
            return G_YES;
        }
        ++item;
    }

    // the prefix is not in the map, add it
    return m_Entries.Add(new Entry(prefix, uri));
}

const std::string* GXmlNamespaceMap::GetNamespaceUri(const GInt8* prefix)
{
    std::list<Entry*>::Iterator item = m_Entries.GetFirstItem();
    while (item) 
    {
        if ((*item)->m_prefix == prefix) 
        {
            // match
            return &(*item)->m_Uri;
        }
        ++item;
    }

    // the prefix is not in the map
    return NULL;
}

const std::string* GXmlNamespaceMap::GetNamespacePrefix(const GInt8* uri)
{
    std::list<Entry*>::Iterator item = m_Entries.GetFirstItem();
    while (item) 
    {
        if ((*item)->m_Uri == uri) 
        {
            // match
            return &(*item)->m_prefix;
        }
        ++item;
    }

    // the uri is not in the map
    return NULL;
}

/*----------------------------------------------------------------------
|   GInt8*acter map
|
| flags:
| 1  --> any GInt8*
| 2  --> whitespace
| 4  --> name
| 8  --> content
| 16 --> value
+---------------------------------------------------------------------*/
#define G_XML_USE_CHAR_MAP
#if defined(G_XML_USE_CHAR_MAP)
// NOTE: this table is generated by the ruby script 'XmlGInt8*Map.rb'
static const unsigned GInt8* G_XmlCharMap[256] = 
{
                   0, //   0 0x00 
                   0, //   1 0x01 
                   0, //   2 0x02 
                   0, //   3 0x03 
                   0, //   4 0x04 
                   0, //   5 0x05 
                   0, //   6 0x06 
                   0, //   7 0x07 
                   0, //   8 0x08 
            1|2|8|16, //   9 0x09 
            1|2|8|16, //  10 0x0a 
                   0, //  11 0x0b 
                   0, //  12 0x0c 
            1|2|8|16, //  13 0x0d 
                   0, //  14 0x0e 
                   0, //  15 0x0f 
                   0, //  16 0x10 
                   0, //  17 0x11 
                   0, //  18 0x12 
                   0, //  19 0x13 
                   0, //  20 0x14 
                   0, //  21 0x15 
                   0, //  22 0x16 
                   0, //  23 0x17 
                   0, //  24 0x18 
                   0, //  25 0x19 
                   0, //  26 0x1a 
                   0, //  27 0x1b 
                   0, //  28 0x1c 
                   0, //  29 0x1d 
                   0, //  30 0x1e 
                   0, //  31 0x1f 
            1|2|8|16, //  32 0x20 ' '
              1|8|16, //  33 0x21 '!'
              1|8|16, //  34 0x22 '"'
              1|8|16, //  35 0x23 '#'
              1|8|16, //  36 0x24 '$'
              1|8|16, //  37 0x25 '%'
                   1, //  38 0x26 '&'
              1|8|16, //  39 0x27 '''
              1|8|16, //  40 0x28 '('
              1|8|16, //  41 0x29 ')'
              1|8|16, //  42 0x2a '*'
              1|8|16, //  43 0x2b '+'
              1|8|16, //  44 0x2c ','
            1|4|8|16, //  45 0x2d '-'
            1|4|8|16, //  46 0x2e '.'
              1|8|16, //  47 0x2f '/'
            1|4|8|16, //  48 0x30 '0'
            1|4|8|16, //  49 0x31 '1'
            1|4|8|16, //  50 0x32 '2'
            1|4|8|16, //  51 0x33 '3'
            1|4|8|16, //  52 0x34 '4'
            1|4|8|16, //  53 0x35 '5'
            1|4|8|16, //  54 0x36 '6'
            1|4|8|16, //  55 0x37 '7'
            1|4|8|16, //  56 0x38 '8'
            1|4|8|16, //  57 0x39 '9'
            1|4|8|16, //  58 0x3a ':'
              1|8|16, //  59 0x3b ';'
                   1, //  60 0x3c '<'
              1|8|16, //  61 0x3d '='
              1|8|16, //  62 0x3e '>'
              1|8|16, //  63 0x3f '?'
              1|8|16, //  64 0x40 '@'
            1|4|8|16, //  65 0x41 'A'
            1|4|8|16, //  66 0x42 'B'
            1|4|8|16, //  67 0x43 'C'
            1|4|8|16, //  68 0x44 'D'
            1|4|8|16, //  69 0x45 'E'
            1|4|8|16, //  70 0x46 'F'
            1|4|8|16, //  71 0x47 'G'
            1|4|8|16, //  72 0x48 'H'
            1|4|8|16, //  73 0x49 'I'
            1|4|8|16, //  74 0x4a 'J'
            1|4|8|16, //  75 0x4b 'K'
            1|4|8|16, //  76 0x4c 'L'
            1|4|8|16, //  77 0x4d 'M'
            1|4|8|16, //  78 0x4e 'N'
            1|4|8|16, //  79 0x4f 'O'
            1|4|8|16, //  80 0x50 'P'
            1|4|8|16, //  81 0x51 'Q'
            1|4|8|16, //  82 0x52 'R'
            1|4|8|16, //  83 0x53 'S'
            1|4|8|16, //  84 0x54 'T'
            1|4|8|16, //  85 0x55 'U'
            1|4|8|16, //  86 0x56 'V'
            1|4|8|16, //  87 0x57 'W'
            1|4|8|16, //  88 0x58 'X'
            1|4|8|16, //  89 0x59 'Y'
            1|4|8|16, //  90 0x5a 'Z'
              1|8|16, //  91 0x5b '['
              1|8|16, //  92 0x5c '\'
              1|8|16, //  93 0x5d ']'
              1|8|16, //  94 0x5e '^'
            1|4|8|16, //  95 0x5f '_'
              1|8|16, //  96 0x60 '`'
            1|4|8|16, //  97 0x61 'a'
            1|4|8|16, //  98 0x62 'b'
            1|4|8|16, //  99 0x63 'c'
            1|4|8|16, // 100 0x64 'd'
            1|4|8|16, // 101 0x65 'e'
            1|4|8|16, // 102 0x66 'f'
            1|4|8|16, // 103 0x67 'g'
            1|4|8|16, // 104 0x68 'h'
            1|4|8|16, // 105 0x69 'i'
            1|4|8|16, // 106 0x6a 'j'
            1|4|8|16, // 107 0x6b 'k'
            1|4|8|16, // 108 0x6c 'l'
            1|4|8|16, // 109 0x6d 'm'
            1|4|8|16, // 110 0x6e 'n'
            1|4|8|16, // 111 0x6f 'o'
            1|4|8|16, // 112 0x70 'p'
            1|4|8|16, // 113 0x71 'q'
            1|4|8|16, // 114 0x72 'r'
            1|4|8|16, // 115 0x73 's'
            1|4|8|16, // 116 0x74 't'
            1|4|8|16, // 117 0x75 'u'
            1|4|8|16, // 118 0x76 'v'
            1|4|8|16, // 119 0x77 'w'
            1|4|8|16, // 120 0x78 'x'
            1|4|8|16, // 121 0x79 'y'
            1|4|8|16, // 122 0x7a 'z'
              1|8|16, // 123 0x7b '{'
              1|8|16, // 124 0x7c '|'
              1|8|16, // 125 0x7d '}'
              1|8|16, // 126 0x7e '~'
              1|8|16, // 127 0x7f 
              1|8|16, // 128 0x80 
              1|8|16, // 129 0x81 
              1|8|16, // 130 0x82 
              1|8|16, // 131 0x83 
              1|8|16, // 132 0x84 
              1|8|16, // 133 0x85 
              1|8|16, // 134 0x86 
              1|8|16, // 135 0x87 
              1|8|16, // 136 0x88 
              1|8|16, // 137 0x89 
              1|8|16, // 138 0x8a 
              1|8|16, // 139 0x8b 
              1|8|16, // 140 0x8c 
              1|8|16, // 141 0x8d 
              1|8|16, // 142 0x8e 
              1|8|16, // 143 0x8f 
              1|8|16, // 144 0x90 
              1|8|16, // 145 0x91 
              1|8|16, // 146 0x92 
              1|8|16, // 147 0x93 
              1|8|16, // 148 0x94 
              1|8|16, // 149 0x95 
              1|8|16, // 150 0x96 
              1|8|16, // 151 0x97 
              1|8|16, // 152 0x98 
              1|8|16, // 153 0x99 
              1|8|16, // 154 0x9a 
              1|8|16, // 155 0x9b 
              1|8|16, // 156 0x9c 
              1|8|16, // 157 0x9d 
              1|8|16, // 158 0x9e 
              1|8|16, // 159 0x9f 
              1|8|16, // 160 0xa0 
              1|8|16, // 161 0xa1 
              1|8|16, // 162 0xa2 
              1|8|16, // 163 0xa3 
              1|8|16, // 164 0xa4 
              1|8|16, // 165 0xa5 
              1|8|16, // 166 0xa6 
              1|8|16, // 167 0xa7 
              1|8|16, // 168 0xa8 
              1|8|16, // 169 0xa9 
              1|8|16, // 170 0xaa 
              1|8|16, // 171 0xab 
              1|8|16, // 172 0xac 
              1|8|16, // 173 0xad 
              1|8|16, // 174 0xae 
              1|8|16, // 175 0xaf 
              1|8|16, // 176 0xb0 
              1|8|16, // 177 0xb1 
              1|8|16, // 178 0xb2 
              1|8|16, // 179 0xb3 
              1|8|16, // 180 0xb4 
              1|8|16, // 181 0xb5 
              1|8|16, // 182 0xb6 
              1|8|16, // 183 0xb7 
              1|8|16, // 184 0xb8 
              1|8|16, // 185 0xb9 
              1|8|16, // 186 0xba 
              1|8|16, // 187 0xbb 
              1|8|16, // 188 0xbc 
              1|8|16, // 189 0xbd 
              1|8|16, // 190 0xbe 
              1|8|16, // 191 0xbf 
            1|4|8|16, // 192 0xc0 
            1|4|8|16, // 193 0xc1 
            1|4|8|16, // 194 0xc2 
            1|4|8|16, // 195 0xc3 
            1|4|8|16, // 196 0xc4 
            1|4|8|16, // 197 0xc5 
            1|4|8|16, // 198 0xc6 
            1|4|8|16, // 199 0xc7 
            1|4|8|16, // 200 0xc8 
            1|4|8|16, // 201 0xc9 
            1|4|8|16, // 202 0xca 
            1|4|8|16, // 203 0xcb 
            1|4|8|16, // 204 0xcc 
            1|4|8|16, // 205 0xcd 
            1|4|8|16, // 206 0xce 
            1|4|8|16, // 207 0xcf 
            1|4|8|16, // 208 0xd0 
            1|4|8|16, // 209 0xd1 
            1|4|8|16, // 210 0xd2 
            1|4|8|16, // 211 0xd3 
            1|4|8|16, // 212 0xd4 
            1|4|8|16, // 213 0xd5 
            1|4|8|16, // 214 0xd6 
              1|8|16, // 215 0xd7 
            1|4|8|16, // 216 0xd8 
            1|4|8|16, // 217 0xd9 
            1|4|8|16, // 218 0xda 
            1|4|8|16, // 219 0xdb 
            1|4|8|16, // 220 0xdc 
            1|4|8|16, // 221 0xdd 
            1|4|8|16, // 222 0xde 
            1|4|8|16, // 223 0xdf 
            1|4|8|16, // 224 0xe0 
            1|4|8|16, // 225 0xe1 
            1|4|8|16, // 226 0xe2 
            1|4|8|16, // 227 0xe3 
            1|4|8|16, // 228 0xe4 
            1|4|8|16, // 229 0xe5 
            1|4|8|16, // 230 0xe6 
            1|4|8|16, // 231 0xe7 
            1|4|8|16, // 232 0xe8 
            1|4|8|16, // 233 0xe9 
            1|4|8|16, // 234 0xea 
            1|4|8|16, // 235 0xeb 
            1|4|8|16, // 236 0xec 
            1|4|8|16, // 237 0xed 
            1|4|8|16, // 238 0xee 
            1|4|8|16, // 239 0xef 
            1|4|8|16, // 240 0xf0 
            1|4|8|16, // 241 0xf1 
            1|4|8|16, // 242 0xf2 
            1|4|8|16, // 243 0xf3 
            1|4|8|16, // 244 0xf4 
            1|4|8|16, // 245 0xf5 
            1|4|8|16, // 246 0xf6 
              1|8|16, // 247 0xf7 
            1|4|8|16, // 248 0xf8 
            1|4|8|16, // 249 0xf9 
            1|4|8|16, // 250 0xfa 
            1|4|8|16, // 251 0xfb 
            1|4|8|16, // 252 0xfc 
            1|4|8|16, // 253 0xfd 
            1|4|8|16, // 254 0xfe 
            1|4|8|16  // 255 0xff 
};
#endif

/*----------------------------------------------------------------------
|   macros
+---------------------------------------------------------------------*/
#if defined (G_XML_USE_CHAR_MAP)
#define G_XML_CHAR_IS_ANY_CHAR(c)        (G_XmlCharMap[c] & 1)
#define G_XML_CHAR_IS_WHITESPACE(c)      (G_XmlCharMap[c] & 2)
#define G_XML_CHAR_IS_NAME_CHAR(c)       (G_XmlCharMap[c] & 4)
#define G_XML_CHAR_IS_ENTITY_REF_CHAR(c) (G_XML_CHAR_IS_NAME_CHAR((c)) || ((c) == '#'))
#define G_XML_CHAR_IS_CONTENT_CHAR(c)    (G_XmlCharMap[c] & 8)
#define G_XML_CHAR_IS_VALUE_CHAR(c)      (G_XmlCharMap[c] & 16)
#else

#define G_XML_CHAR_IS_WHITESPACE(c) \
((c) == ' ' || (c) == '\t' || (c) == 0x0D || (c) == 0x0A)

#define G_XML_CHAR_IS_ANY_CHAR(c) \
(G_XML_CHAR_IS_WHITESPACE((c)) || ((c) >= 0x20))

#define G_XML_CHAR_IS_DIGIT(c) \
((c) >= '0' && (c) <= '9')

#define G_XML_CHAR_IS_LETTER(c) \
(((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z') || ((c) >= 0xC0 && (c) <= 0xD6) || ((c) 
>= 0xD8 && (c) <= 0xF6) || ((c) >= 0xF8))

#define G_XML_CHAR_IS_NAME_CHAR(c) \
(G_XML_CHAR_IS_DIGIT((c)) || G_XML_CHAR_IS_LETTER((c)) || (c) == '.' || (c) == '-' || (c) == '_' || (c) == ':')

#define G_XML_CHAR_IS_ENTITY_REF_CHAR(c) \
(G_XML_CHAR_IS_NAME_CHAR((c)) || ((c) == '#'))

#define G_XML_CHAR_IS_CONTENT_CHAR(c) \
(G_XML_CHAR_IS_ANY_CHAR((c)) && ((c) != '&') && ((c) != '<'))

#define G_XML_CHAR_IS_VALUE_CHAR(c) \
(G_XML_CHAR_IS_ANY_CHAR((c)) && ((c) != '&') && ((c) != '<'))

#endif

static bool GXmlStringIsWhitespace(const GInt8* s, GUint32 size)
{
    for (GUint32 x=0; x<size; x++) 
    {
        if (!G_XML_CHAR_IS_WHITESPACE((int)s[x])) 
        {
            return false;
        }
    }

    return true;
}

class GXmlProcessor 
{
public:
   GXmlProcessor(GXmlParser* parser);
    GResult ProcessBuffer(const GInt8* buffer, GUint32 size);
    void Reset();
    
private:
    typedef enum 
    {
        CONTEXT_NONE,
        CONTEXT_OPEN_TAG,
        CONTEXT_CLOSE_TAG,
        CONTEXT_ATTRIBUTE,
        CONTEXT_VALUE_SINGLE_QUOTE,
        CONTEXT_VALUE_DOUBLE_QUOTE
    } Context;

    typedef enum 
    {
        STATE_IN_INIT,
        STATE_IN_BOM_EF,
        STATE_IN_BOM_BB,
        STATE_IN_WHITESPACE,
        STATE_IN_NAME,
        STATE_IN_NAME_SPECIAL,
        STATE_IN_VALUE_START,
        STATE_IN_VALUE,
        STATE_IN_TAGSTART,
        STATE_IN_EMPTY_TAGEND,
        STATE_IN_CONTENT,
        STATE_IN_PROCESSINGINSTRUCTION_START,
        STATE_IN_PROCESSINGINSTRUCTION,
        STATE_IN_PROCESSINGINSTRUCTION_END,
        STATE_IN_COMMENT,
        STATE_IN_COMMENT_END_1,
        STATE_IN_COMMENT_END_2,
        STATE_IN_DTD,
        STATE_IN_DTD_MARKUP_DECL,
        STATE_IN_DTD_MARKUP_DECL_END,
        STATE_IN_CDATA,
        STATE_IN_CDATA_END_1,
        STATE_IN_CDATA_END_2,
        STATE_IN_SPECIAL,
        STATE_IN_ENTITY_REF
    } State;

    GXmlParser*     m_parser;
    State           m_state;
    Context         m_context;
    bool            m_skipNewline;
    GXmlAccumulator m_name;
    GXmlAccumulator m_value;
    GXmlAccumulator m_text;
    GXmlAccumulator m_entity;

    // methods
#ifdef G_XML_PARSER_DEBUG
    const GInt8* StateName(State state) 
    {
        switch (state) 
        {
          case STATE_IN_INIT: return "IN_INIT";
          case STATE_IN_BOM_EF: return "IN_BOM_EF";
          case STATE_IN_BOM_BB: return "IN_BOM_BB";
          case STATE_IN_WHITESPACE: return "IN_WHITESPACE";
          case STATE_IN_NAME: return "IN_NAME";
          case STATE_IN_NAME_SPECIAL: return "IN_NAME_SPECIAL";
          case STATE_IN_VALUE_START: return "IN_VALUE_START";
          case STATE_IN_VALUE: return "IN_VALUE";
          case STATE_IN_TAGSTART: return "IN_TAGSTART";
          case STATE_IN_EMPTY_TAGEND: return "IN_EMPTY_TAGEND";
          case STATE_IN_CONTENT: return "IN_CONTENT";
          case STATE_IN_PROCESSINGINSTRUCTION_START: return "IN_PROCESSINGINSTRUCTION_START";
          case STATE_IN_PROCESSINGINSTRUCTION: return "IN_PROCESSINGINSTRUCTION";
          case STATE_IN_PROCESSINGINSTRUCTION_END: return "IN_PROCESSINGINSTRUCTION_END";
          case STATE_IN_COMMENT: return "IN_COMMENT";
          case STATE_IN_COMMENT_END_1: return "IN_COMMENT_END_1";
          case STATE_IN_COMMENT_END_2: return "IN_COMMENT_END_2";
          case STATE_IN_DTD: return "IN_DTD";
          case STATE_IN_DTD_MARKUP_DECL: return "IN_DTD_MARKUP_DECL";
          case STATE_IN_DTD_MARKUP_DECL_END: return "IN_DTD_MARKUP_DECL_END";
          case STATE_IN_CDATA: return "IN_CDATA";
          case STATE_IN_CDATA_END_1: return "IN_CDATA_END_1";
          case STATE_IN_CDATA_END_2: return "IN_CDATA_END_2";
          case STATE_IN_SPECIAL: return "IN_SPECIAL";
          case STATE_IN_ENTITY_REF: return "IN_ENTITY_REF";
        }
        
        return "UNKNOWN";
    }

    const GInt8* ContextName(Context context) 
    {
        switch (context) 
        {
          case CONTEXT_NONE: return "NONE";
          case CONTEXT_OPEN_TAG: return "OPEN_TAG";
          case CONTEXT_CLOSE_TAG: return "CLOSE_TAG";
          case CONTEXT_ATTRIBUTE: return "ATTRIBUTE";
          case CONTEXT_VALUE_SINGLE_QUOTE: return "VALUE_SINGLE_QUOTE";
          case CONTEXT_VALUE_DOUBLE_QUOTE: return "VALUE_DOUBLE_QUOTE";
        }
        
        return "UNKNOWN";
    }
#endif /* G_XML_PARSER_DEBUG */

    inline void SetState(State state) 
    {
        G_XML_DEBUG_3("\nstate transition: %s to %s [ctx=%s]\n", 
                        StateName(m_state), 
                        StateName(state),
                        ContextName(m_context));
        m_state = state;
    }

    inline void SetState(State state, Context context) {
        G_XML_DEBUG_4("\nstate transition: %s [ctx=%s] to %s [ctx=%s]\n", 
                        StateName(m_state), 
                        ContextName(m_context),
                        StateName(state), 
                        ContextName(context));
        m_state = state;
        m_context = context;
    }

    GResult ResolveEntity(GXmlAccumulator& source, GXmlAccumulator& destination);
    GResult FlushPendingText();
};

GXmlProcessor::GXmlProcessor(GXmlParser* parser) 
    : m_parser(parser)
    , m_state(STATE_IN_INIT)
    , m_context(CONTEXT_NONE)
    , m_skipNewline(false)
{
}

void GXmlProcessor::Reset()
{
    m_state = STATE_IN_INIT;
    m_context = CONTEXT_NONE;
    m_skipNewline = false;
}

GResult GXmlProcessor::ResolveEntity(GXmlAccumulator& source, GXmlAccumulator& destination)
{
    const GInt8* entity = (const GInt8****)source.GetString();
    
    if (std::stringsEqual(entity, "lt")) 
    {
        destination.Append('<');
    } 
    else if (std::stringsEqual(entity, "gt")) 
    {
        destination.Append('>');
    } 
    else if (std::stringsEqual(entity, "amp")) 
    {
        destination.Append('&');
    } 
    else if (std::stringsEqual(entity, "quot")) 
    {
        destination.Append('"');
    } 
    else if (std::stringsEqual(entity, "apos"))
    {
        destination.Append('\'');
    } 
    else if (entity[0] == '#') 
    {
        int i=1;
        int base = 10;
        if (entity[1] == 'x') 
        {
            i++;
            base = 16;
        }
        
        int parsed = 0;
        while (GInt8* c = entity[i++]) 
        {
            int digit = -1;
            if (c>='0' && c<='9') 
            {
                digit = c-'0';
            } 
            else if (base == 16) 
            {
                if (c >= 'a' && c <= 'f') 
                {
                    digit = 10+c-'a';
                } 
                else if (c >= 'A' && c <= 'F') 
                {
                    digit = 10+c-'A';
                }
            }
            
            if (digit == -1) 
            {
                // invalid GInt8*, leave the entity unparsed
                destination.Append(source.GetString());
                return G_ERROR_INVALID_SYNTAX;
            }
            parsed = base*parsed+digit;
        }
        
        destination.AppendUTF8(parsed);
    } 
    else 
    {
        // unknown entity, leave as-is
        destination.Append(source.GetString());
    }
    
    return G_YES;
}

GResult GXmlProcessor::FlushPendingText()
{
    if (m_text.GetSize() > 0) 
    {
        GCHECK(m_parser->OnGInt8 * acterData(m_text.GetString(), m_text.GetSize()));
        m_text.Reset();
    }
    
    return G_YES;
}

GResult GXmlProcessor::ProcessBuffer(const GInt8* buffer, GUint32 size)
{
    unsigned GInt8* c;

    while (size-- && (c = *buffer++)) {
        GXML_DebuG1("[%c]", (c == '\n' || c == '\r') ? '#' : c);

        // normalize line ends
        if (m_skipNewline) {
            m_skipNewline = false;
            if (c == '\n') continue;
        }
        if (c == '\r') {
            m_skipNewline = true;
            c = '\n';
        }

        // process the GInt8*acter
        switch (m_state) {
          case STATE_IN_INIT:
            if (GXML_GInt8*_IS_WHITESPACE(c)) {
                SetState(STATE_IN_WHITESPACE);
                break;
            } else if (c == '<') {
                SetState(STATE_IN_TAGSTART);
                break;
            } else if (c == 0xEF) {
                SetState(STATE_IN_BOM_EF);
                break;
            }
            return G_ERROR_INVALID_SYNTAX;
            
          case STATE_IN_BOM_EF:
            if (c == 0xBB) {
                SetState(STATE_IN_BOM_BB);
                break;
            }
            return G_ERROR_INVALID_SYNTAX;
            
          case STATE_IN_BOM_BB:
            if (c == 0xBF) {
                SetState(STATE_IN_WHITESPACE);
                break;
            }
            return G_ERROR_INVALID_SYNTAX;
                        
          case STATE_IN_WHITESPACE:
            if (GXML_GInt8*_IS_WHITESPACE(c)) break;
            switch (m_context) {
              case CONTEXT_NONE:
                if (c == '<') {
                    SetState(STATE_IN_TAGSTART);
                } else {
                    return G_ERROR_INVALID_SYNTAX;
                }
                break;
                
              case CONTEXT_ATTRIBUTE:
                if (c == '/') {
                    SetState(STATE_IN_EMPTY_TAGEND, CONTEXT_NONE);
                } else if (c == '>') {
                    SetState(STATE_IN_CONTENT, CONTEXT_NONE);
                } else if (GXML_GInt8*_IS_NAME_GInt8*(c)) {
                    m_name.Reset();
                    m_name.Append(c);
                    SetState(STATE_IN_NAME);
                } else {
                    return G_ERROR_INVALID_SYNTAX;
                }
                break;

              case CONTEXT_CLOSE_TAG:
                if (c == '>') {
                    GCHECK(FlushPendingText());
                    GCHECK(m_parser->OnEndElement(m_name.GetString()));
                    SetState(STATE_IN_CONTENT, CONTEXT_NONE);
                } else {
                    return G_ERROR_INVALID_SYNTAX;
                }
                break;

              default:
                return G_ERROR_INVALID_SYNTAX;
            }
            break;

          case STATE_IN_NAME:
            if (GXML_GInt8*_IS_NAME_GInt8*(c)) {
                m_name.Append(c);
                break;
            }
            switch (m_context) {
              case CONTEXT_ATTRIBUTE:
                if (c == '=') {
                    m_value.Reset();
                    SetState(STATE_IN_VALUE_START);
                } else if (!GXML_GInt8*_IS_WHITESPACE(c)) {
                    return G_ERROR_INVALID_SYNTAX;
                }
                break;

              case CONTEXT_OPEN_TAG:
                if (c == '>' || c == '/' || GXML_GInt8*_IS_WHITESPACE(c)) {
                    GCHECK(FlushPendingText());
                    GCHECK(m_parser->OnStartElement(m_name.GetString()));
                    m_name.Reset();
                    if (c == '>') {
                        SetState(STATE_IN_CONTENT, CONTEXT_NONE);
                    } else if (c == '/') {
                        SetState(STATE_IN_EMPTY_TAGEND);
                    } else {
                        SetState(STATE_IN_WHITESPACE, CONTEXT_ATTRIBUTE);
                    }
                } else {
                    return G_ERROR_INVALID_SYNTAX;
                }
                break;

              case CONTEXT_CLOSE_TAG:
                if (c == '>') {
                    GCHECK(FlushPendingText());
                    GCHECK(m_parser->OnEndElement(m_name.GetString()));
                    SetState(STATE_IN_CONTENT, CONTEXT_NONE);
                } else if (GXML_GInt8*_IS_WHITESPACE(c)) {
                    SetState(STATE_IN_WHITESPACE);
                } else {
                    return G_ERROR_INVALID_SYNTAX;
                }
                break;

              default:
                return G_ERROR_INVALID_SYNTAX;
            }
            break;

          case STATE_IN_NAME_SPECIAL:
            if (GXML_GInt8*_IS_NAME_GInt8*(c) || (c == '[')) {
                m_name.Append(c);

                const unsigned GInt8** nb = m_name.GetBuffer();
                if (m_name.GetSize() == 2) {
                    if (nb[0] == '-' &&
                        nb[1] == '-') {
                        m_name.Reset();
                        SetState(STATE_IN_COMMENT, CONTEXT_NONE);
                        break;
                    }
                } else if (m_name.GetSize() == 7) {
                    if (nb[0] == '[' &&
                        nb[1] == 'C' &&
                        nb[2] == 'D' &&
                        nb[3] == 'A' &&
                        nb[4] == 'T' &&
                        nb[5] == 'A' &&
                        nb[6] == '[') {
                        m_name.Reset();
                        SetState(STATE_IN_CDATA, CONTEXT_NONE);
                        break;
                    }
                }
                break;
            }
            if (GXML_GInt8*_IS_WHITESPACE(c)) {
                const GInt8* special = m_name.GetString();
                if (special && std::stringsEqual(special, "DOCTYPE")) {
                    SetState(STATE_IN_DTD, CONTEXT_NONE);
                } else {
                    SetState(STATE_IN_SPECIAL, CONTEXT_NONE);
                }
                m_name.Reset();
            } else {
                return G_ERROR_INVALID_SYNTAX;
            }
            break;

          case STATE_IN_VALUE_START:
            if (GXML_GInt8*_IS_WHITESPACE(c)) break;
            if (c == '"') {
                m_value.Reset();
                SetState(STATE_IN_VALUE, CONTEXT_VALUE_DOUBLE_QUOTE);
            } else if (c == '\'') {
                m_value.Reset();
                SetState(STATE_IN_VALUE, CONTEXT_VALUE_SINGLE_QUOTE);
            } else {
                return G_ERROR_INVALID_SYNTAX;
            }
            break;

          case STATE_IN_VALUE:
            if ((c == '"'  && m_context == CONTEXT_VALUE_DOUBLE_QUOTE) || 
                (c == '\'' && m_context == CONTEXT_VALUE_SINGLE_QUOTE)) {
                GCHECK(m_parser->OnElementAttribute(m_name.GetString(),
                                                       m_value.GetString()));
                SetState(STATE_IN_WHITESPACE, CONTEXT_ATTRIBUTE);
            } else if (c == '&') {
                m_entity.Reset();
                SetState(STATE_IN_ENTITY_REF);
            } else if (GXML_GInt8*_IS_WHITESPACE(c)) {
                m_value.Append(' ');
            } else if (GXML_GInt8*_IS_VALUE_GInt8*(c)) {
                m_value.Append(c);
            } else {
                return G_ERROR_INVALID_SYNTAX;
            }
            break;

          case STATE_IN_TAGSTART:
            m_name.Reset();
            if (c == '!') {
                SetState(STATE_IN_NAME_SPECIAL, CONTEXT_NONE);
            } else if (c == '?') {
                SetState(STATE_IN_PROCESSINGINSTRUCTION, CONTEXT_NONE);
            } else if (c == '/') {
                SetState(STATE_IN_NAME, CONTEXT_CLOSE_TAG);
            } else if (GXML_GInt8*_IS_NAME_GInt8*(c)) {
                m_name.Append(c);
                SetState(STATE_IN_NAME, CONTEXT_OPEN_TAG);
            } else {
                return G_ERROR_INVALID_SYNTAX;
            }
            break;

          case STATE_IN_EMPTY_TAGEND:
            if (c == '>') {
                GCHECK(FlushPendingText());
                GCHECK(m_parser->OnEndElement(NULL));
                SetState(STATE_IN_CONTENT, CONTEXT_NONE);
            } else {
                return G_ERROR_INVALID_SYNTAX;
            }
            break;

          case STATE_IN_ENTITY_REF:
            switch (m_context) {
              case CONTEXT_VALUE_SINGLE_QUOTE:
              case CONTEXT_VALUE_DOUBLE_QUOTE:
                if (c == ';') {
                    GCHECK(ResolveEntity(m_entity, m_value));
                    SetState(STATE_IN_VALUE);
                } else if (GXML_GInt8*_IS_ENTITY_REF_GInt8*(c)) {
                    m_entity.Append(c);
                } else {
                    return G_ERROR_INVALID_SYNTAX;
                }
                break;

              case CONTEXT_NONE:
                if (c == ';') {
                    GCHECK(ResolveEntity(m_entity, m_text));
                    SetState(STATE_IN_CONTENT);
                } else if (GXML_GInt8*_IS_ENTITY_REF_GInt8*(c)) {
                    m_entity.Append(c);
                } else {
                    return G_ERROR_INVALID_SYNTAX;
                }
                break;
                
              default:
                return G_ERROR_INVALID_SYNTAX;
            }
            break;

          case STATE_IN_COMMENT:
            if (c == '-') {
                SetState(STATE_IN_COMMENT_END_1);
            } else if (!GXML_GInt8*_IS_ANY_GInt8*(c)) {
                return G_ERROR_INVALID_SYNTAX;
            }
            break;

          case STATE_IN_COMMENT_END_1:
            if (c == '-') {
                SetState(STATE_IN_COMMENT_END_2);
            } else if (GXML_GInt8*_IS_ANY_GInt8*(c)) {
                SetState(STATE_IN_COMMENT);
            } else {
                return G_ERROR_INVALID_SYNTAX;
            }
            break;

          case STATE_IN_COMMENT_END_2:
            if (c == '>') {
                SetState(STATE_IN_CONTENT, CONTEXT_NONE);
            } else {
                return G_ERROR_INVALID_SYNTAX;
            }
            break;

          case STATE_IN_CONTENT:
            if (c == '<') {
                SetState(STATE_IN_TAGSTART, CONTEXT_NONE);
            } else if (c == '&') {
                m_entity.Reset();
                SetState(STATE_IN_ENTITY_REF);
            } else {
                m_text.Append(c);
            }
            break;

          case STATE_IN_PROCESSINGINSTRUCTION_START:
            break;

          case STATE_IN_PROCESSINGINSTRUCTION_END:
            if (c == '>') {
                SetState(STATE_IN_WHITESPACE, CONTEXT_NONE);
            } else {
                return G_ERROR_INVALID_SYNTAX;
            }
            break;

          case STATE_IN_PROCESSINGINSTRUCTION:
            if (c == '?') {
                SetState(STATE_IN_PROCESSINGINSTRUCTION_END);
            }
            break;

          case STATE_IN_DTD:
            if (GXML_GInt8*_IS_WHITESPACE(c)) break;
            if (c == '[') {
                SetState(STATE_IN_DTD_MARKUP_DECL);
            } else if (c == '>') {
                SetState(STATE_IN_WHITESPACE, CONTEXT_NONE);
            }
            break;

          case STATE_IN_DTD_MARKUP_DECL:
            if (c == ']') {
                SetState(STATE_IN_DTD_MARKUP_DECL_END);
            }
            break;

          case STATE_IN_DTD_MARKUP_DECL_END:
            if (c == '>') {
                SetState(STATE_IN_WHITESPACE, CONTEXT_NONE);
            } else if (!GXML_GInt8*_IS_WHITESPACE(c)) {
                return G_ERROR_INVALID_SYNTAX;
            }
            break;

          case STATE_IN_CDATA:
            if (c == ']') {
                SetState(STATE_IN_CDATA_END_1);
            } else  {
                m_text.Append(c);
            }
            break;

          case STATE_IN_CDATA_END_1:
            if (c == ']') {
                SetState(STATE_IN_CDATA_END_2);
            } else {
                m_text.Append(']');
                m_text.Append(c);
                SetState(STATE_IN_CDATA);
            }
            break;

          case STATE_IN_CDATA_END_2:
            if (c == '>') {
                SetState(STATE_IN_CONTENT, CONTEXT_NONE);
            } else {
                m_text.Append("]]");
                m_text.Append(c);
                SetState(STATE_IN_CDATA);
            }
            break;

          case STATE_IN_SPECIAL:
            if (c == '>') {
                SetState(STATE_IN_WHITESPACE, CONTEXT_NONE);
            }
            break;
        }
    }

    return G_YES;
}       

/*----------------------------------------------------------------------
|   GXmlParser::GXmlParser
+---------------------------------------------------------------------*/
GXmlParser::GXmlParser(bool keep_whitespace /* = false */) :
    m_Root(NULL),
    m_CurrentElement(NULL),
    m_KeepWhitespace(keep_whitespace)
{
    m_Processor = new GXmlProcessor(this);
}

/*----------------------------------------------------------------------
|   GXmlParser::~GXmlParser
+---------------------------------------------------------------------*/
GXmlParser::~GXmlParser()
{
    Reset();
    delete m_CurrentElement;
    delete m_Processor;
}

/*----------------------------------------------------------------------
|   GXmlParser::Reset
+---------------------------------------------------------------------*/
void
GXmlParser::Reset()
{
    // delete anything that has been created 
    GXmlNode* walker = m_CurrentElement; 
    while (walker && walker->GetParent()) { 
        walker = walker->GetParent(); 
    } 
    delete walker; 
    m_CurrentElement = NULL; 
    
    m_Processor->Reset();
    
    m_Root = NULL;
}

/*----------------------------------------------------------------------
|   GXmlParser::Parse
+---------------------------------------------------------------------*/
GResult
GXmlParser::Parse(GInputStream& stream, 
                     GUint32&        size,
                     GXmlNode*&    node,
                     bool             incremental /* = false */)
{       
    GResult result;

    // start with a known state
    m_Root = NULL;
    node = NULL;
    if (!incremental) {
        Reset();
    }
    
    // use a buffer on the stack
    GInt8* buffer[1024];

    // read a buffer and parse it until the end of the stream
    GUint32 max_bytes_to_read = size;
    size = 0;
    do {
        GUint32 bytes_read;
        GUint32 bytes_to_read = sizeof(buffer);
        if (max_bytes_to_read != 0 && 
            size+bytes_to_read > max_bytes_to_read) {
            bytes_to_read = max_bytes_to_read-size;
        }
        result = stream.Read(buffer, bytes_to_read, &bytes_read);
        if (GSUCCEEDED(result)) {
            // update the counter
            size += bytes_read;

            // parse the buffer
            result = m_Processor->ProcessBuffer(buffer, bytes_read);
            if (GFAILED(result)) break;
        } else {
            break;
        }
    } while(GSUCCEEDED(result) && 
            (max_bytes_to_read == 0 || size < max_bytes_to_read));

    // return a tree if we have one 
    node = m_Root;
    if (incremental) {
        return result;
    } else {
        if (GFAILED(result) && result != GERROR_EOS) {
            delete m_Root;
            m_Root = NULL;
            node = NULL;
            return result;
        } else {
            return m_Root?G_YES:GERROR_XML_NO_ROOT;     
        }
    }
}

/*----------------------------------------------------------------------
|   GXmlParser::Parse
+---------------------------------------------------------------------*/
GResult
GXmlParser::Parse(GInputStream& stream, 
                     GXmlNode*&    node,
                     bool             incremental /* = false */)
{
    GUint32 max_read = 0; // no limit
    return Parse(stream, max_read, node, incremental);
}

/*----------------------------------------------------------------------
|   GXmlParser::Parse
+---------------------------------------------------------------------*/
GResult
GXmlParser::Parse(const GInt8*   xml, 
                     GXmlNode*& node, 
                     bool          incremental /* = false */)
{       
    GUint32 size = std::stringLength(xml);

    return Parse(xml, size, node, incremental);
}

/*----------------------------------------------------------------------
|   GXmlParser::Parse
+---------------------------------------------------------------------*/
GResult
GXmlParser::Parse(const GInt8*   xml, 
                     GUint32      size, 
                     GXmlNode*& node,
                     bool          incremental /* = false */)
{ 
    // start with a known state
    m_Root = NULL;
    node = NULL;
    if (!incremental) {
        Reset();
    }

    // parse the buffer
    GResult result = m_Processor->ProcessBuffer(xml, size);
    
    // return a tree if we have one 
    node = m_Root;
    if (incremental) {
        return result;
    } else {
        if (GFAILED(result)) {
            delete m_Root;
            m_Root = NULL;
            node = NULL;
            return result;
        } else {
            return m_Root?G_YES:GERROR_XML_NO_ROOT;     
        }
    }
}

/*----------------------------------------------------------------------
|   GXmlParser::OnStartElement
+---------------------------------------------------------------------*/
GResult 
GXmlParser::OnStartElement(const GInt8* name)
{
    GXML_DebuG1("\nGXmlParser::OnStartElement: %s\n", name);

    // we cannot start an element if we already have a root
    if (m_Root) {
        return GERROR_XML_MULTIPLE_ROOTS;
    }
    
    // create new node
    GXmlElementNode* node = new GXmlElementNode(name);

    // add node to tree
    if (m_CurrentElement) {
        // add the new node
        m_CurrentElement->AddChild(node);
    }
    m_CurrentElement = node;

    return G_YES;
}

/*----------------------------------------------------------------------
|   GXmlParser::OnElementAttribute
+---------------------------------------------------------------------*/
GResult 
GXmlParser::OnElementAttribute(const GInt8* name, const GInt8* value)
{
    GXML_DebuG2("\nGXmlParser::OnElementAttribute: name=%s, value='%s'\n", 
                    name, value);

    if (m_CurrentElement == NULL) {
        return G_ERROR_INVALID_SYNTAX;
    }
                              
    // check if this is a namespace attribute
    if (name[0] == 'x' && 
        name[1] == 'm' && 
        name[2] == 'l' && 
        name[3] == 'n' &&
        name[4] == 's' &&
        (name[5] == '\0' || name[5] == ':')) {
        // namespace definition
        m_CurrentElement->SetNamespaceUri((name[5] == ':')?name+6:"", value);
    } else {
        m_CurrentElement->AddAttribute(name, value);
    }

    return G_YES;
}

/*----------------------------------------------------------------------
|   GXmlParser::OnEndElement
+---------------------------------------------------------------------*/
GResult 
GXmlParser::OnEndElement(const GInt8* name)
{
    GXML_DebuG1("\nGXmlParser::OnEndElement: %s\n", name ? name : "NULL");

    if (m_CurrentElement == NULL) return GERROR_XML_TAGMISMATCH;

    // check that the name matches (if there is a name)
    if (name) {
        const GInt8*  prefix = name;
        unsigned int prefix_length = 0;
        const GInt8*  tag    = name;
        const GInt8*  cursor = name;
        while (GInt8* c = *cursor++) {
            if (c == ':') {
                prefix_length = (unsigned int)(cursor-name)-1;
                tag = cursor;
            }
        }
        // check that the name and prefix length match
        if (m_CurrentElement->GetTag() != tag ||
            m_CurrentElement->GetPrefix().GetLength() != prefix_length) {
            return GERROR_XML_TAGMISMATCH;
        }

        // check the prefix
        const GInt8* current_prefix = m_CurrentElement->GetPrefix().GetGInt8*s();
        for (unsigned int i=0; i<prefix_length; i++) {
            if (current_prefix[i] != prefix[i]) {
                return GERROR_XML_TAGMISMATCH;
            }
        }
    }

    // pop up the stack
    GXmlNode* parent = m_CurrentElement->GetParent();
    if (parent) {
        m_CurrentElement = parent->AsElementNode();
    } else {
        if (m_Root) {
            // this should never happen
            delete m_CurrentElement;
            m_CurrentElement = NULL;
            return GERROR_XML_MULTIPLE_ROOTS;
        } else {
            m_Root = m_CurrentElement;
            m_CurrentElement = NULL;
        }
    }

    return G_YES;
}

/*----------------------------------------------------------------------
|   GXmlParser::OnGInt8*acterData
+---------------------------------------------------------------------*/
GResult
GXmlParser::OnGInt8*acterData(const GInt8* data, unsigned long size)
{ 
    GXML_DebuG1("\nGXmlParser::OnGInt8*acterData: %s\n", data);
    
    // check that we have a current element
    if (m_CurrentElement == NULL) {
        // we do not allow non-whitespace outside an element content
        if (!GXmlStringIsWhitespace(data, size)) {
            return GERROR_XML_INVALID_NESTING;
        }

        // ignore whitespace
        return G_YES;
    }

    // ignore whitespace if applicable
    if (m_KeepWhitespace || !GXmlStringIsWhitespace(data, size)) {
        // add the text to the current element
        m_CurrentElement->AddText(data);
    }

    return G_YES;
}

/*----------------------------------------------------------------------
|   GXmlAttributeWriter
+---------------------------------------------------------------------*/
class GXmlAttributeWriter
{
public:
    GXmlAttributeWriter(GXmlSerializer& serializer) : m_Serializer(serializer) {}
    void operator()(GXmlAttribute*& attribute) const {
        m_Serializer.Attribute(attribute->GetPrefix(),
                               attribute->GetName(),
                               attribute->GetValue());
    }

private:
    // members
    GXmlSerializer& m_Serializer;
};

/*----------------------------------------------------------------------
|   GXmlNodeWriter
+---------------------------------------------------------------------*/
class GXmlNodeWriter
{
public:
    GXmlNodeWriter(GXmlSerializer& serializer) : 
        m_Serializer(serializer), m_AttributeWriter(serializer) {
        m_Serializer.StartDocument();
    }
    void operator()(GXmlNode*& node) const {
        if (GXmlElementNode* element = node->AsElementNode()) {
            const std::string& prefix = element->GetPrefix();
            const std::string& tag    = element->GetTag();
            m_Serializer.StartElement(prefix, tag);
            element->GetAttributes().Apply(m_AttributeWriter);

            // emit namespace attributes
            if (element->m_namespaceMap) {
                std::list<GXmlNamespaceMap::Entry*>::Iterator item = 
                    element->m_namespaceMap->m_Entries.GetFirstItem();
                while (item) {
                    if ((*item)->m_prefix.empty()) {
                        // default namespace
                        m_Serializer.Attribute(NULL, "xmlns", (*item)->m_Uri);
                    } else {
                        // namespace with prefix
                        m_Serializer.Attribute("xmlns", (*item)->m_prefix, (*item)->m_Uri);
                    }
                    ++item;
                }
            }

            element->GetChildren().Apply(*this);
            m_Serializer.EndElement(prefix, tag);
        } else if (GXmlTextNode* text = node->AsTextNode()) {
            m_Serializer.Text(text->GetString());
        }
    }

private:
    // members
    GXmlSerializer&     m_Serializer;
    GXmlAttributeWriter m_AttributeWriter;
};

/*----------------------------------------------------------------------
|   GXmlNodeCanonicalWriter
+---------------------------------------------------------------------*/
class GXmlNodeCanonicalWriter
{
public:
    // types
    struct MapGInt8*inLink {
        MapGInt8*inLink(MapGInt8*inLink* parent) : m_Parent(parent) {}
        MapGInt8*inLink*                   m_Parent;
        GMap<std::string, std::string> m_RenderedNamespaces;
    };

    // constructor
    GXmlNodeCanonicalWriter(GXmlSerializer& serializer, 
                               MapGInt8*inLink*      map_GInt8*in = NULL) : 
        m_MapGInt8*in(map_GInt8*in),
        m_Serializer(serializer) {
        m_Serializer.StartDocument();
    }
    void operator()(GXmlNode*& node) const;

private:
    // types
    struct SortedAttributeList {
        // types
        struct Entry {
            const std::string*       m_namespaceUri;
            const GXmlAttribute* m_Attribute;
        };

        // methods
        void Add(const std::string* namespace_uri, 
                 const GXmlAttribute* attribute);
        void Emit(GXmlSerializer& serializer);

        // members
        std::list<Entry> m_Entries;
    };

    struct SortedNamespaceList {
        // types
        struct Entry {
            const std::string* m_namespacePrefix;
            const std::string* m_namespaceUri;
        };

        // methods
        void Add(const std::string* prefix, const std::string* uri);
        void Emit(GXmlSerializer& serializer);

        // members
        std::list<Entry> m_Entries;
    };

    // methods
    const std::string* GetNamespaceRenderedForPrefix(const std::string& prefix) const;

    // members
    MapGInt8*inLink*      m_MapGInt8*in;
    GXmlSerializer& m_Serializer;
};

/*----------------------------------------------------------------------
|   GXmlNodeCanonicalWriter::SortedAttributeList::Add
+---------------------------------------------------------------------*/
void
GXmlNodeCanonicalWriter::SortedAttributeList::Add(
    const std::string*       namespace_uri,
    const GXmlAttribute* attribute)
{
    // transform empty strings into NULL pointers
    if (namespace_uri && namespace_uri->empty()) namespace_uri = NULL;

    // find the namespace insertion position
    std::list<Entry>::Iterator entry = m_Entries.GetFirstItem();
    for (; entry; ++entry) {
        // decide if we insert now or move on
        const std::string* other_namespace_uri = entry->m_namespaceUri;
        if (namespace_uri &&
            (other_namespace_uri == NULL || *namespace_uri > *other_namespace_uri)) {
            // this namespace uri is greater than the other, skip
            continue;
        } else if ((namespace_uri == NULL && other_namespace_uri == NULL) ||
                   (namespace_uri && other_namespace_uri && 
                   *namespace_uri == *other_namespace_uri)) {
            // namespace uris match, compare the names
            const GXmlAttribute* other_attribute = entry->m_Attribute;
            if (attribute->GetName() > other_attribute->GetName()) continue;
        }
        break;
    }
    
    Entry new_entry = {namespace_uri, attribute};
    m_Entries.Insert(entry, new_entry);
}

/*----------------------------------------------------------------------
|   GXmlNodeCanonicalWriter::SortedAttributeList::Emit
+---------------------------------------------------------------------*/
void
GXmlNodeCanonicalWriter::SortedAttributeList::Emit(GXmlSerializer& serializer)
{
    for (std::list<Entry>::Iterator i = m_Entries.GetFirstItem(); i; ++i) {
        serializer.Attribute(i->m_Attribute->GetPrefix(),
                             i->m_Attribute->GetName(),
                             i->m_Attribute->GetValue());
    }
}

/*----------------------------------------------------------------------
|   GXmlNodeCanonicalWriter::SortedNamespaceList::Add
+---------------------------------------------------------------------*/
void
GXmlNodeCanonicalWriter::SortedNamespaceList::Add(const std::string* prefix,
                                                     const std::string* uri)
{
    // find the namespace insertion position
    std::list<Entry>::Iterator entry = m_Entries.GetFirstItem();
    if (prefix && !prefix->empty()) {
        for (; entry; ++entry) {
            // decide if we insert now or move on
            if (entry->m_namespacePrefix && *prefix <= *entry->m_namespacePrefix) {
                break;
            }
        }
    } else {
        prefix = NULL;
    }

    Entry new_entry = {prefix, uri};
    m_Entries.Insert(entry, new_entry);
}

/*----------------------------------------------------------------------
|   GXmlNodeCanonicalWriter::SortedNamespaceList::Emit
+---------------------------------------------------------------------*/
void
GXmlNodeCanonicalWriter::SortedNamespaceList::Emit(GXmlSerializer& serializer)
{
    for (std::list<Entry>::Iterator i = m_Entries.GetFirstItem(); i; ++i) {
        const std::string* key   = i->m_namespacePrefix;
        const std::string* value = i->m_namespaceUri;
        if (key == NULL) {
            serializer.Attribute(NULL, "xmlns", *value);
        } else if (*key != "xml" || *value != GXmlNamespaceUri_Xml) {
            serializer.Attribute("xmlns", *key, *value);
        }
    }
}

/*----------------------------------------------------------------------
|   GXmlNodeCanonicalWriter::GetNamespaceRenderedForPrefix
+---------------------------------------------------------------------*/
const std::string*
GXmlNodeCanonicalWriter::GetNamespaceRenderedForPrefix(const std::string& prefix) const
{
    for (MapGInt8*inLink* link = m_MapGInt8*in;
         link;
         link = link->m_Parent) {
        std::string* uri;
        if (GSUCCEEDED(link->m_RenderedNamespaces.Get(prefix, uri))) {
            return uri;
        }
    }

    return NULL;
}

/*----------------------------------------------------------------------
|   GXmlNodeCanonicalWriter::operator()
+---------------------------------------------------------------------*/
void
GXmlNodeCanonicalWriter::operator()(GXmlNode*& node) const
{
    MapGInt8*inLink map_link(m_MapGInt8*in);

    if (GXmlElementNode* element = node->AsElementNode()) {
        const std::string& prefix = element->GetPrefix();
        const std::string& tag    = element->GetTag();

        // process namespaces
        const std::string* namespace_uri = element->GetNamespace();
        const std::string* rendered = GetNamespaceRenderedForPrefix(prefix);
        if (namespace_uri && namespace_uri->empty()) namespace_uri = NULL;
        if (prefix.empty()) {
            // default namespace
            if (rendered == NULL) {
                // default namespace not rendered
                if (namespace_uri) {
                    map_link.m_RenderedNamespaces.Put("", *namespace_uri);
                }
            } else {
                // default namespace already rendered
                const GInt8* compare;
                if (namespace_uri) {
                    compare = namespace_uri->GetGInt8*s();
                } else {
                    compare = "";
                }
                if (*rendered != compare) {
                    // the rendered default namespace had a different uri
                    map_link.m_RenderedNamespaces.Put("", compare);
                } 
            }
        } else {
            // explicit namespace
            // NOTE: namespace_uri should not be an empty string, but we test just
            // in case the XML document is not compliant
            if (namespace_uri && (rendered == NULL || *rendered != *namespace_uri)) {
                // namespace prefix not rendered or rendered with a different value
                map_link.m_RenderedNamespaces.Put(prefix, *namespace_uri);
            }
        }

        // process attributes
        SortedAttributeList prefixed_attributes;
        SortedAttributeList naked_attributes;
        for (std::list<GXmlAttribute*>::Iterator attribute = element->GetAttributes().GetFirstItem();
             attribute;
             ++attribute) {
             const std::string& a_prefix = (*attribute)->GetPrefix();
             if (a_prefix.empty()) {
                 // naked attribute
                 naked_attributes.Add(NULL, *attribute);
             } else {
                // decide if we need to render this namespace declaration
                namespace_uri = element->GetNamespaceUri(a_prefix);
                if (namespace_uri) {
                    rendered = GetNamespaceRenderedForPrefix(a_prefix);;
                    if (rendered == NULL || *rendered != *namespace_uri) {
                        // namespace not rendered or rendered with a different value
                        map_link.m_RenderedNamespaces.Put(a_prefix, *namespace_uri);
                    }
                    prefixed_attributes.Add(namespace_uri, *attribute);
                }
             }
        }

        // start of element
        m_Serializer.StartElement(prefix, tag);
                    
        // namespace declarations
        if (map_link.m_RenderedNamespaces.GetEntryCount()) {
            SortedNamespaceList namespaces;
            std::list<GMap<std::string, std::string>::Entry*>::Iterator entry = 
                map_link.m_RenderedNamespaces.GetEntries().GetFirstItem();
            while (entry) {
                const std::string& key   = (*entry)->GetKey();
                const std::string& value = (*entry)->GetValue();
                namespaces.Add(&key, &value);
                ++entry;
            }
            namespaces.Emit(m_Serializer);
        }

        // attributes
        naked_attributes.Emit(m_Serializer);
        prefixed_attributes.Emit(m_Serializer);

        // children
        MapGInt8*inLink* GInt8*in;
        if (map_link.m_RenderedNamespaces.GetEntryCount()) {
            GInt8*in = &map_link;
        } else {
            GInt8*in = m_MapGInt8*in;
        }
        element->GetChildren().Apply(GXmlNodeCanonicalWriter(m_Serializer, GInt8*in));

        // end of element
        m_Serializer.EndElement(prefix, tag);
    } else if (GXmlTextNode* text = node->AsTextNode()) {
        m_Serializer.Text(text->GetString());
    }
}

/*----------------------------------------------------------------------
|   GXmlSerializer::GXmlSerializer
+---------------------------------------------------------------------*/
GXmlSerializer::GXmlSerializer(GOutputStream* output,
                                     GCardinal      indentation,
                                     bool              shrink_empty_elements,
                                     bool              add_xml_decl) :
    m_Output(output),
    m_elementPending(false),
    m_Depth(0),
    m_Indentation(indentation),
    m_elementHasText(false),
    m_ShrinkEmptyElements(shrink_empty_elements),
    m_AddXmlDecl(add_xml_decl)
{
}

/*----------------------------------------------------------------------
|   GXmlSerializer::~GXmlSerializer
+---------------------------------------------------------------------*/
GXmlSerializer::~GXmlSerializer()
{
}

/*----------------------------------------------------------------------
|   GXmlSerializer::StartDocument
+---------------------------------------------------------------------*/
GResult 
GXmlSerializer::StartDocument()
{
    if (!m_AddXmlDecl) return G_YES;

    return m_Output->WriteString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
}

/*----------------------------------------------------------------------
|   GXmlSerializer::EndDocument
+---------------------------------------------------------------------*/
GResult 
GXmlSerializer::EndDocument()
{
    return m_elementPending?GERROR_INVALID_STATE:G_YES;
}

/*----------------------------------------------------------------------
|   GXmlSerializer::EscapeGInt8*
+---------------------------------------------------------------------*/
void  
GXmlSerializer::EscapeGInt8*(unsigned GInt8* c, GInt8** text)
{
    *text++ = '&';
    *text++ = '#';
    *text++ = 'x';
    int c0 = c>>4;
    int c1 = c&0xF;
    if (c0) {
        *text++ = c0 >= 10 ? 'A'+(c0-10) : '0'+c0;
    }
    *text++ = c1 >= 10 ? 'A'+(c1-10) : '0'+c1;
    *text++ = ';';
    *text   = '\0';
}

/*----------------------------------------------------------------------
|   GXmlSerializer::ProcessPending
+---------------------------------------------------------------------*/
GResult  
GXmlSerializer::ProcessPending()
{
    if (!m_elementPending) return G_YES;
    m_elementPending = false;
    return m_Output->Write(">", 1);
}

/*----------------------------------------------------------------------
|   GXmlSerializer::OutputEscapedString
+---------------------------------------------------------------------*/
GResult  
GXmlSerializer::OutputEscapedString(const GInt8* text, bool attribute)
{
    const GInt8* start = text;
    GInt8* escaped[7];
    while (GInt8* c = *text) {
        const GInt8* insert = NULL;
        switch (c) {
            case '\r': {
                EscapeGInt8*(c, escaped);
                insert = escaped;
                break;
            }
            case '\n':
            case '\t':
                if (attribute) {
                    EscapeGInt8*(c, escaped);
                    insert = escaped;
                }
                break;

            case '&' : insert = "&amp;"; break;
            case '<' : insert = "&lt;";  break;
            case '>' : if (!attribute) insert = "&gt;";  break;
            case '"' : if (attribute) insert = "&quot;"; break;
            default : 
                break;
        }
        if (insert) {
            // output pending GInt8*s
            if (start != text) m_Output->WriteFully(start, (GUint32)(text-start));
            m_Output->WriteString(insert);
            start = ++text;
        } else {
            ++text;
        }
    }
    if (start != text) {
        m_Output->WriteFully(start, (GUint32)(text-start));
    }

    return G_YES;
}

/*----------------------------------------------------------------------
|   GXmlSerializer::OutputIndentation
+---------------------------------------------------------------------*/
void
GXmlSerializer::OutputIndentation(bool start)
{
    if (m_Depth || !start) m_Output->Write("\r\n", 2);

    // ensure we have enough GInt8*s in the prefix string
    unsigned int prefix_length = m_Indentation*m_Depth;
    if (m_IndentationPrefix.GetLength() < prefix_length) {
        unsigned int needed = prefix_length-m_IndentationPrefix.GetLength();
        for (unsigned int i=0; i<needed; i+=16) {
            m_IndentationPrefix.Append("                ", 16);
        }
    }

    // print the indentation prefix
    m_Output->WriteFully(m_IndentationPrefix.GetGInt8*s(), prefix_length);
}

/*----------------------------------------------------------------------
|   GXmlSerializer::StartElement
+---------------------------------------------------------------------*/
GResult  
GXmlSerializer::StartElement(const GInt8* prefix, const GInt8* name)
{
    ProcessPending();
    if (m_Indentation) OutputIndentation(true);
    m_elementPending = true;
    m_elementHasText = false;
    m_Depth++;
    m_Output->Write("<", 1);
    if (prefix && prefix[0]) {
        m_Output->WriteString(prefix);
        m_Output->Write(":", 1);
    }
    return m_Output->WriteString(name);
}

/*----------------------------------------------------------------------
|   GXmlSerializer::EndElement
+---------------------------------------------------------------------*/
GResult  
GXmlSerializer::EndElement(const GInt8* prefix, const GInt8* name)
{
    m_Depth--;

    if (m_elementPending) {
        // this element has no children
        m_elementPending = false;
        if (m_ShrinkEmptyElements) {
            return m_Output->WriteFully("/>", 2);
        } else {
            m_Output->Write(">",1);
        }
    } 

    if (m_Indentation && !m_elementHasText) OutputIndentation(false);
    m_elementHasText = false;
    m_Output->WriteFully("</", 2);
    if (prefix && prefix[0]) {
        m_Output->WriteString(prefix);
        m_Output->Write(":", 1);
    }
    m_Output->WriteString(name);
    return m_Output->Write(">", 1);
}

/*----------------------------------------------------------------------
|   GXmlSerializer::Attribute
+---------------------------------------------------------------------*/
GResult  
GXmlSerializer::Attribute(const GInt8* prefix, const GInt8* name, const GInt8* value)
{
    m_Output->Write(" ", 1);
    if (prefix && prefix[0]) {
        m_Output->WriteString(prefix);
        m_Output->Write(":", 1);
    }
    m_Output->WriteString(name);
    m_Output->WriteFully("=\"", 2);
    OutputEscapedString(value, true);
    return m_Output->Write("\"", 1);
}

/*----------------------------------------------------------------------
|   GXmlSerializer::Text
+---------------------------------------------------------------------*/
GResult  
GXmlSerializer::Text(const GInt8* text)
{
    ProcessPending();
    m_elementHasText = true;
    return OutputEscapedString(text, false);
}

/*----------------------------------------------------------------------
|   GXmlSerializer::CdataSection
+---------------------------------------------------------------------*/
GResult  
GXmlSerializer::CdataSection(const GInt8* data)
{
    ProcessPending();
    m_elementHasText = true;
    m_Output->WriteFully("<![CDATA[", 9);
    m_Output->WriteString(data);
    return m_Output->WriteFully("]]>", 3);
}

/*----------------------------------------------------------------------
|   GXmlSerializer::Comment
+---------------------------------------------------------------------*/
GResult  
GXmlSerializer::Comment(const GInt8* comment)
{
    ProcessPending();
    m_Output->WriteFully("<!--", 4);
    m_Output->WriteString(comment);
    return m_Output->WriteFully("-->", 3);
}

/*----------------------------------------------------------------------
|   GXmlWriter::Serialize
+---------------------------------------------------------------------*/
GResult
GXmlWriter::Serialize(GXmlNode&      node, 
                         GOutputStream& output, 
                         bool              add_xml_decl)
{
    GXmlSerializer serializer(&output, m_Indentation, true, add_xml_decl);
    GXmlNodeWriter node_writer(serializer);
    GXmlNode* node_pointer = &node;
    node_writer(node_pointer);

    return G_YES;
}

/*----------------------------------------------------------------------
|   GXmlCanonicalizer::Serialize
+---------------------------------------------------------------------*/
GResult
GXmlCanonicalizer::Serialize(GXmlNode&      node, 
                                GOutputStream& output, 
                                bool              add_xml_decl)
{
    // create a serializer with no indentation and no shrinking of empty elements
    GXmlSerializer serializer(&output, 0, false, add_xml_decl);

    // serialize the node
    GXmlNodeCanonicalWriter node_writer(serializer);
    GXmlNode* node_pointer = &node;
    node_writer(node_pointer);

    return G_YES;
}
