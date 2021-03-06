#include "stdafx.h"
#include "xml_decl.h"

namespace sax_parser {
	xml_decl::xml_decl(xml_buffer *buffer, xml_handler *handler) 
		: xml_token(buffer, handler), m_encoding(), m_version_iter(m_version), m_standalone_iter(m_standalone)
	{
		strcpy(m_version, "1.0");
		strcpy(m_standalone, "yes");
		m_encoding.reserve(32);
	}

	int xml_decl::get_column(char *str, int length) {
		// <	>	\s ? = "  '  _	.	-	:	x	m	l	v	e	r	s	i	o	n	c	d	g	t	a	y	letter	digit
		if (str[0] == '<') return 0;
		if (str[0] == '>') return 1;	
		if (str[0] == '?') return 3;
		if (str[0] == '=') return 4 ;
		if (str[0] == '"') return 5;
		if (str[0] == '\'') return 6;
		if (str[0] == '_') return 7 ;
		if (str[0] == '.') return 8;
		if (str[0] == '-') return 9 ;
		if (str[0] == ':') return 10;
		if (str[0] == 'x') return 11;
		if (str[0] == 'm') return 12;
		if (str[0] == 'l') return 13 ;
		if (str[0] == 'v') return 14;
		if (str[0] == 'e') return 15;
		if (str[0] == 'r') return 16;
		if (str[0] == 's') return 17;
		if (str[0] == 'i') return 18;
		if (str[0] == 'o') return 19;
		if (str[0] == 'n') return 20;
		if (str[0] == 'c') return 21;
		if (str[0] == 'd') return 22;
		if (str[0] == 'g') return 23;
		if (str[0] == 't') return 24;
		if (str[0] == 'a') return 25;
		if (str[0] == 'y') return 26;
		
		if (!(str[0] & 0x80)) {
			if (isspace(str[0])) return 2;
			if (isalpha(str[0])) return 27;
			if (isdigit(str[0])) return 28;
		}

		return 29;
	}
	int xml_decl::is_dfa_final_state(int state) {
		return (state == 24);
	}
	int xml_decl::get_nfa_final_state() {
		return 85;
	}
	
	int xml_decl::dfa_table[][30] = {
		{ 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,12,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,13,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,14,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,15,16,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,-1, },
		{ -1,-1,-1,-1,-1,19,-1,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,-1, },
		{ -1,-1,-1,-1,-1,-1,19,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,-1, },
		{ -1,-1,20,21,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,20,21,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,22,-1,23,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,24,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,25,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,26,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,27,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,28,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,29,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,30,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,31,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,32,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,33,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,34,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,35,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,36,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,37,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,38,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,39,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,40,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,41,42,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,43,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,-1,-1, },
		{ -1,-1,-1,-1,46,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,47,-1,44,44,44,-1,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,-1, },
		{ -1,-1,-1,-1,-1,-1,47,45,45,45,-1,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,-1, },
		{ -1,-1,-1,-1,-1,48,49,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,50,21,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,51,-1,-1,-1,-1,-1,52,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,53,-1,-1,-1,-1,-1,54,-1,-1,-1, },
		{ -1,-1,50,21,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,23,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,55,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,56,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,57,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,58,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,59,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,55,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,59,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,57,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
		{ -1,-1,59,21,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, },
	};
	int xml_decl::dfa_inv_table[][2] = {
		{ 0 << 16 | 1 << 8 | 1,0 },
		{ 1 << 16 | 2 << 8 | 2,1 },
		{ 2 << 16 | 3 << 8 | 3,2 },
		{ 3 << 16 | 4 << 8 | 4,3 },
		{ 4 << 16 | 5 << 8 | 5,4 },
		{ 5 << 16 | 6 << 8 | 5,5 },
		{ 5 << 16 | 6 << 8 | 7,5 },
		{ 6 << 16 | 6 << 8 | 5,5 },
		{ 6 << 16 | 6 << 8 | 7,5 },
		{ 6 << 16 | 7 << 8 | 8,7 },
		{ 7 << 16 | 8 << 8 | 9,8 },
		{ 8 << 16 | 9 << 8 | 10,9 },
		{ 9 << 16 | 10 << 8 | 11,10 },
		{ 10 << 16 | 11 << 8 | 12,11 },
		{ 11 << 16 | 12 << 8 | 13,12 },
		{ 12 << 16 | 13 << 8 | 14,13 },
		{ 13 << 16 | 14 << 8 | 15,14 },
		{ 14 << 16 | 15 << 8 | 16,15 },
		{ 14 << 16 | 16 << 8 | 20,15 },
		{ 15 << 16 | 17 << 8 | 16,16 },
		{ 15 << 16 | 17 << 8 | 18,16 },
		{ 16 << 16 | 18 << 8 | 20,20 },
		{ 16 << 16 | 18 << 8 | 22,20 },
		{ 17 << 16 | 17 << 8 | 16,16 },
		{ 17 << 16 | 17 << 8 | 18,16 },
		{ 17 << 16 | 19 << 8 | 25,18 },
		{ 17 << 16 | 19 << 8 | 50,18 },
		{ 17 << 16 | 19 << 8 | 81,18 },
		{ 17 << 16 | 19 << 8 | 83,18 },
		{ 18 << 16 | 18 << 8 | 20,20 },
		{ 18 << 16 | 18 << 8 | 22,20 },
		{ 18 << 16 | 19 << 8 | 25,22 },
		{ 18 << 16 | 19 << 8 | 50,22 },
		{ 18 << 16 | 19 << 8 | 81,22 },
		{ 18 << 16 | 19 << 8 | 83,22 },
		{ 19 << 16 | 20 << 8 | 25,25 },
		{ 19 << 16 | 20 << 8 | 27,25 },
		{ 19 << 16 | 20 << 8 | 50,50 },
		{ 19 << 16 | 20 << 8 | 52,50 },
		{ 19 << 16 | 20 << 8 | 81,81 },
		{ 19 << 16 | 20 << 8 | 83,81 },
		{ 19 << 16 | 21 << 8 | 84,83 },
		{ 20 << 16 | 20 << 8 | 25,25 },
		{ 20 << 16 | 20 << 8 | 27,25 },
		{ 20 << 16 | 20 << 8 | 50,50 },
		{ 20 << 16 | 20 << 8 | 52,50 },
		{ 20 << 16 | 20 << 8 | 81,81 },
		{ 20 << 16 | 20 << 8 | 83,81 },
		{ 20 << 16 | 21 << 8 | 84,83 },
		{ 20 << 16 | 22 << 8 | 28,27 },
		{ 20 << 16 | 23 << 8 | 53,52 },
		{ 21 << 16 | 24 << 8 | 85,84 },
		{ 22 << 16 | 25 << 8 | 29,28 },
		{ 23 << 16 | 26 << 8 | 54,53 },
		{ 25 << 16 | 27 << 8 | 30,29 },
		{ 26 << 16 | 28 << 8 | 55,54 },
		{ 27 << 16 | 29 << 8 | 31,30 },
		{ 28 << 16 | 30 << 8 | 56,55 },
		{ 29 << 16 | 31 << 8 | 32,31 },
		{ 30 << 16 | 32 << 8 | 57,56 },
		{ 31 << 16 | 33 << 8 | 33,32 },
		{ 32 << 16 | 34 << 8 | 58,57 },
		{ 33 << 16 | 35 << 8 | 34,33 },
		{ 34 << 16 | 36 << 8 | 59,58 },
		{ 35 << 16 | 37 << 8 | 35,34 },
		{ 36 << 16 | 38 << 8 | 60,59 },
		{ 37 << 16 | 39 << 8 | 36,35 },
		{ 38 << 16 | 40 << 8 | 61,60 },
		{ 39 << 16 | 41 << 8 | 37,36 },
		{ 39 << 16 | 42 << 8 | 43,36 },
		{ 40 << 16 | 43 << 8 | 62,61 },
		{ 41 << 16 | 44 << 8 | 39,37 },
		{ 41 << 16 | 44 << 8 | 41,37 },
		{ 42 << 16 | 45 << 8 | 45,43 },
		{ 42 << 16 | 45 << 8 | 47,43 },
		{ 43 << 16 | 46 << 8 | 63,62 },
		{ 44 << 16 | 44 << 8 | 39,39 },
		{ 44 << 16 | 44 << 8 | 41,39 },
		{ 44 << 16 | 47 << 8 | 50,41 },
		{ 44 << 16 | 47 << 8 | 81,41 },
		{ 44 << 16 | 47 << 8 | 83,41 },
		{ 45 << 16 | 45 << 8 | 45,45 },
		{ 45 << 16 | 45 << 8 | 47,45 },
		{ 45 << 16 | 47 << 8 | 50,47 },
		{ 45 << 16 | 47 << 8 | 81,47 },
		{ 45 << 16 | 47 << 8 | 83,47 },
		{ 46 << 16 | 48 << 8 | 64,63 },
		{ 46 << 16 | 49 << 8 | 72,63 },
		{ 47 << 16 | 21 << 8 | 84,83 },
		{ 47 << 16 | 50 << 8 | 50,50 },
		{ 47 << 16 | 50 << 8 | 52,50 },
		{ 47 << 16 | 50 << 8 | 81,81 },
		{ 47 << 16 | 50 << 8 | 83,81 },
		{ 48 << 16 | 51 << 8 | 68,64 },
		{ 48 << 16 | 52 << 8 | 65,64 },
		{ 49 << 16 | 53 << 8 | 76,72 },
		{ 49 << 16 | 54 << 8 | 73,72 },
		{ 50 << 16 | 21 << 8 | 84,83 },
		{ 50 << 16 | 23 << 8 | 53,52 },
		{ 50 << 16 | 50 << 8 | 50,50 },
		{ 50 << 16 | 50 << 8 | 52,50 },
		{ 50 << 16 | 50 << 8 | 81,81 },
		{ 50 << 16 | 50 << 8 | 83,81 },
		{ 51 << 16 | 55 << 8 | 70,68 },
		{ 52 << 16 | 56 << 8 | 66,65 },
		{ 53 << 16 | 57 << 8 | 78,76 },
		{ 54 << 16 | 58 << 8 | 74,73 },
		{ 55 << 16 | 59 << 8 | 81,70 },
		{ 55 << 16 | 59 << 8 | 83,70 },
		{ 56 << 16 | 55 << 8 | 70,66 },
		{ 57 << 16 | 59 << 8 | 81,78 },
		{ 57 << 16 | 59 << 8 | 83,78 },
		{ 58 << 16 | 57 << 8 | 78,74 },
		{ 59 << 16 | 21 << 8 | 84,83 },
		{ 59 << 16 | 59 << 8 | 81,81 },
		{ 59 << 16 | 59 << 8 | 83,81 },
	};

	int xml_decl::match(int offset, int *matched_offset) {
		m_encoding.clear();
		m_version_iter = &m_version[0];
		m_standalone_iter = &m_standalone[0];

		while (isspace(m_buffer->get_ch(offset))) offset++;

		vector<int> nfa_path;

		if (do_match<30>(offset, xml_decl::dfa_table, xml_decl::dfa_inv_table, matched_offset, nfa_path)) {
			vector<int>::const_reverse_iterator nfa_iter = nfa_path.rbegin();
			vector<int>::const_reverse_iterator end = nfa_path.rend();
			char str_u8[4];
			int length = 0;
			
			for (; nfa_iter != end || offset < *matched_offset; nfa_iter++) {
				m_buffer->get_u8(offset, str_u8, &length);
				switch (*nfa_iter)
				{
				case 16: /* version value */
				case 20:
					if (*m_version_iter != str_u8[0]) {
						*matched_offset = offset;

						m_stopped_offset = offset;
						m_status = FAILURE;

						return 0;
					}
					if(*m_version_iter) m_version_iter++;
					break;
				case 37: /* first letter of encoding */
				case 43:
				case 39: /* follow letter of encoding */
				case 45:
					for (int i = 0; i < length; i++) {
						(str_u8[0] & 0x80) ? m_encoding.push_back(str_u8[i]) : m_encoding.push_back(tolower(str_u8[i]));
					}
					break;
				case 41:
				case 47:
					m_encoding.push_back('\0');
					if (strcmp("utf-8", &m_encoding[0]) && strcmp("utf8", &m_encoding[0])) {
						*matched_offset = offset - m_encoding.size() + 1;

						m_stopped_offset = *matched_offset;
						m_status = FAILURE;
						return 0;
					}
					break;
				case 68: /* standalone="no" */
				case 76:
					*matched_offset = offset;

					m_stopped_offset = offset;
					m_status = FAILURE;

					return 0;
					break;
				case 85: /* completed status */
					return 1;
				default:
					break;
				}
				offset += length;
			}
		}
				
		return 0;
	}
	int xml_decl::get_dfa_table_size() {
		return sizeof(xml_decl::dfa_table) / sizeof(int[30]);
	}
	int xml_decl::get_dfa_inv_table_size() {
		return sizeof(xml_decl::dfa_inv_table) / sizeof(int[2]);
	}
}

