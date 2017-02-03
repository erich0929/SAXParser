#pragma once

#include "stdafx.h"
#include <stdexcept>
#include <sstream>

using namespace std;

namespace sax_parser {
	class xml_token;
	class xml_scanner;

	class parsing_exception : public runtime_error
	{
	private:
		string m_error_msg;
		mutable ostringstream m_ss;
	public:
		parsing_exception(string error_msg) : runtime_error("[XML Parsing ERROR]"), m_error_msg(error_msg), m_ss() 
		{
			m_ss << runtime_error::what() << m_error_msg;
			m_error_msg = m_ss.str();
		};
		virtual const char* what() const throw() { 
			return m_error_msg.c_str();
		};
	};

	class xml_handler 
	{
	public:
		virtual void start_element(const string& tagname, const vector< pair<string, string> >& attrs) = 0;
		virtual void end_element(const string& tagname) = 0;
		virtual void characters(const char *char_data, int length) = 0;
	};

	class xml_parser 
	{
	private:
		enum { STAG = 0, EMPTYTAG, ETAG, CDATA, REFERENCE, CHARDATA, COMMENT, DECLARATION };

		int document(xml_scanner& scanner, xml_token *tokens[]) throw(parsing_exception);
		int prolog(xml_scanner& scanner, xml_token *tokens[]) throw(parsing_exception);
		int misc(xml_scanner& scanner, xml_token *tokens[]) throw(parsing_exception);
		int element(xml_scanner& scanner, xml_token *tokens[]) throw(parsing_exception);
		int content(xml_scanner& scanner, xml_token *tokens[]) throw(parsing_exception);

	public:
		xml_parser();

		void parse(string& filename, xml_handler& handler) throw(parsing_exception);
		void parse(istream& stream, xml_handler& handler) throw(parsing_exception);
	};
}