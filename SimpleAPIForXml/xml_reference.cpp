#include "stdafx.h"
#include "xml_reference.h"
#include "xml_util.h"

namespace sax_parser {
	xml_reference::xml_reference(xml_buffer *buffer, xml_handler *handler) : xml_token(buffer, handler)
	{
		memset(m_reference_val, '\0', 4);
	}
	
	int xml_reference::get_column(char *str, int length) {
		if (str[0] == '<') return 0;
		if (str[0] == ':') return 1;
		if (str[0] == '_') return 2;
		if (str[0] == '.') return 3;
		if (str[0] == '-') return 4;
		if (str[0] == '#') return 5;
		if (str[0] == '&') return 6;
		if (str[0] == 'x') return 7;
		if (str[0] == ';') return 8;

		if (!(str[0] & 0x80)) {
			if (isalpha(str[0])) return 9;
			if (isdigit(str[0])) return 10;
		}
		
		return 11;
	}
	int xml_reference::is_dfa_final_state(int state) {
		return (state == 4);
	}
	int xml_reference::get_nfa_final_state() {
		return 22;
	}

	int xml_reference::match(int offset, int *matched_offset) {
		memset(m_reference_val, '\0', 4);

		vector<int> nfa_path;
		if (do_match<12>(offset, xml_reference::dfa_table, xml_reference::dfa_inv_table, matched_offset, nfa_path)) {
			vector<int>::const_reverse_iterator nfa_iter = nfa_path.rbegin();
			vector<int>::const_reverse_iterator end = nfa_path.rend();
			char str_u8[4];
			int length = 0;
			int char_ref = 0;
			int entity_ref = 0;

			for (; nfa_iter != end; nfa_iter++) {
				m_buffer->get_u8(offset, str_u8, &length);
				switch (*nfa_iter)
				{
				case 4: /* char reference (decimal) */
					char_ref *= 10;
					char_ref += str_u8[0];

					break;
				case 7: /* char reference (hex) */
					if (!(str_u8[0] & 0x80)) {
						do {
							if (str_u8[0] >= '0' && str_u8[0] <= '9') {
								char_ref <<= 4;
								char_ref |= str_u8[0] - '0';
								break;
							}
							if (tolower(str_u8[0]) >= 'a' && tolower(str_u8[0]) <= 'f') {
								char_ref <<= 4;
								char_ref |= tolower(str_u8[0]) - 'a' + 10;
								break;
							}

							*matched_offset = offset;

							m_stopped_offset = offset;
							m_status = FAILURE;

							return 0;
						} while (0);
						break; // 여기까지 와야 성공
					}

					*matched_offset = offset;

					m_stopped_offset = offset;
					m_status = FAILURE;

					return 0;

					break;
					
				case 9: /* entity reference */
				case 11:
					entity_ref <<= 8;
					entity_ref |= str_u8[0];
					break;
				case 21:
					/* char reference */
					if (char_ref != 0) {
						char tmp_ref_utf[4];
						memset(tmp_ref_utf, '\0', 4);
						int tmp_ref_length = 0;
						uc2utf8(char_ref, tmp_ref_utf, &tmp_ref_length);
						for (int i = 0; i < tmp_ref_length; i++) m_reference_val[i] = tmp_ref_utf[i];
					}
					/* entity reference */
					if (entity_ref != 0) {
						char entity_char;
						if (get_entity_char(entity_ref, &entity_char)) m_reference_val[0] = entity_char;
					}
					return 1;
					break;
				case 22: /* completed */
					return 1;
					break;
				}
				offset += length;
			}
		}

		return 0;
	}

	void xml_reference::do_handle()
	{
		m_handler->characters(m_reference_val, strlen(m_reference_val));
	}

	int xml_reference::dfa_table[][12] = {
		{ -1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1, },
		{ -1,2,2,-1,-1,3,-1,2,-1,2,-1,-1, },
		{ -1,2,2,2,2,-1,-1,2,4,2,2,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,5,-1,-1,6,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,7,7,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,4,-1,6,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,4,7,7,-1, },
	};
	int xml_reference::dfa_inv_table[][2] = {
		{ 0 << 16 | 1 << 8 | 2,0 },
		{ 0 << 16 | 1 << 8 | 9,0 },
		{ 1 << 16 | 2 << 8 | 11,9 },
		{ 1 << 16 | 2 << 8 | 13,9 },
		{ 1 << 16 | 2 << 8 | 21,9 },
		{ 1 << 16 | 3 << 8 | 4,2 },
		{ 1 << 16 | 3 << 8 | 6,2 },
		{ 2 << 16 | 2 << 8 | 11,11 },
		{ 2 << 16 | 2 << 8 | 13,11 },
		{ 2 << 16 | 2 << 8 | 21,11 },
		{ 2 << 16 | 4 << 8 | 22,21 },
		{ 3 << 16 | 5 << 8 | 7,6 },
		{ 3 << 16 | 6 << 8 | 4,4 },
		{ 3 << 16 | 6 << 8 | 21,4 },
		{ 5 << 16 | 7 << 8 | 7,7 },
		{ 5 << 16 | 7 << 8 | 21,7 },
		{ 6 << 16 | 4 << 8 | 22,21 },
		{ 6 << 16 | 6 << 8 | 4,4 },
		{ 6 << 16 | 6 << 8 | 21,4 },
		{ 7 << 16 | 4 << 8 | 22,21 },
		{ 7 << 16 | 7 << 8 | 7,7 },
		{ 7 << 16 | 7 << 8 | 21,7 },
	};
	
	int xml_reference::get_dfa_table_size() {
		return sizeof(xml_reference::dfa_table) / sizeof(int[12]);
	}
	int xml_reference::get_dfa_inv_table_size() {
		return sizeof(xml_reference::dfa_inv_table) / sizeof(int[2]);
	}
	
}
