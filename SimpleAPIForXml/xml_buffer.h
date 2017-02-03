#pragma once

#include "stdafx.h"
#include <istream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>

using namespace std;

namespace sax_parser {
	class xml_buffer {
	private:
		istream			*m_xml_stream;
		vector<char>	m_xml_buffer;
		list<size_t>		m_lines;
	public:
		explicit xml_buffer(istream *xml_stream);
		virtual ~xml_buffer();

		void get_u8(int offset, char *u8, int *length);
		char get_ch(int offset);
		size_t get_line_no(int offset);
		int get_index(size_t line_no);
		const char* get_cstr() const;
	};
}
