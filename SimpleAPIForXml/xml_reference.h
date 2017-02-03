#pragma once

#include "stdafx.h"
#include "xml_token.h"

namespace sax_parser {
	class xml_reference : public xml_token
	{
	private:
		static int dfa_table[][12];
		static int dfa_inv_table[][2];

		virtual int get_column(char *str, int length);
		virtual int is_dfa_final_state(int state);
		virtual int get_nfa_final_state();
		virtual int get_dfa_table_size();
		virtual int get_dfa_inv_table_size();

		char m_reference_val[4];
	public:
		explicit xml_reference(xml_buffer *buffer, xml_handler *handler);
		virtual int match(int offset, int *matched_offset);

	protected:
		virtual void do_handle();
	};
}