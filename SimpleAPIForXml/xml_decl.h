#pragma once

#include "stdafx.h"
#include <string>
#include "xml_token.h"

using namespace std;

namespace sax_parser {
	class xml_decl : public xml_token
	{
	private:
		static int dfa_table[][30];
		static int dfa_inv_table[][2];

		virtual int get_column(char *str, int length);
		virtual int is_dfa_final_state(int state);
		virtual int get_nfa_final_state();
		virtual int get_dfa_table_size();
		virtual int get_dfa_inv_table_size();

		char m_version[4];
		char *m_version_iter;
		
		vector<char> m_encoding;

		char m_standalone[4];
		char *m_standalone_iter;

	public:
		explicit xml_decl(xml_buffer *buffer, xml_handler *handler);
		virtual int match(int offset, int *matched_offset);
	};
}

