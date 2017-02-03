#include "stdafx.h"
#include <cassert>
#include <sstream>
#include "xml_token.h"

namespace sax_parser {

	xml_token::xml_token(xml_buffer * buffer, xml_handler *handler) : m_buffer(buffer), m_status(MATCHED), m_stopped_offset(-1), m_handler(handler)
	{
	}

	void xml_token::handle()
	{
		do_handle();
	}

	void xml_token::do_handle()
	{
		/* default - do nothing */
	}

	void xml_token::throw_parsing_error() throw(parsing_exception)
	{
		assert(m_status == FAILURE);
		stringstream str_msg;

		int line_no, old_lineno;
		line_no = old_lineno = m_buffer->get_line_no(m_stopped_offset);
		int offset = m_buffer->get_index(old_lineno);
		int line_start = offset;

		char u8[10];
		int length = 0;
		
		str_msg << " " << old_lineno << "줄, " << m_stopped_offset - line_start << "칸에서 오류가 발생하였습니다.\n";
		while (old_lineno == line_no) {
			m_buffer->get_u8(offset, u8, &length);
			for (int i = 0; i < length; i++) {
				
				str_msg << u8[i];
			}
			offset += length;
			line_no = m_buffer->get_line_no(offset);
		}
		
		for (int i = 0; line_start + i < offset; i++) {
			if ((line_start + i) == m_stopped_offset) str_msg << "^";
			else str_msg << "_";
		}
		
		throw parsing_exception(str_msg.str());
	}

}