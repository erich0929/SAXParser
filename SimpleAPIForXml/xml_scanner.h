#pragma once

#include "stdafx.h"
#include "xml_buffer.h"
#include "xml_token.h"
#include "xml_parser.h"

namespace sax_parser {
	class xml_scanner 
	{
	private:
		xml_buffer *m_buffer;
		xml_token **m_tokens_ptr;
		size_t m_tokens_count;

		xml_token *m_current_token;
		int m_old_offset;
		int m_offset;
	public:
		explicit xml_scanner(xml_buffer *buffer, xml_token *tokens_ptr[], size_t tokens_count);
		int skipws();
		int match(xml_token *token_ptr);
		int advance();
		int match_advance(xml_token *token_ptr);
		int required(xml_token *token_ptr) throw (parsing_exception);
		xml_token* get_currunt_token();
		void throw_parsing_error(char* perr_msg);
	};
}
