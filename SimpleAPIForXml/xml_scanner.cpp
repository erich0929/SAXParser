#include "stdafx.h"
#include "xml_scanner.h"

namespace sax_parser {
	xml_scanner::xml_scanner(xml_buffer *buffer, xml_token * tokens_ptr[], size_t tokens_count)
		: m_buffer(buffer), m_tokens_ptr(tokens_ptr), m_tokens_count(tokens_count), m_current_token(), m_old_offset(0), m_offset(0)
	{
	}

	int xml_scanner::skipws()
	{
		m_old_offset = m_offset;
		if (isspace(m_buffer->get_ch(m_offset))) {
			do {
				m_offset++;
			} while (isspace(m_buffer->get_ch(m_offset)));

			return 1;
		}
		return 0;
	}

	int xml_scanner::match(xml_token * token_ptr)
	{
		if (token_ptr == m_current_token) {
			if (m_current_token->status() == xml_token::MATCHED) {
				m_current_token->handle();
				return 1;
			}
			else {
				m_current_token->throw_parsing_error();
			}
		}

		return 0;
	}

	int xml_scanner::advance()
	{
		xml_token *second_token = m_tokens_ptr[0];
		int next_offset = 0, second_offset = 0;
		
		m_old_offset = m_offset;

		for (int i = 0; i < m_tokens_count; i++) {
			if (m_tokens_ptr[i]->match(m_offset, &next_offset)) {
				m_current_token = m_tokens_ptr[i];
				m_offset = next_offset;
				return 1;
			}
			else {
				if (next_offset > second_offset) {
					second_offset = next_offset;
					second_token = m_tokens_ptr[i];
				}
			}
		}

		m_current_token = second_token;
		
		return 0;
	}

	int xml_scanner::match_advance(xml_token * token_ptr)
	{
		if (!match(token_ptr)) return 0;
		advance();
		return 1;
	}

	int xml_scanner::required(xml_token * token_ptr) throw(parsing_exception)
	{
		if (token_ptr != m_current_token) {
			throw_parsing_error("잘못된 요소입니다.");
		}
		
		if (m_current_token->status() == xml_token::FAILURE) {
			m_current_token->throw_parsing_error();
		}
		
		m_current_token->handle();
		advance();
		return 1;
	}

	xml_token * xml_scanner::get_currunt_token()
	{
		return m_current_token;
	}

	void xml_scanner::throw_parsing_error(char* perr_msg)
	{
		ostringstream str_msg;
		int line_no, old_lineno;
		line_no = old_lineno = m_buffer->get_line_no(m_old_offset);
		int offset = m_buffer->get_index(old_lineno);
		int line_start = offset;

		char u8[10];
		int length = 0;

		str_msg << " " << old_lineno << "줄, " << m_old_offset - line_start << "칸에서" <<  perr_msg << '\n';
		while (old_lineno == line_no) {
			memset(u8, '\0', 4);
			m_buffer->get_u8(offset, u8, &length);
			for (int i = 0; i < length; i++) {
				str_msg << u8[i];
			}
			offset += length;
			line_no = m_buffer->get_line_no(offset);
		}

		for (int i = 0; line_start + i < offset; i++) {
			if ((line_start + i) == m_old_offset) str_msg << "^";
			else str_msg << "_";
		}

		str_msg << endl;
		throw parsing_exception(str_msg.str());
	}

	
}

