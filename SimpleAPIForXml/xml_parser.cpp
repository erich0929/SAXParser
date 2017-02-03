#include "stdafx.h"
#include <cassert>
#include <sstream>
#include "xml_scanner.h"
#include "xml_buffer.h"
#include "xml_parser.h"
#include "xml_cdata.h"
#include "xml_chardata.h"
#include "xml_comment.h"
#include "xml_decl.h"
#include "xml_emptytag.h"
#include "xml_etag.h"
#include "xml_reference.h"
#include "xml_stag.h"

namespace sax_parser {
	
	xml_parser::xml_parser()
	{
	}
	void xml_parser::parse(string & filename, xml_handler& handler) throw(parsing_exception)
	{
		fstream xml_file(filename);
		parse(xml_file, handler);
	}

	void xml_parser::parse(istream& stream, xml_handler& handler) throw(parsing_exception)
	{
		xml_buffer buffer(&stream);
		
		xml_stag		STAG(&buffer, &handler);
		xml_emptytag	EMPTYTAG(&buffer, &handler);
		xml_etag		ETAG(&buffer, &handler);
		xml_cdata		CDATA(&buffer, &handler);
		xml_reference	REFERENCE(&buffer, &handler);
		xml_chardata	CHARDATA(&buffer, &handler);
		xml_comment		COMMENT(&buffer, &handler);
		xml_decl		DECLARATION(&buffer, &handler);
		
		xml_token *tokens[] =
		{
			&STAG, &EMPTYTAG, &ETAG, &CDATA, &REFERENCE, &CHARDATA, &COMMENT, &DECLARATION
		};

		xml_scanner scanner(&buffer, tokens, sizeof(tokens) / sizeof(xml_token*));
		scanner.advance();
		document(scanner, tokens);
	}

	// document := prolog element misc*
	int xml_parser::document(xml_scanner & scanner, xml_token *_[]) throw(parsing_exception)
	{
		xml_token **tokens = &_[0];
		prolog(scanner, tokens);
		element(scanner, tokens);
		misc(scanner, tokens);

		return 0;
	}

	// prolog := xmldecl? misc*
	int xml_parser::prolog(xml_scanner & scanner, xml_token *_[]) throw(parsing_exception)
	{
		xml_token **tokens = &_[0];
		scanner.match_advance(_[DECLARATION]);
		while (misc(scanner, tokens));

		return 1;
	}

	int xml_parser::misc(xml_scanner & scanner, xml_token *_[]) throw(parsing_exception)
	{
		xml_token **tokens = &_[0];
		if (scanner.match_advance(_[COMMENT])) return 1;
		
		return scanner.skipws();
	}

	// element := emptytag | stag content etag
	int xml_parser::element(xml_scanner & scanner, xml_token *_[]) throw(parsing_exception)
	{
		xml_token **tokens = &_[0];
		if (scanner.match(_[EMPTYTAG])) {
			if (_[EMPTYTAG]->status()) {
				scanner.advance();
				return 1;
			}
			else {
				_[EMPTYTAG]->throw_parsing_error();
			}
		}

		if (scanner.match(_[STAG])) {
			string stagname = static_cast<xml_stag*>(_[STAG])->get_tagname();
			scanner.advance();
			while (content(scanner, tokens));
			string etagname = static_cast<xml_etag*>(_[ETAG])->get_tagname();
			scanner.required(_[ETAG]);
			if (stagname != etagname) {
				scanner.throw_parsing_error(" 시작태그의 이름과 종료태그의 이름이 일치하지 않습니다.");
			}
			return 1;
		}

		return 0;
	}

	// content := element | chardata | reference | cdata | comment
	int xml_parser::content(xml_scanner & scanner, xml_token *_[]) throw(parsing_exception)
	{
		xml_token **tokens = &_[0];

		if (element(scanner, tokens)) return 1;
		if (scanner.match_advance(_[CHARDATA])) return 1;
		if (scanner.match_advance(_[REFERENCE])) return 1;
		if (scanner.match_advance(_[CDATA])) return 1;
		if (scanner.match_advance(_[COMMENT])) return 1;

		return 0;
	}
}