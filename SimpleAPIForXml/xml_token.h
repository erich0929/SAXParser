#pragma once

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "xml_buffer.h"
#include "xml_parser.h"

using namespace std;

namespace sax_parser {
	class xml_token {
	protected:
		xml_buffer *m_buffer;
		int m_status;
		int m_stopped_offset;
		xml_handler *m_handler;

		template<int ncols>
		int do_match(int offset, int dfa_table[][ncols], int dfa_inv_table[][2], int *matched_offset, vector<int>& nfa_path);
	
	private:
		virtual int get_column(char *str, int length) = 0;
		virtual int is_dfa_final_state(int state) = 0;
		virtual int get_nfa_final_state() = 0;
		virtual int get_dfa_table_size() = 0;
		virtual int get_dfa_inv_table_size() = 0;

	public:
		enum { FAILURE = 0, MATCHED = 1 };
		
		explicit xml_token(xml_buffer *buffer, xml_handler *handler);

		virtual int match(int offset, int *matched_offset) = 0;
		void handle();
		int status() const { return m_status; };
		void throw_parsing_error() throw (parsing_exception);

	protected:
		virtual void do_handle();
	};

	template<int ncols>
	inline int xml_token::do_match(int offset, int dfa_table[][ncols], int dfa_inv_table[][2], int * matched_offset, vector<int>& nfa_path)
	{
		//size_t str_length = str_input.length();
		//vector<char> v;
		//v.reserve(str_length + 1);
		//v.assign(str_input.begin(), str_input.end());
		//v.push_back('\n');

		/* Init status */
		m_status = MATCHED;
		m_stopped_offset = -1;

		int cur_state = 0;
		int cur_col = 0;
		vector<int> dfa_path;
		dfa_path.reserve(512);
		dfa_path.push_back(cur_state);
		
		char str_u8[10];
		int length = 0;
		memset(str_u8, '\0', 10);
		m_buffer->get_u8(offset, str_u8, &length);
		int ret = 0;

		for (; length != 0; memset(str_u8, '\0', 10), m_buffer->get_u8(offset, str_u8, &length)) {
			cur_col = get_column(str_u8, length);

			int next_state = dfa_table[cur_state][cur_col];

			if (next_state == -1) {
				//offset += length;
				*matched_offset = offset;
				m_stopped_offset = offset;
				m_status = FAILURE;
				return 0;
			}
			dfa_path.push_back(next_state);
			if (is_dfa_final_state(next_state)) {
				ret = 1;
				offset += length;
				break;
			}
			cur_state = next_state;
			offset += length;
		}
		if (!ret) {
			*matched_offset = offset;
			m_stopped_offset = offset;
			m_status = FAILURE;
			return 0;
		}

		size_t dfa_path_size = dfa_path.size();
		//vector<int> nfa_path;
		nfa_path.reserve(dfa_path_size);
		vector<int>::const_reverse_iterator dfa_path_rbegin = dfa_path.rbegin();
		//vector<int>::const_reverse_iterator dfa_path_end = dfa_path.rend();
		int nfa_to_state = get_nfa_final_state();
		nfa_path.push_back(nfa_to_state);
		for (int i = 0; i < dfa_path_size - 1; i++) {
			int nfa_from_state = -1;
			vector<int>::const_reverse_iterator dfa_to_state = dfa_path_rbegin + i;
			vector<int>::const_reverse_iterator dfa_from_state = dfa_path_rbegin + i + 1;
			int key = *dfa_from_state << 16 | *dfa_to_state << 8 | nfa_to_state;
			int left = 0;
			int right = get_dfa_inv_table_size();
			int mid = right / 2;
			while (left <= right) {
				int mid_value = dfa_inv_table[mid][0];
				if (key == mid_value) {
					nfa_from_state = dfa_inv_table[mid][1]; break;
				}
				else if (key < mid_value) { right = mid - 1; }
				else { left = mid + 1; }
				mid = (left + right) / 2;
			}
			if (nfa_from_state == -1) {
				cerr << "[ERROR] dfa_inv_table has critial problem." << endl;
				abort();
			}
			nfa_path.push_back(nfa_from_state);
			nfa_to_state = nfa_from_state;
		}

		*matched_offset = offset;
		return 1;
	}
}