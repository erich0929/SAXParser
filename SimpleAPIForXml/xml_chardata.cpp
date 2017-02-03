#include "stdafx.h"
#include "xml_chardata.h"

namespace sax_parser {
	xml_chardata::xml_chardata(xml_buffer * buffer, xml_handler *handler) : xml_token(buffer, handler), m_chardata_start(-1), m_chardata_end(-1)
	{
	}

	int xml_chardata::get_column(char *str, int length) {
		if (str[0] == '<') return 0;
		if (str[0] == '&') return 1;
		if (str[0] == ']') return 2;
		if (str[0] == '>') return 3;

		return 4;
	}
	int xml_chardata::is_dfa_final_state(int state) {
		return (state == 1 || state == 4);
	}
	int xml_chardata::get_nfa_final_state() {
		return 7;
	}

	int xml_chardata::match(int offset, int * matched_offset)
	{
		m_chardata_start = -1;
		m_chardata_end = -1;

		vector<int> nfa_path;
		if (do_match<5>(offset, xml_chardata::dfa_table, xml_chardata::dfa_inv_table, matched_offset, nfa_path)) {
			vector<int>::const_reverse_iterator nfa_iter = nfa_path.rbegin();
			vector<int>::const_reverse_iterator end = nfa_path.rend();
			
			m_chardata_start = offset;

			char str_u8[4];
			int length = 0;
			for (; nfa_iter != end; nfa_iter++) {
				m_buffer->get_u8(offset, str_u8, &length);
				switch (*nfa_iter)
				{
				case 3: // 3번은 ]]> 인식상태 - 성공이 아니다. 
					*matched_offset -= 3;
					m_chardata_start = -1;
					m_chardata_end = -1;

					m_stopped_offset = *matched_offset;
					m_status = FAILURE;

					return 0;
					break;
				case 5:
					(*matched_offset)--; // 성공시 마지막 < & 글자는 인식하지 않는다.
					if (offset == *matched_offset) {
						m_chardata_start = -1;
						m_chardata_end = -1;

						m_stopped_offset = offset;
						m_status = FAILURE;

						return 0; // 0글자 인식성공은 실패다.
					}
					m_chardata_end = *matched_offset - 1;
					return 1;
				}
			}
		}
				
		return 0;
	}

	void xml_chardata::do_handle()
	{
		int length = m_chardata_end - m_chardata_start + 1;
		const char *char_data = m_buffer->get_cstr() + m_chardata_start;
		m_handler->characters(char_data, length);
	}

	int xml_chardata::dfa_table[][5] = {
		{ 1,1,2,0,0, },
		{ -1,-1,-1,-1,-1, },
		{ 1,1,3,0,0, },
		{ 1,1,3,4,0, },
		{ 1,1,2,0,0, },
	};
	int xml_chardata::dfa_inv_table[][2] = {
		{ 0 << 16 | 0 << 8 | 1,5 },
		{ 0 << 16 | 0 << 8 | 5,5 },
		{ 0 << 16 | 1 << 8 | 7,5 },
		{ 0 << 16 | 2 << 8 | 1,5 },
		{ 0 << 16 | 2 << 8 | 2,1 },
		{ 0 << 16 | 2 << 8 | 5,5 },
		{ 2 << 16 | 0 << 8 | 1,5 },
		{ 2 << 16 | 0 << 8 | 5,5 },
		{ 2 << 16 | 1 << 8 | 7,5 },
		{ 2 << 16 | 3 << 8 | 1,5 },
		{ 2 << 16 | 3 << 8 | 2,1 },
		{ 2 << 16 | 3 << 8 | 3,2 },
		{ 2 << 16 | 3 << 8 | 5,5 },
		{ 3 << 16 | 0 << 8 | 1,5 },
		{ 3 << 16 | 0 << 8 | 5,5 },
		{ 3 << 16 | 1 << 8 | 7,5 },
		{ 3 << 16 | 3 << 8 | 1,5 },
		{ 3 << 16 | 3 << 8 | 2,1 },
		{ 3 << 16 | 3 << 8 | 3,2 },
		{ 3 << 16 | 3 << 8 | 5,5 },
		{ 3 << 16 | 4 << 8 | 1,5 },
		{ 3 << 16 | 4 << 8 | 5,5 },
		{ 3 << 16 | 4 << 8 | 7,3 },
		{ 4 << 16 | 0 << 8 | 1,5 },
		{ 4 << 16 | 0 << 8 | 5,5 },
		{ 4 << 16 | 1 << 8 | 7,5 },
		{ 4 << 16 | 2 << 8 | 1,5 },
		{ 4 << 16 | 2 << 8 | 2,1 },
		{ 4 << 16 | 2 << 8 | 5,5 },
	};

	

	int xml_chardata::get_dfa_table_size() {
		return sizeof(xml_chardata::dfa_table) / sizeof(int[5]);
	}
	int xml_chardata::get_dfa_inv_table_size() {
		return sizeof(xml_chardata::dfa_inv_table) / sizeof(int[2]);
	}
}

