#include "stdafx.h"
#include "xml_etag.h"

namespace sax_parser {
	
	xml_etag::xml_etag(xml_buffer *buffer, xml_handler *handler) : xml_token(buffer, handler), m_tagname()
	{
		m_tagname.reserve(128);
	}
	
	int xml_etag::get_column(char *str, int length) {
		if (str[0] == '<') return 0;
		if (str[0] == ':') return 1;
		if (str[0] == '_') return 2;
		if (str[0] == '.') return 3;
		if (str[0] == '-') return 4;
		if (str[0] == '#') return 6;
		if (str[0] == 'x') return 7;
		if (str[0] == '>') return 10;
		if (str[0] == '/') return 12;

		if (!(str[0] & 0x80)) {
			if (isspace(str[0])) return 5;
			if (isalpha(str[0])) return 8;
			if (isdigit(str[0])) return 9;
		}

		return 11;
	}
	int xml_etag::is_dfa_final_state(int state) {
		return (state == 5);
	}
	int xml_etag::get_nfa_final_state() {
		return 17;
	}

	int xml_etag::match(int offset, int *matched_offset) {
		m_tagname.clear();

		while (isspace(m_buffer->get_ch(offset))) offset++;

		vector<int> nfa_path;
		if (do_match<13>(offset, xml_etag::dfa_table, xml_etag::dfa_inv_table, matched_offset, nfa_path)) {
			vector<int>::const_reverse_iterator nfa_iter = nfa_path.rbegin();
			vector<int>::const_reverse_iterator end = nfa_path.rend();
			char str_u8[4];
			int length = 0;
			for (; nfa_iter != end; nfa_iter++) {
				m_buffer->get_u8(offset, str_u8, &length);
				switch (*nfa_iter)
				{
				case 2: /* tagname */
				case 4:
					for(int i=0; i<length; i++) m_tagname.push_back(str_u8[i]);
					break;
				case 6: /* #x.... */
					*matched_offset = offset;

					m_stopped_offset = offset;
					m_status = FAILURE;

					return 0;
				case 17: /* completed */
					return 1;
					break;
				}
				offset += length;
			}
			
		}

		return 0;
	}

	string xml_etag::get_tagname()
	{
		return string(m_tagname.begin(), m_tagname.end());
	}

	void xml_etag::do_handle()
	{
		m_handler->end_element(string(m_tagname.begin(), m_tagname.end()));
	}
	
	int xml_etag::dfa_table[][13] = {
		{ 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,2, },
		{ -1,3,3,-1,-1,-1,-1,3,3,-1,-1,-1,-1, },
		{ -1,3,3,3,3,4,-1,3,3,3,5,-1,-1, },
		{ -1,-1,-1,-1,-1,4,-1,-1,-1,-1,5,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
	};
	int xml_etag::dfa_inv_table[][2] = {
		{ 0 << 16 | 1 << 8 | 1,0 },
		{ 1 << 16 | 2 << 8 | 2,1 },
		{ 2 << 16 | 3 << 8 | 4,2 },
		{ 2 << 16 | 3 << 8 | 6,2 },
		{ 2 << 16 | 3 << 8 | 14,2 },
		{ 2 << 16 | 3 << 8 | 16,2 },
		{ 3 << 16 | 3 << 8 | 4,4 },
		{ 3 << 16 | 3 << 8 | 6,4 },
		{ 3 << 16 | 3 << 8 | 14,4 },
		{ 3 << 16 | 3 << 8 | 16,4 },
		{ 3 << 16 | 4 << 8 | 14,14 },
		{ 3 << 16 | 4 << 8 | 16,14 },
		{ 3 << 16 | 5 << 8 | 17,16 },
		{ 4 << 16 | 4 << 8 | 14,14 },
		{ 4 << 16 | 4 << 8 | 16,14 },
		{ 4 << 16 | 5 << 8 | 17,16 },
	};
			
	int xml_etag::get_dfa_table_size() {
		return sizeof(xml_etag::dfa_table) / sizeof(int[13]);
	}
	int xml_etag::get_dfa_inv_table_size() {
		return sizeof(xml_etag::dfa_inv_table) / sizeof(int[2]);
	}
}
