//
//
//

#include <iostream>
#include <xercesc/sax/AttributeList.hpp>
#include "handler.hpp"

using namespace std;

DictionaryHandler::DictionaryHandler() :
		errorCount(0), warningCount(0), haveKey(false), key(""), value(""),
		dictName (""), dict(0)
{
}

DictionaryHandler::~DictionaryHandler()
{
	if ( dict != 0 ) {
		delete dict;
		dict = 0;
	}
}

//
//  Getter methods
//
size_t DictionaryHandler::getErrorCount() const
{
	return errorCount;
}

size_t DictionaryHandler::getWarningCount() const
{
	return warningCount;
}

map<string,string> *DictionaryHandler::getDictionary() const
{
	return dict;
}

string DictionaryHandler::getDictionaryName() const
{
	return dictName;
}

//
//  Handlers for the SAX DocumentHandler interface
//
void DictionaryHandler::startDocument()
{
	//cout << __FUNCTION__ << endl;
	if ( dict != 0 ) {
		delete dict;
		dict = 0;
	}
	dict = new map<string,string>;
}

void DictionaryHandler::endDocument()
{
	//cout << __FUNCTION__ << endl;
}

void DictionaryHandler::resetDocument()
{
	//cout << __FUNCTION__ << endl;
	errorCount = 0;
	warningCount = 0;
	locator = 0;
}

void DictionaryHandler::startElement(
		const XMLCh* const xmlName,
		AttributeList& attributes)
{
	//cout << __FUNCTION__ << endl;
	char *name = XMLString::transcode( xmlName );
	//cout << "Element: " << name << endl;
	if ( strncmp( name, "value", 5 ) == 0 ) {
		key = findKey( attributes );
		if ( ! key.empty() )
			haveKey = true;
	//cout << __FUNCTION__ << " value:key " << key << endl;
	}
	else if ( strncmp( name, "dictionary", 10 ) == 0 ) {
		dictName = findName( attributes );
	}
	XMLString::release( &name );
}

void DictionaryHandler::endElement()
{
	// This function is never called!
	cout << __FUNCTION__ << endl;
	if ( haveKey ) {
		haveKey = false;
		key.clear();
		value.clear();
	}
}

void DictionaryHandler::characters(
		const XMLCh* const xmlChars,
		const XMLSize_t length)
{
	//cout << __FUNCTION__ << endl;
	if ( haveKey && length > 0 ) {
		char *chars = XMLString::transcode( xmlChars );
		value = string( chars );
		XMLString::release( &chars );
		//cout << __FUNCTION__ << " ";
		//cout << key << " -> " << value << endl;
		(*dict)[ key ] = value;
		haveKey = false;
		key.clear();
		value.clear();
	}
}

void DictionaryHandler::ignorableWhitespace(
		const XMLCh* const chars,
		const XMLSize_t length)
{
	//cout << __FUNCTION__ << endl;
}

void DictionaryHandler::processingInstruction(
		const XMLCh * const target,
		const XMLCh * const data)
{
	//cout << __FUNCTION__ << endl;
}

void DictionaryHandler::setDocumentLocator(
		const Locator * const locator)
{
	this->locator = locator;
}

void DictionaryHandler::printLocation() const
{
	if ( locator == 0 ) {
		cout << "No locator." << endl;
		return;
	}
	const XMLCh *xmlString = locator->getPublicId();
	if ( xmlString ) {
		char *publicId = XMLString::transcode( xmlString );
		cout << "publicId : " << publicId << endl;
		XMLString::release( &publicId );
	}

	xmlString = locator->getSystemId();
	if ( xmlString ) {
		char *systemId = XMLString::transcode( xmlString );
		cout << "systemId : " << systemId << endl;
		XMLString::release( &systemId );
	}

	XMLFileLoc line = locator->getLineNumber();
	cout << "line     : " << line << endl;
	XMLFileLoc column = locator->getLineNumber();
	cout << "column   : " << column << endl;
}

//
//  Handlers for the SAX ErrorHandler interface
//
void DictionaryHandler::warning(const SAXParseException& e)
{
	warningCount++;

	char *message = XMLString::transcode( e.getMessage() );
	cout << "Warning: " << message << endl;
	XMLString::release( &message );
	printLocation();
}

void DictionaryHandler::error(const SAXParseException& e)
{
	errorCount++;

	char *message = XMLString::transcode( e.getMessage() );
	cout << "Error: " << message << endl;
	XMLString::release( &message );
	printLocation();
}

void DictionaryHandler::fatalError(const SAXParseException& e)
{
	errorCount++;

	char *message = XMLString::transcode( e.getMessage() );
	cout << "Fatal error: " << message << endl;
	XMLString::release( &message );
	printLocation();
}

void DictionaryHandler::resetErrors()
{
	errorCount = 0;
	warningCount = 0;
}

string DictionaryHandler::findKey( AttributeList& attributes )
{
	string str;
	XMLSize_t i = 0;
	XMLSize_t numAttributes = attributes.getLength();

	for ( i = 0; i < numAttributes; i++ ) {
		const XMLCh *xmlName = attributes.getName( i );
		char *name = XMLString::transcode( xmlName );
		if ( strncmp( name, "key", 4 ) == 0 ) {
			const XMLCh *xmlValue = attributes.getValue( i );
			char *value = XMLString::transcode( xmlValue );
			str = string( static_cast<const char *>( value ) );
			XMLString::release( &value );
		}
		XMLString::release( &name );
	}

	return str;
}

string DictionaryHandler::findName( AttributeList& attributes )
{
	string str;
	XMLSize_t i = 0;
	XMLSize_t numAttributes = attributes.getLength();

	for ( i = 0; i < numAttributes; i++ ) {
		const XMLCh *xmlName = attributes.getName( i );
		char *name = XMLString::transcode( xmlName );
		if ( strncmp( name, "name", 5 ) == 0 ) {
			const XMLCh *xmlValue = attributes.getValue( i );
			char *value = XMLString::transcode( xmlValue );
			str = string( static_cast<const char *>( value ) );
			XMLString::release( &value );
		}
		XMLString::release( &name );
	}

	return str;
}

