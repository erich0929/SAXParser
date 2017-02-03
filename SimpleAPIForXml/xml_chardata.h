#pragma once
#include "stdafx.h"
#include "xml_token.h"

namespace sax_parser {
	class xml_chardata : public xml_token
	{
	private:
		static int dfa_table[][5];
		static int dfa_inv_table[][2];

		virtual int get_column(char *str, int length);
		virtual int is_dfa_final_state(int state);
		virtual int get_nfa_final_state();
		virtual int get_dfa_table_size();
		virtual int get_dfa_inv_table_size();

		int m_chardata_start;
		int m_chardata_end;

	public:
		explicit xml_chardata(xml_buffer *buffer, xml_handler *handler);
		virtual int match(int offset, int *matched_offset);

	protected:
		virtual void do_handle();
	};
}