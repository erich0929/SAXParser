#include "stdafx.h"
#include "xml_cdata.h"

namespace sax_parser {
	xml_cdata::xml_cdata(xml_buffer *buffer, xml_handler *handler ) : xml_token(buffer, handler), m_cdata_start(-1), m_cdata_end(-1)
	{}

	int xml_cdata::get_column(char *str, int length) {
		if (str[0] == '<') return 0;
		if (str[0] == '!') return 1;
		if (str[0] == '[') return 2;
		if (str[0] == 'C') return 3;
		if (str[0] == 'D') return 4;
		if (str[0] == 'A') return 5;
		if (str[0] == 'T') return 6;
		if (str[0] == ']') return 7;
		if (str[0] == '>') return 8;

		return 9;
	}
	int xml_cdata::is_dfa_final_state(int state) {
		return (state == 12);
	}
	int xml_cdata::get_nfa_final_state() {
		return 14;
	}

	int xml_cdata::match(int offset, int *matched_offset) {
		m_cdata_start = -1;
		m_cdata_end = -1;

		vector<int> nfa_path;
		if (do_match<10>(offset, xml_cdata::dfa_table, xml_cdata::dfa_inv_table, matched_offset, nfa_path)) {
			vector<int>::const_reverse_iterator nfa_iter = nfa_path.rbegin();
			vector<int>::const_reverse_iterator end = nfa_path.rend();
			char str_u8[4];
			int length = 0;
			for (; nfa_iter != end; nfa_iter++) {
				m_buffer->get_u8(offset, str_u8, &length);
				switch (*nfa_iter)
				{
				case 8: /* <!CDATA[ */
					m_cdata_start = offset + 1;
					break;
				case 10: /* ]]> */
					if (offset == m_cdata_start) {
						m_cdata_start = -1;
						m_cdata_end = -1;
					}
					else {
						m_cdata_end = offset - 1;
					}
					return 1;
					break;
				case 14: /* completed */
					return 1;
					break;
				}
				offset += length;
			}

		}
				
		return 0;
	}

	void xml_cdata::do_handle()
	{
		int length = m_cdata_end - m_cdata_start + 1;
		const char *char_data = m_buffer->get_cstr() + m_cdata_start;
		m_handler->characters(char_data, length);
	}

	int xml_cdata::dfa_table[][10] = {
		{ 1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,2,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,3,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,4,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,5,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,6,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,7,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,8,-1,-1,-1,-1, },
		{ -1,-1,9,-1,-1,-1,-1,-1,-1,-1, },
		{ 9,9,9,9,9,9,9,10,9,9, },
		{ 9,9,9,9,9,9,9,11,9,9, },
		{ 9,9,9,9,9,9,9,11,12,9, },
		{ 9,9,9,9,9,9,9,10,9,9, },
	};
	int xml_cdata::dfa_inv_table[][2] = {
		{ 0 << 16 | 1 << 8 | 1,0 },
		{ 1 << 16 | 2 << 8 | 2,1 },
		{ 2 << 16 | 3 << 8 | 3,2 },
		{ 3 << 16 | 4 << 8 | 4,3 },
		{ 4 << 16 | 5 << 8 | 5,4 },
		{ 5 << 16 | 6 << 8 | 6,5 },
		{ 6 << 16 | 7 << 8 | 7,6 },
		{ 7 << 16 | 8 << 8 | 8,7 },
		{ 8 << 16 | 9 << 8 | 10,8 },
		{ 8 << 16 | 9 << 8 | 13,8 },
		{ 9 << 16 | 9 << 8 | 10,13 },
		{ 9 << 16 | 9 << 8 | 13,13 },
		{ 9 << 16 | 10 << 8 | 10,13 },
		{ 9 << 16 | 10 << 8 | 11,10 },
		{ 9 << 16 | 10 << 8 | 13,13 },
		{ 10 << 16 | 9 << 8 | 10,13 },
		{ 10 << 16 | 9 << 8 | 13,13 },
		{ 10 << 16 | 11 << 8 | 10,13 },
		{ 10 << 16 | 11 << 8 | 11,10 },
		{ 10 << 16 | 11 << 8 | 12,11 },
		{ 10 << 16 | 11 << 8 | 13,13 },
		{ 11 << 16 | 9 << 8 | 10,13 },
		{ 11 << 16 | 9 << 8 | 13,13 },
		{ 11 << 16 | 11 << 8 | 10,13 },
		{ 11 << 16 | 11 << 8 | 11,10 },
		{ 11 << 16 | 11 << 8 | 12,11 },
		{ 11 << 16 | 11 << 8 | 13,13 },
		{ 11 << 16 | 12 << 8 | 10,13 },
		{ 11 << 16 | 12 << 8 | 13,13 },
		{ 11 << 16 | 12 << 8 | 14,12 },
		{ 12 << 16 | 9 << 8 | 10,13 },
		{ 12 << 16 | 9 << 8 | 13,13 },
		{ 12 << 16 | 10 << 8 | 10,13 },
		{ 12 << 16 | 10 << 8 | 11,10 },
		{ 12 << 16 | 10 << 8 | 13,13 },
	};
	

	
	int xml_cdata::get_dfa_table_size() {
		return sizeof(xml_cdata::dfa_table) / sizeof(int[10]);
	}
	int xml_cdata::get_dfa_inv_table_size() {
		return sizeof(xml_cdata::dfa_inv_table) / sizeof(int[2]);
	}
}

