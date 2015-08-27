//
// The ''game'' is a simple print test program that uses the test classes
// and prints out their state.
//
#include <iostream>
#include <map>

#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

#include "handler.hpp"

using namespace std;
using namespace xercesc;

static void printDict( string name, map<string,string> * dict )
{
	cout << "Dictionary: " << name  << endl;
	if ( dict == 0 )
		return;
	cout << "size = " << dict->size() << endl;
	for ( auto i : *dict ) {
		cout << i.first << " -> "  << i.second << endl;
	}
}

int main( int argc, char **argv )
{
	if ( argc < 2 ) {
		cout << "Usage: parser file1.xml [ file2.xml ... ]" << endl;
		return -1;
	}

	try {
		// Initialize the Xerces system.
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& e) {
		cerr << "Xerces system initialization failed: " << e.getMessage() << endl;
		return 1;
	}

	// Parse document here
	SAXParser *parser = new SAXParser;
	parser->setValidationScheme( SAXParser::Val_Auto );
	parser->setDoNamespaces( false );
	parser->setDoSchema( false );
	parser->setHandleMultipleImports( true );
	parser->setValidationSchemaFullChecking( false );

	DictionaryHandler handler;
	parser->setDocumentHandler( &handler );
	parser->setErrorHandler( &handler );

	int errors = 0;

	for (int i = 1; i < argc; i++) {
		try {
			cout << "File     : " << argv [ i ] << endl;

			handler.resetErrors();
			parser->parse( argv[i] );

			cout << "Warnings : " << handler.getWarningCount() << endl;
			cout << "Errors   : " << handler.getErrorCount() << endl;

			// use hanlder output here
			printDict( handler.getDictionaryName(), handler.getDictionary() );
		}
		catch (const OutOfMemoryException&) {
			cerr << "OutOfMemoryException" << endl;
			errors++;
		}
		catch (const XMLException& e) {
			cerr << "XMLException" << endl;
			cerr << "\tfile:\t" << argv[ i ] << endl;
			char *message = XMLString::transcode( e.getMessage() );
			cerr << "\tmessage:\t" << message << endl;
			XMLString::release( &message );
			errors++;
		}
	}

	delete parser;

	// Shutdown the Xerces system.
	XMLPlatformUtils::Terminate();

	return errors;
}

