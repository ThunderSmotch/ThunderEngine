export module ButtonGame.XML;

import std;
import ThunderLib.Base;

/**********************************
* Code related to parsing XML files
* 
* Currently only parses XML files ignoring all non-element content.
*	- Eg. in an HTML file the text inside nodes would be ignored.
***********************************/

// NOTE: Only the ELEMENT type is being used as of now, this is here for future-proofing
// as we may want to store every time of node of an XML file in the future.
export {

	enum class NodeType
	{
		UNKNOWN = 0,  // <!D and <!A and <!E fit here
		ELEMENT, // <name> </name> or <name/>
		PROCESSING_INSTRUCTION, // <? ?>
		COMMENT, // <!-- -->
		CDATA, // <![CDATA[  ]]>
	};

	class XMLNode
	{
	public:
		NodeType type = NodeType::UNKNOWN;
		XMLNode* parent = nullptr;
		std::string name;
		std::unordered_map<std::string, std::string> attributes;

		explicit XMLNode(NodeType type);
		size_t GetChildCount() const { return _number_of_children; }
		void AddChild(std::unique_ptr<XMLNode>&& child);
		XMLNode* GetChild(size_t index);
		XMLNode* GetFirstChild(std::string_view name);

	private:
		std::unique_ptr<XMLNode> _next_sibling = nullptr;
		XMLNode* _previous_sibling = nullptr;

		std::unique_ptr<XMLNode> _child = nullptr;
		size_t _number_of_children = 0;
	};

	class XMLParser
	{
	private:
		u32* _beg = nullptr;
		u32* _end = nullptr;
		u32* _cur = nullptr;

		std::string path;

		std::unique_ptr<XMLNode> _root = nullptr;

	public:
		explicit XMLParser(std::vector<u32>& data);
		explicit XMLParser(std::string_view path);
		bool Parse();
		XMLNode* GetRootElement() { return _root.get(); }

	private:
		void Throw(const char* error, const u32* at) const;

		void ParseProlog();

		std::unique_ptr<XMLNode> ParseElement(XMLNode* parent = nullptr);
		bool ParseStartTag(XMLNode* element);
		void ParseAttribute(XMLNode* element);
		std::string ParseName();
		void ParseEndTag(const XMLNode* element);

		void ParseComment();
		void ParseProcessingInstruction();

		// Utility functions
		void SkipWhiteSpace();
	};
}