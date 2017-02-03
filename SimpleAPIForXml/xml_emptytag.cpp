#include "stdafx.h"
#include "xml_emptytag.h"
#include "xml_util.h"

namespace sax_parser {
	xml_emptytag::xml_emptytag(xml_buffer * buffer, xml_handler *handler) : xml_token(buffer, handler), m_tagname(), m_attrs()
	{
		m_tagname.reserve(128);
		m_attrs.reserve(32);
	}

	int xml_emptytag::get_column(char *str, int length) {
		// <	:	_	.	-	\s	=	#	&	x	"	"'"	;	letter	digit	>	/	else
		if (str[0] == '<') return 0;
		if (str[0] == ':') return 1;
		if (str[0] == '_') return 2;
		if (str[0] == '.') return 3;
		if (str[0] == '-') return 4;
		if (str[0] == '=') return 6;
		if (str[0] == '#') return 7;
		if (str[0] == '&') return 8;
		if (str[0] == 'x') return 9;
		if (str[0] == '"') return 10;
		if (str[0] == '\'') return 11;
		if (str[0] == ';') return 12;
		if (str[0] == '>') return 15;
		if (str[0] == '/') return 16;
		
		if (!(str[0] & 0x80)) {
			if (isspace(str[0])) return 5;
			if (isalpha(str[0])) return 13;
			if (isdigit(str[0])) return 14;
		}

		return 17;
	}
	int xml_emptytag::is_dfa_final_state(int state) {
		return (state == 6);
	}
	int xml_emptytag::get_nfa_final_state() {
		return 95;
	}

	int xml_emptytag::match(int offset, int * matched_offset)
	{
		m_tagname.clear();
		m_attrs.clear();

		while (isspace(m_buffer->get_ch(offset))) offset++;

		vector<int> nfa_path;
		if (do_match<18>(offset, xml_emptytag::dfa_table, xml_emptytag::dfa_inv_table, matched_offset, nfa_path)) {
			vector<int>::const_reverse_iterator nfa_iter = nfa_path.rbegin();
			vector<int>::const_reverse_iterator end = nfa_path.rend();
			char str_u8[4];
			int length = 0;
			//int name_hex = 0;
			int char_ref = 0;
			int entity_ref = 0;
			vector<char> tmp_attr_name;
			vector<char> tmp_attr_value;

			tmp_attr_name.reserve(128);
			tmp_attr_value.reserve(128);

			for (; nfa_iter != end; nfa_iter++) {
				m_buffer->get_u8(offset, str_u8, &length);
				switch (*nfa_iter)
				{
				case 1: /* tagname */
				case 3:
					for (int i = 0; i<length; i++) m_tagname.push_back(str_u8[0]);
					break;
				case 5:  /* #x... is invalid char in tagname */
				case 19: /* #x... is invalid char in attr name */
					*matched_offset = offset;

					m_stopped_offset = offset;
					m_status = FAILURE;

					return 0;
				case 15: /* attr name */
				case 17:
					for (int i = 0; i < length; i++) tmp_attr_name.push_back(str_u8[0]);
					break;
				case 35: /* attr value - one char */
				case 63:
					for (int i = 0; i < length; i++) tmp_attr_value.push_back(str_u8[0]);
					break;
				case 36: /* attr_value init char reference, entity reference */
				case 64:
					char_ref = 0;
					entity_ref = 0;
					break;
				case 40: /* attr value - char reference (decimal) */
				case 68:
					char_ref *= 10;
					char_ref += str_u8[0];
					break;
				case 43: /* attr value - char reference (hex) */
				case 71:
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

				case 45: /* attr value - entity reference */
				case 47:
				case 73:
				case 75:
					entity_ref <<= 8;
					entity_ref |= str_u8[0];
					break;

				case 49: /* #x... is invalid char in attr value */
				case 77:
					*matched_offset = offset;

					m_stopped_offset = offset;
					m_status = FAILURE;

					return 0;
					break;
				case 57: /* attr value - &...; char reference, entity reference flushed */
				case 85:
				{
					/* char reference */
					if (char_ref != 0) {
						char tmp_ref_utf[4];
						memset(tmp_ref_utf, '\0', 4);
						int tmp_ref_length = 0;
						uc2utf8(char_ref, tmp_ref_utf, &tmp_ref_length);
						for (int i = 0; i < tmp_ref_length; i++) tmp_attr_value.push_back(tmp_ref_utf[i]);
					}
					/* entity reference */
					if (entity_ref != 0) {
						char entity_char;
						if (get_entity_char(entity_ref, &entity_char)) 	tmp_attr_value.push_back(entity_char);
					}
					break;
				}

				case 60: /* attr name = "attr value" | 'attr value' */
				case 88:
				{
					tmp_attr_name.push_back('\0');
					tmp_attr_value.push_back('\0');
					m_attrs.push_back(pair<string, string>(string(tmp_attr_name.begin(), tmp_attr_name.end()), string(tmp_attr_value.begin(), tmp_attr_value.end())));
					tmp_attr_name.clear();
					tmp_attr_value.clear();
				}
				break;

				case 95:
					return 1;
				}
				offset += length;
			}
		}

		return 0;
	}

	void xml_emptytag::do_handle()
	{
		m_handler->start_element(string(m_tagname.begin(), m_tagname.end()), m_attrs);
	}

	int xml_emptytag::dfa_table[][18] = {
		{ 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,2,2,-1,-1,-1,-1,-1,-1,2,-1,-1,-1,2,-1,-1,-1,-1, },
		{ -1,2,2,2,2,3,-1,-1,-1,2,-1,-1,-1,2,2,-1,4,-1, },
		{ -1,5,5,-1,-1,3,-1,-1,-1,5,-1,-1,-1,5,-1,-1,4,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,6,-1,-1, },
		{ -1,5,5,5,5,7,8,-1,-1,5,-1,-1,-1,5,5,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,7,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,8,-1,-1,-1,-1,9,10,-1,-1,-1,-1,-1,-1, },
		{ -1,9,9,9,9,9,9,9,11,9,12,-1,9,9,9,9,9,9, },
		{ -1,10,10,10,10,10,10,10,13,10,-1,12,10,10,10,10,10,10, },
		{ -1,14,14,-1,-1,-1,-1,15,-1,14,-1,-1,-1,14,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,4,-1, },
		{ -1,16,16,-1,-1,-1,-1,17,-1,16,-1,-1,-1,16,-1,-1,-1,-1, },
		{ -1,14,14,14,14,-1,-1,-1,-1,14,-1,-1,9,14,14,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,18,-1,-1,-1,-1,19,-1,-1,-1, },
		{ -1,16,16,16,16,-1,-1,-1,-1,16,-1,-1,10,16,16,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,20,-1,-1,-1,-1,21,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,22,22,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,9,-1,19,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,23,23,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,10,-1,21,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,9,22,22,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,10,23,23,-1,-1,-1, },
	};
	int xml_emptytag::dfa_inv_table[][2] = {
		{ 0 << 16 | 1 << 8 | 1,0 },
		{ 1 << 16 | 2 << 8 | 3,1 },
		{ 1 << 16 | 2 << 8 | 5,1 },
		{ 1 << 16 | 2 << 8 | 13,1 },
		{ 1 << 16 | 2 << 8 | 91,1 },
		{ 1 << 16 | 2 << 8 | 93,1 },
		{ 2 << 16 | 2 << 8 | 3,3 },
		{ 2 << 16 | 2 << 8 | 5,3 },
		{ 2 << 16 | 2 << 8 | 13,3 },
		{ 2 << 16 | 2 << 8 | 91,3 },
		{ 2 << 16 | 2 << 8 | 93,3 },
		{ 2 << 16 | 3 << 8 | 13,13 },
		{ 2 << 16 | 3 << 8 | 15,13 },
		{ 2 << 16 | 3 << 8 | 91,91 },
		{ 2 << 16 | 3 << 8 | 93,91 },
		{ 2 << 16 | 4 << 8 | 94,93 },
		{ 3 << 16 | 3 << 8 | 13,13 },
		{ 3 << 16 | 3 << 8 | 15,13 },
		{ 3 << 16 | 3 << 8 | 91,91 },
		{ 3 << 16 | 3 << 8 | 93,91 },
		{ 3 << 16 | 4 << 8 | 94,93 },
		{ 3 << 16 | 5 << 8 | 17,15 },
		{ 3 << 16 | 5 << 8 | 19,15 },
		{ 3 << 16 | 5 << 8 | 27,15 },
		{ 3 << 16 | 5 << 8 | 29,15 },
		{ 4 << 16 | 6 << 8 | 95,94 },
		{ 5 << 16 | 5 << 8 | 17,17 },
		{ 5 << 16 | 5 << 8 | 19,17 },
		{ 5 << 16 | 5 << 8 | 27,17 },
		{ 5 << 16 | 5 << 8 | 29,17 },
		{ 5 << 16 | 7 << 8 | 27,27 },
		{ 5 << 16 | 7 << 8 | 29,27 },
		{ 5 << 16 | 8 << 8 | 31,29 },
		{ 5 << 16 | 8 << 8 | 33,29 },
		{ 7 << 16 | 7 << 8 | 27,27 },
		{ 7 << 16 | 7 << 8 | 29,27 },
		{ 7 << 16 | 8 << 8 | 31,29 },
		{ 7 << 16 | 8 << 8 | 33,29 },
		{ 8 << 16 | 8 << 8 | 31,31 },
		{ 8 << 16 | 8 << 8 | 33,31 },
		{ 8 << 16 | 9 << 8 | 35,33 },
		{ 8 << 16 | 9 << 8 | 36,33 },
		{ 8 << 16 | 9 << 8 | 60,33 },
		{ 8 << 16 | 10 << 8 | 63,33 },
		{ 8 << 16 | 10 << 8 | 64,33 },
		{ 8 << 16 | 10 << 8 | 88,33 },
		{ 9 << 16 | 9 << 8 | 35,35 },
		{ 9 << 16 | 9 << 8 | 36,35 },
		{ 9 << 16 | 9 << 8 | 60,35 },
		{ 9 << 16 | 11 << 8 | 38,36 },
		{ 9 << 16 | 11 << 8 | 45,36 },
		{ 9 << 16 | 12 << 8 | 13,60 },
		{ 9 << 16 | 12 << 8 | 91,60 },
		{ 9 << 16 | 12 << 8 | 93,60 },
		{ 10 << 16 | 10 << 8 | 63,63 },
		{ 10 << 16 | 10 << 8 | 64,63 },
		{ 10 << 16 | 10 << 8 | 88,63 },
		{ 10 << 16 | 12 << 8 | 13,88 },
		{ 10 << 16 | 12 << 8 | 91,88 },
		{ 10 << 16 | 12 << 8 | 93,88 },
		{ 10 << 16 | 13 << 8 | 66,64 },
		{ 10 << 16 | 13 << 8 | 73,64 },
		{ 11 << 16 | 14 << 8 | 47,45 },
		{ 11 << 16 | 14 << 8 | 49,45 },
		{ 11 << 16 | 14 << 8 | 57,45 },
		{ 11 << 16 | 15 << 8 | 40,38 },
		{ 11 << 16 | 15 << 8 | 42,38 },
		{ 12 << 16 | 3 << 8 | 13,13 },
		{ 12 << 16 | 3 << 8 | 15,13 },
		{ 12 << 16 | 3 << 8 | 91,91 },
		{ 12 << 16 | 3 << 8 | 93,91 },
		{ 12 << 16 | 4 << 8 | 94,93 },
		{ 13 << 16 | 16 << 8 | 75,73 },
		{ 13 << 16 | 16 << 8 | 77,73 },
		{ 13 << 16 | 16 << 8 | 85,73 },
		{ 13 << 16 | 17 << 8 | 68,66 },
		{ 13 << 16 | 17 << 8 | 70,66 },
		{ 14 << 16 | 9 << 8 | 35,57 },
		{ 14 << 16 | 9 << 8 | 36,57 },
		{ 14 << 16 | 9 << 8 | 60,57 },
		{ 14 << 16 | 14 << 8 | 47,47 },
		{ 14 << 16 | 14 << 8 | 49,47 },
		{ 14 << 16 | 14 << 8 | 57,47 },
		{ 15 << 16 | 18 << 8 | 43,42 },
		{ 15 << 16 | 19 << 8 | 40,40 },
		{ 15 << 16 | 19 << 8 | 57,40 },
		{ 16 << 16 | 10 << 8 | 63,85 },
		{ 16 << 16 | 10 << 8 | 64,85 },
		{ 16 << 16 | 10 << 8 | 88,85 },
		{ 16 << 16 | 16 << 8 | 75,75 },
		{ 16 << 16 | 16 << 8 | 77,75 },
		{ 16 << 16 | 16 << 8 | 85,75 },
		{ 17 << 16 | 20 << 8 | 71,70 },
		{ 17 << 16 | 21 << 8 | 68,68 },
		{ 17 << 16 | 21 << 8 | 85,68 },
		{ 18 << 16 | 22 << 8 | 43,43 },
		{ 18 << 16 | 22 << 8 | 57,43 },
		{ 19 << 16 | 9 << 8 | 35,57 },
		{ 19 << 16 | 9 << 8 | 36,57 },
		{ 19 << 16 | 9 << 8 | 60,57 },
		{ 19 << 16 | 19 << 8 | 40,40 },
		{ 19 << 16 | 19 << 8 | 57,40 },
		{ 20 << 16 | 23 << 8 | 71,71 },
		{ 20 << 16 | 23 << 8 | 85,71 },
		{ 21 << 16 | 10 << 8 | 63,85 },
		{ 21 << 16 | 10 << 8 | 64,85 },
		{ 21 << 16 | 10 << 8 | 88,85 },
		{ 21 << 16 | 21 << 8 | 68,68 },
		{ 21 << 16 | 21 << 8 | 85,68 },
		{ 22 << 16 | 9 << 8 | 35,57 },
		{ 22 << 16 | 9 << 8 | 36,57 },
		{ 22 << 16 | 9 << 8 | 60,57 },
		{ 22 << 16 | 22 << 8 | 43,43 },
		{ 22 << 16 | 22 << 8 | 57,43 },
		{ 23 << 16 | 10 << 8 | 63,85 },
		{ 23 << 16 | 10 << 8 | 64,85 },
		{ 23 << 16 | 10 << 8 | 88,85 },
		{ 23 << 16 | 23 << 8 | 71,71 },
		{ 23 << 16 | 23 << 8 | 85,71 },
	};

	
	
	int xml_emptytag::get_dfa_table_size() {
		return sizeof(xml_emptytag::dfa_table) / sizeof(int[18]);
	}
	int xml_emptytag::get_dfa_inv_table_size() {
		return sizeof(xml_emptytag::dfa_inv_table) / sizeof(int[2]);
	}
	
}


