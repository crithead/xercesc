//
// SAX Parser Handler
//   derives from HandlerBase
//   overrides functions from DocumentHandler and ErrorHandler
//
#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <map>
#include <string>
#include <xercesc/sax/HandlerBase.hpp>

XERCES_CPP_NAMESPACE_USE

//
// The DictionaryHandler populates an STL map with key-value pairs
// defined the input XML file.
//
class DictionaryHandler : public HandlerBase
{
public:
	DictionaryHandler();
	~DictionaryHandler();

	//
	//  Getter methods
	//
	size_t getErrorCount() const;
	size_t getWarningCount() const;
	std::map<std::string,std::string> *getDictionary() const;
	std::string getDictionaryName() const;

	//
	//  SAX DocumentHandler interface
	//
	void startDocument();
	void endDocument();
	void resetDocument();

	void startElement(const XMLCh* const name, AttributeList& attributes);
	void endElement();

	void characters(const XMLCh* const chars, const XMLSize_t length);
	void ignorableWhitespace(const XMLCh* const chars, const XMLSize_t length);
	void processingInstruction(const XMLCh * const target, const XMLCh * const data);
	void setDocumentLocator(const Locator * const locator);

	//
	//  SAX ErrorHandler interface
	//
	void warning(const SAXParseException& exc);
	void error(const SAXParseException& exc);
	void fatalError(const SAXParseException& exc);
	void resetErrors();

private:
	std::string findKey( AttributeList& attributes );
	std::string findName( AttributeList& attributes );

	int errorCount;
	int warningCount;
	bool haveKey;
	std::string key;
	std::string value;
	std::string dictName;
	std::map<std::string,std::string> *dict;
};

#endif // HANDLER_HPP

