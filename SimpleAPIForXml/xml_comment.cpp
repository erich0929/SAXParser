#include "stdafx.h"
#include "xml_comment.h"

namespace sax_parser {
	
	xml_comment::xml_comment(xml_buffer *buffer, xml_handler *handler) : xml_token(buffer, handler)
	{}

	int xml_comment::get_column(char *str, int length) {
		if (str[0] == '<') return 0;
		if (str[0] == '!') return 1;
		if (str[0] == '-') return 2;
		if (str[0] == '>') return 3;

		return 4;
	}
	int xml_comment::is_dfa_final_state(int state) {
		return (state == 7);
	}
	int xml_comment::get_nfa_final_state() {
		return 9;
	}
	
	int xml_comment::dfa_table[][5] = {
		{ 1,-1,-1,-1,-1, },
		{ -1,2,-1,-1,-1, },
		{ -1,-1,3,-1,-1, },
		{ -1,-1,4,-1,-1, },
		{ 4,4,5,4,4, },
		{ 4,4,6,4,4, },
		{ 4,4,6,7,4, },
		{ 4,4,5,4,4, },
	};
	int xml_comment::dfa_inv_table[][2] = {
		{ 0 << 16 | 1 << 8 | 1,0 },
		{ 1 << 16 | 2 << 8 | 2,1 },
		{ 2 << 16 | 3 << 8 | 3,2 },
		{ 3 << 16 | 4 << 8 | 5,3 },
		{ 3 << 16 | 4 << 8 | 8,3 },
		{ 4 << 16 | 4 << 8 | 5,8 },
		{ 4 << 16 | 4 << 8 | 8,8 },
		{ 4 << 16 | 5 << 8 | 5,8 },
		{ 4 << 16 | 5 << 8 | 6,5 },
		{ 4 << 16 | 5 << 8 | 8,8 },
		{ 5 << 16 | 4 << 8 | 5,8 },
		{ 5 << 16 | 4 << 8 | 8,8 },
		{ 5 << 16 | 6 << 8 | 5,8 },
		{ 5 << 16 | 6 << 8 | 6,5 },
		{ 5 << 16 | 6 << 8 | 7,6 },
		{ 5 << 16 | 6 << 8 | 8,8 },
		{ 6 << 16 | 4 << 8 | 5,8 },
		{ 6 << 16 | 4 << 8 | 8,8 },
		{ 6 << 16 | 6 << 8 | 5,8 },
		{ 6 << 16 | 6 << 8 | 6,5 },
		{ 6 << 16 | 6 << 8 | 7,6 },
		{ 6 << 16 | 6 << 8 | 8,8 },
		{ 6 << 16 | 7 << 8 | 5,8 },
		{ 6 << 16 | 7 << 8 | 8,8 },
		{ 6 << 16 | 7 << 8 | 9,7 },
		{ 7 << 16 | 4 << 8 | 5,8 },
		{ 7 << 16 | 4 << 8 | 8,8 },
		{ 7 << 16 | 5 << 8 | 5,8 },
		{ 7 << 16 | 5 << 8 | 6,5 },
		{ 7 << 16 | 5 << 8 | 8,8 },
	};

	int xml_comment::match(int offset, int *matched_offset) {
		while (isspace(m_buffer->get_ch(offset))) offset++;

		vector<int> nfa_path;
		if (!do_match<5>(offset, xml_comment::dfa_table, xml_comment::dfa_inv_table, matched_offset, nfa_path)) {
			m_stopped_offset = offset;
			m_status = FAILURE;

			return 0;
		}

		return 1;
	}

	int xml_comment::get_dfa_table_size() {
		return sizeof(xml_comment::dfa_table) / sizeof(int [5]);
	}
	int xml_comment::get_dfa_inv_table_size() {
		return sizeof(xml_comment::dfa_inv_table) / sizeof(int [2]);
	}
}

