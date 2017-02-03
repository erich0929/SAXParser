#pragma once
#include "stdafx.h"

namespace sax_parser {
	int is_combining_char(int hex_value);
	int is_extender_char(int hex_value);
	void uc2utf8(int uc, char str_u8[], int *length);
	int get_entity_char(int key, char *ch);
}