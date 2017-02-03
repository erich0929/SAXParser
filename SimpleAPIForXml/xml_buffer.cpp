#include "stdafx.h"
#include "xml_buffer.h"

namespace sax_parser {
	
	xml_buffer::xml_buffer(istream *xml_stream) : m_xml_stream(xml_stream), m_xml_buffer(), m_lines() 
	{
		m_lines.push_back(0);
		m_xml_buffer.reserve(1024 * 1024);
		string xml_string;
		std::getline(*m_xml_stream, xml_string);
		std::copy(xml_string.begin(), xml_string.end(), back_inserter(m_xml_buffer));
		if (m_xml_stream->good()) m_xml_buffer.push_back('\n');
	}
	
	xml_buffer::~xml_buffer()
	{
		//delete m_xml_stream;
	}

	void xml_buffer::get_u8(int offset, char *u8, int *length)
	{
		*length = 0;
		if (offset >= m_xml_buffer.size()) {
			while (m_xml_stream->good() && offset + 512 >= m_xml_buffer.size()) {
				string xml_string;
				std::getline(*m_xml_stream, xml_string);
				if (m_xml_buffer.back() == '\n') m_lines.push_back(m_xml_buffer.size());
				std::copy(xml_string.begin(), xml_string.end(), back_inserter(m_xml_buffer));
				if (m_xml_stream->good()) m_xml_buffer.push_back('\n');
			}
		}
		int size = m_xml_buffer.size();
		vector<char>::const_iterator iter;
		vector<char>::const_iterator end;
		int u8_length = 0;
		if (offset >= size) {
			*length = 0;
			return;
		}
		else {
			iter = m_xml_buffer.begin() + offset;
			end = m_xml_buffer.end();
			u8_length = (*iter & 0xF0);
			if (!(u8_length & 0x80)) u8_length = 0;
		}
		do {
			*u8++ = *iter++;
			u8_length <<= 1;
			(*length)++;
		} while (iter != end && u8_length & 0x80);
		return;
	}

	char xml_buffer::get_ch(int offset)
	{
		if (offset >= m_xml_buffer.size()) {
			while (m_xml_stream->good() && offset + 512 >= m_xml_buffer.size()) {
				string xml_string;
				std::getline(*m_xml_stream, xml_string);
				if (m_xml_buffer.back() == '\n') m_lines.push_back(m_xml_buffer.size());
				std::copy(xml_string.begin(), xml_string.end(), back_inserter(m_xml_buffer));
				if (m_xml_stream->good()) m_xml_buffer.push_back('\n');
			}
		}
		int size = m_xml_buffer.size();
		if (offset >= size) {
			return '\0';
		}
		return m_xml_buffer[offset];
	}

	size_t xml_buffer::get_line_no(int offset)
	{
		size_t line_no = 0;
		char tmp[10];
		int tmp_len;
		get_u8(offset, tmp, &tmp_len);
		list<size_t>::const_iterator iter = m_lines.begin();
		list<size_t>::const_iterator end = m_lines.end();
		size_t left = 0;
		size_t right = 0;
		for (iter; iter != end; iter++) {
			right = *iter;
			if (left <= offset && offset < right) break;
			left = right;
			line_no++;
		}
		return line_no;
	}

	int xml_buffer::get_index(size_t line_no)
	{
		if (line_no < 1) return -1;
		list<size_t>::const_iterator iter = m_lines.begin();
		list<size_t>::const_iterator end = m_lines.end();
		for (iter; iter != end; iter++) {
			if (--line_no == 0) return *iter;
		}
		return -1;
	}
	const char * xml_buffer::get_cstr() const
	{
		return &m_xml_buffer[0];
	}
}

